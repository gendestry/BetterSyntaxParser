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

        m_tokenParser.print();

        for(auto& token : m_tokenParser.getTokens())
        {
            m_tokens.push_back(new AstToken(token));
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
        Ast* step;
        // while(isEOF())
        // {

            step = matchStep();
        // }

        return step != nullptr;
    }
    
    Ast* Matcher::matchStep()
    {
        // Ast* max = new Ast(rules[0].name());
        std::println("{}Match Step{}", Utils::Font::fgreen, Utils::Font::reset);

        const auto& rules = m_syntaxParser.rules();
        Ast* max = matchRule2(rules[0].name());

        // uint32_t maxMatch = 0;
        // uint32_t maxMatchTokenIndex = 0;
        // std::size_t prevToken = m_currentToken;
        // for(uint32_t i = 0; i < rules[0].patterns().size(); i++)
        // {
        //     auto& pattern = rules[0].patterns()[i];
        //     std::vector<Ast*> temp = matchPattern(pattern);
        //     if(temp.size() > 0)
        //     {
        //         auto diff = m_currentToken - prevToken;
        //         if(diff > maxMatch)
        //         {
        //             max->nodes.clear();
        //             max->nodes = temp;
        //             maxMatchTokenIndex = m_currentToken;
        //             maxMatch = diff;
        //         }
        //     }

        //     m_currentToken = prevToken;
        // }

        // std::print("CurrentToken: {}\n{}", maxMatchTokenIndex, max->toString());
        if(max)
        {
            std::println("{}Match Step Successfull, index: {}{}", Utils::Font::fgreen, m_currentToken, Utils::Font::reset);
        }
        else
        {
            std::println("{}Match Step Error, index: {}{}", Utils::Font::fred, m_currentToken, Utils::Font::reset);
        }
        return max;
    }

    std::vector<Ast*> Matcher::matchPattern(const Parsing::Syntax::Pattern& pattern)
    {
        std::println("{}Matching pattern: {}{}", Utils::Font::fwhite, pattern.toString(), Utils::Font::reset);
        // auto& tokens = pattern.tokens();
        auto& tokens = pattern.tokens();
        uint16_t index = 0;
        std::vector<Ast*> ast;
        for(; index < tokens.size(); index++)
        {
            // std::println("Index: {}, m_currentToken: {}", index, m_currentToken);
            auto patternToken = tokens[index];
            auto inToken = m_tokenParser[m_currentToken];
            if(inToken.has_value())
            {
                std::string inTokenName = inToken.value().get().name;
                if(patternToken.token == inTokenName)
                {
                    std::println("{}[T] In: {}, Pattern: {}{}", Utils::Font::fwhite, inTokenName, patternToken.token, Utils::Font::reset);
                    m_currentToken++;
                    ast.push_back(new AstToken(inToken.value()));
                }
                else if(m_syntaxParser.isValidRule(patternToken.token))
                {
                    std::println("{}[R] Index: {}, m_currentToken: {}{}", Utils::Font::fwhite, index, m_currentToken, Utils::Font::reset);
                    Ast* temp = matchRule(patternToken.token); 
                    if(!temp)
                    {
                        std::println("{}Pattern not matched: {} {}", Utils::Font::fred, pattern.toString(), Utils::Font::reset);
                        ast.clear();
                        return ast;
                    }
                    ast.push_back(temp);
                    m_currentToken = temp->tokenIndex;
                }
                else
                {
                    break;
                }
            }

        }

        if(index == tokens.size())
        {
            // std::println("Matched: {}", pattern.toString());
            std::println("{}Pattern matched: {} {}", Utils::Font::fgreen, pattern.toString(), Utils::Font::reset);
            return ast;
        }

        ast.clear();
        return ast;
    };

    Ast* Matcher::matchRule(const std::string& ruleName)
    {
        Ast* ast = new Ast(ruleName);
        // std::string oldRule = m_currentRule;
        // m_currentRule = ruleName;
        std::println("{}Matching rule: {}{}", Utils::Font::fcyan, ruleName, Utils::Font::reset);
        for(auto& pattern : m_syntaxParser[ruleName].value().get().patterns())
        {
            std::vector<Ast*> temp = matchPattern(pattern);
            if(temp.size() > 0)
            {
                ast->tokenIndex = m_currentToken;
                ast->nodes = std::move(temp);
                return ast;
            }
        }

        // m_currentRule = oldRule;
        delete ast;
        return nullptr;
    };

    Ast* Matcher::matchRule2(const std::string& name)
    {
        std::println("{}Matching rule2: {}{}", Utils::Font::fmagenta, name, Utils::Font::reset);
        const auto& rule = m_syntaxParser[name].value().get();
        Ast* max = new Ast(rule.name());

        uint32_t maxMatch = 0;
        uint32_t maxMatchTokenIndex = 0;
        std::size_t prevToken = m_currentToken;
        for(uint32_t i = 0; i < rule.patterns().size(); i++)
        {
            auto& pattern = rule.patterns()[i];
            std::vector<Ast*> temp = matchPattern(pattern);
            if(temp.size() > 0)
            {
                auto diff = m_currentToken - prevToken;
                if(diff > maxMatch)
                {
                    max->nodes.clear();
                    max->nodes = temp;
                    maxMatchTokenIndex = m_currentToken;
                    maxMatch = diff;
                }
            }

            m_currentToken = prevToken;
            temp.clear();
        }

        if(maxMatch == 0)
        {
            // std::println("{}")
            delete max;
        }
        else
        {
            max->tokenIndex = maxMatchTokenIndex;
            max->numTokens = maxMatchTokenIndex - prevToken;
            m_currentToken = maxMatchTokenIndex;
            std::print("{}Matched Rule2, currentToken: {}\n{}{}", Utils::Font::fmagenta, maxMatchTokenIndex, Utils::Font::reset, max->toString());
        }

        return max;
    }

    // bool Matcher::matchNext()
    // {
    //     const auto& inputTokens = m_tokenParser.getTokens();
    //     const auto& rules = m_syntaxParser.rules();
    //     std::size_t prevToken = m_currentToken;
        
    //     // std::println("{}[MAIN]    Matching rule2 {}", Utils::Font::forange, Utils::Font::reset);

    //     std::size_t bestMatch = 0;
    //     for(auto& rule : rules)
    //     {
    //         auto& patterns = rule.patterns();

    //         if(isEOF())
    //         {
    //             return true;
    //         }
    //         std::println("{}[MAIN]    Matching rule {}{}", Utils::Font::forange, rule.name(), Utils::Font::reset);
    //         for(auto& p : patterns)
    //         {
    //             // std::println("{}[MAIN]    -- Matching pattern {}{}", Utils::Font::forange, p.toString(), Utils::Font::reset);
    //             if(matchPattern(p))
    //             {
    //                 // std::println("{}[MAIN]    ---- Matched pattern {}{}", Utils::Font::fgreen, p.toString(), Utils::Font::reset);
    //                 // std::println("{}[MAIN]    ---- Pushed custom ast {}{}", Utils::Font::fgreen, rule.name(), Utils::Font::reset);
    //                 if(isEOF())
    //                 {
    //                     return true;
    //                 }
    //                 bestMatch = m_currentToken - prevToken;
    //                 m_currentToken = prevToken;
    //                 // return true;
    //             }
    //             else
    //             {
    //                 std::println("{}[MAIN]    ---- Pattern not matched {}{}", Utils::Font::fred, p.toString(), Utils::Font::reset);
    //                 m_currentToken = prevToken;
    //             }
    //         }
    //     }

    //     if(bestMatch)
    //     {
    //         return true;
    //     }

    //     return false;
    // }

    // bool Matcher::matchRule(const Parsing::Syntax::Rule& rule)
    // {
    //     std::string name = rule.name();
    //     std::println("{}[RULE] Match rule '{}' {}{}", Utils::Font::fmagenta, name, m_currentToken, Utils::Font::reset);
    //     std::size_t storedIndex = m_currentToken;
    //     auto& patterns = m_syntaxParser[name].value().get().patterns();

    //     for(auto& pattern : patterns)
    //     {
    //         if(matchPattern(pattern))
    //         {
    //             std::println("[RULE] -- Matched rule {}", name);
    //             return true;
    //         }
    //     }

    //     m_currentToken = storedIndex;
    //     return false;
    // }

    // bool Matcher::matchPattern(const Parsing::Syntax::Pattern& pattern)
    // {
    //     std::println("{}[PATTERN] Match pattern{} {}", Utils::Font::fmagenta, m_currentToken, pattern.toString()), Utils::Font::reset;
    //     std::size_t savedIndex = m_currentToken;
    //     auto& tokens = pattern.tokens();
    //     std::size_t offset = 0;
        
    //     uint32_t matched = 0;
    //     for(; matched < tokens.size(); matched++)
    //     {
    //         auto& token = tokens[matched];
    //         auto selectedToken = m_tokenParser[m_currentToken];
    //         std::println("{}{}[PATTERN] -- Match token '{}' {}", Utils::Font::byColorCode(90,90,90), Utils::Font::italic, token.token, Utils::Font::reset);
            
    //         if(selectedToken.has_value() && selectedToken.value().get().name == token.token)
    //         {
    //             m_currentToken++;
    //             std::println("[PATTERN] -- PREDEFINED token {}, index: {}", token.toString(), m_currentToken);
                
    //         }
    //         else if(m_syntaxParser.isValidRule(token.token))
    //         {
    //             std::println("[PATTERN] -- RULE for token {}, index {}", token.toString(), m_currentToken);
    //             if(!matchRule(token.token))
    //             {
    //                 std::println("[PATTERN] ---- Rule not matched");
    //                 m_currentToken = savedIndex;
    //                 return false;
    //             }
    //             else
    //             {
    //                 if(isEOF())
    //                 {return true;}
    //                 // Ast* first = new Ast(token.token);
    //                 // first->index = m_currentToken;
    //                 // m_tempAst.push_back(first);
    //                 // m_savedToken = first;
    //                 // m_currentToken
    //             }
    //         }
    //         else
    //         {
    //             m_currentToken = savedIndex;
    //             return false;
    //         }
    //     }

    //     // if(matched)
    //     // m_tokenList.push_back()
    //     // std::print("[PATTERN] -- Matched{}: ", m_currentToken);

    //     // for(auto& t : tokens)
    //     // {
    //     //     std::print("{} ", t.toString());
    //     // }

    //     if(isEOF())
    //     {
    //         return true;
    //     }

    //     std::println("");
    //     return true;
    // }


    // bool Matcher::matchRule(const std::string& name)
    // {
    //     std::println("{}[RULE] Match rule '{}' {}{}", Utils::Font::fmagenta, name, m_currentToken, Utils::Font::reset);
    //     std::size_t storedIndex = m_currentToken;
    //     auto& patterns = m_syntaxParser[name].value().get().patterns();

    //     for(auto& pattern : patterns)
    //     {
    //         if(matchPattern(pattern))
    //         {
    //             std::println("[RULE] -- Matched rule {}", name);
    //             return true;
    //         }
    //     }

    //     m_currentToken = storedIndex;
    //     return false;
    // }

}