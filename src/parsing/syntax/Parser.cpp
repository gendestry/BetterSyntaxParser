#include "Parser.h"
#include "utils/utils.h"
#include "utils/font.h"
#include "utils/lineCounter.h"

#include <print>

using Utils::Font;


namespace Parsing::Syntax
{
    Parser::Parser(const std::string& syntax_path)
    {
        parseSyntaxFile(syntax_path);
    };

    bool Parser::parseSyntaxFile(const std::string& inputFilePath)
    {
        std::string input;

        try
        {
            input = Utils::getFileString(inputFilePath);
        }
        catch (const std::exception &e)
        {
            std::println("{}Exception triggered in syntax parser: '{}'{}", Font::fred, e.what(), Font::reset);
            // std::cerr << e.what() << '\n';
            return false;
        }

        std::string syntaxStr = Utils::getFileString(inputFilePath);
        if (syntaxStr.size() < 2)
            return false;

        // check if we have trailing ';;'
        {
            std::string testStr = Utils::removeWhitespaces(syntaxStr);
            size_t size = testStr.size();

            // print teststr and size
            if (testStr[size - 1] != ';' && testStr[size - 2] != ';')
            {
                // std::println
                std::println("{}Error: Syntax file does not end with a semicolon{}", Font::fred, Font::reset);
                std::println("{}Syntax: '{}'{}", Font::fred, testStr, Font::reset);
                // std::cout << Utils::Font::fred << "Error: Syntax file does not end with a semicolon" << Utils::Font::reset << std::endl;
                // std::cout << Utils::Font::fred << "Syntax: '" << testStr << "'" << Utils::Font::reset << std::endl;
                return false;
            }
        }

        Utils::StringVec rules = Utils::split(syntaxStr, ";;");
        Utils::LineCounter counter(syntaxStr);

        // print rules
        unsigned int lineIndex = 0;
        for (auto &rule : rules)
        {
            if (rule.empty() || Utils::removeWhitespaces(rule).empty())
                continue;

            bool res = parseSyntaxRule(rule, (counter[lineIndex] + 2));
            if(!res)
            {
                return false;
            }

            lineIndex += rule.size();
        }
        return true;
    }

    bool Parser::parseSyntaxRule(const std::string &rule, uint32_t position)
    {
        std::string rulePart = Utils::replaceNewlineBySpace(rule);
        std::string rulePartSanitized = Utils::removeEnclosingWhitespaces(Utils::sanitizeWhitespace(rulePart));
        // std::println("{}{} Sanitized rule: [{}]{}", Font::italic, Font::byColorCode(90, 90, 90), rulePartSanitized, Font::reset);

        Utils::StringVec nameAndRule = Utils::split(rulePartSanitized, "->");
        if (nameAndRule.size() != 2)
        {
            std::println("{}Error[{}]: Rule is not in the correct format: '{}'{}", Font::fred, position, rule, Font::reset);
            return false;
        }

        std::string left = Utils::removeEnclosingWhitespaces(nameAndRule[0]);
        std::string right = Utils::removeEnclosingWhitespaces(nameAndRule[1]);

        Rule ruleObj(left, right);

        if (m_rulesMap.find(ruleObj.name()) == m_rulesMap.end())
        {
            m_rules.push_back(ruleObj);
            m_rulesMap[ruleObj.name()] = m_rules.size() - 1;
            // m_asts[ruleObj.name()] = new Ast(ruleObj.name());
        }
        else
        {
            std::println("{}Rule: '{}' already exists!{}", Font::fred, ruleObj.name(), Font::reset);
            return false;
        }

        return true;
    }

    bool Parser::validateInput(Tokens::Parser& parser)
    {
        for(auto& r : m_rules)
        {
            // current = origi
            for(auto& p : r.patterns())
            {
                // Ast* temp = new Ast(); 
                for(auto& t : p.tokens())
                {
                    if(parser.isTokenDefined(t.token))
                    {
                        continue;
                    }
                    else if(m_rulesMap.contains(t.token))
                    {
                        continue;
                    }

                    return false;
                }   
            }
        }   
        return true;
    }

    void Parser::printAsts() const
    {
        for(auto&[key, value] : m_asts)
        {
            std::println("{}", value->toString());
            // for()
        }
    }

    void Parser::printRules() const
    {
        // print size
        std::println("\n{} ==== PARSED SYNTAX ==== {}", Font::fgreen, Font::reset);
        // std::cout << "\n"
        //           << Font::fgreen << " ==== PARSED SYNTAX ==== \n"
        //           << Font::reset;
        for (auto &rule : m_rules)
        {
            rule.print();
        }
        
    }
}