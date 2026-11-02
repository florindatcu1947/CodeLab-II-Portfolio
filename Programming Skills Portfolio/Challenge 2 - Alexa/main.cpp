#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

struct Joke {
    std::string setup;
    std::string punchline;
};

std::vector<Joke> loadJokes(const std::string& filename) {
    std::vector<Joke> jokes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return jokes;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('?');
        if (delimiterPos != std::string::npos) {
            Joke j;
            j.setup = line.substr(0, delimiterPos + 1);
            j.punchline = line.substr(delimiterPos + 1);
            jokes.push_back(j);
        }
    }
    file.close();
    return jokes;
}

int main() {
    std::vector<Joke> jokes = loadJokes("randomJokes.txt");

    if (jokes.empty()) {
        std::cerr << "No jokes found or file failed to load.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, jokes.size() - 1);

    std::cout << "Type 'Alexa, tell me a joke' or 'quit' to exit.\n";

    std::string input;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        if (input == "quit") {
            std::cout << "Goodbye!\n";
            break;
        } else if (input == "Alexa, tell me a joke") {
            size_t index = dist(gen);
            std::cout << "\n" << jokes[index].setup << "\n";
            std::cout << "[Press Enter to reveal punchline...]";
            std::string waitKey;
            std::getline(std::cin, waitKey);
            std::cout << jokes[index].punchline << "\n";
        } else {
            std::cout << "Command not recognized. Type 'Alexa, tell me a joke' or 'quit'.\n";
        }
    }

    return 0;
}