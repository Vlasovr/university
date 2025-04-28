package com.vlasov.phonecodeservice.service;
import org.springframework.stereotype.Service;
import java.util.concurrent.atomic.AtomicInteger;


@Service
public class RequestCounterService {
    private final AtomicInteger count = new AtomicInteger(0);

    public int increment() {
        return count.incrementAndGet();
    }

    public int getCount() {
        return count.get();
    }
}