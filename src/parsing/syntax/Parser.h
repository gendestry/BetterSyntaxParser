#pragma once
#include "Rule.h"
#include <vector>
#include <unordered_map>
#include <cstdint>

#include "parsing/tokens/Parser.h"

namespace Parsing::Syntax
{
    class Parser
    {
        std::vector<Rule> m_rules;
        std::unordered_map<std::string, std::size_t> m_rulesMap;

        bool parseSyntaxRule(const std::string &rule, uint32_t position);
        
    public:
        Parser() {}
        Parser(const std::string& syntax_path);
        
        bool parseSyntaxFile(const std::string& inputFilePath);
        bool validateInput(Tokens::Parser& parser);

        [[nodiscard]] bool isValidRule(const std::string& name) const { return m_rulesMap.contains(name); }
        [[nodiscard]] std::optional<std::reference_wrapper<Rule>> operator[](const std::string& name)
        {
            if(isValidRule(name))
            {
                return m_rules[m_rulesMap[name]];
            }

            return std::optional<std::reference_wrapper<Rule>>();
        }
        [[nodiscard]] const std::vector<Rule>& rules() const { return m_rules; }

        void printRules() const;
    };
}