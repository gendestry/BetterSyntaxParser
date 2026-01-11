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
        
        uint32_t m_padOffset = 0;
        Level m_level = NOTSET;
        std::string m_scope;
        
        mutable std::string usedScope;
        
        std::string getScopePadding() const;
        std::string scopePadding() const
        {
            return usedScope + Utils::Stream::pad(m_padOffset, "  ");
        }

    public:
        explicit Logger(std::string scope);
        explicit Logger(Logger& other, std::string scope);

        static void setLevel(Level level)
        {
            s_level = level;
        }

        void setLoggerLevel(Level level)
        {
            m_level = level;
        }

        void incPadOffset()
        {
            m_padOffset++;
        }

        void decPadOffset()
        {
            m_padOffset--;
        }

        void toggleScope();
        std::string scopeStr() const;

        template<typename... Args>
        void print(const std::string& format, Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::print("{}{}", scopePadding(), msg);
        }

        template<typename... Args>
        void println(const std::string& format, Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}", scopePadding(), msg);
        }

        template<typename... Args>
        void printColor(const std::string& color,
                        const std::string& format,
                        Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::print("{}{}{}{}", scopePadding(), color, msg, Font::reset);
        }

        template<typename... Args>
        void printlnColor(const std::string& color,
                          const std::string& format,
                          Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}{}{}", scopePadding(), color, msg, Font::reset);
        }

        template<typename... Args>
        void debug(const std::string& format,
                    Args&&... args)
        {
            Level level = m_level == NOTSET ? s_level : m_level;
            if(level <= Level::DEBUGGING)
            {
                auto msg = std::vformat(format, std::make_format_args(args...));
                std::println("{}{}{}{}{}", Utils::Font::italic, Utils::Font::byColorCode(140,140,140), scopePadding(), msg, Font::reset);
            }
        }

        template<typename... Args>
        void error(const std::string& format,
                    Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            std::println("{}{}{}{}", Utils::Font::fred, scopePadding(), msg, Font::reset);
        }
    };
}

