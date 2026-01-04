#pragma once
#include "Rule.h"
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "Ast.h"

#include "parsing/tokens/Parser.h"

namespace Parsing::Syntax
{
    class Parser
    {
        std::vector<Rule> m_rules;
        std::unordered_map<std::string, std::size_t> m_rulesMap;
        std::unordered_map<std::string, Ast*> m_tree;
        Ast* m_mainTree;

        bool parseSyntaxRule(const std::string &rule, uint32_t position); 
        void buildTree();       
    public:
        Parser() {}
        Parser(const std::string& syntax_path);
        
        bool parseSyntaxFile(const std::string& inputFilePath);
        bool validateInput(Tokens::Parser& parser);

        Ast* getAstRoot() const
        { return m_mainTree; }

        Ast* getNode(const std::string& name)
        {
            if(m_tree.contains(name))
            {
                return m_tree[name];
            }

            return nullptr;
        }

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

        void printAsts() const;
        void printRules() const;
    };
}