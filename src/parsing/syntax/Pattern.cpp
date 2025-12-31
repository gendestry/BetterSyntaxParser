#include "Pattern.h"
#include "utils/utils.h"
#include <sstream>
#include <algorithm>

namespace Parsing::Syntax
{

    Pattern::Pattern(const std::string& pattern)
    {
        parseTokens(pattern);
    }
    
    void Pattern::parseTokens(const std::string& pattern)
    {
        Utils::StringVec vec = Utils::split(pattern, " ");
        for(auto s : vec)
        {
            Token t{s, false};
            if(t.token.find("+") != t.token.npos)
            {
                t.token = s.substr(0, s.size() - 1);
                t.modifier = true;
            }
            m_tokens.push_back(t);
        }
    }

    const std::string Pattern::toString() const
    { 
        std::stringstream ss;
        std::for_each(m_tokens.begin(), m_tokens.end(), [&ss](const Token& t) 
        {
            ss << t.token;
            if(t.modifier)
            {
                ss << "+";
            }
            ss << " ";
        });

        return ss.str();
    }
}