#pragma once
#include "parsing/tokens/Parser.h"
#include "parsing/syntax/Parser.h"
#include "Ast.h"
#include <functional>
#include <list>

namespace Matching
{
    class Matcher
    {
        std::size_t m_currentToken = 0;
        std::string m_inputFile;
        Parsing::Tokens::Parser m_tokenParser;
        Parsing::Syntax::Parser m_syntaxParser;

        Ast* m_savedToken;
        std::vector<Ast*> m_currentAstNodes;

        // bool iterateOverRule(const Parsing::Syntax::Rule& rule, std::function<bool(const Parsing::Syntax::Rule& rule)> func)

    public:
        Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile);

        bool match();
        bool matchNext();
        bool matchPattern(const Parsing::Syntax::Pattern& pattern);
        bool matchRule(const std::string& name);
        bool matchRule(const Parsing::Syntax::Rule& rule);
    };
}