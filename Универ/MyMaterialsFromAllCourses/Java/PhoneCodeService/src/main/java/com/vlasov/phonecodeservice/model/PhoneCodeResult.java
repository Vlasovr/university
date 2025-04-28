package com.vlasov.phonecodeservice.model;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;

@Entity
public class PhoneCodeResult {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String queryParameter;
    private String phoneCode;

    public Long getId() {
        return id;
    }

    public String getQueryParameter() {
        return queryParameter;
    }

    public String getPhoneCode() {
        return phoneCode;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public void setQueryParameter(String queryParameter) {
        this.queryParameter = queryParameter;
    }

    public void setPhoneCode(String phoneCode) {
        this.phoneCode = phoneCode;
    }
}