#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

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

    json data;
    try{
        data = json::parse(content);
    } catch (const json::parse_error& e){
        std::cout << "ERROR: invalid JSON format" << std::endl;
        std::cout << " detail:" << e.what() << std::endl;
        return 1;
    }

    if(!data.is_array()){
        std::cout << "ERROR: top-level JSON must be an array" << std::endl;
        return 1;
    }

    int valid = 0;
    int skipped = 0;

    for(const auto& s : data){
        if(!s.is_object()){
            std::cout << "WARN: skip - record is not an object" << std::endl;
            ++skipped;
            continue;
        }
        if(!s.contains("id") || !s["id"].is_string()){
            std::cout << "WARN: skip - missing or invalid id" << std::endl;
            ++skipped;
            continue;
        }
        if(!s.contains("name") || !s["name"].is_string()){
            std::cout << "WARN: skip " << s["id"].get<std::string>()
                      << " - missing or invalid name" << std::endl;
            ++skipped;
            continue;
        }
        if (!s.contains("score") || !s["score"].is_number()) {
            std::cout << "WARN: skip " << s["id"].get<std::string>()
                      << " - missing or invalid score" << std::endl;
            ++skipped;
            continue;
        }

        ++valid;
        std::cout << "  ok: " << s["id"].get<std::string>()
                  << "  " << s["name"].get<std::string>()
                  << "  " << s["score"].get<int>() << std::endl;
    }

    std::cout << "Loaded " << valid << " valid, skipped " << skipped << std::endl;

    if (valid == 0) {
        std::cout << "ERROR: no valid student records" << std::endl;
        return 1;
    }
    return 0;
}
