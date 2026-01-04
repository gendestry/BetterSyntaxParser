// #include "Matcher.h"
// #include <print>
// #include <string>
// #include "utils/font.h"

// namespace Matching
// {
//     Matcher::Matcher(const std::string& tokenFile, const std::string& syntaxFile, const std::string& inputFile)
//         : m_inputFile(inputFile)
//     {
//         if(!m_tokenParser.parseDefinedTokens(tokenFile))
//         {
//             std::println("[TOKEN] Error");
//             return;
//         }
        
//         if(!m_tokenParser.parseTokens(inputFile))
//         {
//             std::println("[TOKEN] Input tokens error");
//             return;
//         }

//         m_tokenParser.print();
        

//         if(!m_syntaxParser.parseSyntaxFile(syntaxFile))
//         {
//             std::println("[SYNTAX] Error");
//             return;
//         }

//         if(!m_syntaxParser.validateInput(m_tokenParser))
//         {
//             std::println("[SYNTAX] Validate input error");
//             return;
//         }

//         m_syntaxParser.printRules();
//     }

//     bool Matcher::match()
//     {
//         const auto& inputTokens = m_tokenParser.getTokens();
//         uint8_t counter = 0;
//         while(m_currentToken < inputTokens.size())
//         {
//             if(!matchNext())
//             {
//                 std::println("{}Match unsuccessful{}", Utils::Font::fred, Utils::Font::reset);
//                 return false;
//             }

//             if(m_currentToken != inputTokens.size() -1)
//                 return false;
//         }

//             // counter++;
//             // if(counter>10)
//             // {return false;}
            
//         // }

//         return true;
//     }

//     bool Matcher::matchNext()
//     {
//         const auto& rules = m_syntaxParser.rules();
//         for(auto& rule : rules)
//         {
//             auto& patterns = rule.patterns();
//             for(auto& pattern : patterns)
//             {
//                 if(matchPattern(pattern))
//                 {
//                     std::println(" for rule {}", rule.name());
//                     return true;
//                 }
//             }
//         }

//         return false;
//     }

//     bool Matcher::matchPattern(const Parsing::Syntax::Pattern& pattern)
//     {
//         std::size_t savedIndex = m_currentToken;
//         auto& tokens = pattern.tokens();
//         uint32_t matched = 0;

//         for(; matched < tokens.size(); matched++)
//         {
//             auto& token = tokens[matched];
//             auto selectedToken = m_tokenParser[m_currentToken];
//             if(selectedToken.has_value() && selectedToken.value().get().name == token.token)
//             {
//                 m_currentToken++;
//             }
//             else if(m_syntaxParser.isValidRule(token.token))
//             {
//                 if(!matchRule(token.token))
//                 {
//                     m_currentToken = savedIndex;
//                     return false;
//                 }
//             }
//             else
//             {
//                 m_currentToken = savedIndex;
//                 return false;
//             }
//         }

//         // if(matched)
//         std::print("Matched: ");

//         for(auto& t : tokens)
//         {
//             std::print("{} ", t.toString());
//         }

//         std::println("");
//         return true;
//     }


//     bool Matcher::matchRule(const std::string& name)
//     {
//         std::size_t storedIndex = m_currentToken;
//         auto& patterns = m_syntaxParser[name].value().get().patterns();

//         for(auto& pattern : patterns)
//         {
//             if(matchPattern(pattern))
//             {
//                 return true;
//             }
//         }

//         m_currentToken = storedIndex;
//         return false;
//     }

// }

