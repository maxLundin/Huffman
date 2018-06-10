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
#include "ReadWriter.h"

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
    std::pair<size_t, Node> __temp[513];
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

    void codePart(std::vector<uint8_t> &mas, size_t size, bitSeq *bitSeq1) const;

    void getSeqs(size_t node, bitSeq *outSeq, std::vector<uint8_t> *chars) const;

    void buildTree(std::ifstream &in, const char *outputFile);
};

Node1 *__temp1 = nullptr;

Tree::Tree() {
    this->fileName = ("input.txt");
    this->k = 0;
    std::cout << "Tree is built" << std::endl;
    __temp1 = new Node1();
}

Tree::Tree(const char *string) {
    this->fileName = string;
    __temp1 = new Node1();
    k = 0;
}

bool comp(std::pair<size_t, Node> &a, std::pair<size_t, Node> &b) {
    return (a.first > b.first);
}

bool compRev(std::pair<size_t, Node> &a, std::pair<size_t, Node> &b) {
    return (a.first < b.first);
}

void Tree::doit(size_t node, bitSeq *prefix) {
    if (__temp[node].second.left == -1 && __temp[node].second.right == -1) {
        __array[(__temp[node].second.letter)] = (*prefix);
    } else {
        (*prefix).addBit(true);
        doit((size_t) __temp[node].second.left, prefix);
        (*prefix).delBit();
        (*prefix).addBit(false);
        doit((size_t) __temp[node].second.right, prefix);
        (*prefix).delBit();
    }
}

size_t Tree::fill() {
    size_t __iter = 0;
    while (__temp[__iter].first == 0 && __iter < 256)
        ++__iter;
    for (int i = 256; i < 513; i++) {
        __temp[i].first = std::numeric_limits<size_t>::max();
    }
    for (int i = 256; i < 513; i++) {
        __temp[i].first = std::numeric_limits<size_t>::max();
    }
    return __iter;
}

void Tree::printInfo(bool __order) {
    std::cout << "ORDER: " << (__order ? "Reversed" : "Direct") << std::endl;
    size_t __iter = 0;
    if (!__order) {
        sort(__temp, __temp + 256, comp);
        while (__temp[__iter].first != 0 && __iter != 256) {
            std::cout << __temp[__iter].second.letter << " : " << __temp[__iter].first << "\n";
            ++__iter;
        }
        std::cout << std::endl;
    } else {
        sort(__temp, __temp + 256, compRev);
        while (__temp[__iter].first == 0)
            ++__iter;

        while (__iter != 256) {
            std::cout << __temp[__iter].second.letter << " : " << __temp[__iter].first << "\n";
            ++__iter;
        }
        std::cout << std::endl;
    }

}

void Tree::countFrequencies(size_t __iter) {
    if (flagBuilded) {
        size_t i = __iter;
        size_t j = 256;
        k = 0;
        for (; k < (255 - __iter); k++) {
            size_t first;
            size_t __numb1;
            size_t __numb2;
            if (i < 256 && __temp[i].first < __temp[j].first) {
                __numb1 = i;
                first = __temp[i++].first;
            } else {
                __numb1 = j;
                first = __temp[j++].first;
            }
            size_t second;
            if (i < 256 && __temp[i].first < __temp[j].first) {
                __numb2 = i;
                second = __temp[i++].first;
            } else {
                __numb2 = j;
                second = __temp[j++].first;
            }
            __temp[256 + k].first = first + second;
            __temp[256 + k].second.left = __numb1;
            __temp[256 + k].second.right = __numb2;
        }
    } else {
        throw std::runtime_error("Tree is not builded");
    }
}

