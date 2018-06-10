//
// Created by max on 17.05.18.
//

#ifndef HAFFMAN_BITSEQ_H
#define HAFFMAN_BITSEQ_H

#include <iostream>
#include <vector>

class bitSeq {
    std::vector<uint64_t> bits = {};
    size_t leng;

public:
    bitSeq();

    bitSeq(std::vector<uint64_t> &temp, size_t leng);

    void addBit(bool bit);

    bool getLastBit() const;

    void delBit();

    size_t size() const;

    std::vector<uint64_t> &data();

    void append(const bitSeq &seq);

    void out() const;

    ~bitSeq();
};


#endif //HAFFMAN_BITSEQ_H
