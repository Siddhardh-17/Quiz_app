#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <vector>

struct Question {
    std::string questionText;
    std::string optionA;
    std::string optionB;
    std::string optionC;
    std::string optionD;
    char correctOption;
};

class Quiz {
private:
    std::vector<Question> questions;

public:
    void loadQuestions(const std::string& category);
    const std::vector<Question>& getQuestions() const;
};

#endif
