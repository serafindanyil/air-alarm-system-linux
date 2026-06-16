#pragma once

#include <string>

class Logging {
public:
    explicit Logging(const std::string& state);

    void print(const std::string& message) const;

private:
    std::string state_;

    static std::string getTimestamp();
};