#include "Parser.h"
#include "utils/utils.h"
#include "utils/font.h"
#include "utils/lineCounter.h"

using Utils::Font;


namespace Parsing::Syntax
{
    Parser::Parser() 
        : logger("SYNTAX")
    {}

    Parser::Parser(const std::string& syntax_path)
        : logger("SYNTAX")
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
            logger.error("Exception triggered in syntax parser: '{}'", e.what());
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
                logger.error("Error: Syntax file does not end with a semicolon");
                logger.error("Syntax: '{}'", testStr);
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
            logger.error("Error[{}]: Rule is not in the correct format: '{}'", position, rule);
            return false;
        }

        std::string left = Utils::removeEnclosingWhitespaces(nameAndRule[0]);
        std::string right = Utils::removeEnclosingWhitespaces(nameAndRule[1]);

        Rule ruleObj(left, right);
        const std::string& name = ruleObj.name();

        if (m_rulesMap.find(name) == m_rulesMap.end())
        {
            m_rules.push_back(ruleObj);
            m_rulesMap[name] = m_rules.size() - 1;
            m_tree[name] = new Ast(name);
        }
        else
        {
            logger.error("Rule: '{}' already exists!", ruleObj.name());
            return false;
        }

        return true;
    }

    bool Parser::validateInput(Tokens::Parser& parser)
    {
        for(auto& r : m_rules)
        {
            for(auto& p : r.patterns())
            {
                for(auto& t : p.tokens())
                {
                    if(parser.isTokenDefined(t.token))
                    {
                        if(!m_tree.contains(t.token))
                        {
                            Ast* a = new Ast(t.token);
                            a->terminated = true;
                            m_tree[t.token] = a;
                        }
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

        buildTree();
        return true;
    }

    void Parser::buildTree()
    {

        for(auto& r : m_rules)
        {
            for(auto& p : r.patterns())
            {
                if(p.tokens().size() == 1)
                {
                    auto& t = p.tokens()[0];
                    if(m_tree.contains(t.token))
                    {
                        m_tree[r.name()]->nodes.push_back(m_tree[t.token]);
                        m_tree[t.token]->references.push_back(m_tree[r.name()]);
                    }
                    continue;
                }

                Ast* temp = new Ast("temp");
                for(auto& t : p.tokens())
                {
                    if(m_tree.contains(t.token))
                    {
                        temp->nodes.push_back(m_tree[t.token]);
                    }
                }
                temp->references.push_back(m_tree[r.name()]);
                m_tree[r.name()]->nodes.push_back(temp);
            }
        }

        m_mainTree = m_tree[m_rules[0].name()];
    }

    void Parser::printAsts() const
    {
        logger.println("{}", m_mainTree->toString());
    }

    void Parser::printRules() const
    {
        logger.printlnColor(Utils::Font::fgreen, " ==== PARSED SYNTAX ==== ");
        logger.toggleScope();
        for (auto &rule : m_rules)
        {
            rule.print();
        }
        logger.println("");
        logger.toggleScope();
    }
}