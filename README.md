# ❤️‍🔥INI-Parser
This is a parser which can be used to parse configuration files with INI format.(INI文件解析器)     

## 💬Instructions
Structure of INI files(INI文件结构):
```
;this is a comment
[section]
key = value
```

The above file structure can be mapped to the following data structures:
```cpp
key: std::string
value: std::string  // value's type can also be converted to int/unsigned/bool.
section: INIMap<std::string>  // vector< pair< string, string >, ... >
file: INIMap<INIMap<std::string>>  // vector< section, ... >
```

