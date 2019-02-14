//
// Created by max on 02.06.18.
//

#include "Tree.h"

Tree::Tree() : root(new Node1()), fileName("input.txt"), k(0) {
    std::cout << "Tree is built" << std::endl;
}

Tree::Tree(const std::string string) : root(new Node1()), fileName(string), k(0) {
}

bool comp(std::pair<size_t, Node> &a, std::pair<size_t, Node> &b) {
    return (a.first > b.first);
}

bool compRev(std::pair<size_t, Node> &a, std::pair<size_t, Node> &b) {
    return (a.first < b.first);
}

void Tree::doit(size_t node, bitSeq *prefix) {
    if (temp[node].second.left == -1 && temp[node].second.right == -1) {
        my_array[(temp[node].second.letter)] = (*prefix);
    } else {
        prefix->addBit(true);
        doit((size_t) temp[node].second.left, prefix);
        prefix->delBit();
        prefix->addBit(false);
        doit((size_t) temp[node].second.right, prefix);
        prefix->delBit();
    }
}

size_t Tree::fill() {
    size_t my_iter = 0;
    while (temp[my_iter].first == 0 && my_iter < 256)
        ++my_iter;
    for (int i = 256; i < 513; i++) {
        temp[i].first = std::numeric_limits<size_t>::max();
    }
    for (int i = 256; i < 513; i++) {
        temp[i].first = std::numeric_limits<size_t>::max();
    }
    return my_iter;
}

void Tree::printInfo(bool order) {
    std::cout << "ORDER: " << (order ? "Reversed" : "Direct") << std::endl;
    size_t __iter = 0;
    if (!order) {
        sort(temp, temp + 256, comp);
        while (temp[__iter].first != 0 && __iter != 256) {
            std::cout << temp[__iter].second.letter << " : " << temp[__iter].first << "\n";
            ++__iter;
        }
        std::cout << std::endl;
    } else {
        sort(temp, temp + 256, compRev);
        while (temp[__iter].first == 0)
            ++__iter;

        while (__iter != 256) {
            std::cout << temp[__iter].second.letter << " : " << temp[__iter].first << "\n";
            ++__iter;
        }
        std::cout << std::endl;
    }

}

void Tree::countFrequencies(size_t my_iter) {
    if (flagBuilt) {
        size_t i = my_iter;
        size_t j = 256;
        k = 0;
        for (; k < (255 - my_iter); k++) {
            size_t first;
            size_t first_numb;
            size_t second_numb;
            if (i < 256 && temp[i].first < temp[j].first) {
                first_numb = i;
                first = temp[i++].first;
            } else {
                first_numb = j;
                first = temp[j++].first;
            }
            size_t second;
            if (i < 256 && temp[i].first < temp[j].first) {
                second_numb = i;
                second = temp[i++].first;
            } else {
                second_numb = j;
                second = temp[j++].first;
            }
            temp[256 + k].first = first + second;
            temp[256 + k].second.left = first_numb;
            temp[256 + k].second.right = second_numb;
        }
    } else {
        throw std::runtime_error("Tree is not builded");
    }
}

void Tree::buildTree(bool info, bool order, std::string outFile) {
    flagBuilt = true;
    std::ifstream in(fileName, std::ios::binary);
    std::vector<uint8_t> c(128000);

    for (int i = 0; i < 256; i++) {
        temp[i].second = Node((uint8_t) i);
        temp[i].first = 0;
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
            temp[c[i]].first += 1;
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
    if (info) {
        printInfo(order);
    }
    //end print info

    //sorting
    if (info) {
        if (!order) {
            reverse(temp, temp + 256);
        }
    } else {
        sort(temp, temp + 256, compRev);
    }
    //end sorting

    size_t iter = fill();

    countFrequencies(iter);

    bitSeq bs = bitSeq();
    if (k != 0) {
        doit(k + 255, &bs);
    } else {
        bs.addBit(true);
        my_array[(temp[255].second.letter)] = bs;
    }
    std::cout << "Tree is built" << std::endl;
}

void Tree::codePart(char *mas, size_t size, bitSeq *bitSeq1) const {
    if (flagBuilt) {
        for (size_t i = 0; i < size; i++) {
            (*bitSeq1).append(my_array[(uint8_t) mas[i]]);
        }
    } else {
        throw std::runtime_error("Tree is not builded");
    }
}

void Tree::getSeqs(size_t node, bitSeq *outSeq, std::vector<uint8_t> *chars) const {
    if (flagBuilt) {
        if (temp[node].second.left != -1 || temp[node].second.right != -1) {
            (*outSeq).addBit(true);
            getSeqs((size_t) (temp[node].second.left), outSeq, chars);
            (*outSeq).addBit(false);
            getSeqs((size_t) (temp[node].second.right), outSeq, chars);
        } else {
            (*chars).push_back(temp[node].second.letter);
        }
    }
}

void Tree::buildTree(bitSeq &bSeq, std::vector<uint8_t> &charVect) {
    flagBuilt = true;
    Node1 *node = root.get();
    size_t charNumber = 0;
    for (size_t i = 0; i < bSeq.size(); i++) {
        bool bit = ((bSeq.data()[i >> 6] & (1ull << (i & 63))) != 0);
        if (bit) {
            (node)->left.reset(new Node1());
            (node)->left->parent = node;
            node = (node)->left.get();
        } else {
            (node)->letter = charVect[charNumber++];
            node = node->parent;
            while (node->right != nullptr && node->parent != nullptr) {
                node = node->parent;
            }
            if (node->parent == nullptr && node->right != nullptr) {
                throw std::runtime_error("Tree is damaged");
            }
            node->right.reset(new Node1());
            node->right->parent = node;
            node = node->right.get();
        }
    }
    node->letter = charVect[charNumber];
}

void Tree::buildTree(std::ifstream &in, const std::string outputFile) {
    flagBuilt = true;
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

Tree::~Tree() = default;
