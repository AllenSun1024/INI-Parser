//
// Created by Abner Allen on 2023/2/25.
//

#ifndef INI_PARSER_INIMAPIMPL_H
#define INI_PARSER_INIMAPIMPL_H

#include "INIMap.h"

namespace ns_INI::ns_StringUtil{

    void trim(std::string& str){
        str.erase(str.find_last_not_of(whitespaceDelimiters) + 1);
        str.erase(0, str.find_first_not_of(whitespaceDelimiters));
    }

    void toLower(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), [](const char c){
            return static_cast<char>(std::tolower(c));
        });
    }

    void replace(std::string& str, std::string const& a, std::string const& b){
        if(!a.empty()){
            std::size_t pos = 0;
            while(std::string::npos != (pos = str.find(a, pos))){
                str.replace(pos, a.size(), b);
                pos += b.size();
            }
        }
    }

}

namespace ns_INI{

    template<typename T>
    std::size_t INIMap<T>::setEmpty(std::string &key){
        std::size_t index = data.size();
        dataIndexMap[key] = index;  // key is the index-th one to join the container
        data.emplace_back(key, T());
        return index;
    }

    template<typename T>
    INIMap<T>::INIMap(const INIMap<T> &that){
        std::size_t data_size = that.data.size();
        for(std::size_t i = 0; i < data_size; i++){
            auto const& key = that.data[i].first;
            auto const& obj = that.data[i].second; // obj可以是key对应的value，也可以是section对应的key-value集合
            data.emplace_back(key, obj);
        }
        dataIndexMap = T_DataIndexMap(that.dataIndexMap);
    }

    template<typename T>
    T& INIMap<T>::operator[](std::string key){
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        std::size_t index = (dataIndexMap.end() == it) ? setEmpty(key) : it->second;
        return data[index].second;
    }

    template<typename T>
    T INIMap<T>::get(std::string key) const{
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        if(dataIndexMap.end() == it){
            return T();
        }
        return T(data[it->second].second);
    }

    template<typename T>
    int INIMap<T>::get_int(std::string key) const {
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        if(dataIndexMap.end() == it)
            return INT_MAX;
        std::string value = data[it->second].second;
        return std::stoi(value);
    }

    template<typename T>
    unsigned INIMap<T>::get_unsigned(std::string key) const {
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        if(dataIndexMap.end() == it)
            return UINT_MAX;
        std::string value = data[it->second].second;
        return std::stoul(value);
    }

    template<typename T>
    bool INIMap<T>::get_bool(std::string key) const {
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        if(dataIndexMap.end() == it)
            return false;
        std::string value = data[it->second].second;
        return 0 == value.compare("true");
    }

    template<typename T>
    bool INIMap<T>::has(std::string key) const{
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        return (dataIndexMap.count(key) == 1);
    }

    template<typename T>
    void INIMap<T>::set(std::string key, T obj){
        ns_StringUtil::trim(key);
        ns_StringUtil::toLower(key);
        auto it = dataIndexMap.find(key);
        if(dataIndexMap.end() != it){ // 在map中，更新key的值
            data[it->second].second = obj;
        }
        else{ // 不在map中，添加key-value对
            dataIndexMap[key] = data.size();
            data.emplace_back(key, obj);
        }
    }

    template<typename T>
    void INIMap<T>::set(INIMap::T_MultiArgs const& multiArgs){
        for(auto const& it: multiArgs){
            auto const& key = it->first;
            auto const& obj = it->second;
            set(key, obj);
        }
    }

    template<typename T>
    void INIMap<T>::clear(){
        data.clear();
        dataIndexMap.clear();
    }

    template<typename T>
    std::size_t INIMap<T>::size() const{
        return data.size();
    }

    template<typename T>
    typename INIMap<T>::const_iterator INIMap<T>::begin() const{
        return data.begin();
    }

    template<typename T>
    typename INIMap<T>::const_iterator INIMap<T>::end() const{
        return data.end();
    }

}

namespace ns_INI::ns_Parse{

    PDataType parseStatement(std::string statement, T_ParseValues& parseData){
        parseData.first.clear();
        parseData.second.clear();
        ns_StringUtil::trim(statement);  // 不能调用toLower，因为value对大小写敏感

        // 空语句
        if(statement.empty()){
            return PDataType::PDATA_NONE;
        }

        char firstCharacter = statement[0];

        // 注释
        if(';' == firstCharacter){
            return PDataType::PDATA_COMMENT;
        }

        // Section
        if('[' == firstCharacter){
            // 切掉可能跟在Section后面的注释
            auto commentAt = statement.find_first_of(';');
            if(std::string::npos != commentAt){
                statement = statement.substr(0, commentAt);
            }

            auto closingBracketAt = statement.find_last_of(']');
            if(std::string::npos != closingBracketAt){
                auto section = statement.substr(1, closingBracketAt - 1);  // 切掉首尾的[]
                ns_StringUtil::toLower(section);  // section name大小写不敏感
                parseData.first = section;  // second是key-value pairs，需要进一步解析
                return PDataType::PDATA_SECTION;
            }
        }

        // key-value pair
        auto statementNorm = statement;
        ns_StringUtil::replace(statementNorm, "\\=", "  "); // 处理转义字符
        auto equalsAt = statementNorm.find_first_of('=');
        if(std::string::npos != equalsAt){
            auto key = statement.substr(0, equalsAt);
            ns_StringUtil::trim(key);
            ns_StringUtil::replace(key, "\\=", "=");
            auto value = statement.substr(equalsAt + 1);
            ns_StringUtil::trim(value);
            parseData.first = key;
            parseData.second = value;
            return PDataType::PDATA_KEYVALUE;
        }

        // 非法语句
        return PDataType::PDATA_UNKNOWN;

    }

}


#endif //INI_PARSER_INIMAPIMPL_H
