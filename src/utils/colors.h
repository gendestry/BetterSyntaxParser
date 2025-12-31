#pragma once
#include <string>
#include "font.h"

namespace Color
{

    std::string txt(std::string &text)
    {
        return Font::fyellow + text + Font::reset;
    }
};