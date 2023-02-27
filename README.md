# INI-Parser💘
This is a parser which can be used to parse configuration files with INI format.(INI文件解析器)     

## Basic Info💬
Structure of INI files:
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

If you're not familiar with the INI format, please refer to [INI_file](https://en.wikipedia.org/wiki/INI_file).

The parser conforms to the following format:

1. Names of section & key are both case insensitive.
2. Whitespace delimiters around sections, keys & values are all ignored.
3. Keys which do not belong to a section are ignored.
4. Each entry exists on a single line.(Multi-line is not supported.)

