#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Student {
    std::string id;
    std::string name;
    int score;
};

int main() {
    SetConsoleOutputCP(65001);

    std::ifstream file("data/students.json");
    if (!file.is_open()) {
        std::cout << "错误：无法打开 data/students.json" << std::endl;
        return 1;
    }

    std::string content, line;
    while (std::getline(file, line)) content += line + "\n";
    file.close();

    if (content.find_first_not_of(" \t\r\n") == std::string::npos) {
        std::cout << "错误：文件是空的" << std::endl;
        return 1;
    }

    json data;
    try {
        data = json::parse(content);
    } catch (const json::parse_error& e) {
        std::cout << "错误：JSON 格式不正确" << std::endl;
        std::cout << "  详情：" << e.what() << std::endl;
        return 1;
    }

    if (!data.is_array()) {
        std::cout << "错误：JSON 顶层必须是数组" << std::endl;
        return 1;
    }

    std::vector<Student> students;
    int skipped = 0;
    for (const auto& s : data) {
        if (!s.is_object()) { ++skipped; continue; }
        if (!s.contains("id") || !s["id"].is_string()) { ++skipped; continue; }
        if (!s.contains("name") || !s["name"].is_string()) { ++skipped; continue; }
        if (!s.contains("score") || !s["score"].is_number()) { ++skipped; continue; }

        Student stu;
        stu.id    = s["id"].get<std::string>();
        stu.name  = s["name"].get<std::string>();
        stu.score = s["score"].get<int>();
        students.push_back(stu);
    }

    if (students.empty()) {
        std::cout << "错误：没有有效的学生记录" << std::endl;
        return 1;
    }

    int sum = 0;
    int max_score = students[0].score;
    int min_score = students[0].score;
    int pass_count = 0;
    for (const auto& stu : students) {
        sum += stu.score;
        if (stu.score > max_score) max_score = stu.score;
        if (stu.score < min_score) min_score = stu.score;
        if (stu.score >= 60) ++pass_count;
    }
    double average = static_cast<double>(sum) / students.size();

    std::cout << "已加载 " << students.size() << " 名学生，跳过 " << skipped << " 条无效数据" << std::endl;

    // ===== 交互菜单 =====
    while (true) {
        std::cout << std::endl;
        std::cout << "===== 菜单 =====" << std::endl;
        std::cout << "  1. 查询总体数据" << std::endl;
        std::cout << "  2. 按学号查询学生" << std::endl;
        std::cout << "  0. 退出" << std::endl;
        std::cout << "请输入选项：";

        std::string choice;
        if (!std::getline(std::cin, choice)) break;

        if (choice == "0") {
            std::cout << "已退出。" << std::endl;
            break;
        } else if (choice == "1") {
            std::cout << std::endl << "===== 总体数据 =====" << std::endl;
            std::cout << "  学生人数   " << students.size() << std::endl;
            std::cout << "  平均分     " << average << std::endl;
            std::cout << "  最高分     " << max_score << std::endl;
            std::cout << "  最低分     " << min_score << std::endl;
            std::cout << "  及格人数   " << pass_count << std::endl;
        } else if (choice == "2") {
            while (true) {
                std::cout << std::endl << "请输入要查询学生的学号（输入 -1 返回上一页面）：";
                std::string target;
                if (!std::getline(std::cin, target)) break;
                if (target == "-1") break;

                bool found = false;
                for (const auto& stu : students) {
                    if (stu.id == target) {
                        std::cout << "  姓名：" << stu.name << std::endl;
                        std::cout << "  分数：" << stu.score << std::endl;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "  未找到学号为 " << target << " 的学生" << std::endl;
                }
            }
        } else {
            std::cout << "无效选项，请输入 0、1 或 2" << std::endl;
        }
    }
    return 0;
}