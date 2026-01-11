#include "logger.h"

namespace Utils
{
    uint32_t Logger::scopeSize = 0;
    Logger::Level Logger::s_level = Level::ERROR;

    Logger::Logger(std::string scope)
        : m_scope(std::move(scope)) 
    {
        if(scope.size() > scopeSize)
        {
            scopeSize = scope.size();
        }
        toggleScope();
    }

    Logger::Logger(Logger& other, std::string scope)
    {
        m_scope = other.m_scope + scope;
        if(scope.size() > scopeSize)
        {
            scopeSize = scope.size();
        }
        toggleScope();
    }

    std::string Logger::getScopePadding() const
    {
        // Utils::Stream s;
        // for(uint32_t i = 0; i < (9 - m_scope.size()); i++){
        //     s<< " ";
        // }

        // return s.end();
        return "";
    }

    void Logger::toggleScope()
    {
        if(usedScope.empty())
        {
            usedScope = Utils::String::concat("[", m_scope, getScopePadding(), "] ");
        }
        else
        {
            usedScope.clear();
        }
    }

    std::string Logger::scopeStr() const
    {
        return "[" + m_scope + getScopePadding() + "] ";
    }
}