#include "dfxam.h"

using namespace dfxam;

int main() {
    dfxam::repl::ExecutionEngine eng;

    while (true) {
        std::cout << ">> ";

        std::string input;
        getline(std::cin, input);        

        eng << input; 
        std::cout << std::endl;
    }

    return 0;
}
