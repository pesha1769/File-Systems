#include "converter.hpp"


static inline uint8_t get_10xxxxxx(uint32_t x) {
    return 0b10000000 + (uint8_t)(x & 0b00111111);
}

static inline bool is_10xxxxxx(uint8_t x) {
    return ((x & (1 << 7)) && !(x & (1 << 6)));
}


using namespace std;


int pack_utf32_to_utf8 (uint32_t utf32_symbol, vector<uint8_t> &result) {
    /*
    Determines how many non-null bytes (8-bit octets) the 32-bit UTF-32 encoded
    symbol has and packs it into the according number of 8-bit UTF-8 characters.
    */

    if (utf32_symbol <= (uint32_t)LAST_CODE_POINT_FOR_ONE_BYTE)
        result.push_back((uint8_t)utf32_symbol);
    else if (utf32_symbol <= (uint32_t)LAST_CODE_POINT_FOR_TWO_BYTES) {
        result.push_back((uint8_t)(0b11000000 + (uint8_t)(utf32_symbol >> 6)));
        result.push_back(get_10xxxxxx(utf32_symbol));
    }
    else if (utf32_symbol <= (uint32_t)LAST_CODE_POINT_FOR_THREE_BYTES) {
        result.push_back((uint8_t)(0b11100000 + (uint8_t)(utf32_symbol >> 12)));
        result.push_back(get_10xxxxxx(utf32_symbol >> 6));
        result.push_back(get_10xxxxxx(utf32_symbol));
    }
    else if (utf32_symbol <= (uint32_t)LAST_CODE_POINT_FOR_FOUR_BYTES) {
        result.push_back((uint8_t)(0b11110000 + (uint8_t)(utf32_symbol >> 18)));
        result.push_back(get_10xxxxxx(utf32_symbol >> 12));
        result.push_back(get_10xxxxxx(utf32_symbol >> 6));
        result.push_back(get_10xxxxxx(utf32_symbol));
    }
    else {
        throw runtime_error(Formatter()
                                 << "This character cannot be fit into UTF-32: "
                                 << utf32_symbol);
        return ERROR;
    }

    return OK;
}

vector<uint8_t>to_utf8(const vector<uint32_t> &utf32_string) {
    vector<uint8_t> result;
    for (int i = 0; i < utf32_string.size(); ++i) {
        try {
            pack_utf32_to_utf8(utf32_string[i], result);
        } catch (const std::runtime_error& error) {
            throw error;
        }
    }

    return result;
}

vector<uint32_t>from_utf8(const vector<uint8_t> &utf8_string) {
    vector<uint32_t> result;
    uint32_t temp = 0;

    for (int i = 0; i < utf8_string.size();)
    {
        if (((utf8_string[i] >> 3) ^ 0b00011110) == 0) {
            temp += ((uint32_t)utf8_string[i++] & 0b00000111) << 18;
            for (int j = 0; j < 3; ++j) {
                if (is_10xxxxxx(utf8_string[i]))
                    temp += ((uint32_t)utf8_string[i++] & 0b00111111)
                            << (2 - j)*6;
                else {
                    throw runtime_error(Formatter() << "Expected 4 bytes but "
                                        << "byte #" << j+2 << " is "
                                        << bitset<8>(utf8_string[i]));
                }
            }
        }
        else if (((utf8_string[i] >> 4) ^ 0b0001110) == 0) {
            temp += ((uint32_t)utf8_string[i++] & 0b00001111) << 12;
            for (int j = 0; j < 2; ++j) {
                if (is_10xxxxxx(utf8_string[i]))
                    temp += ((uint32_t)utf8_string[i++] & 0b00111111)
                            << (1 - j)*6;
                else {
                    throw runtime_error(Formatter() << "Expected 3 bytes but "
                                        << "byte #" << j+2 << " is "
                                        << bitset<8>(utf8_string[i]));
                }
            }
        }
        else if (((utf8_string[i] >> 5) ^ 0b000110) == 0) {
            temp += ((uint32_t)utf8_string[i++] & 0b00011111) << 6;
            if (is_10xxxxxx(utf8_string[i]))
                temp += ((uint32_t)utf8_string[i++] & 0b00111111);
            else {
                throw runtime_error(Formatter() << "Expected 2 bytes but "
                                    << "byte #" << 2 << " is "
                                    << bitset<8>(utf8_string[i]));
            }
        }
        else
            temp = (uint32_t)utf8_string[i++];
        result.push_back(temp);
        temp = 0;
    }

    return result;
}
