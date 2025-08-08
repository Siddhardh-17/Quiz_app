#include "quiz.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

std::map<std::string, std::string> parseFormData() {
    std::map<std::string, std::string> formData;
    std::string input;
    std::getline(std::cin, input);
    std::istringstream ss(input);
    std::string kv;
    while (std::getline(ss, kv, '&')) {
        auto eq = kv.find('=');
        if (eq != std::string::npos) {
            formData[kv.substr(0, eq)] = kv.substr(eq + 1);
        }
    }
    return formData;
}

int main() {
    std::cout << "Content-type: text/html\r\n\r\n";
    std::map<std::string, std::string> answers = parseFormData();
    std::string category = answers["category"];

    // debug
    std::ofstream dbg("/tmp/debug_log.txt", std::ios::app);
    dbg << "[submit.cgi] Category: " << category << "\n";
    dbg.close();

    Quiz quiz;
    quiz.loadQuestions(category);
    const auto& questions = quiz.getQuestions();

    int score = 0;
    for (size_t i = 0; i < questions.size(); ++i) {
        std::string key = "q" + std::to_string(i);
        if (answers.find(key) != answers.end()) {
            char ua = answers[key][0];
            if (ua == questions[i].correctOption) ++score;
        }
    }

    std::cout << "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>";
    std::cout << "<title>Results</title><link rel='stylesheet' href='/style.css'></head><body><div class='container'>";
    std::cout << "<div class='card'><h1>Results</h1>";
    std::cout << "<p class='result'>Your Score: " << score << " out of " << questions.size() << "</p>";
    std::cout << "<p class='result-breakdown'><a href='/index.html' class='btn secondary'>Back to Home</a></p>";
    std::cout << "</div></div></body></html>";
    return 0;
}
