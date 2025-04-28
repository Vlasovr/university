package com.vlasov.phonecodeservice.service;
import com.vlasov.phonecodeservice.cache.PhoneCodeCache;
import com.vlasov.phonecodeservice.exception.InternalServerErrorException;
import com.vlasov.phonecodeservice.exception.ValidationException;
import com.vlasov.phonecodeservice.model.Country;
import com.vlasov.phonecodeservice.model.PhoneCodeBulkRequest;
import com.vlasov.phonecodeservice.model.PhoneCodeResponse;
import com.vlasov.phonecodeservice.model.PhoneCodeResult;
import com.vlasov.phonecodeservice.repository.PhoneCodeResultRepository;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import org.springframework.http.ResponseEntity;
import org.springframework.http.HttpStatus;
import org.springframework.web.client.RestClientException;
import org.springframework.scheduling.annotation.Scheduled;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.List;
import java.util.stream.Collectors;
import java.util.ArrayList;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Service
public class PhoneCodeService {

    private static final Logger logger = LoggerFactory.getLogger(PhoneCodeService.class);
    private static final String EXTERNAL_API_URL = "https://restcountries.com/v3.1/all?fields=name,cca2,idd";

    private final RestTemplate restTemplate;
    private final PhoneCodeCache cache;
    private final PhoneCodeResultRepository phoneCodeResultRepository;

    @Autowired
    public PhoneCodeService(RestTemplate restTemplate,
                            PhoneCodeCache cache,
                            PhoneCodeResultRepository phoneCodeResultRepository) {
        this.restTemplate = restTemplate;
        this.cache = cache;
        this.phoneCodeResultRepository = phoneCodeResultRepository;
        loadDataToCache();
    }

    private void loadDataToCache() {
        try {
            logger.info("Loading data into cache from external API.");
            ResponseEntity<Country[]> response = restTemplate.getForEntity(EXTERNAL_API_URL, Country[].class);
            if (response.getStatusCode() == HttpStatus.OK && response.getBody() != null) {
                for (Country country : response.getBody()) {
                    String commonName = country.getName().getCommon().toLowerCase();
                    String cca2 = country.getCca2().toLowerCase();

                    String iddRoot = country.getIdd().getRoot();
                    List<String> suffixes = country.getIdd().getSuffixes();

                    String phoneCode = (iddRoot != null ? iddRoot : "") +
                            (suffixes != null && !suffixes.isEmpty() ? suffixes.get(0) : "");

                    if (!phoneCode.isEmpty()) {
                        cache.addToCache(commonName, phoneCode);
                        cache.addToCache(cca2, phoneCode);
                        cache.addToReverseCache(phoneCode, commonName);
                        logger.debug("Added to reverseCache: phoneCode='{}', countryName='{}'", phoneCode, commonName);
                    } else {
                        logger.warn("No phone code available for country: {}", commonName);
                    }
                }
                logger.info("Data loaded into cache successfully.");
            } else {
                logger.error("Failed to load data from external API. Status Code: {}", response.getStatusCode());
                throw new InternalServerErrorException("Unable to load country data.");
            }
        } catch (RestClientException e) {
            logger.error("RestClientException while loading data into cache: {}", e.getMessage());
            throw new InternalServerErrorException("Unable to load country data due to network error.");
        } catch (Exception e) {
            logger.error("Exception while loading data into cache: {}", e.getMessage());
            throw new InternalServerErrorException("An unexpected error occurred while loading data.");
        }
    }

    public PhoneCodeResponse getCountryByPhoneCode(String phoneCode) {
        logger.info("Received request to get country by phone code: {}", phoneCode);
        if (phoneCode == null || phoneCode.trim().isEmpty()) {
            logger.warn("Validation failed: 'phoneCode' parameter is missing or empty.");
            throw new ValidationException("Parameter 'phoneCode' must be provided.", "Please provide a valid phone code.");
        }

        phoneCode = phoneCode.replace(" ", "+").trim();
        if (!phoneCode.startsWith("+")) {
            phoneCode = "+" + phoneCode;
        }

        List<String> countries = cache.getFromReverseCache(phoneCode);
        if (countries == null || countries.isEmpty()) {
            logger.warn("No countries found for phone code: {}", phoneCode);
            throw new ValidationException("No countries found for the provided phone code.", "Please check the provided phone code.");
        }

        String countriesStr = String.join(", ", countries);

        PhoneCodeResult result = new PhoneCodeResult();
        result.setQueryParameter(phoneCode);
        result.setPhoneCode(countriesStr);
        phoneCodeResultRepository.save(result);

        logger.info("Countries for phone code '{}': {}", phoneCode, countriesStr);

        return new PhoneCodeResponse(phoneCode, countriesStr);
    }

