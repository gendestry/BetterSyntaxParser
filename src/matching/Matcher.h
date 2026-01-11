#pragma once
#include "parsing/tokens/Parser.h"
#include "parsing/syntax/Parser.h"
#include "parsing/syntax/Ast.h"
#include <functional>
#include <list>
#include <memory>
#include "Ast.h"
#include "utils/logger.h"

namespace Matching
{
    class Matcher
    {
        std::size_t m_currentToken = 0;
        std::string m_inputFile;
        std::string m_currentRule;
        Parsing::Tokens::Parser m_tokenParser;
        Parsing::Syntax::Parser m_syntaxParser;

        mutable Utils::Logger logger;

        std::vector<std::shared_ptr<Ast>> m_tokens;

        bool isEOF() const;
        std::shared_ptr<Ast> matchStep();
        std::vector<std::shared_ptr<Ast>> matchPattern(const Parsing::Syntax::Pattern& pattern);
        std::shared_ptr<Ast> matchRule(const std::string& name);
        std::shared_ptr<Ast> matchRule2(const std::string& name);
    public:
        Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile);

        bool match();
    };
}