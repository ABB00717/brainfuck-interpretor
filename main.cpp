#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    // Read the input from file or STDIN
    std::string code;
    if (argc == 1) {
        std::cout << "Reading from STDIN" << std::endl;
        std::cout << "> ";
        std::cin >> code;
    } else if (argc == 2) {
        std::cout << "Reading from file" << std::endl;
        std::ifstream file(argv[1]);
        if (file.is_open()) {
            // Read the entire file into the string
            code.assign((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
            file.close();
        } else {
            std::cerr << "Unable to open file" << std::endl;
            return 1;
        }
        std::cout << "The code: " << code << std::endl;
    } else {
        std::cerr << "Usage: " << argv[0] << " [file]" << std::endl;
        return 1;
    }

    // Init everything the interpreter needs
    std::vector<char> tape(TAPE_SIZE, 0);
    int tape_i = 0;
    int instr_i = 0;

    // handle the input_char until EOF
    while (instr_i < code.length()) {
        switch (code[instr_i]) {
            case '>':
                tape_i++;
                // Dynamic tape expansion
                if (tape_i >= tape.size()) {
                    tape.resize(tape.size() * 2, 0);
                }
                break;
            case '<':
                if (tape_i > 0) {
                    tape_i--;
                }
                break;
            case '+':
                tape[tape_i]++;
                break;
            case '-':
                tape[tape_i]--;
                break;
            case '.':
                std::cout << tape[tape_i];
                break;
            case ',':
                std::cin >> tape[tape_i];
                break;
            case '[':
                // if the current cell is 0, jump to the matching ']' to skip
                // the loop
                if (tape[tape_i] == 0) {
                    int loop_depth = 1;
                    while (loop_depth > 0) {
                        instr_i++;

                        if (code[instr_i] == '[') {
                            loop_depth++;
                        } else if (code[instr_i] == ']') {
                            loop_depth--;
                        }
                    }
                }
                break;
            case ']':
                // if the current cell is not 0, jump back to the matching '['
                if (tape[tape_i] != 0) {
                    int loop_depth = 1;
                    while (loop_depth > 0) {
                        instr_i--;

                        if (code[instr_i] == ']') {
                            loop_depth++;
                        } else if (code[instr_i] == '[') {
                            loop_depth--;
                        }
                    }
                }
                break;
            default:
                break;
        }
        instr_i++;
    }
}
