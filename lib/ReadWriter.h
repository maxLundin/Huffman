//
// Created by max on 17.05.18.
//

#ifndef HAFFMAN_READWRITER_H
#define HAFFMAN_READWRITER_H

#include <fstream>
#include <cstring>
#include "Tree.h"

class ReadWriter {

public:
    static void compressor(char *, size_t, bitSeq &ans, Tree const &tree);

    static size_t decompressor(bitSeq &bitSeq, char *out, Node1 *root);

    static void write(const std::string inputFileName, const std::string outFileName, const Tree &tree);

    static void read(std::ifstream &in, const Tree &tree, const std::string fileName);

    static void writeBitSeq(std::ofstream &out, bitSeq &bs);

    static void writeTree(std::ofstream &out, const Tree &tree);
};


#endif //HAFFMAN_READWRITER_H
