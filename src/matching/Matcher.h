#pragma once
#include "parsing/tokens/Parser.h"
#include "parsing/syntax/Parser.h"
#include "parsing/syntax/Ast.h"
#include <functional>
#include <list>
#include "Ast.h"

namespace Matching
{
    class Matcher
    {
        std::size_t m_currentToken = 0;
        std::string m_inputFile;
        std::string m_currentRule;
        Parsing::Tokens::Parser m_tokenParser;
        Parsing::Syntax::Parser m_syntaxParser;

        std::vector<Ast*> m_tokens;

        // std::vector<Parsing::Syntax::Ast*> m_parsedAst;

        // std::unordered_map<std::string, Ast*> m_astNodes;

        // Ast* m_savedToken;
        // std::vector<Ast*> m_currentAstNodes;

        // bool iterateOverRule(const Parsing::Syntax::Rule& rule, std::function<bool(const Parsing::Syntax::Rule& rule)> func)

        bool isEOF() const;
        Ast* matchStep();
        std::vector<Ast*> matchPattern(const Parsing::Syntax::Pattern& pattern);
        // Ast* matchPattern(const Parsing::Syntax::Pattern& pattern);
        Ast* matchRule(const std::string& name);
        Ast* matchRule2(const std::string& name);
    public:
        Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile);

        bool match();
    };
}