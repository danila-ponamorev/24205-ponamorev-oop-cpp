#include "bit_array.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

const int BitArray::BITS_PER_LONG;

BitArray::BitArray() : num_bits_(0) {}

BitArray::~BitArray() = default;

BitArray::BitArray(int num_bits, unsigned long value) : num_bits_(num_bits) {
    if (num_bits < 0) {
        throw std::invalid_argument("Number of bits cannot be negative");
    }
    
    int num_longs = (num_bits + BITS_PER_LONG - 1) / BITS_PER_LONG;
    data_.resize(num_longs, 0);
    
    if (num_bits > 0) {
        data_[0] = value;
        // Clear any extra bits beyond the requested size
        if (num_bits < BITS_PER_LONG) {
            unsigned long mask = (1UL << num_bits) - 1;
            data_[0] &= mask;
        }
    }
}

BitArray::BitArray(const BitArray& other) 
    : data_(other.data_), num_bits_(other.num_bits_) {}

void BitArray::swap(BitArray& other) {
    std::swap(data_, other.data_);
    std::swap(num_bits_, other.num_bits_);
}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this != &other) {
        BitArray temp(other);
        swap(temp);
    }
    return *this;
}

int BitArray::get_data_index(int bit_index) const {
    return bit_index / BITS_PER_LONG;
}

unsigned long BitArray::get_bit_mask(int bit_index) const {
    return 1UL << (bit_index % BITS_PER_LONG);
}

void BitArray::validate_index(int index) const {
    if (index < 0 || index >= num_bits_) {
        throw std::out_of_range("Bit index out of range");
    }
}

void BitArray::validate_same_size(const BitArray& other) const {
    if (num_bits_ != other.num_bits_) {
        throw std::invalid_argument("BitArrays must have same size for bitwise operations");
    }
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Number of bits cannot be negative");
    }
    
    if (num_bits == num_bits_) {
        return;
    }
    
    int old_num_bits = num_bits_;
    int new_num_longs = (num_bits + BITS_PER_LONG - 1) / BITS_PER_LONG;
    
    if (num_bits > num_bits_) {
        // Expanding
        data_.resize(new_num_longs, 0);
        
        // Set new bits to the specified value
        for (int i = old_num_bits; i < num_bits; ++i) {
            if (value) {
                data_[get_data_index(i)] |= get_bit_mask(i);
            } else {
                data_[get_data_index(i)] &= ~get_bit_mask(i);
            }
        }
    } else {
        // Shrinking - we need to clear any bits beyond the new size
        // in the last affected long
        if (num_bits > 0) {
            int last_index = get_data_index(num_bits - 1);
            int bits_in_last_long = num_bits % BITS_PER_LONG;
            if (bits_in_last_long == 0) {
                bits_in_last_long = BITS_PER_LONG;
            }
            
            unsigned long mask = (1UL << bits_in_last_long) - 1;
            data_[last_index] &= mask;
        }
        
        // Resize vector if needed (remove unused longs)
        data_.resize(new_num_longs);
    }
    
    num_bits_ = num_bits;
}

void BitArray::clear() {
    data_.clear();
    num_bits_ = 0;
}

void BitArray::push_back(bool bit) {
    int old_size = num_bits_;
    resize(num_bits_ + 1, false);
    set(old_size, bit);
}

BitArray& BitArray::operator&=(const BitArray& other) {
    validate_same_size(other);
    
    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] &= other.data_[i];
    }
    
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& other) {
    validate_same_size(other);
    
    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] |= other.data_[i];
    }
    
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& other) {
    validate_same_size(other);
    
    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i] ^= other.data_[i];
    }
    
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Shift amount cannot be negative");
    }
    
    if (n == 0 || num_bits_ == 0) {
        return *this;
    }
    
    if (n >= num_bits_) {
        // Shift all bits out
        reset();
        return *this;
    }
    
    // Perform the shift from left to right
    for (int i = 0; i < num_bits_ - n; ++i) {
        set(i, (*this)[i + n]);
    }
    
    // Fill the remaining bits with zeros
    for (int i = num_bits_ - n; i < num_bits_; ++i) {
        reset(i);
    }
    
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) {
        throw std::invalid_argument("Shift amount cannot be negative");
    }
    
    if (n == 0 || num_bits_ == 0) {
        return *this;
    }
    
    if (n >= num_bits_) {
        // Shift all bits out
        reset();
        return *this;
    }
    
    // Perform the shift from right to left
    for (int i = num_bits_ - 1; i >= n; --i) {
        set(i, (*this)[i - n]);
    }
    
    // Fill the beginning bits with zeros
    for (int i = 0; i < n; ++i) {
        reset(i);
    }
    
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

BitArray& BitArray::set(int n, bool val) {
    validate_index(n);
    
    if (val) {
        data_[get_data_index(n)] |= get_bit_mask(n);
    } else {
        data_[get_data_index(n)] &= ~get_bit_mask(n);
    }
    
    return *this;
}

BitArray& BitArray::set() {
    unsigned long fill_value = ~0UL;
    for (auto& item : data_) {
        item = fill_value;
    }
    
    // Clear any extra bits beyond the actual size
    if (num_bits_ > 0) {
        int last_index = get_data_index(num_bits_ - 1);
        int bits_in_last_long = num_bits_ % BITS_PER_LONG;
        if (bits_in_last_long == 0) {
            bits_in_last_long = BITS_PER_LONG;
        }
        
        unsigned long mask = (1UL << bits_in_last_long) - 1;
        data_[last_index] &= mask;
    }
    
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    for (auto& item : data_) {
        item = 0;
    }
    return *this;
}

bool BitArray::any() const {
    for (const auto& item : data_) {
        if (item != 0) {
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
    
    for (size_t i = 0; i < result.data_.size(); ++i) {
        result.data_[i] = ~result.data_[i];
    }
    
    // Clear any extra bits beyond the actual size in the last long
    if (num_bits_ > 0) {
        int last_index = get_data_index(num_bits_ - 1);
        int bits_in_last_long = num_bits_ % BITS_PER_LONG;
        if (bits_in_last_long == 0) {
            bits_in_last_long = BITS_PER_LONG;
        }
        
        unsigned long mask = (1UL << bits_in_last_long) - 1;
        result.data_[last_index] &= mask;
    }
    
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < num_bits_; ++i) {
        if ((*this)[i]) {
            ++count;
        }
    }
    return count;
}

bool BitArray::operator[](int i) const {
    validate_index(i);
    return (data_[get_data_index(i)] & get_bit_mask(i)) != 0;
}

int BitArray::size() const {
    return num_bits_;
}

bool BitArray::empty() const {
    return num_bits_ == 0;
}

std::string BitArray::to_string() const {
    std::string result;
    result.reserve(num_bits_);
    
    // Convert from most significant to least significant for natural reading
    for (int i = num_bits_ - 1; i >= 0; --i) {
        result += ((*this)[i] ? '1' : '0');
    }
    
    return result;
}

// Non-member functions

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

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}