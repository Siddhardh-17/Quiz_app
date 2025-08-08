#include "quiz.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Quiz::loadQuestions(const std::string& category) {
    std::ofstream debug("/tmp/debug_log.txt", std::ios::app);

    questions.clear();

    // Use absolute path from your workspace root
    std::string filename = "/workspaces/Quiz_app/questions/" + category + ".txt";
    std::ifstream file(filename);

    if (!file) {
        debug << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        Question q;
        q.questionText = line;

        if (!std::getline(file, q.optionA) ||
            !std::getline(file, q.optionB) ||
            !std::getline(file, q.optionC) ||
            !std::getline(file, q.optionD) ||
            !(file >> q.correctOption)) {
            debug << "Error reading question block in: " << filename << "\n";
            break;
        }

        file.ignore(); // Skip newline after correctOption
        questions.push_back(q);

        debug << "Loaded question: " << q.questionText << " | Answer: " << q.correctOption << "\n";
    }

    debug << "Total questions loaded from " << filename << ": " << questions.size() << "\n";
    debug.close();
}

const std::vector<Question>& Quiz::getQuestions() const {
    return questions;
}
