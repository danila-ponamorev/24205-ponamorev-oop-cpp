#include "bit_array.h"
#include <gtest/gtest.h>

TEST(BitArray, DefaultConstructor) {
    BitArray ba;
    EXPECT_TRUE(ba.empty());
    EXPECT_EQ(0, ba.size());
}

TEST(BitArray, ConstructorWithBits) {
    BitArray ba(5, 0b10110);
    EXPECT_EQ(5, ba.size());
    EXPECT_FALSE(ba.empty());
    EXPECT_EQ("01101", ba.to_string()); // Младшие биты first
}

TEST(BitArray, CopyConstructor) {
    BitArray original(3, 0b101);
    BitArray copy(original);
    EXPECT_EQ(original.size(), copy.size());
    EXPECT_EQ(original.to_string(), copy.to_string());
}

TEST(BitArray, AssignmentOperator) {
    BitArray original(4, 0b1100);
    BitArray copy;
    copy = original;
    EXPECT_EQ(original.size(), copy.size());
    EXPECT_EQ(original.to_string(), copy.to_string());
}

TEST(BitArray, Swap) {
    BitArray a(3, 0b101);
    BitArray b(2, 0b10);
    a.swap(b);
    EXPECT_EQ(2, a.size());
    EXPECT_EQ(3, b.size());
    EXPECT_EQ("01", a.to_string());
    EXPECT_EQ("101", b.to_string());
}

TEST(BitArray, Resize) {
    BitArray ba(3, 0b101);
    ba.resize(5, true);
    EXPECT_EQ(5, ba.size());
    EXPECT_EQ("10111", ba.to_string());
}

TEST(BitArray, Clear) {
    BitArray ba(4, 0b1111);
    ba.clear();
    EXPECT_TRUE(ba.empty());
    EXPECT_EQ(0, ba.size());
}

TEST(BitArray, PushBack) {
    BitArray ba;
    ba.push_back(true);
    ba.push_back(false);
    EXPECT_EQ(2, ba.size());
    EXPECT_EQ("10", ba.to_string());
}

TEST(BitArray, BitwiseAND) {
    BitArray a(3, 0b101); // "101" в строковом представлении
    BitArray b(3, 0b011); // "110" в строковом представлении
    a &= b;
    // 1&1=1, 0&1=0, 1&0=0 -> "100"
    EXPECT_EQ("100", a.to_string());
}

TEST(BitArray, BitwiseOR) {
    BitArray a(3, 0b101); // "101"
    BitArray b(3, 0b011); // "110" 
    a |= b;
    // 1|1=1, 0|1=1, 1|0=1 -> "111"
    EXPECT_EQ("111", a.to_string());
}

TEST(BitArray, BitwiseXOR) {
    BitArray a(3, 0b101); // "101"
    BitArray b(3, 0b011); // "110"
    a ^= b;
    // 1^1=0, 0^1=1, 1^0=1 -> "011"
    EXPECT_EQ("011", a.to_string());
}

TEST(BitArray, LeftShift) {
    BitArray ba(4, 0b1101); // "1011" в строковом представлении
    ba <<= 2;
    // Сдвиг влево на 2: "1100" 
    EXPECT_EQ("0010", ba.to_string());
}

TEST(BitArray, RightShift) {
    BitArray ba(4, 0b1101); // "1011" в строковом представлении
    ba >>= 1;
    // Сдвиг вправо на 1: "0101"
    EXPECT_EQ("0110", ba.to_string());
}

TEST(BitArray, SetBit) {
    BitArray ba(3, 0b101); // "101"
    ba.set(1, false); // Устанавливаем бит 1 в false (уже false)
    EXPECT_EQ("101", ba.to_string());
    
    ba.set(0, false); // Устанавливаем бит 0 в false
    EXPECT_EQ("001", ba.to_string());
}

TEST(BitArray, ResetBit) {
    BitArray ba(3, 0b101); // "101"
    ba.reset(0); // Сбрасываем бит 0
    EXPECT_EQ("001", ba.to_string());
}

TEST(BitArray, Any) {
    BitArray ba1(3, 0b000);
    BitArray ba2(3, 0b010);
    EXPECT_FALSE(ba1.any());
    EXPECT_TRUE(ba2.any());
}

TEST(BitArray, None) {
    BitArray ba1(3, 0b000);
    BitArray ba2(3, 0b001);
    EXPECT_TRUE(ba1.none());
    EXPECT_FALSE(ba2.none());
}

TEST(BitArray, Count) {
    BitArray ba(5, 0b11010); // "01011" -> 3 единицы
    EXPECT_EQ(3, ba.count());
}

TEST(BitArray, IndexOperator) {
    BitArray ba(3, 0b101); // "101"
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST(BitArray, ToString) {
    BitArray ba(4, 0b1010); // "0101"
    EXPECT_EQ("0101", ba.to_string());
}

TEST(BitArray, Exceptions) {
    BitArray ba(3);
    EXPECT_THROW(ba.set(5), std::out_of_range);
    EXPECT_THROW(ba[5], std::out_of_range);
    EXPECT_THROW(BitArray(-1), std::invalid_argument);
    
    BitArray a(3), b(4);
    EXPECT_THROW(a &= b, std::invalid_argument);
    EXPECT_THROW(a |= b, std::invalid_argument);
    EXPECT_THROW(a ^= b, std::invalid_argument);
}

TEST(BitArray, GlobalOperators) {
    BitArray a(3, 0b101); // "101"
    BitArray b(3, 0b011); // "110"
    
    BitArray and_result = a & b;
    EXPECT_EQ("100", and_result.to_string());
    
    BitArray or_result = a | b;
    EXPECT_EQ("111", or_result.to_string());
    
    BitArray xor_result = a ^ b;
    EXPECT_EQ("011", xor_result.to_string());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}