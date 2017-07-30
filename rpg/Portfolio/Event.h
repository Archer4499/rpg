#pragma once
#include <string>

class Event {
 public:
    std::string message;
    int health_mod;
    Event();
    explicit Event(const std::string& line);
};

std::ostream& operator<<(std::ostream& os, const Event& event);
