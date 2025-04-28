package com.vlasov.phonecodeservice.model;

import java.util.List;

public class PhoneCodeBulkRequest {
    private List<PhoneCodeQuery> queries;

    public List<PhoneCodeQuery> getQueries() {
        return queries;
    }

    public void setQueries(List<PhoneCodeQuery> queries) {
        this.queries = queries;
    }

    public static class PhoneCodeQuery {
        private String name;
        private String cca2;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getCca2() {
            return cca2;
        }

        public void setCca2(String cca2) {
            this.cca2 = cca2;
        }
    }
}
