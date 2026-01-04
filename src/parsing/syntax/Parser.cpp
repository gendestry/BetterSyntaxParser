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
        const std::string& name = ruleObj.name();

        if (m_rulesMap.find(name) == m_rulesMap.end())
        {
            m_rules.push_back(ruleObj);
            m_rulesMap[name] = m_rules.size() - 1;
            m_tree[name] = new Ast(name);
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
        // for(auto& t : parser.)
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
                    // if(m_definedTokensTree.contains(t.token))
                    // {
                        // m_tree[r.name()]->nodes.push_back(m_definedTokensTree[t.token]);
                    // }
                    if(m_tree.contains(t.token))
                    {
                        m_tree[r.name()]->nodes.push_back(m_tree[t.token]);
                        m_tree[t.token]->references.push_back(m_tree[r.name()]);
                        // temp->nodes.push_back(m_tree[t.token]);
                    }
                    continue;
                }

                Ast* temp = new Ast("temp");
                // Ast* temp = new Ast(); 
                for(auto& t : p.tokens())
                {
                    // if(m_definedTokensTree.contains(t.token))
                    // {
                        // temp->nodes.push_back(m_definedTokensTree[t.token]);
                    // }
                    if(m_tree.contains(t.token))
                    {
                        temp->nodes.push_back(m_tree[t.token]);
                        // m_tree[r.name()]->references.push_back(temp);
                    }
                }
                temp->references.push_back(m_tree[r.name()]);
                m_tree[r.name()]->nodes.push_back(temp);
            }
        }

        m_mainTree = m_tree[m_rules[0].name()];

        // std::println("{}", m_tree["artihExpr"]->nodes[0]->nodes[0]->nodes.size());
    }

    void Parser::printAsts() const
    {
        std::println("{}", m_mainTree->toString());
        // return;
        // for(auto[key, value] : m_tree)
        // {
        //     std::println("{}", value->toString());
        //     // for()
        // }
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