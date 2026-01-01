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

        std::println("Here");

        // m_tokenParser.print();
        std::println("Here");
        
        // for(auto& token : m_tokenParser.getTokens())
        // {
        //     m_tokenList.push_back(token);
        // }
        

        if(!m_syntaxParser.parseSyntaxFile(syntaxFile))
        {
            std::println("[SYNTAX] Error");
            return;
        }
        std::println("Here");

        if(!m_syntaxParser.validateInput(m_tokenParser))
        {
            std::println("[SYNTAX] Validate input error");
            return;
        }
        std::println("Here");

        // m_rules = m_syntaxParser.rules();
        m_syntaxParser.printRules();
    }


    bool Matcher::match()
    {
        std::println("{}[MAIN]    Matching rule1 {}", Utils::Font::forange, Utils::Font::reset);
     
        const auto& inputTokens = m_tokenParser.getTokens();
        const auto& rules = m_syntaxParser.rules();
        auto isEOF = [this, &inputTokens]()
        {
            return this->m_currentToken == inputTokens.size();
        };


        std::size_t prevToken = m_currentToken;

        uint8_t counter = 0;
        std::println("{}[MAIN]    Matching rule1 {}", Utils::Font::forange, Utils::Font::reset);

        while(!isEOF())
        {
                std::println("{}[MAIN]    Matching rule2 {}", Utils::Font::forange, Utils::Font::reset);

            for(auto& rule : rules)
            {
                auto& patterns = rule.patterns();

                std::println("{}[MAIN]    Matching rule {}{}", Utils::Font::forange, rule.name(), Utils::Font::reset);
                for(auto& p : patterns)
                {
                    std::println("{}[MAIN]    -- Matching pattern {}{}", Utils::Font::forange, p.toString(), Utils::Font::reset);
                    if(matchPattern(p))
                    {
                        std::println("{}[MAIN]    ---- Matched pattern {}{}", Utils::Font::fgreen, p.toString(), Utils::Font::reset);
                        if(isEOF())
                        {
                            if(m_savedToken)
                            {
                                std::println("{}", m_savedToken->toString());
                            }
                            return true;
                        }
                        else
                        {
                            std::println("{}[MAIN]    ---- Pushed custom ast {}{}", Utils::Font::fgreen, rule.name(), Utils::Font::reset);
                            // ast->name = "expr";
                            if(m_savedToken)
                            {
                                Ast* ast = new Ast("expr");
                                ast->nodes.push_back(m_savedToken);
                                
                                for(auto& ast : m_currentAstNodes)
                                {
                                    ast->nodes.push_back(ast);
                                }
                                m_currentAstNodes.clear();
                                m_savedToken = ast;
                                std::println("-{}", m_savedToken->toString());

                            //     // if(m_tempAst.size() != 0)
                            //     // {
                            //     //     ast->index = m_tempAst.back()->index + m_currentToken - prevToken;
                            //     // }
                            //     // else
                            //     // {
                            //     //     ast->index = m_savedToken->index + m_currentToken - prevToken;
                            //     // }

                            //     // m_tempAst.push_back(ast);
                            }
                            else
                            {
                                Ast* first = new Ast(inputTokens[m_currentToken - 1].name);
                                first->index = m_currentToken;
                                m_savedToken = first;
                                std::println("+{}", m_savedToken->toString());

                            }
    
                            std::println("{}", m_savedToken->toString());
    
                            m_currentToken = prevToken;
                        }
                    }
                    else
                    {
                        std::println("{}[MAIN]    ---- Pattern not matched {}{}", Utils::Font::fred, p.toString(), Utils::Font::reset);
                        m_currentToken = prevToken;
                    }
                }
            }

        if(counter ++ > 10)
        {return false;}

        }
    

        if(m_savedToken)
        {
            std::println("{}", m_savedToken->toString());
        }

        return false;
    }

    bool Matcher::matchRule(const Parsing::Syntax::Rule& rule)
    {
        std::println("{}[RULE] Match rule '{}' {}", Utils::Font::fmagenta ,rule.name(), m_currentToken);
        std::size_t storedIndex = m_currentToken;
        auto& patterns = rule.patterns();

        for(auto& pattern : patterns)
        {
            if(matchPattern(pattern))
            {
                std::println("[RULE] -- Matched rule {}", rule.name());
                return true;
            }
        }

        m_currentToken = storedIndex;
        return false;
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
            
            // if(m_savedToken)
            {
            //     if(offset == 0)
            //     {
            //         if(token.token == m_savedToken->name)
            //         {
            //             std::println("[PATTERN] -- Matching custom token{} {}", m_currentToken, token.token);
            //             // m_savedToken++;
            //             matched += m_savedToken->index;
            //             continue;
            //         }
            //     }
            // }
                // if(offset == 0)
                // {

            // if(token.token == m_savedToken->name)
            // {
                // std::println("[PATTERN] -- Matching custom token{} {}", m_currentToken, token.token);
                // m_currentAstNodes.push_back(new Ast(token.token));
                // m_savedToken++;
                // matched += m_savedToken->index;
                // continue;
            // }
        // }
                // else
                {

                }

                offset++;
            }
            if(selectedToken.has_value() && selectedToken.value().get().name == token.token)
            {
                m_currentToken++;
                std::println("[PATTERN] -- Matched predefined token {}, index: {}", token.toString(), m_currentToken);
                
            }
            else if(m_syntaxParser.isValidRule(token.token))
            {
                std::println("[PATTERN] -- Matching rule for token {}, index {}", token.toString(), m_currentToken);
                if(!matchRule(token.token))
                {
                    std::println("[PATTERN] ---- Rule not matched");
                    m_currentToken = savedIndex;
                    return false;
                }
                else
                {
                    // Ast* first = new Ast(token.token);
                    // first->index = m_currentToken;
                    // m_tempAst.push_back(first);
                    // m_savedToken = first;
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
        std::print("[PATTERN] -- Matched{}: ", m_currentToken);

        for(auto& t : tokens)
        {
            std::print("{} ", t.toString());
        }

        std::println("");
        return true;
    }


    bool Matcher::matchRule(const std::string& name)
    {
        std::println("{}[RULE] Match rule '{}' {}{}", Utils::Font::fmagenta, name, m_currentToken, Utils::Font::reset);
        std::size_t storedIndex = m_currentToken;
        auto& patterns = m_syntaxParser[name].value().get().patterns();

        // if(m_savedToken< m_savedTokens.size())
        // {

        //     // std::println("[PATTERN] -- Matching rule for token {}, index {}", token.toString(), m_currentToken);
        //     if(m_savedTokens[m_savedToken].name == name)
        //     {
                
        //     }
        // }
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