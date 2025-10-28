#ifndef BITARRAY_H
#define BITARRAY_H

#include <string>
#include <vector>

/**
 * @class BitArray
 * @brief A dynamic array of bits that provides efficient storage and bitwise operations.
 * 
 * The BitArray class stores a sequence of bits in a compact form using unsigned long
 * as the underlying storage unit. It supports various bitwise operations, resizing,
 * and common bit manipulation functions.
 */
class BitArray
{
private:
    std::vector<unsigned long> data_;  ///< Internal storage for bits, packed into unsigned long integers
    int num_bits_;                     ///< Number of bits currently stored in the array
    static const int BITS_PER_LONG = sizeof(unsigned long) * 8; ///< Number of bits in an unsigned long

    /**
     * @brief Gets the index in the data vector for a given bit position
     * @param bit_index The position of the bit (0-based)
     * @return Index in the data vector
     */
    int get_data_index(int bit_index) const;

    /**
     * @brief Gets the bit mask for a given bit position within a long
     * @param bit_index The position of the bit (0-based)
     * @return Bit mask with the corresponding bit set
     */
    unsigned long get_bit_mask(int bit_index) const;

    /**
     * @brief Validates that a bit index is within bounds
     * @param index The bit index to validate
     * @throws std::out_of_range if index is invalid
     */
    void validate_index(int index) const;

    /**
     * @brief Validates that two BitArrays have the same size for bitwise operations
     * @param other The other BitArray to compare size with
     * @throws std::invalid_argument if sizes don't match
     */
    void validate_same_size(const BitArray& other) const;

public:
    /**
     * @brief Default constructor. Creates an empty bit array.
     */
    BitArray();
    
    /**
     * @brief Destructor. Cleans up resources.
     */
    ~BitArray();

    /**
     * @brief Constructs a bit array with specified number of bits.
     * @param num_bits Number of bits to store (must be non-negative)
     * @param value Initial value for the first sizeof(long) bits (default: 0)
     * @throws std::invalid_argument if num_bits is negative
     */
    explicit BitArray(int num_bits, unsigned long value = 0);
    
    /**
     * @brief Copy constructor.
     * @param other The BitArray to copy from
     */
    BitArray(const BitArray& other);

    /**
     * @brief Swaps contents with another BitArray.
     * @param other The BitArray to swap with
     */
    void swap(BitArray& other);

    /**
     * @brief Copy assignment operator.
     * @param other The BitArray to assign from
     * @return Reference to this BitArray
     */
    BitArray& operator=(const BitArray& other);

    /**
     * @brief Resizes the bit array.
     * @param num_bits New size in bits (must be non-negative)
     * @param value Value to initialize new bits with if expanding (default: false)
     * @throws std::invalid_argument if num_bits is negative
     */
    void resize(int num_bits, bool value = false);
    
    /**
     * @brief Clears all bits and sets size to zero.
     */
    void clear();
    
    /**
     * @brief Appends a bit to the end of the array.
     * @param bit The bit value to append (true for 1, false for 0)
     */
    void push_back(bool bit);

    // Bitwise operations (require arrays of same size)
    
    /**
     * @brief Performs bitwise AND assignment with another BitArray.
     * @param other The BitArray to AND with
     * @return Reference to this BitArray
     * @throws std::invalid_argument if sizes don't match
     */
    BitArray& operator&=(const BitArray& other);
    
    /**
     * @brief Performs bitwise OR assignment with another BitArray.
     * @param other The BitArray to OR with
     * @return Reference to this BitArray
     * @throws std::invalid_argument if sizes don't match
     */
    BitArray& operator|=(const BitArray& other);
    
    /**
     * @brief Performs bitwise XOR assignment with another BitArray.
     * @param other The BitArray to XOR with
     * @return Reference to this BitArray
     * @throws std::invalid_argument if sizes don't match
     */
    BitArray& operator^=(const BitArray& other);

    // Bit shift operations
    
    /**
     * @brief Performs left shift assignment (fills with zeros).
     * @param n Number of positions to shift (must be non-negative)
     * @return Reference to this BitArray
     * @throws std::invalid_argument if n is negative
     */
    BitArray& operator<<=(int n);
    
