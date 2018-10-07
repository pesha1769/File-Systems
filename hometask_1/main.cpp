#include <bitset>
#include <iconv.h>
#include <iostream>
#include "converter.hpp"
#include <stdexcept>

#define ERROR_MSG "conversion FAILED"
#define ERROR -1

using namespace std;

std::vector<uint32_t> original_string{L'𐍈'};
				 
 
int main(int argc, char const *argv[])
{
    // UTF-32
    cout << endl << "Original string (UTF-32):" << endl;
    for (int i = 0; i < original_string.size(); ++i)
        cout << original_string[i] << " - " << bitset<32>(original_string[i]) << endl;

    // UTF-32 -> UTF-8
    std::vector<uint8_t> utf8_string;
    try{
        utf8_string = to_utf8(original_string);
    } catch (const runtime_error& error) {
        cerr << error.what() << endl;
        return ERROR;
    }

    // UTF-8
    cout << endl << "Converted to UTF-8:" << endl;
    for (int i = 0; i < utf8_string.size(); ++i)
        cout << bitset<8>(utf8_string[i]) << endl;


    // UTF-8 -> UTF-32
    std::vector<uint32_t> utf32_string;
    try {
        utf32_string = from_utf8(utf8_string);
    } catch (const std::runtime_error& error) {
        cerr << error.what() << endl;
        return ERROR;
    }
    // UTF-32 
    cout << endl << "Converted back to UTF-32:" << endl;
    for (int i = 0; i < utf32_string.size(); ++i)
        cout << utf32_string[i] << " - " << bitset<32>(utf32_string[i]) << endl;

    // UTF-32 back check
    if (original_string != utf32_string) {
        cerr << "from_utf8 FAILED" << endl;
        return ERROR;
    }
    else
        cout << endl << "Finished" << endl;

    return 0;
}
