#include <iostream>
#include "bit_array.h"

int main(void) {
    BitArray array(8, 255u);
    std::cout << array.to_string() << std::endl;
    array[2] = 0;
    array[3] = 0;
    array[7] = 0;
    std::cout << array.to_string() << std::endl;

    return 0;
}
