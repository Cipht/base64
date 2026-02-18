#include <iostream>
#include <cassert>
#include <cstring>
#include "base64.h"

// ANSI color codes for output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void test_case(const std::string& name, bool condition) {
    if (condition) {
        std::cout << GREEN << "✓ PASS: " << RESET << name << std::endl;
        tests_passed++;
    } else {
        std::cout << RED << "✗ FAIL: " << RESET << name << std::endl;
        tests_failed++;
    }
}

void test_encoding() {
    std::cout << "\n" << YELLOW << "=== Testing base64 Encoding ===" << RESET << std::endl;

    // RFC 4648 test vectors
    test_case("Encode empty string", base64("") == "");
    test_case("Encode 'f'", base64("f") == "Zg==");
    test_case("Encode 'fo'", base64("fo") == "Zm8=");
    test_case("Encode 'foo'", base64("foo") == "Zm9v");
    test_case("Encode 'foob'", base64("foob") == "Zm9vYg==");
    test_case("Encode 'fooba'", base64("fooba") == "Zm9vYmE=");
    test_case("Encode 'foobar'", base64("foobar") == "Zm9vYmFy");

    // Additional test cases
    test_case("Encode 'Hello, World!'", base64("Hello, World!") == "SGVsbG8sIFdvcmxkIQ==");
    test_case("Encode 'Base64'", base64("Base64") == "QmFzZTY0");
    test_case("Encode 'test'", base64("test") == "dGVzdA==");
    test_case("Encode 'testing'", base64("testing") == "dGVzdGluZw==");
}

