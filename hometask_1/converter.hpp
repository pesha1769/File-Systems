#include <cstdint>
#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept>
#define ERROR -1
#define OK 0

#define LAST_CODE_POINT_FOR_ONE_BYTE 0x7f
#define LAST_CODE_POINT_FOR_TWO_BYTES 0x7ff
#define LAST_CODE_POINT_FOR_THREE_BYTES 0xffff
#define LAST_CODE_POINT_FOR_FOUR_BYTES 0x10ffff

std::vector<uint8_t>to_utf8(const std::vector<uint32_t> &x);
std::vector<uint32_t>from_utf8(const std::vector<uint8_t> &x);

