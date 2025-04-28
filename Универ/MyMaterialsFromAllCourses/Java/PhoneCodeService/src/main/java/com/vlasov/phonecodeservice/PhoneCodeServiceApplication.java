package com.vlasov.phonecodeservice;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling // Включает поддержку планировщиков
public class PhoneCodeServiceApplication {
    public static void main(String[] args) {
        SpringApplication.run(PhoneCodeServiceApplication.class, args);
    }
}
