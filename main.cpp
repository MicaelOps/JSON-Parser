#include <iostream>
#include "minejson.h"

int main() {

    std::cout << "input string \n";
    std::string output;
    std::cin >> output;

    json_node* node = readJsonString(output);

    if(!node) {
        std::cout << "invalid json";
        return -1;
    }

    std::cout << "valid json";
    return 0;
}
