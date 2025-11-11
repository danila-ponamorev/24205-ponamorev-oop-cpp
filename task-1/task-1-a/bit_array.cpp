#include "bit_array.h"
#include <algorithm>
#include <climits>

BitArray::BitArray() : bit_count_(0), valid_bits_in_last_block_(0) {}

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

    // Инициализация значением value
    for (int i = 0; i < num_bits && i < static_cast<int>(sizeof(value) * CHAR_BIT); ++i) {
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

BitArray::~BitArray() = default;

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
        // Уменьшение размера
        bit_count_ = num_bits;
        valid_bits_in_last_block_ = num_bits % BITS_PER_BLOCK;
        if (valid_bits_in_last_block_ == 0 && num_bits > 0) {
            valid_bits_in_last_block_ = BITS_PER_BLOCK;
        }
        data_.resize((num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK);
        sanitize_last_block();
    } else {
        // Увеличение размера
        int old_count = bit_count_;
        bit_count_ = num_bits;
        valid_bits_in_last_block_ = num_bits % BITS_PER_BLOCK;
        if (valid_bits_in_last_block_ == 0 && num_bits > 0) {
            valid_bits_in_last_block_ = BITS_PER_BLOCK;
        }

        data_.resize((num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK);
        
        // Установка новых битов в значение value
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

    if (bit_shift == 0) {
        // Простой случай - сдвиг на целое число блоков
        for (int i = static_cast<int>(data_.size()) - 1; i >= block_shift; --i) {
            data_[i] = data_[i - block_shift];
        }
    } else {
        // Сложный случай - комбинированный сдвиг
        for (int i = static_cast<int>(data_.size()) - 1; i > block_shift; --i) {
            data_[i] = (data_[i - block_shift] << bit_shift) | 
                       (data_[i - block_shift - 1] >> (BITS_PER_BLOCK - bit_shift));
        }
        data_[block_shift] = data_[0] << bit_shift;
    }

    // Заполняем освободившиеся блоки нулями
    std::fill(data_.begin(), data_.begin() + block_shift, 0);
    
    // Обнуляем неиспользуемые биты в последнем блоке
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
    int last_block = static_cast<int>(data_.size()) - 1;

    if (bit_shift == 0) {
        // Простой случай - сдвиг на целое число блоков
        for (int i = 0; i <= last_block - block_shift; ++i) {
            data_[i] = data_[i + block_shift];
        }
    } else {
        // Сложный случай - комбинированный сдвиг
        for (int i = 0; i < last_block - block_shift; ++i) {
            data_[i] = (data_[i + block_shift] >> bit_shift) | 
                       (data_[i + block_shift + 1] << (BITS_PER_BLOCK - bit_shift));
        }
        data_[last_block - block_shift] = data_[last_block] >> bit_shift;
    }

    // Заполняем освободившиеся блоки нулями
    std::fill(data_.end() - block_shift, data_.end(), 0);
    
    // Обнуляем неиспользуемые биты в последнем блоке
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
    std::fill(data_.begin(), data_.end(), ~0U);
    sanitize_last_block();
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    std::fill(data_.begin(), data_.end(), 0);
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
        // Используем встроенную функцию для подсчета битов
        count += __builtin_popcount(block);
    }
    return count;
}

bool BitArray::operator[](int index) const {
    validate_index(index);
    return (data_[block_index(index)] & bit_mask(index)) != 0;
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

// Вспомогательные методы
void BitArray::validate_index(int index) const {
    if (index < 0 || index >= bit_count_) {
        throw std::out_of_range("Bit index out of range");
    }
}

void BitArray::sanitize_last_block() {
    if (data_.empty() || valid_bits_in_last_block_ == BITS_PER_BLOCK) {
        return;
    }
    
    uint32_t mask = (1U << valid_bits_in_last_block_) - 1;
    data_.back() &= mask;
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