    /**
     * @brief Performs right shift assignment (fills with zeros).
     * @param n Number of positions to shift (must be non-negative)
     * @return Reference to this BitArray
     * @throws std::invalid_argument if n is negative
     */
    BitArray& operator>>=(int n);
    
    /**
     * @brief Returns a left-shifted copy of the BitArray.
     * @param n Number of positions to shift (must be non-negative)
     * @return New BitArray containing the shifted result
     * @throws std::invalid_argument if n is negative
     */
    BitArray operator<<(int n) const;
    
    /**
     * @brief Returns a right-shifted copy of the BitArray.
     * @param n Number of positions to shift (must be non-negative)
     * @return New BitArray containing the shifted result
     * @throws std::invalid_argument if n is negative
     */
    BitArray operator>>(int n) const;

    // Bit manipulation
    
    /**
     * @brief Sets a specific bit to the given value.
     * @param n Bit index to set (0-based)
     * @param val Value to set (true for 1, false for 0, default: true)
     * @return Reference to this BitArray
     * @throws std::out_of_range if index is out of bounds
     */
    BitArray& set(int n, bool val = true);
    
    /**
     * @brief Sets all bits to true.
     * @return Reference to this BitArray
     */
    BitArray& set();
    
    /**
     * @brief Sets a specific bit to false.
     * @param n Bit index to reset (0-based)
     * @return Reference to this BitArray
     * @throws std::out_of_range if index is out of bounds
     */
    BitArray& reset(int n);
    
    /**
     * @brief Sets all bits to false.
     * @return Reference to this BitArray
     */
    BitArray& reset();

    // Queries
    
    /**
     * @brief Checks if any bit is set to true.
     * @return true if at least one bit is true, false otherwise
     */
    bool any() const;
    
    /**
     * @brief Checks if all bits are set to false.
     * @return true if all bits are false, false otherwise
     */
    bool none() const;
    
    /**
     * @brief Returns the bitwise NOT of the array.
     * @return New BitArray with all bits inverted
     */
    BitArray operator~() const;
    
    /**
     * @brief Counts the number of bits set to true.
     * @return Number of true bits in the array
     */
    int count() const;

    // Accessors
    
    /**
     * @brief Accesses a bit at the specified position (read-only).
     * @param i Bit index to access (0-based)
     * @return Value of the bit at position i
     * @throws std::out_of_range if index is out of bounds
     */
    bool operator[](int i) const;
    
    /**
     * @brief Gets the number of bits in the array.
     * @return Current size in bits
     */
    int size() const;
    
    /**
     * @brief Checks if the array is empty.
     * @return true if size is 0, false otherwise
     */
    bool empty() const;
    
    /**
     * @brief Converts the bit array to a string representation.
     * @return String where '1' represents true and '0' represents false
     */
    std::string to_string() const;
};

// Non-member comparison operators

/**
 * @brief Checks if two BitArrays are equal.
 * @param a First BitArray to compare
 * @param b Second BitArray to compare
 * @return true if arrays have same size and all bits match, false otherwise
 */
bool operator==(const BitArray& a, const BitArray& b);

/**
 * @brief Checks if two BitArrays are not equal.
 * @param a First BitArray to compare
 * @param b Second BitArray to compare
 * @return true if arrays differ in size or any bit, false otherwise
 */
bool operator!=(const BitArray& a, const BitArray& b);

// Non-member bitwise operators

/**
 * @brief Returns bitwise AND of two BitArrays.
 * @param b1 First BitArray
 * @param b2 Second BitArray
 * @return New BitArray containing the AND result
 * @throws std::invalid_argument if sizes don't match
 */
BitArray operator&(const BitArray& b1, const BitArray& b2);

/**
 * @brief Returns bitwise OR of two BitArrays.
 * @param b1 First BitArray
 * @param b2 Second BitArray
 * @return New BitArray containing the OR result
 * @throws std::invalid_argument if sizes don't match
 */
BitArray operator|(const BitArray& b1, const BitArray& b2);

/**
 * @brief Returns bitwise XOR of two BitArrays.
 * @param b1 First BitArray
 * @param b2 Second BitArray
 * @return New BitArray containing the XOR result
 * @throws std::invalid_argument if sizes don't match
 */
BitArray operator^(const BitArray& b1, const BitArray& b2);

#endif // BITARRAY_H