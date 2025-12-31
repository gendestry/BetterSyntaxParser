#include "utils.h"
#include <fstream>
#include <utility>

namespace Utils
{
    std::string getFileString(std::string path)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            throw std::runtime_error("Error: file '" + path + "' not found");
            return "";
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));

        file.close();
        return content;
    }

    StringVec getFileStringVec(std::string path)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            throw std::runtime_error("Error: file '" + path + "' not found");
            return {};
        }

        StringVec content;
        std::string line;

        while (std::getline(file, line))
        {
            content.push_back(line);
        }

        file.close();
        return content;
    }

    std::string sanitizeString(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = 0;
        while ((p = sanitized.find('\n', p)) != std::string::npos)
        {
            sanitized.replace(p, 1, "\\n");
            p += 2; // Move past the inserted "\\n"
        }

        return sanitized;
    }

    std::string sanitizeWhitespace(const std::string &str)
    {
        std::string sanitized = str;
        const std::vector<char> whitespace = {' ', '\t', '\n'};

        size_t p = 0;
        size_t r = 0;
        while ((p = sanitized.find_first_of(" \t\n", p)) != std::string::npos)
        {
            r = sanitized.find_first_not_of(" \t\n", p);

            if (r == std::string::npos)
                break;

            if (r - p > 1)
            {
                sanitized = sanitized.replace(p, r - p, " ");
            }

            p += 1;
        }

        return sanitized;
    }

    std::string replaceNewlineBySpace(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = 0;
        while ((p = sanitized.find('\n', p)) != std::string::npos)
        {
            sanitized.replace(p, 1, " ");
            p += 1; // Move past the inserted " "
        }

        return sanitized;
    }

    std::string removeNewline(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = 0;
        while ((p = sanitized.find('\n', p)) != std::string::npos)
        {
            sanitized.replace(p, 1, "");
            p += 0; // Move past the inserted "\\n"
        }

        return sanitized;
    }

    std::string removeWhitespaces(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = 0;
        while ((p = sanitized.find_first_of(" \t\n", p)) != std::string::npos)
        {
            sanitized.replace(p, 1, "");
            p += 0; // Move past the inserted "\\n"
        }

        return sanitized;
    }

    // the input can be "\n" or if you want all whitespace: " \t\n"
    std::string removeByInput(const std::string &str, const std::string &input)
    {
        std::string sanitized = str;

        size_t p = 0;
        while ((p = sanitized.find_first_of(input, p)) != std::string::npos)
        {
            sanitized.replace(p, 1, "");
            p += 0; // Move past the inserted "\\n"
        }

        return sanitized;
    }

    std::string removeLeadingWhitespace(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = 0;
        while (sanitized[p] == ' ' || sanitized[p] == '\t')
        {
            sanitized.replace(p, 1, "");
        }

        return sanitized;
    }

    std::string removeTrailingWhitespace(const std::string &str)
    {
        std::string sanitized = str;

        size_t p = sanitized.length() - 1;
        while (sanitized[p] == ' ' || sanitized[p] == '\t')
        {
            sanitized.replace(p, 1, "");
            p--;
        }

        return sanitized;
    }

    std::string removeEnclosingWhitespaces(const std::string &str)
    {
        return removeLeadingWhitespace(removeTrailingWhitespace(str));
    }

    StringVec split(const std::string &str, const std::string &delim)
    {
        StringVec tokens;
        size_t prev = 0, pos = 0;

        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos)
                pos = str.length();

            std::string token = str.substr(prev, pos - prev);
            if (!token.empty())
                tokens.push_back(token);

            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());

        return tokens;
    }

    StringVec splitByWhitespace(const std::string &str)
    {
        StringVec tokens;
        size_t prev = 0, pos = 0;

        do
        {
            pos = str.find_first_of(" \t", prev);
            if (pos == std::string::npos)
                pos = str.length();

            std::string token = str.substr(prev, pos - prev);
            if (!token.empty())
                tokens.push_back(token);

            prev = pos + 1;
        } while (pos < str.length() && prev < str.length());

        return tokens;
    }

};
