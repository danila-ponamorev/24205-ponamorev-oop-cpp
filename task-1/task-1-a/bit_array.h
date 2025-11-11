#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

//Убрать метод string, проверить все операторы, возможно сделать основу вектора изменяемой.

class BitArray {
public:
    BitArray();
    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& other);
    ~BitArray();

    void swap(BitArray& other);
    BitArray& operator=(const BitArray& other);

    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& other);
    BitArray& operator|=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);

    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    BitArray& set(int n, bool value = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    bool operator[](int index) const;
    int size() const;
    bool empty() const;
    std::string to_string() const;

private:
    std::vector<uint32_t> data_;  // Храним биты в 32-битных блоках
    int bit_count_;               // Общее количество битов
    int valid_bits_in_last_block_; // Количество значимых битов в последнем блоке

    // Вспомогательные методы
    static constexpr int BITS_PER_BLOCK = 32;
    static constexpr int BLOCK_MASK = BITS_PER_BLOCK - 1;
    static constexpr int BLOCK_SHIFT = 5; // log2(BITS_PER_BLOCK)

    int block_index(int bit_index) const { return bit_index >> BLOCK_SHIFT; }
    int bit_offset(int bit_index) const { return bit_index & BLOCK_MASK; }
    uint32_t bit_mask(int bit_index) const { return 1U << bit_offset(bit_index); }

    void validate_index(int index) const;
    void ensure_capacity(int required_blocks);
    void sanitize_last_block();
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);
BitArray operator&(const BitArray& a, const BitArray& b);
BitArray operator|(const BitArray& a, const BitArray& b);
BitArray operator^(const BitArray& a, const BitArray& b);

#endif