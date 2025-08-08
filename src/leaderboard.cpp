#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct Entry {
    std::string name;
    int score;
};

int main() {
    std::cout << "Content-type: text/html\n\n";

    std::ifstream infile("leaderboard.txt");
    std::vector<Entry> entries;
    std::string name;
    int score;

    while (infile >> name >> score) {
        entries.push_back({name, score});
    }
    infile.close();

    std::sort(entries.begin(), entries.end(), [](auto &a, auto &b){
        return b.score < a.score;
    });

    std::cout << "<html><head><title>Leaderboard</title></head><body>";
    std::cout << "<h1>🏆 Leaderboard</h1><ol>";
    for (auto &e : entries) {
        std::cout << "<li>" << e.name << " - " << e.score << "</li>";
    }
    std::cout << "</ol></body></html>";
}
