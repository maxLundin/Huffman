//
// Created by max on 02.06.18.
//


#include <fstream>
#include "ReadWriter.h"

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
        chars.push_back(tree.temp[255].second.letter);
    }
    ReadWriter::writeBitSeq(out, bs1);
    size_t temp = chars.size();
    out.write((char *) &temp, sizeof(uint32_t));
    out.write((char *) chars.data(), chars.size());
}

void ReadWriter::compressor(char *input, size_t charNumber, bitSeq &ans, Tree const &tree) {
    tree.codePart(input, charNumber, &ans);
}

void ReadWriter::write(const std::string inputFileName, const std::string outFileName, const Tree &tree) {
    std::ifstream in(inputFileName, std::ios::binary);
    std::ofstream out(outFileName, std::ios::binary);

    if (!in.is_open()) {
        std::string str = (char *) ("This file doesn't exist:");
        str += inputFileName;
        throw std::runtime_error(str);
    }

    writeTree(out, tree);

    uint8_t buffer[128000];

    size_t charNumber = 0;
    std::cout << "Starting writing to the file" << std::endl;
    while (!in.eof()) {
        std::memset(buffer, 0, 128000 * sizeof(uint8_t));
        in.read((char *) buffer, 128000 * sizeof(uint8_t));
        charNumber += in.gcount();
        bitSeq bs;
        compressor((char *) &buffer, in.gcount(), bs, tree);
        writeBitSeq(out, bs);
    }

    in.close();
    out.close();
}

size_t ReadWriter::decompressor(bitSeq &bitSeq, char *out, Node1 *root) {
    Node1 *node = root;
    size_t last = 0;
    for (size_t i = 0; i < bitSeq.size(); i++) {
        bool bit = ((bitSeq.data()[i >> 6] & (1ull << (i & 63))) != 0);
        if (!bit) {
            node = node->right.get();
        } else {
            node = node->left.get();
        }
        if (node->left == nullptr && node->right == nullptr) {
            out[last] = (node->letter);
            last++;
            node = root;
        }
    }
    return last;
}

void ReadWriter::read(std::ifstream &in, const Tree &tree, const std::string fileName) {
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
        char out1[128000];
        size_t last = decompressor(bitSeq1, out1, tree.root.get());
        out.write((char *) out1, last);
    }
    out.close();
}
