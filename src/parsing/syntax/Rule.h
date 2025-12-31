#pragma once
#include <vector>
#include <string>
#include "traits/Stringify.h"
#include "Pattern.h"

namespace Parsing::Syntax
{
    class Rule : public Traits::Stringify
    {
        std::string m_name;
        std::string m_return;
        std::vector<Pattern> m_patterns;

        bool parseLeft(const std::string& str);
        bool parseRight(const std::string& str);
    public:
        Rule(const std::string& name, const std::string& right);

        [[nodiscard]] const std::string& name() const { return m_name; }
        [[nodiscard]] const std::vector<Pattern>& patterns() const { return m_patterns; } 

        [[nodiscard]] const std::string toString() const override;
        void print() const override;
    };
};