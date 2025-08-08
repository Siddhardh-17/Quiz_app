#include "quiz.h"
#include <iostream>
#include <string>
#include <cstdlib>      // for getenv
#include <fstream>      // for ofstream

std::string getCategoryFromQuery() {
    const char* query = getenv("QUERY_STRING");
    if (query == nullptr) return "";

    std::string queryStr(query);
    std::string prefix = "category=";
    size_t pos = queryStr.find(prefix);
    if (pos != std::string::npos) {
        return queryStr.substr(pos + prefix.length());
    }
    return "";
}

int main() {
    std::cout << "Content-type: text/html\r\n\r\n";

    std::string category = getCategoryFromQuery();  // ✅ now category is defined

    // Debug log
    std::ofstream debug("debug_log.txt", std::ios::app);
    debug << "Received category: " << category << "\n";
    debug.close();

    Quiz quiz;
    quiz.loadQuestions(category);  // ✅ Load questions based on category

    const auto& questions = quiz.getQuestions();

    std::cout << "<html><head><title>Quiz</title></head><body>";
    std::cout << "<h1>Quiz Questions (" << category << ")</h1>";
    std::cout << "<form method='post' action='/cgi-bin/submit.cgi'>";
    std::cout << "<input type='hidden' name='category' value='" << category << "'>";

    if (questions.empty()) {
        std::cout << "<p>No questions found for this category.</p>";
    } else {
        for (size_t i = 0; i < questions.size(); ++i) {
            const auto& q = questions[i];
            std::cout << "<p>" << q.questionText << "</p>";
            std::cout << "<input type='radio' name='q" << i << "' value='A'>" << q.optionA << "<br>";
            std::cout << "<input type='radio' name='q" << i << "' value='B'>" << q.optionB << "<br>";
            std::cout << "<input type='radio' name='q" << i << "' value='C'>" << q.optionC << "<br>";
            std::cout << "<input type='radio' name='q" << i << "' value='D'>" << q.optionD << "<br>";
        }
    }

    std::cout << "<br><input type='submit' value='Submit'></form></body></html>";
    return 0;
}
