package com.vlasov.phonecodeservice.exception;

import com.vlasov.phonecodeservice.model.ErrorResponse;

public abstract class ServiceException extends RuntimeException {
    private String help;

    public ServiceException(String message, String help) {
        super(message);
        this.help = help;
    }

    public String getHelp() {
        return help;
    }

    public void setHelp(String help) {
        this.help = help;
    }

    public abstract ErrorResponse getErrorResponse();
}