#pragma once
#include "parsing/tokens/Parser.h"
#include "parsing/syntax/Parser.h"

namespace Matching
{
    class Matcher
    {
        std::size_t m_currentToken = 0;
        std::size_t m_savedToken = 0;

        std::string m_inputFile;
        Parsing::Tokens::Parser m_tokenParser;
        Parsing::Syntax::Parser m_syntaxParser;

    public:
        Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile);

        bool match();
        bool matchNext();
        bool matchPattern(const Parsing::Syntax::Pattern& pattern);
        bool matchRule(const std::string& name);
    };
}