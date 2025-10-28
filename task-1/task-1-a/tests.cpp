#include "bit_array.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(BitArrayTest, DefaultConstructor) {
    BitArray ba;
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

TEST(BitArrayTest, ConstructorWithSize) {
    BitArray ba(10);
    EXPECT_EQ(ba.size(), 10);
    EXPECT_FALSE(ba.empty());
    
    // Check that all bits are initialized to 0
    for (int i = 0; i < 10; ++i) {
        EXPECT_FALSE(ba[i]);
    }
}

TEST(BitArrayTest, ConstructorWithSizeAndValue) {
    BitArray ba(5, 0b10101); // Binary: 10101
    EXPECT_EQ(ba.size(), 5);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);
    EXPECT_TRUE(ba[4]);
}

TEST(BitArrayTest, ConstructorWithNegativeSize) {
    EXPECT_THROW(BitArray(-1), std::invalid_argument);
}

TEST(BitArrayTest, CopyConstructor) {
    BitArray ba1(5, 0b10101);
    BitArray ba2(ba1);
    
    EXPECT_EQ(ba1.size(), ba2.size());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(ba1[i], ba2[i]);
    }
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray ba1(5, 0b10101);
    BitArray ba2;
    ba2 = ba1;
    
    EXPECT_EQ(ba1.size(), ba2.size());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(ba1[i], ba2[i]);
    }
}

TEST(BitArrayTest, SelfAssignment) {
    BitArray ba(5, 0b10101);
    ba = ba;
    
    EXPECT_EQ(ba.size(), 5);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);
    EXPECT_TRUE(ba[4]);
}

TEST(BitArrayTest, Swap) {
    BitArray ba1(3, 0b101);
    BitArray ba2(4, 0b1100);
    
    ba1.swap(ba2);
    
    EXPECT_EQ(ba1.size(), 4);
    EXPECT_EQ(ba2.size(), 3);
    
    EXPECT_FALSE(ba1[0]);
    EXPECT_FALSE(ba1[1]);
    EXPECT_TRUE(ba1[2]);
    EXPECT_TRUE(ba1[3]);
    
    EXPECT_TRUE(ba2[0]);
    EXPECT_FALSE(ba2[1]);
    EXPECT_TRUE(ba2[2]);
}

TEST(BitArrayTest, ResizeExpand) {
    BitArray ba(3, 0b101);
    ba.resize(5, true);
    
    EXPECT_EQ(ba.size(), 5);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_TRUE(ba[3]); // New bit
    EXPECT_TRUE(ba[4]); // New bit
}

