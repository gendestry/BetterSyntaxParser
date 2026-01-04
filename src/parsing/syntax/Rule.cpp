#include "Rule.h"

#include "utils/utils.h"
#include "utils/font.h"
#include "utils/stringutils.h"
#include <print>
#include <format>
#include <sstream>


using Utils::Font;

namespace Parsing::Syntax
{
    Rule::Rule(const std::string& name, const std::string& right)
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
            std::println("{}Invalid rule: empty string!{}", Font::fred, Font::reset);
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
            std::println("{}Invalid rule: empty string!{}", Font::fred, Font::reset);
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
        std::stringstream ss;
        ss << Font::fyellow << m_name;

        if (m_return.size() > 0)
        {
            ss << " -> " << m_return;
        }

        ss << std::format(" (\n{}", Font::reset);
        for (auto &pattern : m_patterns)
        {
            ss << Utils::String::concat("  ", Font::fcyan, pattern.toString(), Font::reset, "\n");
        }

        ss << std::format("{}){}", Font::fyellow, Font::reset);
        // for(auto* ast : asts)
        // {
        //     std::println("{}", ast->toString());
        // }
        return ss.str();
    }


    void Rule::print() const
    {
        std::println("{}", toString());
    }
}