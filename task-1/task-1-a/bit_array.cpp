#include "bit_array.h"

#include <vector>
#include <string>
#include <stdexcept> // invalid_argument и т.д
#include <algorithm> // swap, fill
#include <climits>

BitReference::BitReference(BitArray& array, int index) : bit_array(array), bit_index(index) {}

BitReference::operator bool() const {
    bit_array.validate_index(bit_index);
    return (bit_array.data_[bit_array.block_index(bit_index)] & 
            bit_array.bit_mask(bit_index)) != 0;
}

BitReference& BitReference::operator=(bool value) {
    bit_array.validate_index(bit_index);
    if (value) {
        bit_array.data_[bit_array.block_index(bit_index)] |= 
            bit_array.bit_mask(bit_index);
    } else {
        bit_array.data_[bit_array.block_index(bit_index)] &= 
            ~bit_array.bit_mask(bit_index);
    }
    return *this;
}

BitReference& BitReference::operator=(const BitReference& other) {
    *this = static_cast<bool>(other);
    return *this;
}

BitReference& BitReference::flip() {
    bit_array.validate_index(bit_index);
    bit_array.data_[bit_array.block_index(bit_index)] ^= 
        bit_array.bit_mask(bit_index);
    return *this;
}


BitArray::BitArray() : data_(), bit_count_(0), valid_bits_in_last_block_(0) {}

BitArray::BitArray(int num_bits, unsigned long value) : bit_count_(num_bits) {
    if (num_bits < 0) {
        throw std::invalid_argument("Negative number of bits");
    }

    int blocks_needed = (num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    data_.resize(blocks_needed, 0);
    valid_bits_in_last_block_ = num_bits % BITS_PER_BLOCK;
    if (valid_bits_in_last_block_ == 0 && num_bits > 0) {
        valid_bits_in_last_block_ = BITS_PER_BLOCK;
    }

    for (int i = 0; i < num_bits && i < static_cast<int>(sizeof(value) * 8); ++i) {
        if (value & (1UL << i)) {
            data_[block_index(i)] |= bit_mask(i);
        }
    }
    sanitize_last_block();
}

BitArray::BitArray(const BitArray& other) 
    : data_(other.data_), 
      bit_count_(other.bit_count_),
      valid_bits_in_last_block_(other.valid_bits_in_last_block_) {}

BitArray::~BitArray() {};

void BitArray::swap(BitArray& other) {
    data_.swap(other.data_);
    std::swap(bit_count_, other.bit_count_);
    std::swap(valid_bits_in_last_block_, other.valid_bits_in_last_block_);
}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this != &other) {
        data_ = other.data_;
        bit_count_ = other.bit_count_;
        valid_bits_in_last_block_ = other.valid_bits_in_last_block_;
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Negative number of bits");
    }

    if (num_bits == bit_count_) {
        return;
    }

    if (num_bits < bit_count_) {
        bit_count_ = num_bits;
        valid_bits_in_last_block_ = num_bits % BITS_PER_BLOCK;
        if (valid_bits_in_last_block_ == 0 && num_bits > 0) {
            valid_bits_in_last_block_ = BITS_PER_BLOCK;
        }
        data_.resize((num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK);
        sanitize_last_block();
    } else {
        int old_count = bit_count_;
        bit_count_ = num_bits;
        valid_bits_in_last_block_ = num_bits % BITS_PER_BLOCK;
        if (valid_bits_in_last_block_ == 0 && num_bits > 0) {
            valid_bits_in_last_block_ = BITS_PER_BLOCK;
        }

        data_.resize((num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK);

        if (value) {
            for (int i = old_count; i < num_bits; ++i) {
                data_[block_index(i)] |= bit_mask(i);
            }
        }
    }
}

void BitArray::clear() {
    data_.clear();
    bit_count_ = 0;
    valid_bits_in_last_block_ = 0;
}

void BitArray::push_back(bool bit) {
    int new_bit_count = bit_count_ + 1;
    int blocks_needed = (new_bit_count + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
    
    if (blocks_needed > static_cast<int>(data_.size())) {
        data_.resize(blocks_needed, 0);
    }

    bit_count_ = new_bit_count;
    valid_bits_in_last_block_ = new_bit_count % BITS_PER_BLOCK;
    if (valid_bits_in_last_block_ == 0) {
        valid_bits_in_last_block_ = BITS_PER_BLOCK;
    }

    if (bit) {
        data_[block_index(bit_count_ - 1)] |= bit_mask(bit_count_ - 1);
    } else {
        data_[block_index(bit_count_ - 1)] &= ~bit_mask(bit_count_ - 1);
    }
}

BitArray& BitArray::operator&=(const BitArray& other) {
    if (bit_count_ != other.bit_count_) {
        throw std::invalid_argument("BitArray sizes must be equal for &=");
    }

    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] &= other.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& other) {
    if (bit_count_ != other.bit_count_) {
        throw std::invalid_argument("BitArray sizes must be equal for |=");
    }

    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] |= other.data_[i];
    }
    sanitize_last_block();
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& other) {
    if (bit_count_ != other.bit_count_) {
        throw std::invalid_argument("BitArray sizes must be equal for ^=");
    }

    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] ^= other.data_[i];
    }
    sanitize_last_block();
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Negative shift amount");
    }

    if (n >= bit_count_ || bit_count_ == 0) {
        std::fill(data_.begin(), data_.end(), 0);
        return *this;
    }

    if (n == 0) {
        return *this;
    }

    int block_shift = n / BITS_PER_BLOCK;
    int bit_shift = n % BITS_PER_BLOCK;

    if (block_shift > 0) {
        for (int i = 0; i < static_cast<int>(data_.size()) - block_shift; ++i) {
            data_[i] = data_[i + block_shift];
        }
        std::fill(data_.end() - block_shift, data_.end(), 0);
    }

    if (bit_shift > 0) {
        for (int i = 0; i < static_cast<int>(data_.size()) - 1; ++i) {
            data_[i] = (data_[i] << bit_shift) | (data_[i + 1] >> (BITS_PER_BLOCK - bit_shift));
        }
        data_.back() <<= bit_shift;
    }

    sanitize_last_block();
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Negative shift amount");
    }

    if (n >= bit_count_ || bit_count_ == 0) {
        std::fill(data_.begin(), data_.end(), 0);
        return *this;
    }

    if (n == 0) {
        return *this;
    }

    int block_shift = n / BITS_PER_BLOCK;
    int bit_shift = n % BITS_PER_BLOCK;
    int last_index = static_cast<int>(data_.size()) - 1;

    if (block_shift > 0) {
        for (int i = last_index; i >= block_shift; --i) {
            data_[i] = data_[i - block_shift];
        }
        std::fill(data_.begin(), data_.begin() + block_shift, 0);
    }

    if (bit_shift > 0) {
        for (int i = last_index; i > 0; --i) {
            data_[i] = (data_[i] >> bit_shift) | (data_[i - 1] << (BITS_PER_BLOCK - bit_shift));
        }
        data_[0] >>= bit_shift;
    }

    sanitize_last_block();
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool value) {
    validate_index(n);
    
    if (value) {
        data_[block_index(n)] |= bit_mask(n);
    } else {
        data_[block_index(n)] &= ~bit_mask(n);
    }
    return *this;
}

