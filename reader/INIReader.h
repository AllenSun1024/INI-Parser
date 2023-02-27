//
// Created by Abner Allen on 2023/2/25.
//

#pragma once

#ifndef INI_PARSER_INIREADER_H
#define INI_PARSER_INIREADER_H


#include "../common/INIMap.h"

#include <fstream>

namespace ns_INI{

    class INIReader{

    public:
        using T_Statements = std::vector<std::string>;
        using T_StatementsPtr = std::shared_ptr<T_Statements>;
        bool isBOM = false;

        INIReader(std::string const& filename, bool keepLineData = false);
        ~INIReader()= default;

        bool operator>>(INIStructure& data);

        T_StatementsPtr getStatements();

    private:
        std::ifstream fileReadStream;
        T_StatementsPtr statements;

        // @brief 读取INI文件内容
        T_Statements readFile();

    };

}

#include "INIReaderImpl.h"

#endif //INI_PARSER_INIREADER_H
