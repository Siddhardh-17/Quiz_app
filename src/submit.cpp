#include "../include/quiz.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>

std::map<std::string, std::string> parseFormData() {
    std::map<std::string, std::string> formData;
    std::string input;
    std::getline(std::cin, input);

    std::istringstream ss(input);
    std::string keyValue;

    while (std::getline(ss, keyValue, '&')) {
        auto eqPos = keyValue.find('=');
        if (eqPos != std::string::npos) {
            std::string key = keyValue.substr(0, eqPos);
            std::string value = keyValue.substr(eqPos + 1);
            formData[key] = value;
        }
    }
    return formData;
}

int main() {
    std::cout << "Content-type: text/html\n\n";

    std::map<std::string, std::string> answers = parseFormData();

    std::string category = answers["category"];
    Quiz quiz;
    quiz.loadQuestions(category);
    const auto& questions = quiz.getQuestions();

    int score = 0;

    for (size_t i = 0; i < questions.size(); ++i) {
        std::string key = "q" + std::to_string(i);
        if (answers.find(key) != answers.end()) {
            char userAnswer = answers[key][0];
            if (userAnswer == questions[i].correctOption) {
                ++score;
            }
        }
    }

    std::cout << "<html><head><title>Results</title></head><body>";
    std::cout << "<h1>Your Score: " << score << " out of " << questions.size() << "</h1>";
    std::cout << "<a href='/index.html'>Back to Home</a>";
    std::cout << "</body></html>";

    return 0;
}
