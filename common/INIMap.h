//
// Created by Abner Allen on 2023/2/24.
//

#ifndef INI_PARSER_INIMAP_H
#define INI_PARSER_INIMAP_H


#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>

namespace ns_INI{

    namespace ns_StringUtil{

        // @var whitespaceDelimiters: 空白符序列
        const char* const whitespaceDelimiters = " \t\n\v\r\f";

        // @brief 清除字符串两端的空白符
        // @param str: 两端可能含有空白符的字符串
        void trim(std::string& str);

        // @brief Section and property names大小写不敏感
        // @param str: 可能包含大写字母的字符串
        void toLower(std::string& str);

        // @brief 子串(全部)替换
        // @param str: 主串
        // @param a: 需被替换子串
        // @param b: 目标串
        void replace(std::string& str, std::string const& a, std::string const& b);

    }

    template<typename T>
    class INIMap {

    private:
        using T_DataIndexMap = std::unordered_map<std::string, std::size_t>;
        /*
        * pair(1): key = value
        * pair(2): sectionName = <pair(1), ...>
        */
        using T_DataItem = std::pair<std::string, T>;
        using T_DataContainer = std::vector<T_DataItem>;
        using T_MultiArgs = typename std::vector<std::pair<std::string, T>>;

        T_DataIndexMap dataIndexMap;
        T_DataContainer data;

        // @brief 设置key-index映射
        // @return 参数key得到的映射值
        // @param key: section name或者key name
        std::size_t setEmpty(std::string &key);

    public:
        using const_iterator = typename T_DataContainer::const_iterator;

        INIMap() = default;

        // @brief copy constructor
        // @param that: 被拷贝的INIMap对象
        INIMap(INIMap const &that);

        // @brief operator[] overload
        // @return value of type T
        // @param key: 可以是section name，也可以是key-value中的key
        T& operator[](std::string key);

        // @brief 获取key对应的value
        // @return 值
        // @param key: 键
        T get(std::string key) const;

        // @brief 获取key对应的value，并将value转换成字符串类型
        // 仅限INIMap<std::string>对象调用
        // 无需实现，直接调用get即可，因为INIStructure = INIMap<INIMap<std::string>>，当INIMap<std::string>对象调用时，T = std::string
        // std::string getString(std::string key) const;

        // @brief 获取key对应的value，并将value转换成int类型
        int get_int(std::string key) const;

        // @brief 获取key对应的value，并将value转换成unsigned int类型
        unsigned get_unsigned(std::string key) const;

        // @brief 获取key对应的value，并将value转换成bool类型
        bool get_bool(std::string key) const;

        // @brief 判断是否存在对应数据项
        // @return true: exist  false: not exist
        bool has(std::string key) const;

        // @brief update key
        void set(std::string key, T obj);

        // @brief update key(batch op)
        void set(T_MultiArgs const &multiArgs);

        // @brief 清空容器
        void clear();

        // @brief 返回数据个数
        std::size_t size() const;

        // @brief 返回迭代器
        const_iterator begin() const;

        const_iterator end() const;

    };


    using INIStructure = INIMap<INIMap<std::string>>;

    namespace ns_Parse{

        using T_ParseValues = std::pair<std::string, std::string>;

        // INI中出现的语句类型
        enum class PDataType: char{
            PDATA_NONE,  // 空语句行
            PDATA_COMMENT,  // 注释
            PDATA_SECTION,  // Section
            PDATA_KEYVALUE,  // 键值对
            PDATA_UNKNOWN  // 非法语句行
        };

        // @brief 分类解析INI语句行
        PDataType parseStatement(std::string statement, T_ParseValues& parseData);

    }

}

#include "INIMapImpl.h"

#endif //INI_PARSER_INIMAP_H
