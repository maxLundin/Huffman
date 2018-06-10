//
// Created by max on 17.05.18.
//

#ifndef HAFFMAN_READWRITER_H
#define HAFFMAN_READWRITER_H

#include <fstream>
#include <cstring>
#include "bitSeq.h"
#include "Tree.h"

class ReadWriter {

public:
    static void write(const char *inputFileName, const char *outFileName, const Tree &tree);

    static void read(std::ifstream &in, const Tree &tree, const char *fileName);

    static void writeBitSeq(std::ofstream &out, bitSeq &bs);

    static void writeTree(std::ofstream &out, const Tree &tree);
};

void ReadWriter::writeBitSeq(std::ofstream &out, bitSeq &bs) {
    size_t size = (bs).size();
    out.write((char *) &size, sizeof(uint32_t));
    size_t ost = bs.size() % (sizeof(uint64_t) * 8);
    if (ost != 0) {
        out.write((char *) bs.data().data(), (bs.size() / (sizeof(uint64_t) * 8) + 1) * sizeof(uint64_t));
    } else {
        out.write((char *) bs.data().data(), bs.size() / (sizeof(uint64_t) * 8) * sizeof(uint64_t));
    }
}

void ReadWriter::writeTree(std::ofstream &out, const Tree &tree) {
    bitSeq bs1 = bitSeq();
    std::vector<uint8_t> chars;
    if (tree.k != 0) {
        tree.getSeqs(tree.k + 255, &bs1, &chars);
    } else {
        bs1.addBit(true);
        chars.push_back(tree.__temp[255].second.letter);
    }

    ReadWriter::writeBitSeq(out, bs1);

    size_t temp = chars.size();
    out.write((char *) &temp, sizeof(uint32_t));
    out.write((char *) chars.data(), chars.size());
}

void ReadWriter::write(const char *inputFileName, const char *outFileName, const Tree &tree) {
    std::ifstream in(inputFileName, std::ios::binary);
    std::ofstream out(outFileName, std::ios::binary);

    if (!in.is_open()) {
        std::string str = (char *) ("This file doesn't exist:");
        str += inputFileName;
        throw std::runtime_error(str);
    }

    writeTree(out, tree);

    std::vector<uint8_t> c(128000);

    size_t charNumber = 0;
    std::cout << "Starting writing to the file" << std::endl;
    while (!in.eof()) {
        std::memset(c.data(), 0, c.size());
        in.read((char *) c.data(), c.size());
        charNumber += in.gcount();
        bitSeq bs;
        tree.codePart(c, (size_t) (in.gcount()), &bs);
        writeBitSeq(out, bs);
    }

    in.close();
    out.close();
}

void ReadWriter::read(std::ifstream &in, const Tree &tree, const char *fileName) {
    std::ofstream out(fileName, std::ios::binary);
    uint32_t dataSize = 0;
    while (true) {
        if (in.read((char *) &dataSize, sizeof(uint32_t)).gcount() == 0)
            break;

        std::vector<uint64_t> data((dataSize >> 6) + (size_t) ((dataSize & 63) != 0));
        if (in.read((char *) data.data(),
                    ((dataSize >> 6) + (size_t) ((dataSize & 63) != 0)) * sizeof(uint64_t)).gcount() == 0) {
            throw std::runtime_error("Wrong file format");
        }
        bitSeq bitSeq1(data, (size_t) (dataSize));
        Node1 *node = __temp1;
        for (size_t i = 0; i < bitSeq1.size(); i++) {
            bool bit = ((bitSeq1.data()[i >> 6] & (1ull << (i & 63))) != 0);
            if (!bit) {
                node = node->right;
            } else {
                node = node->left;
            }
            if (node->left == nullptr && node->right == nullptr) {
                out.write((char *) &node->letter, 1);
                node = __temp1;
            }
        }
    }
    out.close();
}

#endif //HAFFMAN_READWRITER_H
