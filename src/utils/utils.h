#pragma once
#include <string>
#include <vector>

namespace Utils
{
    using StringVec = std::vector<std::string>;

    std::string getFileString(std::string path);
    StringVec getFileStringVec(std::string path);

    // removes newlines and replaces them with '\n'
    std::string sanitizeString(const std::string &str);

    // replaces all whitespace with a single space
    std::string sanitizeWhitespace(const std::string &str);
    std::string replaceNewlineBySpace(const std::string &str);

    // removes all newlines from str (replaces them with "")
    std::string removeNewline(const std::string &str);
    std::string removeWhitespaces(const std::string &str);

    // removes all whitespace from str (replaces them with "")
    // example inputs: " " or "\t\n "
    std::string removeByInput(const std::string &str, const std::string &input);

    std::string removeLeadingWhitespace(const std::string &str);
    std::string removeTrailingWhitespace(const std::string &str);
    std::string removeEnclosingWhitespaces(const std::string &str);

    StringVec split(const std::string &str, const std::string &delim);
    StringVec splitByWhitespace(const std::string &str);

    // template <typename T, typename U>
    // std::pair<T, U> splitVecByString(const std::vector<std::string> &vec, const std::string &delim)
    // {
    //     for (auto it = vec.begin(); it != vec.end(); it++)
    //     {
    //         if (*it == str)
    //         {
    //             vecStr patterns(vec.begin(), it);
    //             vecStr tokens(it + 1, vec.end());
    //             return {patterns, tokens};
    //         }
    //     }

    //     return {};
    // }
};
