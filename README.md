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

1. Names of section & key are both **case insensitive**.😘
2. **Whitespace delimiters** around sections, keys & values are all ignored.😘
3. Keys which do not belong to a section are ignored.😘
4. **Each entry exists on a single line**.(Multi-line is not supported.)😘

## User Guide🔎

### Q1: How to create a file reader?
```cpp
ns_INI::INIReader reader("../test_data.ini");
ns_INI::INIStructure fileData;
reader >> fileData;  // parse contents in test_data.ini to fileData
```

### Q2: How to get a section?
```cpp
/* You can get a section by two ways. */
std::string sectionName = "ama_all_in_000";


// The first way: **INIMap::get()**

if(fileData.has(sectionName)){  // Make sure the section exists
  auto section = fileData.get(sectionName);
}


// The second way: **INIMap::operator[]**

if(fileData.has(sectionName)){
  auto section = fileData[sectionName];
}
```

### Q3: How to get a value by key?
```cpp
/* You can get a value by two ways. */
std::string key = "enable";

// The first way: **INIMap::get(), get_int()/get_unsigned()/get_bool() also provided**
if(section.has(key)){
  std::string val_str = section.get(key);  // val_str = "true", equivalent to: val_str = fileData.get(sectionName).get(key);
  bool val_bool = section.get_bool(key);  // val_bool = true;
}


// The second way: **INIMap::operator[]**
if(section.has(key)){
  std::string val = section[key];  // equivalent to: val = fileData[sectionName][key];
}
```

## Task List🎆
- [x] Design the data structure of INI
- [x] Write the INI reader
- [ ] Write the INI writer
- [ ] More possible features?