TEST(BitArrayTest, ResizeShrink) {
    BitArray ba(5, 0b10101);
    ba.resize(3);
    
    EXPECT_EQ(ba.size(), 3);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST(BitArrayTest, ResizeNegative) {
    BitArray ba;
    EXPECT_THROW(ba.resize(-1), std::invalid_argument);
}

TEST(BitArrayTest, Clear) {
    BitArray ba(5, 0b10101);
    ba.clear();
    
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

TEST(BitArrayTest, PushBack) {
    BitArray ba;
    ba.push_back(true);
    ba.push_back(false);
    ba.push_back(true);
    
    EXPECT_EQ(ba.size(), 3);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST(BitArrayTest, SetSingleBit) {
    BitArray ba(3);
    ba.set(1, true);
    
    EXPECT_FALSE(ba[0]);
    EXPECT_TRUE(ba[1]);
    EXPECT_FALSE(ba[2]);
}

TEST(BitArrayTest, SetAllBits) {
    BitArray ba(3);
    ba.set();
    
    EXPECT_TRUE(ba[0]);
    EXPECT_TRUE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST(BitArrayTest, ResetSingleBit) {
    BitArray ba(3);
    ba.set();
    ba.reset(1);
    
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST(BitArrayTest, ResetAllBits) {
    BitArray ba(3);
    ba.set();
    ba.reset();
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_FALSE(ba[i]);
    }
}

TEST(BitArrayTest, IndexOutOfBounds) {
    BitArray ba(3);
    EXPECT_THROW(ba[3], std::out_of_range);
    EXPECT_THROW(ba.set(3), std::out_of_range);
    EXPECT_THROW(ba.reset(3), std::out_of_range);
    EXPECT_THROW(ba[-1], std::out_of_range);
}

TEST(BitArrayTest, AnyAndNone) {
    BitArray ba1(3); // All zeros
    EXPECT_FALSE(ba1.any());
    EXPECT_TRUE(ba1.none());
    
    BitArray ba2(3);
    ba2.set(1, true); // One one
    EXPECT_TRUE(ba2.any());
    EXPECT_FALSE(ba2.none());
    
    BitArray ba3(3);
    ba3.set(); // All ones
    EXPECT_TRUE(ba3.any());
    EXPECT_FALSE(ba3.none());
}

TEST(BitArrayTest, Count) {
    BitArray ba(5, 0b10101);
    EXPECT_EQ(ba.count(), 3);
    
    BitArray ba2(3);
    EXPECT_EQ(ba2.count(), 0);
    
    BitArray ba3(3);
    ba3.set();
    EXPECT_EQ(ba3.count(), 3);
}

TEST(BitArrayTest, BitwiseNOT) {
    BitArray ba(3, 0b101);
    BitArray result = ~ba;
    
    EXPECT_FALSE(result[0]);
    EXPECT_TRUE(result[1]);
    EXPECT_FALSE(result[2]);
}

TEST(BitArrayTest, BitwiseAND) {
    BitArray ba1(4, 0b1010);
    BitArray ba2(4, 0b1100);
    
    ba1 &= ba2;
    
    EXPECT_FALSE(ba1[0]);
    EXPECT_FALSE(ba1[1]);
    EXPECT_FALSE(ba1[2]);
    EXPECT_FALSE(ba1[3]);
}

TEST(BitArrayTest, BitwiseOR) {
    BitArray ba1(4, 0b1010);
    BitArray ba2(4, 0b1100);
    
    ba1 |= ba2;
    
    EXPECT_FALSE(ba1[0]);
    EXPECT_TRUE(ba1[1]);
    EXPECT_TRUE(ba1[2]);
    EXPECT_FALSE(ba1[3]);
}

TEST(BitArrayTest, BitwiseXOR) {
    BitArray ba1(4, 0b1010);
    BitArray ba2(4, 0b1100);
    
    ba1 ^= ba2;
    
    EXPECT_FALSE(ba1[0]);
    EXPECT_TRUE(ba1[1]);
    EXPECT_TRUE(ba1[2]);
    EXPECT_FALSE(ba1[3]);
}

TEST(BitArrayTest, BitwiseOperationsDifferentSizes) {
    BitArray ba1(3);
    BitArray ba2(4);
    
    EXPECT_THROW(ba1 &= ba2, std::invalid_argument);
    EXPECT_THROW(ba1 |= ba2, std::invalid_argument);
    EXPECT_THROW(ba1 ^= ba2, std::invalid_argument);
}

TEST(BitArrayTest, LeftShift) {
    BitArray ba(4, 0b1101);
    ba <<= 1;
    
    EXPECT_FALSE(ba[0]);
    EXPECT_TRUE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);
}

TEST(BitArrayTest, RightShift) {
    BitArray ba(4, 0b1101);
    ba >>= 1;
    
    EXPECT_FALSE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_TRUE(ba[3]);
}

TEST(BitArrayTest, ShiftBeyondSize) {
    BitArray ba(3, 0b101);
    ba <<= 5;
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_FALSE(ba[i]);
    }
    
    BitArray ba2(3, 0b101);
    ba2 >>= 5;
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_FALSE(ba2[i]);
    }
}

TEST(BitArrayTest, ShiftNegative) {
    BitArray ba(3);
    EXPECT_THROW(ba <<= -1, std::invalid_argument);
    EXPECT_THROW(ba >>= -1, std::invalid_argument);
}

TEST(BitArrayTest, NonMemberOperators) {
    BitArray ba1(4, 0b1010);
    BitArray ba2(4, 0b1100);
    
    BitArray and_result = ba1 & ba2;
    BitArray or_result = ba1 | ba2;
    BitArray xor_result = ba1 ^ ba2;
    
    EXPECT_FALSE(and_result[0]);
    EXPECT_FALSE(and_result[1]);
    EXPECT_FALSE(and_result[2]);
    EXPECT_FALSE(and_result[3]);
    
    EXPECT_FALSE(or_result[0]);
    EXPECT_TRUE(or_result[1]);
    EXPECT_TRUE(or_result[2]);
    EXPECT_FALSE(or_result[3]);
    
    EXPECT_FALSE(xor_result[0]);
    EXPECT_TRUE(xor_result[1]);
    EXPECT_TRUE(xor_result[2]);
    EXPECT_FALSE(xor_result[3]);
}

TEST(BitArrayTest, Equality) {
    BitArray ba1(3, 0b101);
    BitArray ba2(3, 0b101);
    BitArray ba3(3, 0b100);
    BitArray ba4(4, 0b1010);
    
    EXPECT_TRUE(ba1 == ba2);
    EXPECT_FALSE(ba1 == ba3);
    EXPECT_FALSE(ba1 == ba4);
    EXPECT_TRUE(ba1 != ba3);
    EXPECT_TRUE(ba1 != ba4);
}

TEST(BitArrayTest, ToString) {
    BitArray ba(4, 0b1010);
    std::string str = ba.to_string();
    EXPECT_EQ(str, "0101"); // Note: bits are stored LSB to MSB in string representation
    
    BitArray ba2(3, 0b101);
    EXPECT_EQ(ba2.to_string(), "101");
}

TEST(BitArrayTest, LargeArray) {
    BitArray ba(1000);
    EXPECT_EQ(ba.size(), 1000);
    
    ba.set(500, true);
    EXPECT_TRUE(ba[500]);
    EXPECT_FALSE(ba[501]);
    
    EXPECT_EQ(ba.count(), 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
