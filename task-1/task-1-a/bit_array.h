#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <vector>
#include <string>
#include <cstdint>

class BitArray;

class BitReference {
private:
    BitArray& bit_array;
    int bit_index;

public:
    BitReference(BitArray& array, int index); // Create bit reference with specified index

    operator bool() const; // Read bit

    BitReference& operator=(bool value); // Set bit
};

class BitArray
{

friend class BitReference;


public:
    BitArray(); // Create empty bit array
    ~BitArray(); // Destroy bit array and free memory
    
    explicit BitArray(int num_bits, unsigned long value = 0); // Create array with specified size, initialize from value
    BitArray(const BitArray& b); // Create copy of existing bit array

    void swap(BitArray& b); // Exchange contents with another bit array
    BitArray& operator=(const BitArray& b); //Assign the content of another bit array.

    void resize(int num_bits, bool value = false); // Change array size, initialize new bits with value
    void clear(); // Remove all bits (set size to zero)
    void push_back(bool bit); // Append single bit to end of array

    BitArray& operator&=(const BitArray& b); // Perform bitwise AND in-place
    BitArray& operator|=(const BitArray& b); // Perform bitwise OR in-place
    BitArray& operator^=(const BitArray& b); // Perform bitwise XOR in-place
 
    BitArray& operator<<=(int n); // Left shift bits in-place
    BitArray& operator>>=(int n); // Right shift bits in-place
    BitArray operator<<(int n) const; // Return left-shifted copy
    BitArray operator>>(int n) const; // Return right-shifted copy

    BitArray& set(int n, bool val = true); // Set specific bit to value
    BitArray& set(); // Set all bits to true
    BitArray& reset(int n); // Set specific bit to false
    BitArray& reset(); // Set all bits to false

    bool any() const; // Return true if any bit is set
    bool none() const; // Return true if no bits are set
    BitArray operator~() const; // Return bitwise NOT copy
    int count() const; // Return number of set bits

    bool operator[](int i) const; // Return value of specific bit
    BitReference operator[](int i); // Return reference to a bit
    int size() const; // Return number of bits in array
    bool empty() const; // Return true if array has no bits
    std::string to_string() const; // Return string representation of bits

private:
    const int BITS_PER_BLOCK = sizeof(uint32_t) * 8;
    std::vector<uint32_t> data_;
    int bit_count_;
    int valid_bits_in_last_block_;

    int block_index(int bit_index) const { return bit_index / BITS_PER_BLOCK; } // Calculate block index containing specified bit
    uint32_t bit_mask(int bit_index) const { return 1UL << (bit_index % BITS_PER_BLOCK); } // Generate bit mask for specified position within block
    
    void validate_index(int index) const; // Validate bit index range
    void sanitize_last_block(); // Clear unused bits in last block to maintain consistency

    int popcount(uint32_t x) const; // Count set bits in 32-bit value
};

bool operator==(const BitArray& a, const BitArray& b); // Compare two BitArrays for equality
bool operator!=(const BitArray& a, const BitArray& b); // Compare two BitArrays for inequality

BitArray operator&(const BitArray& b1, const BitArray& b2); // Compute bitwise AND of two BitArrays
BitArray operator|(const BitArray& b1, const BitArray& b2); // Compute bitwise OR of two BitArrays
BitArray operator^(const BitArray& b1, const BitArray& b2); // Compute bitwise XOR of two BitArrays


#endif