    public PhoneCodeResponse getPhoneCode(String name, String cca2) {
        logger.info("Received request to get phone code by name: '{}' or cca2: '{}'", name, cca2);
        if ((name == null || name.trim().isEmpty()) && (cca2 == null || cca2.trim().isEmpty())) {
            logger.warn("Validation failed: both 'name' and 'cca2' parameters are missing or empty.");
            throw new ValidationException("Either 'name' or 'cca2' parameter must be provided.", "Please provide a valid country name or cca2 code.");
        }

        String key = null;
        if (name != null && !name.trim().isEmpty()) {
            key = name.trim().toLowerCase();
        } else if (cca2 != null && !cca2.trim().isEmpty()) {
            key = cca2.trim().toLowerCase();
        }

        String phoneCode = cache.getFromCache(key);
        if (phoneCode == null) {
            logger.warn("No phone code found for key: {}", key);
            throw new ValidationException("Country not found.", "Please check the provided 'name' or 'cca2' parameter.");
        }

        PhoneCodeResult result = new PhoneCodeResult();
        result.setQueryParameter(key);
        result.setPhoneCode(phoneCode);
        phoneCodeResultRepository.save(result);

        logger.info("Phone code for '{}' is '{}'", key, phoneCode);

        return new PhoneCodeResponse(key, phoneCode);
    }

    public List<PhoneCodeResponse> getPhoneCodesBulk(List<PhoneCodeBulkRequest.PhoneCodeQuery> queries) {
        logger.info("Received bulk request to get phone codes for {} queries", queries.size());

        return queries.stream()
                .map(query -> {
                    String name = query.getName();
                    String cca2 = query.getCca2();
                    try {
                        return getPhoneCode(name, cca2);
                    } catch (ValidationException e) {
                        logger.warn("Validation failed for bulk query: name='{}', cca2='{}'; Error: {}", name, cca2, e.getMessage());
                        return new PhoneCodeResponse(
                                (name != null ? name : cca2),
                                "Error: " + e.getMessage()
                        );
                    } catch (Exception e) {
                        logger.error("Unexpected error for bulk query: name='{}', cca2='{}'; Error: {}", name, cca2, e.getMessage());
                        return new PhoneCodeResponse(
                                (name != null ? name : cca2),
                                "Error: An unexpected error occurred."
                        );
                    }
                })
                .collect(Collectors.toList());
    }

    public List<PhoneCodeResult> getAllPhoneCodeResults() {
        logger.info("Fetching all PhoneCodeResult entries from the database.");
        return phoneCodeResultRepository.findAll();
    }

    @Scheduled(cron = "0 0 2 * * ?")
    public void refreshCache() {
        logger.info("Scheduled task started: Refreshing PhoneCodeCache.");
        try {
            Map<String, String> tempCache = new ConcurrentHashMap<>();
            Map<String, List<String>> tempReverseCache = new ConcurrentHashMap<>();

            ResponseEntity<Country[]> response = restTemplate.getForEntity(EXTERNAL_API_URL, Country[].class);
            if (response.getStatusCode() == HttpStatus.OK && response.getBody() != null) {
                for (Country country : response.getBody()) {
                    String commonName = (country.getName() != null && country.getName().getCommon() != null)
                            ? country.getName().getCommon().toLowerCase()
                            : "unknown";
                    String cca2 = (country.getCca2() != null) ? country.getCca2().toLowerCase() : "unknown";

                    String iddRoot = (country.getIdd() != null) ? country.getIdd().getRoot() : "";
                    List<String> suffixes = (country.getIdd() != null) ? country.getIdd().getSuffixes() : new ArrayList<>();

                    String phoneCode = (iddRoot != null ? iddRoot : "") +
                            (suffixes != null && !suffixes.isEmpty() ? suffixes.get(0) : "");

                    if (!phoneCode.isEmpty()) {
                        tempCache.put(commonName, phoneCode);
                        tempCache.put(cca2, phoneCode);

                        tempReverseCache.computeIfAbsent(phoneCode, k -> new ArrayList<>()).add(commonName);
                        logger.debug("Added to tempReverseCache: phoneCode='{}', countryName='{}'", phoneCode, commonName);
                    } else {
                        logger.warn("No phone code available for country: {}", commonName);
                    }
                }

                cache.updateCaches(tempCache, tempReverseCache);
                logger.info("PhoneCodeCache refreshed successfully.");
            } else {
                logger.error("Failed to refresh data from external API. Status Code: {}", response.getStatusCode());
            }
        } catch (RestClientException e) {
            logger.error("RestClientException during cache refresh: {}", e.getMessage());
        } catch (Exception e) {
            logger.error("Exception during cache refresh: {}", e.getMessage());
        }
    }
}