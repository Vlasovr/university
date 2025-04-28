package com.vlasov.phonecodeservice.cache;
import org.springframework.stereotype.Component;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.List;
import java.util.ArrayList;

@Component
public class PhoneCodeCache {
    private final Map<String, String> cache = new ConcurrentHashMap<>();
    private final Map<String, List<String>> reverseCache = new ConcurrentHashMap<>();

    public String getFromCache(String key) {
        return cache.get(key);
    }

    public void addToCache(String key, String value) {
        cache.put(key, value);
    }

    public void addToReverseCache(String phoneCode, String countryName) {
        reverseCache.computeIfAbsent(phoneCode, k -> new ArrayList<>()).add(countryName);
    }

    public List<String> getFromReverseCache(String phoneCode) {
        return reverseCache.get(phoneCode);
    }

    public synchronized void updateCaches(Map<String, String> newCache, Map<String, List<String>> newReverseCache) {
        cache.clear();
        cache.putAll(newCache);

        reverseCache.clear();
        reverseCache.putAll(newReverseCache);
    }
}