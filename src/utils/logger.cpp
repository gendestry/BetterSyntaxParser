#include "logger.h"

namespace Utils
{
    uint32_t Logger::scopeSize = 0;
    Logger::Level Logger::s_level = Level::ERROR;

    std::string Logger::getScopePadding() const
    {
        // Utils::Stream s;
        // for(uint32_t i = 0; i < (9 - m_scope.size()); i++){
        //     s<< " ";
        // }

        // return s.end();
        return "";
    }
}