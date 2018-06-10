//
// Created by max on 16.05.18.
//

#ifndef HAFFMAN_TREE_H
#define HAFFMAN_TREE_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "bitSeq.h"

struct Node {
    int64_t left;
    int64_t right;
    uint8_t letter;

    Node(uint8_t let = (uint8_t) 'a') {
        left = -1;
        right = -1;
        letter = let;
    }

    ~Node() = default;
};

struct Node1 {
    Node1 *left;
    Node1 *right;
    Node1 *parent;
    uint8_t letter;

    Node1(uint8_t let = (uint8_t) 'a') {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        letter = let;
    }

    ~Node1() = default;
};

struct Tree {
    bool flagBuilded = false;
    const char *fileName;
    Node1 *root = nullptr;
    std::pair<size_t, Node> temp[513];
    size_t k;
    bitSeq __array[256];

private:
    void doit(size_t node, bitSeq *prefix);

    void printInfo(bool __order);

    size_t fill();

    void countFrequencies(size_t __iter);

    void buildTree(bitSeq &bSeq, std::vector<uint8_t> &charVect);

public:

    Tree();

    void buildTree(bool __info, bool __order, char *outFile);

    Tree(const char *string);

    void codePart(char *mas, size_t size, bitSeq *bitSeq1) const;

    void getSeqs(size_t node, bitSeq *outSeq, std::vector<uint8_t> *chars) const;

    void buildTree(std::ifstream &in, const char *outputFile);

    ~Tree();

    void delTree(Node1 *node);
};


#endif //HAFFMAN_TREE_H
