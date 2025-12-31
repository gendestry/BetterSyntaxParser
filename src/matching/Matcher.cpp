#include "Matcher.h"
#include <print>
#include <string>
#include "utils/font.h"

namespace Matching
{
    Matcher::Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile)
        : m_inputFile(inputFile)
    {
        if(!m_tokenParser.parseDefinedTokens(tokenFile))
        {
            std::println("[TOKEN] Error");
            return;
        }
        m_tokenParser.printDefinedTokens();
        
        if(!m_tokenParser.parseTokens(inputFile))
        {
            std::println("[TOKEN] Input tokens error");
            return;
        }
        m_tokenParser.printTokens();
        

        if(!m_syntaxParser.parseSyntaxFile(syntaxFile))
        {
            std::println("[SYNTAX] Error");
            return;
        }

        if(!m_syntaxParser.validateInput(m_tokenParser))
        {
            std::println("[SYNTAX] Validate input error");
            return;
        }

        m_syntaxParser.printRules();
    }

    bool Matcher::match()
    {
        const auto& inputTokens = m_tokenParser.getTokens();
        uint8_t counter = 0;
        while(m_currentToken < inputTokens.size())
        {
            if(!matchNext())
            {
                std::println("{}Match unsuccessful{}", Utils::Font::fred, Utils::Font::reset);
                return false;
            }
            // return false;
        }

            // counter++;
            // if(counter>10)
            // {return false;}
            
        // }

        return true;
    }

    bool Matcher::matchNext()
    {
        const auto& rules = m_syntaxParser.rules();
        for(auto& rule : rules)
        {
            auto& patterns = rule.patterns();
            for(auto& pattern : patterns)
            {
                if(matchPattern(pattern))
                {
                    std::println(" for rule {}", rule.name());
                    return true;
                }
            }
        }

        return false;
    }

    bool Matcher::matchPattern(const Parsing::Syntax::Pattern& pattern)
    {
        std::size_t savedIndex = m_currentToken;
        auto& tokens = pattern.tokens();
        uint32_t matched = 0;

        for(; matched < tokens.size(); matched++)
        {
            auto& token = tokens[matched];
            auto selectedToken = m_tokenParser[m_currentToken];
            if(selectedToken.has_value() && selectedToken.value().get().name == token.token)
            {
                m_currentToken++;
            }
            else if(m_syntaxParser.isValidRule(token.token))
            {
                if(!matchRule(token.token))
                {
                    m_currentToken = savedIndex;
                    return false;
                }
            }
            else
            {
                m_currentToken = savedIndex;
                return false;
            }
        }

        // if(matched)
        std::print("Matched: ");

        for(auto& t : tokens)
        {
            std::print("{} ", t.toString());
        }

        std::println("");
        return true;
    }


    bool Matcher::matchRule(const std::string& name)
    {
        std::size_t storedIndex = m_currentToken;
        auto& patterns = m_syntaxParser[name].value().get().patterns();

        for(auto& pattern : patterns)
        {
            if(matchPattern(pattern))
            {
                return true;
            }
        }

        m_currentToken = storedIndex;
        return false;
    }

}