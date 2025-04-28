#pragma once
#include <stdexcept>
class InputError : public std::exception {
public:
    explicit InputError(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};