void Tree::buildTree(bool __info, bool __order, char *outFile) {
    flagBuilded = true;
    std::ifstream in(fileName, std::ios::binary);
    std::vector<uint8_t> c(128000);

    for (int i = 0; i < 256; i++) {
        __temp[i].second = Node((uint8_t) i);
        __temp[i].first = 0;
    }

    size_t charNumber = 0;
    if (!in.is_open()) {
        std::string str = (char *) ("This file doesn't exist:");
        str += fileName;
        throw std::runtime_error(str);
    }
    memset(c.data(), 0, c.size());
    if (in.read((char *) c.data(), c.size()).gcount() == 0) {
        std::ofstream out(outFile, std::ios::binary);
        out.close();
        exit(0);
    }
    while (true) {
        charNumber += in.gcount();
        auto lastSize = (size_t) (in.gcount());
        for (size_t i = 0; i < lastSize; i++) {
            __temp[c[i]].first += 1;
        }
        if (!in.eof()) {
            memset(c.data(), 0, c.size());
            in.read((char *) c.data(), c.size());
        } else {
            break;
        }
    }
    in.close();

    //printing info
    if (__info) {
        printInfo(__order);
    }
    //end print info

    //sorting
    if (__info) {
        if (!__order) {
            reverse(__temp, __temp + 256);
        }
    } else {
        sort(__temp, __temp + 256, compRev);
    }
    //end sorting

    size_t __iter = fill();

    countFrequencies(__iter);

    bitSeq bs = bitSeq();
    if (k != 0) {
        doit(k + 255, &bs);
    } else {
        bs.addBit(true);
        __array[(__temp[255].second.letter)] = bs;
    }
    std::cout << "Tree is built" << std::endl;
}

void Tree::codePart(std::vector<uint8_t> &mas, size_t size, bitSeq *bitSeq1) const {
    if (flagBuilded) {
        for (size_t i = 0; i < size; i++) {
            (*bitSeq1).append(__array[mas[i]]);
        }
    } else {
        throw std::runtime_error("Tree is not builded");
    }
}

void Tree::getSeqs(size_t node, bitSeq *outSeq, std::vector<uint8_t> *chars) const {
    if (flagBuilded) {
        if (__temp[node].second.left != -1 || __temp[node].second.right != -1) {
            (*outSeq).addBit(true);
            getSeqs((size_t) (__temp[node].second.left), outSeq, chars);
            (*outSeq).addBit(false);
            getSeqs((size_t) (__temp[node].second.right), outSeq, chars);
        } else {
            (*chars).push_back(__temp[node].second.letter);
        }
    }
}

void Tree::buildTree(bitSeq &bSeq, std::vector<uint8_t> &charVect) {
    flagBuilded = true;
    Node1 *node = __temp1;
    size_t charNumber = 0;
    for (size_t i = 0; i < bSeq.size(); i++) {
        bool bit = ((bSeq.data()[i >> 6] & (1ull << (i & 63))) != 0);
        if (bit) {
            (node)->left = new Node1();
            (node)->left->parent = node;
            node = (node)->left;
        } else {
            (node)->letter = charVect[charNumber++];
            node = node->parent;
            while (node->right != nullptr && node->parent != nullptr) {
                node = node->parent;
            }
            if (node->parent == nullptr && node->right != nullptr) {
                throw std::runtime_error("Tree is damaged");
            }
            node->right = new Node1();
            node->right->parent = node;
            node = node->right;
        }
    }
    node->letter = charVect[charNumber];
}

void Tree::buildTree(std::ifstream &in, const char *outputFile) {
    flagBuilded = true;
    if (!in.is_open()) {
        std::cout << "This file doesn't exist: " << fileName << std::endl;
        throw std::runtime_error("Bad file format");
    }

    /* обход дерева: считывание */
    uint32_t dataSize = 0;
    if (in.read((char *) &dataSize, sizeof(uint32_t)).gcount() == 0) {
        std::ofstream out(outputFile, std::ios::binary);
        out.close();
        exit(0);
    }

    uint64_t data = 0;
    uint32_t length = dataSize;
    std::vector<uint64_t> temp_vector;
    while ((dataSize >> 6) != 0) {
        if (in.read((char *) &data, sizeof(uint64_t)).gcount() == 0) {
            throw std::runtime_error("Bad file format");
        }
        temp_vector.push_back(data);
        dataSize -= 64;
    }
    if (dataSize != 0) {
        if (in.read((char *) &data, sizeof(uint64_t)).gcount() == 0) {
            throw std::runtime_error("Bad file format");
        }
        temp_vector.push_back(data);
    }
    /* конец считывания */

    /*reading chars*/
    uint32_t temp = 0;
    in.read((char *) &temp, sizeof(uint32_t));
    std::vector<uint8_t> temp_char_vector;
    uint8_t data1 = 0;
    while ((temp) != 0) {
        if (in.read((char *) &data1, sizeof(uint8_t)).gcount() == 0) {
            throw std::runtime_error("Bad file format");
        }
        temp_char_vector.push_back(data1);
        dataSize -= 8;
        temp--;
    }
    /*end of reading chars*/

    bitSeq bSeq(temp_vector, length);

    buildTree(bSeq, temp_char_vector);

}


#endif //HAFFMAN_TREE_H
