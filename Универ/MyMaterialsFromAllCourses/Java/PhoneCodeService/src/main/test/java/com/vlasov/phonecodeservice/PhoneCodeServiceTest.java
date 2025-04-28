package com.vlasov.phonecodeservice;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

import com.vlasov.phonecodeservice.cache.PhoneCodeCache;
import com.vlasov.phonecodeservice.exception.InternalServerErrorException;
import com.vlasov.phonecodeservice.exception.ValidationException;
import com.vlasov.phonecodeservice.model.Country;
import com.vlasov.phonecodeservice.model.Country.Idd;
import com.vlasov.phonecodeservice.model.Country.Name;
import com.vlasov.phonecodeservice.model.PhoneCodeResponse;
import com.vlasov.phonecodeservice.model.PhoneCodeResult;
import com.vlasov.phonecodeservice.repository.PhoneCodeResultRepository;

import com.vlasov.phonecodeservice.service.PhoneCodeService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.client.RestTemplate;

import java.util.List;

public class PhoneCodeServiceTest {

    private RestTemplate restTemplate;
    private PhoneCodeCache cache;
    private PhoneCodeResultRepository repository;
    private PhoneCodeService service;

    @BeforeEach
    void setUp() {
        restTemplate = mock(RestTemplate.class);
        cache = new PhoneCodeCache();
        repository = mock(PhoneCodeResultRepository.class);

        Country mockCountry = new Country();
        Name mockName = new Name();
        mockName.setCommon("France");
        mockCountry.setName(mockName);
        mockCountry.setCca2("FR");
        Idd mockIdd = new Idd();
        mockIdd.setRoot("+3");
        mockIdd.setSuffixes(List.of("3"));
        mockCountry.setIdd(mockIdd);

        ResponseEntity<Country[]> response = new ResponseEntity<>(new Country[]{mockCountry}, HttpStatus.OK);
        when(restTemplate.getForEntity(anyString(), eq(Country[].class))).thenReturn(response);

        service = new PhoneCodeService(restTemplate, cache, repository);
    }

    @Test
    void testGetPhoneCodeByNameSuccess() {
        PhoneCodeResponse response = service.getPhoneCode("France", null);
        assertNotNull(response);
        assertEquals("france", response.getQuery());
        assertEquals("+33", response.getResult());

        ArgumentCaptor<PhoneCodeResult> captor = ArgumentCaptor.forClass(PhoneCodeResult.class);
        verify(repository).save(captor.capture());
        PhoneCodeResult saved = captor.getValue();
        assertEquals("france", saved.getQueryParameter());
        assertEquals("+33", saved.getPhoneCode());
    }

    @Test
    void testGetPhoneCodeByCca2Success() {
        PhoneCodeResponse response = service.getPhoneCode(null, "FR");
        assertNotNull(response);
        assertEquals("fr", response.getQuery());
        assertEquals("+33", response.getResult());
    }

    @Test
    void testGetPhoneCodeMissingBothParameters() {
        ValidationException ex = assertThrows(ValidationException.class, () -> {
            service.getPhoneCode(null, null);
        });
        assertTrue(ex.getMessage().contains("Either 'name' or 'cca2' parameter must be provided"));
    }

    @Test
    void testGetCountryByPhoneCodeSuccess() {
        PhoneCodeResponse response = service.getCountryByPhoneCode("+33");
        assertNotNull(response);
        assertEquals("+33", response.getQuery());
        assertEquals("france", response.getResult());
    }

    @Test
    void testGetCountryByPhoneCodeNotFound() {
        ValidationException ex = assertThrows(ValidationException.class, () -> {
            service.getCountryByPhoneCode("+99");
        });
        assertTrue(ex.getMessage().contains("No countries found"));
    }

    @Test
    void testGetCountryByPhoneCodeEmptyParameter() {
        ValidationException ex = assertThrows(ValidationException.class, () -> {
            service.getCountryByPhoneCode("");
        });
        assertTrue(ex.getMessage().contains("must be provided"));
    }

    @Test
    void testLoadDataFromApiFailure() {
        RestTemplate failingTemplate = mock(RestTemplate.class);
        when(failingTemplate.getForEntity(anyString(), eq(Country[].class))).thenThrow(new RuntimeException("API failure"));

        assertThrows(InternalServerErrorException.class, () -> {
            new PhoneCodeService(failingTemplate, cache, repository);
        });
    }
}