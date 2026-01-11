#include "Rule.h"
#include "utils/utils.h"
#include "utils/stringutils.h"

using Utils::Font;

namespace Parsing::Syntax
{
    Rule::Rule(const std::string& name, const std::string& right)
        : logger("SYNTAX")
    {
        if(!parseLeft(name))
        {}
        if(!parseRight(right))
        {}
    };

    bool Rule::parseLeft(const std::string& str)
    {
        if (str.size() == 0)
        {
            logger.error("Invalid rule: empty string!");
            return false;
        }

        size_t pos = 0;
        if ((pos = str.find("[")) != std::string::npos)
        {
            m_name = Utils::removeEnclosingWhitespaces(str.substr(0, pos));
            m_return = Utils::removeEnclosingWhitespaces(str.substr(pos + 1, str.size() - pos - 2));
        }
        else
        {
            m_name = Utils::removeEnclosingWhitespaces(str);
            m_return = "";
        }

        return true;
    }

    bool Rule::parseRight(const std::string& str)
    {
        if (str.size() == 0)
        {
            logger.error("Invalid rule: empty string!");
            return false;
        }

        Utils::StringVec patterns = Utils::split(str, "|");

        for (auto &pattern : patterns)
        {
            // Ast* ast = new Ast(m_name);
            m_patterns.push_back(Utils::removeEnclosingWhitespaces(pattern));
            // for(auto& t : m_patterns.back().tokens())
            // {
            //     ast->nodes.push_back(new Ast(t.token));
            // }

            // asts.push_back(ast);
        }

        return true;
    }

    const std::string Rule::toString() const
    {
        Utils::Stream ss;
        ss << logger.scopeStr() << Font::fyellow << m_name;

        if (m_return.size() > 0)
        {
            ss << " -> " << m_return;
        }

        ss.addFormatted(" (\n{}", Font::reset);
        for (auto &pattern : m_patterns)
        {
            ss << logger.scopeStr() << Utils::String::concat("  ", Font::fcyan, pattern.toString(), Font::reset, "\n");
        }

        ss.addFormatted("{}{}){}", logger.scopeStr(), Font::fyellow, Font::reset);
        return ss.end();
    }


    void Rule::print() const
    {
        logger.toggleScope();
        logger.println("{}", toString());
        logger.toggleScope();
    }
}