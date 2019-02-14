//
// Created by max on 02.06.18.
//

#include "bitSeq.h"

void bitSeq::addBit(bool bit) {
    if ((leng >> 6) >= bits.size()) {
        bits.push_back(0);
    }
    if (bit)
        bits[(leng >> 6)] |= ((1ull) << (leng & 63));
    else
        bits[(leng >> 6)] &= ~((1ull) << (leng & 63));
    leng++;
}

bool bitSeq::getLastBit() const {
    return ((bits[((leng - 1) >> 6)] & (1ull << ((leng - 1) & 63))) != 0);
}

void bitSeq::out() const {
    for (size_t i = 0; i < leng; i++) {
        std::cout << ((bits[i >> 6] & (1ull << (i & 63))) != 0) << " ";
    }
    if (leng != 0)
        std::cout << std::endl;
}

void bitSeq::delBit() {
    leng--;

}

bitSeq::bitSeq() {
    leng = 0;
    bits = {0};
}

std::vector<uint64_t> &bitSeq::data() {
    return bits;
}

size_t bitSeq::size() const {
    return leng;
}

void bitSeq::append(const bitSeq &seq) {
    size_t reminder = (leng & 63);
    if (reminder == 0) {
        size_t start = 0;
        if (bits.size() == 1 && leng != 64) {
            bits[0] = seq.bits[0];
            start += 1;
        }
        for (size_t i = start; i < seq.bits.size(); i++) {
            bits.push_back(seq.bits[i]);
        }
        leng += seq.size();
        return;
    }
    size_t rem = 64 - reminder;

    size_t shift = (1ull << (reminder)) - 1;
    shift = ~shift;

    size_t len = (seq.size() >> 6);

    for (size_t i = 0; i < len; i++) {
        bits.back() += (seq.bits[i] << (reminder));
        bits.push_back(0);
        bits.back() += (seq.bits[i] >> (rem));
    }

    if ((seq.size() & 63) != 0) {
        bits.back() += (seq.bits.back() << (reminder));
        if ((seq.size() & 63) > rem) {
            bits.push_back(0);
            bits.back() += (seq.bits.back() >> (rem));
        }
    }

    leng += seq.leng;

}

bitSeq::bitSeq(std::vector<uint64_t> &temp, size_t leng) {
    bits = temp;
    this->leng = leng;
}

bitSeq::~bitSeq() =
default;