BitArray& BitArray::set() {
    for (auto& block : data_) {
        block = ~0U;
    }
    sanitize_last_block();
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    for (auto& block : data_) {
        block = 0;
    }
    return *this;
}

bool BitArray::any() const {
    for (uint32_t block : data_) {
        if (block != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (size_t i = 0; i < data_.size(); ++i) {
        result.data_[i] = ~data_[i];
    }
    result.sanitize_last_block();
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (uint32_t block : data_) {
        count += popcount(block);
    }
    return count;
}

bool BitArray::operator[](int index) const {
    validate_index(index);
    return (data_[block_index(index)] & bit_mask(index)) != 0;
}

BitReference BitArray::operator[](int index) {
    validate_index(index);
    return BitReference(*this, index);
}

int BitArray::size() const {
    return bit_count_;
}

bool BitArray::empty() const {
    return bit_count_ == 0;
}

std::string BitArray::to_string() const {
    std::string result;
    result.reserve(bit_count_);
    for (int i = 0; i < bit_count_; ++i) {
        result.push_back((*this)[i] ? '1' : '0');
    }
    return result;
}

void BitArray::validate_index(int index) const {
    if (index < 0 || index >= bit_count_) {
        throw std::out_of_range("Bit index out of range");
    }
}

void BitArray::sanitize_last_block() {
    if (data_.empty() || valid_bits_in_last_block_ == BITS_PER_BLOCK) {
        return;
    }
    
    uint32_t mask = (1UL << valid_bits_in_last_block_) - 1;
    data_.back() &= mask;
}

int BitArray::popcount(uint32_t x) const {
    int count = 0;
    while (x) {
        count += (x & 1);
        x >>= 1;
    }
    return count;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result &= b;
    return result;
}

BitArray operator|(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result |= b;
    return result;
}

BitArray operator^(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result ^= b;
    return result;
}