#include "Matcher.h"
#include <print>
#include <string>
#include "utils/font.h"
#include "utils/stringutils.h"
#include <algorithm>


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
        
        if(!m_tokenParser.parseTokens(inputFile))
        {
            std::println("[TOKEN] Input tokens error");
            return;
        }


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
        m_syntaxParser.printAsts();
    }

    bool Matcher::isEOF() const
    {
        return m_currentToken >= m_tokenParser.getTokens().size() - 1;
    }


    bool Matcher::match()
    {
        // std::println("{}[MAIN]    Matching rule1 {}", Utils::Font::forange, Utils::Font::reset);
        const auto& inputTokens = m_tokenParser.getTokens();
        uint8_t counter = 0;

        for(auto& token : inputTokens)
        {
            m_parsedAst.push_back(m_syntaxParser.getNode(token.name));
        }

        // Parsing::Syntax::Ast* root = m_syntaxParser.getAstRoot();
        // root->match(m_parsedAst, m_currentToken);

        while(!isEOF())
        {
            if(!matchNext())
            {
                std::println("ISSUE");
                return false;
            }
            if(counter ++ > 10)
            {return false;}

        }

        return true;
    }

    bool Matcher::matchNext()
    {
        const auto& inputTokens = m_tokenParser.getTokens();
        const auto& rules = m_syntaxParser.rules();
        std::size_t prevToken = m_currentToken;
        
        // std::println("{}[MAIN]    Matching rule2 {}", Utils::Font::forange, Utils::Font::reset);

        std::size_t bestMatch = 0;
        for(auto& rule : rules)
        {
            auto& patterns = rule.patterns();

            if(isEOF())
            {
                return true;
            }
            std::println("{}[MAIN]    Matching rule {}{}", Utils::Font::forange, rule.name(), Utils::Font::reset);
            for(auto& p : patterns)
            {
                // std::println("{}[MAIN]    -- Matching pattern {}{}", Utils::Font::forange, p.toString(), Utils::Font::reset);
                if(matchPattern(p))
                {
                    // std::println("{}[MAIN]    ---- Matched pattern {}{}", Utils::Font::fgreen, p.toString(), Utils::Font::reset);
                    // std::println("{}[MAIN]    ---- Pushed custom ast {}{}", Utils::Font::fgreen, rule.name(), Utils::Font::reset);
                    if(isEOF())
                    {
                        return true;
                    }
                    bestMatch = m_currentToken - prevToken;
                    m_currentToken = prevToken;
                    // return true;
                }
                else
                {
                    std::println("{}[MAIN]    ---- Pattern not matched {}{}", Utils::Font::fred, p.toString(), Utils::Font::reset);
                    m_currentToken = prevToken;
                }
            }
        }

        if(bestMatch)
        {
            return true;
        }

        return false;
    }

    bool Matcher::matchRule(const Parsing::Syntax::Rule& rule)
    {
        return matchRule(rule.name());
    }

    bool Matcher::matchPattern(const Parsing::Syntax::Pattern& pattern)
    {
        std::println("{}[PATTERN] Match pattern{} {}", Utils::Font::fmagenta, m_currentToken, pattern.toString()), Utils::Font::reset;
        std::size_t savedIndex = m_currentToken;
        auto& tokens = pattern.tokens();
        std::size_t offset = 0;
        
        uint32_t matched = 0;
        for(; matched < tokens.size(); matched++)
        {
            auto& token = tokens[matched];
            auto selectedToken = m_tokenParser[m_currentToken];
            std::println("{}{}[PATTERN] -- Match token '{}' {}", Utils::Font::byColorCode(90,90,90), Utils::Font::italic, token.token, Utils::Font::reset);
            
            if(selectedToken.has_value() && selectedToken.value().get().name == token.token)
            {
                m_currentToken++;
                std::println("[PATTERN] -- PREDEFINED token {}, index: {}", token.toString(), m_currentToken);
                
            }
            else if(m_syntaxParser.isValidRule(token.token))
            {
                std::println("[PATTERN] -- RULE for token {}, index {}", token.toString(), m_currentToken);
                if(!matchRule(token.token))
                {
                    std::println("[PATTERN] ---- Rule not matched");
                    m_currentToken = savedIndex;
                    return false;
                }
                else
                {
                    if(isEOF())
                    {return true;}
                    // Ast* first = new Ast(token.token);
                    // first->index = m_currentToken;
                    // m_tempAst.push_back(first);
                    // m_savedToken = first;
                    // m_currentToken
                }
            }
            else
            {
                m_currentToken = savedIndex;
                return false;
            }
        }

        // if(matched)
        // m_tokenList.push_back()
        // std::print("[PATTERN] -- Matched{}: ", m_currentToken);

        // for(auto& t : tokens)
        // {
        //     std::print("{} ", t.toString());
        // }

        if(isEOF())
        {
            return true;
        }

        std::println("");
        return true;
    }


    bool Matcher::matchRule(const std::string& name)
    {
        std::println("{}[RULE] Match rule '{}' {}{}", Utils::Font::fmagenta, name, m_currentToken, Utils::Font::reset);
        std::size_t storedIndex = m_currentToken;
        auto& patterns = m_syntaxParser[name].value().get().patterns();

        for(auto& pattern : patterns)
        {
            if(matchPattern(pattern))
            {
                std::println("[RULE] -- Matched rule {}", name);
                return true;
            }
        }

        m_currentToken = storedIndex;
        return false;
    }

}