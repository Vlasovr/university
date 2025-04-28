package com.vlasov.phonecodeservice.controller;
import com.vlasov.phonecodeservice.model.PhoneCodeBulkRequest;
import com.vlasov.phonecodeservice.model.PhoneCodeResult;
import com.vlasov.phonecodeservice.service.PhoneCodeService;
import com.vlasov.phonecodeservice.service.RequestCounterService;
import com.vlasov.phonecodeservice.model.PhoneCodeResponse;
import com.vlasov.phonecodeservice.exception.ServiceException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@RestController
@Validated
public class PhoneCodeController {
    private final PhoneCodeService phoneCodeService;
    private final RequestCounterService requestCounterService;
    private static final Logger log = LoggerFactory.getLogger(PhoneCodeController.class);

    @Autowired
    public PhoneCodeController(PhoneCodeService phoneCodeService, RequestCounterService requestCounterService) {
        this.phoneCodeService = phoneCodeService;
        this.requestCounterService = requestCounterService;
    }

    @GetMapping("/phonecode")
    public ResponseEntity<PhoneCodeResponse> getPhoneCode(
            @RequestParam(required = false) String name,
            @RequestParam(required = false) String cca2) throws ServiceException {
        log.info("GET Request. Total request count: {}", requestCounterService.increment());
        PhoneCodeResponse response = phoneCodeService.getPhoneCode(name, cca2);
        return ResponseEntity.ok(response);
    }

    @GetMapping("/country")
    public ResponseEntity<PhoneCodeResponse> getCountryByPhoneCode(
            @RequestParam String phoneCode) throws ServiceException {
        log.info("GET Request for country by phone code. Total request count: {}", requestCounterService.increment());
        PhoneCodeResponse response = phoneCodeService.getCountryByPhoneCode(phoneCode);
        return ResponseEntity.ok(response);
    }

    @PostMapping("/phonecode/bulk")
    public ResponseEntity<List<PhoneCodeResponse>> getPhoneCodesBulk(
            @RequestBody PhoneCodeBulkRequest bulkRequest) throws ServiceException {
        log.info("POST Bulk Request. Total request count: {}", requestCounterService.increment());
        List<PhoneCodeResponse> responses = phoneCodeService.getPhoneCodesBulk(bulkRequest.getQueries());
        return ResponseEntity.ok(responses);
    }


    @GetMapping("/phonecode/db")
    public ResponseEntity<List<PhoneCodeResult>> getAllPhoneCodeResults() {
        log.info("GET Request to /phonecode/db. Total request count: {}", requestCounterService.increment());
        List<PhoneCodeResult> results = phoneCodeService.getAllPhoneCodeResults();
        return ResponseEntity.ok(results);
    }
}