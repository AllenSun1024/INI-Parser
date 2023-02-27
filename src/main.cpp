#include "../common/INIMap.h"
#include "../reader/INIReader.h"
#include <iostream>

int main(){

    using namespace ns_INI;
    std::string config_path = "../test_data.ini";
    INIReader reader(config_path);
    INIStructure data;
    reader>>data;
    auto items = data.get("ama_all_in_000");
    if(items.size() > 0){
       int port = items.get_int("port");
       std::cout << port << std::endl;
    }

}
