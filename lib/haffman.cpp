//
// Created by max on 2/14/19.
//

#include "haffman.h"
#include "Tree.h"
#include "ReadWriter.h"


void Haffman::code(bool f1, bool f2) {
    Tree tree{input};
    tree.buildTree(f1, f2, output);
    ReadWriter::write(input, output, tree);
    std::cout << "Compressing is done" << std::endl;
}

void Haffman::decode(bool f1, bool f2) {
    Tree tree{};
    std::ifstream in_stream(input, std::ios::binary);
    tree.buildTree(in_stream, output);
    ReadWriter::read(in_stream, tree, output);
    in_stream.close();
    std::cout << "Decompressing is done" << std::endl;
}