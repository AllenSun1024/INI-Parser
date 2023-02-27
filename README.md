# INI-Parser
This is a parser which can be used to parse configuration files with ini format.  
这是一个可以解析ini格式配置文件的解析器。  

## Directory Structure 目录结构
INI_Parser:  
 |  
 |--common  
 |   
 |--|--INIMap.h 定义INI数据结构    
 |    
 |--|--INIMapImpl.h  
 |  
 |--reader  
 |    
 |--|--INIReader.h  读取INI文件  
 |    
 |--|--INIReaderImpl.h  
 |  
 |--src  
 |    
 |--|--main.cpp  
 |  
 |--test_data.ini  ini配置文件  
 |  
 |--CMakeLists.txt  

## Instructions 使用说明
1. INIStructure = INIMap<INIMap<std::string>>可以映射到下面的ini文件结构：    
  INI_File< INI_Section< Key(string):Value(string) > >
2. INIStructure file_obj.get("section_name")的返回值 => INIMap<std::string>
3. INIMap<std::string> section_obj调用不同get接口的返回值:   
  section_obj.get("key") => value(std::string),  section_obj.get_int("key") => value(int)
4. Names of Sections & Keys are case insensitive.   
  Section名和键名是大小写不敏感的。
