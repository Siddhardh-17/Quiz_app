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
    std::cout << "<title>Results</title>";
    std::cout << "<link rel='stylesheet' href='/style.css'>";
    // script will fire confetti when .result exists
    std::cout << "<script src='/script.js' defer></script>";
    std::cout << "</head><body>";
    std::cout << "<div class='container'><div class='card'>";
    std::cout << "<h1 class='result'>Your Score: " << score << " / " << questions.size() << "</h1>";
    std::cout << "<p class='result-breakdown'>Category: " << category << "</p>";
    std::cout << "<p class='result-breakdown'><a class='btn secondary' href='/index.html'>Back to Home</a> <a class='btn' href='/cgi-bin/quiz.cgi?category=" << category << "'>Try Again</a></p>";
    std::cout << "</div></div>";
    // confetti canvas will be injected by script.js
    std::cout << "</body></html>";
    return 0;
}
