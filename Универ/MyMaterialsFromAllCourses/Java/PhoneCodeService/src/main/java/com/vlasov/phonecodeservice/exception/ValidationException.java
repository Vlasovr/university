package com.vlasov.phonecodeservice.exception;

import com.vlasov.phonecodeservice.model.ErrorResponse;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(HttpStatus.BAD_REQUEST)
public class ValidationException extends ServiceException {

    public ValidationException(String message, String help) {
        super(message, help);
    }

    @Override
    public ErrorResponse getErrorResponse() {
        ErrorResponse errorResponse = new ErrorResponse();
        errorResponse.setStatus(HttpStatus.BAD_REQUEST.value());
        errorResponse.setError(HttpStatus.BAD_REQUEST.getReasonPhrase());
        errorResponse.setMessage(getMessage());
        errorResponse.setHelp(getHelp());
        return errorResponse;
    }
}