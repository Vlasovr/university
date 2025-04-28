package com.vlasov.phonecodeservice.model;

public class PhoneCodeResponse {
    private String query;
    private String result;

    public PhoneCodeResponse(String query, String result) {
        this.query = query;
        this.result = result;
    }

    public String getQuery() {
        return query;
    }

    public void setQuery(String query) {
        this.query = query;
    }

    public String getResult() {
        return result;
    }

    public void setResult(String result) {
        this.result = result;
    }
}