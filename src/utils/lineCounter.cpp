#include "lineCounter.h"

namespace Utils
{
    LineCounter::LineCounter()
    {}

    LineCounter::LineCounter(const std::string &input)
    {
        count(input);
    }

    void LineCounter::count(const std::string &input)
    {
        lines.clear();
        lines.push_back(0);
        for (unsigned int i = 0; i < input.size(); i++)
        {
            if (input[i] == '\n')
            {
                lines.push_back(i);
            }
        }
    }


    unsigned int LineCounter::operator[](unsigned int index)
    {
        return accumulate(index);
    }

    unsigned int LineCounter::getXOffset(unsigned int index)
    {
        unsigned int limit = accumulate(index, 0);
        return lines[limit];
    }

    unsigned int LineCounter::numLinesInBetween(unsigned int start, unsigned int end) const
    {
        return accumulate(end) - accumulate(start);
    }

    unsigned int LineCounter::accumulate(unsigned int index, unsigned int accOffset) const
    {
        unsigned int acc = 0;

        for (unsigned int i = 1; i < lines.size(); i++)
        {
            unsigned int line = lines[i];
            if (line < index)
            {
                acc++;
            }
            else if (line == index)
            {
                // acc++;
                break;
            }
        }

        return acc;
    }
};