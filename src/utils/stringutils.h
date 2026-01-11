#pragma once
#include <sstream>
#include <format>
#include <print>

namespace Utils
{
    class String 
    { 
        static void concat(std::stringstream& ss) { } 
        
        template<typename T, typename... Args> 
        static void concat(std::stringstream& ss, T&& value, Args&&... args) 
        { 
            ss << std::forward<T>(value); 
            concat(ss, std::forward<Args>(args)...); 
        } 
    public: 
        template<typename... Args> 
        static std::string concat(Args&&... args) 
        { 
            std::stringstream ss; 
            concat(ss, std::forward<Args>(args)...); 
            return ss.str(); 
        } 

        template<typename... Args>
        static std::string format(const std::string& format, Args&&... args)
        {
            return std::format(format, std::forward<Args>(args)...);
        }
    };

    class Stream
    {
        std::stringstream ss;
    public:
        Stream& operator<<(auto s)
        {
            ss << s;
            return *this;
        }

        template<typename... Args>
        void add(Args&&... args)
        {
            ss << String::concat(std::forward<Args>(args)...);
        }

        std::string end()
        {
            std::string temp = ss.str();
            ss.clear();
            return temp;
        }

        template<typename... Args>
        void addFormatted(std::string format, Args&&... args)
        {
            auto msg = std::vformat(format, std::make_format_args(args...));
            ss << msg;
        }

        static std::string pad(uint32_t n, std::string by)
        {
            Stream s;
            for(uint32_t i = 0; i < n; i++)
            {
                s << by;
            }

            return s.end();
        }
    };

    // class Logger
    // {

    // public:
    //     template<typename... Args>
    //     static void print(const std::string& format, Args&&... args)
    //     {
    //         std::print(format, std::forward<Args>(args)...);
    //     }

    //     template<typename... Args>
    //     static void println(const std::string& format, Args&&... args)
    //     {
    //         std::println(format, args...);
    //     }
    // };
}
