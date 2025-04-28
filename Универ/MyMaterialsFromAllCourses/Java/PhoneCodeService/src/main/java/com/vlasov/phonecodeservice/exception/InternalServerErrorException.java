package com.vlasov.phonecodeservice.exception;

import com.vlasov.phonecodeservice.model.ErrorResponse;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(HttpStatus.INTERNAL_SERVER_ERROR)
public class InternalServerErrorException extends ServiceException {

    public InternalServerErrorException(String message) {
        super(message, "We will try to fix this error as soon as possible.");
    }

    @Override
    public ErrorResponse getErrorResponse() {
        ErrorResponse errorResponse = new ErrorResponse();
        errorResponse.setStatus(HttpStatus.INTERNAL_SERVER_ERROR.value());
        errorResponse.setError(HttpStatus.INTERNAL_SERVER_ERROR.getReasonPhrase());
        errorResponse.setMessage(getMessage());
        errorResponse.setHelp(getHelp());
        return errorResponse;
    }
}