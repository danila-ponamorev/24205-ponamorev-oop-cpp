#include "Rule.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

Rule::Rule(const std::string& ruleStr) {
    setRule(ruleStr);
}

void Rule::setRule(const std::string& ruleStr) {
    birth.clear();
    survival.clear();
    
    size_t b_pos = ruleStr.find('B');
    size_t s_pos = ruleStr.find('S');
    
    if (b_pos == std::string::npos || s_pos == std::string::npos) {
        throw std::invalid_argument("Invalid rule format");
    }

    for (size_t i = b_pos + 1; i < s_pos - 1; ++i) {
        if (isdigit(ruleStr[i])) {
            birth.insert(ruleStr[i] - '0');
        }
    }

    for (size_t i = s_pos + 1; i < ruleStr.size(); ++i) {
        if (isdigit(ruleStr[i])) {
            survival.insert(ruleStr[i] - '0');
        }
    }
}

bool Rule::shouldBorn(int neighbors) const {
    return birth.find(neighbors) != birth.end();
}

bool Rule::shouldSurvive(int neighbors) const {
    return survival.find(neighbors) != survival.end();
}

std::string Rule::toString() const {
    std::stringstream ss;
    ss << "B";
    for (int n : birth) ss << n;
    ss << "/S";
    for (int n : survival) ss << n;
    return ss.str();
}
