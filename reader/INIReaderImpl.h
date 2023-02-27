//
// Created by Abner Allen on 2023/2/25.
//

#ifndef INI_PARSER_INIREADERIMPL_H
#define INI_PARSER_INIREADERIMPL_H

#include "INIReader.h"

namespace ns_INI{

    INIReader::INIReader(const std::string &filename, bool keepLineData){
        fileReadStream.open(filename, std::ios::in | std::ios::binary);
        if(keepLineData){
            statements = std::make_shared<T_Statements>();
        }
    }

    INIReader::T_Statements INIReader::readFile(){
        fileReadStream.seekg(0, std::ios::end);
        const std::size_t fileSize = static_cast<std::size_t>(fileReadStream.tellg());
        fileReadStream.seekg(0, std::ios::beg);
        if(fileSize >= 3){
            const char header[] = {
                    static_cast<char>(fileReadStream.get()),
                    static_cast<char>(fileReadStream.get()),
                    static_cast<char>(fileReadStream.get())
            };
            isBOM = (
                    header[0] == static_cast<char>(0xEF) &&
                    header[1] == static_cast<char>(0xBB) &&
                    header[2] == static_cast<char>(0xBF)
            );
        }
        else{
            isBOM = false;
        }
        std::string fileContents;
        fileContents.resize(fileSize);
        fileReadStream.seekg(isBOM ? 3 : 0, std::ios::beg);
        fileReadStream.read(&fileContents[0], fileSize);  // 文件流入字符串
        fileReadStream.close();

        T_Statements output;
        if(0 == fileSize){
            return output;
        }
        std::string buffer;
        buffer.reserve(128);  // 如果自己的INI最大语句长度超过100，需要重置该值
        for(std::size_t i = 0; i < fileSize; i++){
            char& c = fileContents[i];
            if('\n' == c){
                output.emplace_back(buffer);
                buffer.clear();
                continue;
            }
            if('\0' != c && '\r' != c){
                buffer += c;
            }
        }
        output.emplace_back(buffer);
        return output;
    }

    bool INIReader::operator>>(INIStructure &data){
        if(!fileReadStream.is_open()){
            return false;
        }
        T_Statements fileLines = readFile();
        std::string section;
        bool inSection = false;
        ns_Parse::T_ParseValues parseData;
        for(auto const& line: fileLines){
            auto parseResult = ns_Parse::parseStatement(line, parseData);
            if(ns_Parse::PDataType::PDATA_SECTION == parseResult){
                inSection = true;
                data[section = parseData.first];
            }
            else if(inSection && (ns_Parse::PDataType::PDATA_KEYVALUE == parseResult)){
                auto const& key = parseData.first;
                auto const& value = parseData.second;
                data[section][key] = value;
            }
            if(statements && (ns_Parse::PDataType::PDATA_UNKNOWN != parseResult)){
                if((ns_Parse::PDataType::PDATA_KEYVALUE == parseResult) && !inSection){
                    continue;
                }
                statements->emplace_back(line);
            }
        }
        return true;
    }

    INIReader::T_StatementsPtr  INIReader::getStatements(){
        return statements;
    }

}

#endif //INI_PARSER_INIREADERIMPL_H
