#include <iostream>
#include "CommonFunc.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
namespace SDLCommonFunc {
    bool Random() {
        int number = rand() % 2;
        return number;
    }
    void SaveToFile(const std::string& text, const long long& score) {
        std::ofstream outFile("score.txt", std::ios::app);
        if (outFile) {
            outFile << text << " " << std::to_string(score) << "\n";
            outFile.close();
        }
    }
    struct Person {
        std::string name;
        int score;
        bool operator<(const Person& p) const { return score > p.score; }
    };

    std::string getTopScores(const std::string& filename, int topN = 10) {
        std::ifstream file(filename);
        std::vector<Person> people;
        std::string line, result;

        while (getline(file, line)) {
            std::istringstream iss(line);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) words.push_back(word);
            if (words.size() < 2 || !std::all_of(words.back().begin(), words.back().end(), ::isdigit)) continue;
            people.push_back({ line.substr(0, line.rfind(' ')), std::stoi(words.back()) });
        }

        std::sort(people.begin(), people.end());
        for (size_t i = 0; i < std::min(people.size(), size_t(topN)); ++i)
            result += people[i].name + " " + std::to_string(people[i].score) + "\n";

        return result;
    }

    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
    {
        int left_a = object1.x;
        int right_a = object1.x + object1.w;
        int top_a = object1.y;
        int bottom_a = object1.y + object1.h;

        int left_b = object2.x;
        int right_b = object2.x + object2.w;
        int top_b = object2.y;
        int bottom_b = object2.y + object2.h;

        // Case 1: size object 1 < size object 2
        if (left_a > left_b && left_a < right_b)
        {
            if (top_a > top_b && top_a < bottom_b)
            {
                return true;
            }
        }

        if (left_a > left_b && left_a < right_b)
        {
            if (bottom_a > top_b && bottom_a < bottom_b)
            {
                return true;
            }
        }

        if (right_a > left_b && right_a < right_b)
        {
            if (top_a > top_b && top_a < bottom_b)
            {
                return true;
            }
        }

        if (right_a > left_b && right_a < right_b)
        {
            if (bottom_a > top_b && bottom_a < bottom_b)
            {
                return true;
            }
        }

        // Case 2: size object 1 < size object 2
        if (left_b > left_a && left_b < right_a)
        {
            if (top_b > top_a && top_b < bottom_a)
            {
                return true;
            }
        }

        if (left_b > left_a && left_b < right_a)
        {
            if (bottom_b > top_a && bottom_b < bottom_a)
            {
                return true;
            }
        }

        if (right_b > left_a && right_b < right_a)
        {
            if (top_b > top_a && top_b < bottom_a)
            {
                return true;
            }
        }

        if (right_b > left_a && right_b < right_a)
        {
            if (bottom_b > top_a && bottom_b < bottom_a)
            {
                return true;
            }
        }

        // Case 3: size object 1 = size object 2
        if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
        {
            return true;
        }

        return false;
    }
}