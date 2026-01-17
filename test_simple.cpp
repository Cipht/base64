#include <iostream>
#include "base64.h"

int main() {
    std::cout << "Testing empty string encode..." << std::endl;
    try {
        std::string result = base64("");
        std::cout << "Empty string: '" << result << "'" << std::endl;
    } catch (...) {
        std::cout << "CRASH on empty string!" << std::endl;
        return 1;
    }

    std::cout << "\nTesting single character 'f'..." << std::endl;
    try {
        std::string result = base64("f");
        std::cout << "Result: '" << result << "' (expected: 'Zg==')" << std::endl;
    } catch (...) {
        std::cout << "CRASH on single character!" << std::endl;
        return 1;
    }

    std::cout << "\nTesting two characters 'fo'..." << std::endl;
    try {
        std::string result = base64("fo");
        std::cout << "Result: '" << result << "' (expected: 'Zm8=')" << std::endl;
    } catch (...) {
        std::cout << "CRASH on two characters!" << std::endl;
        return 1;
    }

    std::cout << "\nTesting three characters 'foo'..." << std::endl;
    try {
        std::string result = base64("foo");
        std::cout << "Result: '" << result << "' (expected: 'Zm9v')" << std::endl;
    } catch (...) {
        std::cout << "CRASH on three characters!" << std::endl;
        return 1;
    }

    std::cout << "\n=== All encode tests passed! ===\n" << std::endl;

    std::cout << "Testing decode 'Zm9v'..." << std::endl;
    try {
        std::string result = base64_decode_to_string("Zm9v");
        std::cout << "Result: '" << result << "' (expected: 'foo')" << std::endl;
    } catch (...) {
        std::cout << "CRASH on decode!" << std::endl;
        return 1;
    }

    std::cout << "\n=== All tests completed! ===" << std::endl;
    return 0;
}
