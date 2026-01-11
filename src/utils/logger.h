#pragma once
#include <format>
#include <print>
#include <string>
#include "font.h"
#include "stringutils.h"
#include <cstdint>

namespace Utils
{
    class Logger
    {
    public:
        enum Level : uint8_t
        {
            NOTSET,
            DEBUGGING,
            INFO,
            ERROR,
        };
    private:
        static uint32_t scopeSize;
        static Level s_level;
        Level m_level = NOTSET;
        std::string m_scope;
        mutable std::string usedScope;
        
        std::string getScopePadding() const;

    public:
        explicit Logger(std::string scope)
            : m_scope(std::move(scope)) 
        {
            if(scope.size() > scopeSize)
            {
                scopeSize = scope.size();
            }
            toggleScope();
        }

        explicit Logger(Logger& other, std::string scope)
        {
            m_scope = other.m_scope + scope;
            if(scope.size() > scopeSize)
            {
                scopeSize = scope.size();
            }
            toggleScope();
        }

        static void setLevel(Level level)
        {
            s_level = level;
        }

        void setLoggerLevel(Level level)
        {
            m_level = level;
        }

        void toggleScope()
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

        std::string scopeStr() const
        {
            return "[" + m_scope + getScopePadding() + "] ";
        }

        template<typename... Args>
        void print(const std::string& format, Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::print("{}{}", usedScope, msg);
        }

        template<typename... Args>
        void println(const std::string& format, Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}", usedScope, msg);
        }

        template<typename... Args>
        void printColor(const std::string& color,
                        const std::string& format,
                        Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::print("{}{}{}{}", usedScope, color, msg, Font::reset);
        }

        template<typename... Args>
        void printlnColor(const std::string& color,
                          const std::string& format,
                          Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}{}{}", usedScope, color, msg, Font::reset);
        }

        template<typename... Args>
        void debug(const std::string& format,
                    Args&&... args)
        {
            Level level = m_level == NOTSET ? s_level : m_level;
            if(level <= Level::DEBUGGING)
            {
                auto msg = std::vformat(format, std::make_format_args(args...));
                std::println("{}{}{}{}{}", Utils::Font::italic, Utils::Font::byColorCode(140,140,140), usedScope, msg, Font::reset);
            }
        }

        template<typename... Args>
        void error(const std::string& format,
                    Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}{}{}", Utils::Font::fred, usedScope, msg, Font::reset);
        }
    };
}

