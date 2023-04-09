//#include "Config.h"
#include "TEMap.h"
#include <string>
#include <iostream>
#include <string_view>

void pass(const char* x) {
    std::cout << x << std::endl;
}   

int main() {
	//Config config{"rest.config"};
    //pass(std::string("test"));
    TEMap map;
    map.insert<int>("abc", 1);
    map.insert<std::string>("qaz", "j12j3");

    int a = map.get<int>("abc");
    std::string s = map.get<std::string>("qaz");
	return 0;
}
