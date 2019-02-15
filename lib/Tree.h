//
// Created by max on 16.05.18.
//

#pragma once

#include <cstring>
#include <fstream>
#include <memory>
#include <iostream>
#include <algorithm>
#include "bitSeq.h"

struct Node {
    int64_t left;
    int64_t right;
    char letter;

    Node(char let = 'a') {
        left = -1;
        right = -1;
        letter = let;
    }

    ~Node() = default;
};

struct Node1 {
    std::unique_ptr<Node1> left;
    std::unique_ptr<Node1> right;
    Node1 *parent;
    char letter;

    Node1(char let = 'a') {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        letter = let;
    }

    ~Node1() = default;
};

struct Tree {
    bool flagBuilt = false;
    const std::string fileName;
    std::unique_ptr<Node1> root = nullptr;
    std::pair<size_t, Node> temp[513];
    size_t k;
    bitSeq my_array[256];


public:

    Tree();

    void buildTree(bool info, bool order, std::string outFile);

    Tree(std::string string);

    void codePart(char *mas, size_t size, bitSeq *bitSeq1) const;

    void getSeqs(size_t node, bitSeq *outSeq, std::vector<char> *chars) const;

    void buildTree(std::ifstream &in, const std::string outputFile);

    ~Tree();

private:
    void decode_tree(size_t node, bitSeq *prefix);

    void printInfo(bool order);

    size_t fill();

    void countFrequencies(size_t my_iter);

    void buildTree(bitSeq &bSeq, std::vector<char> &charVect);

};


