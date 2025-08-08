#include "quiz.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

std::string getCategoryFromQuery() {
    const char* query = getenv("QUERY_STRING");
    if (query == nullptr) return "";
    std::string qs(query);
    std::string key = "category=";
    size_t pos = qs.find(key);
    if (pos != std::string::npos) return qs.substr(pos + key.size());
    return "";
}

int main() {
    std::cout << "Content-type: text/html\r\n\r\n";

    std::string category = getCategoryFromQuery();
    std::ofstream dbg("/tmp/debug_log.txt", std::ios::app);
    dbg << "[quiz.cgi] Category: " << category << "\n";
    dbg.close();

    Quiz quiz;
    quiz.loadQuestions(category);
    const auto& questions = quiz.getQuestions();

    // HTML head
    std::cout << "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>";
    std::cout << "<title>Quiz - " << category << "</title>";
    std::cout << "<link rel='stylesheet' href='/style.css'>";
    std::cout << "<script src='/script.js' defer></script>";
    std::cout << "</head><body>";
    // decorative background + grid inserted by script via script.js, but keep a container
    std::cout << "<div class='container'>";
    std::cout << "<header class='app-header'><h1>Quiz App</h1><div class='header-sub'>Category: " << category << "</div></header>";

    std::cout << "<div class='card'>";
    if (questions.empty()) {
        std::cout << "<p class='notice'>No questions found for this category.</p>";
    } else {
        // progress bar wrap
        std::cout << "<div class='progress-wrap'><div class='progress-fill' id='progress-fill'></div></div>";

        std::cout << "<form id='quiz-form' method='post' action='/cgi-bin/submit.cgi'>";
        std::cout << "<input type='hidden' name='category' value='" << category << "'>";
        for (size_t i = 0; i < questions.size(); ++i) {
            const auto &q = questions[i];
            std::cout << "<div class='question-card'>";
            std::cout << "<p class='question-text'>" << (i+1) << ". " << q.questionText << "</p>";
            std::cout << "<div class='options'>";
            std::cout << "<label class='option'><input type='radio' name='q" << i << "' value='A'> <span>A. " << q.optionA << "</span></label>";
            std::cout << "<label class='option'><input type='radio' name='q" << i << "' value='B'> <span>B. " << q.optionB << "</span></label>";
            std::cout << "<label class='option'><input type='radio' name='q" << i << "' value='C'> <span>C. " << q.optionC << "</span></label>";
            std::cout << "<label class='option'><input type='radio' name='q" << i << "' value='D'> <span>D. " << q.optionD << "</span></label>";
            std::cout << "</div></div>";
        }
        std::cout << "<div class='actions'><button class='btn' type='submit'>Submit</button></div>";
        std::cout << "</form>";
    }
    std::cout << "</div>"; // card
    std::cout << "<div class='footer-note'>Good luck — answer honestly 😉</div>";
    std::cout << "</div></body></html>";
    return 0;
}
