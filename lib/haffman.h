//
// Created by max on 2/14/19.
//

#pragma once


#include <string>

class Haffman {

public:

    Haffman(const char *input_file, const char *output_file) : input(input_file), output(output_file) {};

    void code(bool f1 = false, bool f2 = false);

    void decode(bool f1 = false, bool f2 = false);

private :
    std::string input;
    std::string output;
};


