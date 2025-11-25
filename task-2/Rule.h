#ifndef RULE_H
#define RULE_H
#include <string>
#include <set>

class Rule {
public:
    Rule(const std::string& ruleStr = "B3/S23");
    void setRule(const std::string& ruleStr);
    bool shouldBorn(int neighbors) const;
    bool shouldSurvive(int neighbors) const;
    std::string toString() const;

private:
    std::set<int> birth;
    std::set<int> survival;
};

#endif
