#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("data/students.json");
    if (!file.is_open()) {
        std::cout << "ERROR: cannot open data/students.json" << std::endl;
        return 1;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    std::cout << "Read " << content.size() << " chars" << std::endl;
    std::cout << "Content:" << std::endl;
    std::cout << content << std::endl;
    return 0;
}