void test_encoding_char_array() {
    std::cout << "\n" << YELLOW << "=== Testing base64_encode (char array) ===" << RESET << std::endl;

    char test1[] = "foo";
    test_case("Encode char array 'foo'", base64_encode(test1, 3) == "Zm9v");

    char test2[] = "foob";
    test_case("Encode char array 'foob'", base64_encode(test2, 4) == "Zm9vYg==");

    char test3[] = "foobar";
    test_case("Encode char array 'foobar'", base64_encode(test3, 6) == "Zm9vYmFy");

    // Test with binary data
    char binary[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    std::string encoded = base64_encode(binary, 6);
    test_case("Encode binary data", encoded == "AAECAwQF");
}

void test_decoding_to_string() {
    std::cout << "\n" << YELLOW << "=== Testing base64_decode_to_string ===" << RESET << std::endl;

    // RFC 4648 test vectors
    test_case("Decode 'Zg=='", base64_decode_to_string("Zg==") == "f");
    test_case("Decode 'Zm8='", base64_decode_to_string("Zm8=") == "fo");
    test_case("Decode 'Zm9v'", base64_decode_to_string("Zm9v") == "foo");
    test_case("Decode 'Zm9vYg=='", base64_decode_to_string("Zm9vYg==") == "foob");
    test_case("Decode 'Zm9vYmE='", base64_decode_to_string("Zm9vYmE=") == "fooba");
    test_case("Decode 'Zm9vYmFy'", base64_decode_to_string("Zm9vYmFy") == "foobar");

    // Additional tests
    test_case("Decode 'SGVsbG8sIFdvcmxkIQ=='",
              base64_decode_to_string("SGVsbG8sIFdvcmxkIQ==") == "Hello, World!");
    test_case("Decode 'QmFzZTY0'", base64_decode_to_string("QmFzZTY0") == "Base64");
    test_case("Decode 'dGVzdA=='", base64_decode_to_string("dGVzdA==") == "test");
}

void test_decoding_to_vector() {
    std::cout << "\n" << YELLOW << "=== Testing base64_decode_to_vector ===" << RESET << std::endl;

    std::vector<char> result = base64_decode_to_vector("Zm9v");
    std::string result_str(result.begin(), result.end());
    test_case("Decode 'Zm9v' to vector", result_str == "foo");

    result = base64_decode_to_vector("Zm9vYmFy");
    result_str = std::string(result.begin(), result.end());
    test_case("Decode 'Zm9vYmFy' to vector", result_str == "foobar");

    result = base64_decode_to_vector("SGVsbG8sIFdvcmxkIQ==");
    result_str = std::string(result.begin(), result.end());
    test_case("Decode 'SGVsbG8sIFdvcmxkIQ==' to vector", result_str == "Hello, World!");

    // Test with binary data
    result = base64_decode_to_vector("AAECAwQF");
    bool binary_match = (result.size() == 6 &&
                        result[0] == 0x00 && result[1] == 0x01 &&
                        result[2] == 0x02 && result[3] == 0x03 &&
                        result[4] == 0x04 && result[5] == 0x05);
    test_case("Decode binary data to vector", binary_match);
}

void test_round_trip() {
    std::cout << "\n" << YELLOW << "=== Testing Round-Trip (Encode -> Decode) ===" << RESET << std::endl;

    std::vector<std::string> test_strings = {
        "Hello, World!",
        "The quick brown fox jumps over the lazy dog",
        "1234567890",
        "!@#$%^&*()_+-=[]{}|;':,.<>?",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "a",
        "ab",
        "abc",
        "abcd",
        "abcde"
    };

    for (const auto& str : test_strings) {
        std::string encoded = base64(str);
        std::string decoded = base64_decode_to_string(encoded);
        test_case("Round-trip: '" + str + "'", decoded == str);
    }
}

void test_edge_cases() {
    std::cout << "\n" << YELLOW << "=== Testing Edge Cases ===" << RESET << std::endl;

    // Test strings with different lengths to verify padding
    test_case("Length % 3 == 0 (no padding)", base64("abc") == "YWJj");
    test_case("Length % 3 == 1 (== padding)", base64("a") == "YQ==");
    test_case("Length % 3 == 2 (= padding)", base64("ab") == "YWI=");

    // Test with special characters
    test_case("Encode newline", base64("\n") == "Cg==");
    test_case("Encode tab", base64("\t") == "CQ==");
    test_case("Encode null byte", base64(std::string("\0", 1)) == "AA==");

    // Test with longer strings
    std::string long_str(100, 'A');
    std::string encoded_long = base64(long_str);
    std::string decoded_long = base64_decode_to_string(encoded_long);
    test_case("Round-trip with 100 character string", decoded_long == long_str);
}

void test_base64_alphabet() {
    std::cout << "\n" << YELLOW << "=== Testing Base64 Alphabet Coverage ===" << RESET << std::endl;

    // Test string that should use most of the base64 alphabet
    std::string test_str = "\x00\x10\x83\x10Q\x87 \x92\x8b0\xd3\x8f"
                          "A\x14\x93QU\x97a\x96\x9bq\xd7\x9f"
                          "\x82\x18\xa3\x92Y\xa7\xa2\x9a\xab\xb2\xdb\xaf"
                          "\xc3\x1c\xb3\xd3]\xb7\xe3\x9e\xbb\xf3\xdf\xbf";

    std::string encoded = base64(test_str);

    // Check that encoded string contains various base64 characters
    bool has_uppercase = encoded.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos;
    bool has_lowercase = encoded.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
    bool has_digits = encoded.find_first_of("0123456789") != std::string::npos;
    bool has_plus = encoded.find('+') != std::string::npos;
    bool has_slash = encoded.find('/') != std::string::npos;

    test_case("Encoded output contains uppercase letters", has_uppercase);
    test_case("Encoded output contains lowercase letters", has_lowercase);
    test_case("Encoded output contains digits", has_digits);
    test_case("Encoded output contains '+'", has_plus);
    test_case("Encoded output contains '/'", has_slash);
}

int main() {
    std::cout << YELLOW << "===========================================" << RESET << std::endl;
    std::cout << YELLOW << "   Base64 Library Test Suite" << RESET << std::endl;
    std::cout << YELLOW << "===========================================" << RESET << std::endl;

    test_encoding();
    test_encoding_char_array();
    test_decoding_to_string();
    test_decoding_to_vector();
    test_round_trip();
    test_edge_cases();
    test_base64_alphabet();

    std::cout << "\n" << YELLOW << "===========================================" << RESET << std::endl;
    std::cout << YELLOW << "   Test Summary" << RESET << std::endl;
    std::cout << YELLOW << "===========================================" << RESET << std::endl;
    std::cout << GREEN << "Passed: " << tests_passed << RESET << std::endl;
    std::cout << RED << "Failed: " << tests_failed << RESET << std::endl;
    std::cout << "Total:  " << (tests_passed + tests_failed) << std::endl;

    if (tests_failed == 0) {
        std::cout << "\n" << GREEN << "🎉 All tests passed!" << RESET << std::endl;
        return 0;
    } else {
        std::cout << "\n" << RED << "⚠ Some tests failed!" << RESET << std::endl;
        return 1;
    }
}
