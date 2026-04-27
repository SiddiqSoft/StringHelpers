/*
    StringHelpers : Simple wide, narrow, utf8 conversion functions

    BSD 3-Clause License

    Copyright (c) 2021, Siddiq Software LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "gtest/gtest.h"
#include <iostream>


#include "../include/siddiqsoft/conversion-utils.hpp"

namespace siddiqsoft
{
    /* ConversionUtils tests */

    TEST(ConversionUtils, test1a)
    {
        std::string  sample {"صديق"};
        std::wstring sample_w {L"صديق"};

        try {
            auto intermediate = ConversionUtils::convert_to<char, wchar_t>(sample);
            // EXPECT_EQ(sample_w, intermediate);
            auto roundTrip = ConversionUtils::convert_to<wchar_t, char>(intermediate);
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(ConversionUtils, test1a_w)
    {
        std::wstring sample {L"This is a test."};
        std::string  sample_n {"This is a test."};

        try {
            auto intermediate = ConversionUtils::convert_to<wchar_t, char>(sample);
            EXPECT_EQ(sample_n, intermediate);
            std::wstring roundTrip {ConversionUtils::convert_to<char, wchar_t>(intermediate)};
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(ConversionUtils, test2a)
    {
        std::string  sample {"صديق"};
        std::wstring sample_w {L"صديق"};

        try {
            // Note that we cannot compare the intermediate due to differences in encoding literals between Win32 and Unix systems.
            auto roundTrip {ConversionUtils::convert_to<wchar_t, char>(ConversionUtils::convert_to<char, wchar_t>(sample))};
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(ConversionUtils, test2a_w)
    {
        // The reason we do not get sample == sample is due to the idiosyncracy of the
        // various platforms and their encoding scheme. Notably Apple and Windows/Linux differ
        // in their textual encoding.
        // In order to prevent constant mutation by git we're using the raw form.
        std::wstring sample {L"صديق"};
        std::string sample_n {"\xD8\xB5\xD8\xAF\xD9\x8A\xD9\x82"};

        try {
            auto intermediate = ConversionUtils::convert_to<wchar_t, char>(sample);
            EXPECT_EQ(sample_n, intermediate);
            auto roundTrip = ConversionUtils::convert_to<char, wchar_t>(intermediate);
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }


    TEST(ConversionUtils, test4)
    {
#if defined(WIN32)
        std::string  sample_0 {"\x48\x65\x6c\x6c\x6f\x2c\x20\xe4\xb8\x96\xe7\x95\x8c"};
        std::string  sample_0rt {"\x48\x65\x6c\x6c\x6f\x2c\x20\xe4\xb8\x96\xe7\x95\x8c"};
        std::wstring sample_w {L"Hello, 世界"};
#elif defined(__APPLE__)
        std::string  sample_0 {"Banc\xC3\xA9"};
        std::string  sample_0rt {"Bancé"};
        std::wstring sample_w {L"Bancé"};
#elif defined(__linux__)
        std::string  sample_0 {"Hello, 世界"};
        std::string  sample_0rt {"Hello, \xE4\xB8\x96\xE7\x95\x8C"};
        std::wstring sample_w {L"Hello, 世界"};
#endif
        auto intermediate = ConversionUtils::convert_to<char, wchar_t>(sample_0);
        EXPECT_EQ(sample_w, intermediate);
        auto intermediate1 = ConversionUtils::convert_to<wchar_t, char>(intermediate);
        EXPECT_EQ(sample_0rt, intermediate1);
    }


    TEST(ConversionUtils, test5_n)
    {
#if defined(WIN32)
        std::string  sample_0 {"\x48\x65\x6c\x6c\x6f\x2c\x20\xe4\xb8\x96\xe7\x95\x8c"};
        std::string  sample_0rt {"Hello, ä¸\x96ç\x95\x8c"};
        std::wstring sample_w {L"Hello, 世界"};
#elif defined(__APPLE__)
        std::string  sample_0 {"Banc\xC3\xA9"};
        std::string  sample_0rt {"Bancé"};
        std::wstring sample_w {L"Bancé"};
#elif defined(__linux__)
        std::string  sample_0 {"Hello, 世界"};
        std::string  sample_0rt {"Hello, \xE4\xB8\x96\xE7\x95\x8C"};
        std::wstring sample_w {L"Hello, 世界"};
#endif
        std::string result= ConversionUtils::convert_to<char,char>(sample_0);
        EXPECT_EQ(sample_0, result);
    }


    TEST(ConversionUtils, test5_w)
    {
#if defined(WIN32)
        std::string  sample_0 {"\x48\x65\x6c\x6c\x6f\x2c\x20\xe4\xb8\x96\xe7\x95\x8c"};
        std::string  sample_0rt {"Hello, ä¸\x96ç\x95\x8c"};
        std::wstring sample_w {L"Hello, 世界"};
#elif defined(__APPLE__)
        std::string  sample_0 {"Banc\xC3\xA9"};
        std::string  sample_0rt {"Bancé"};
        std::wstring sample_w {L"Bancé"};
#elif defined(__linux__)
        std::string  sample_0 {"Hello, 世界"};
        std::string  sample_0rt {"Hello, \xE4\xB8\x96\xE7\x95\x8C"};
        std::wstring sample_w {L"Hello, 世界"};
#endif
        std::wstring result= ConversionUtils::convert_to<wchar_t,wchar_t>(sample_w);
        EXPECT_EQ(sample_w, result);
    }


    // =========================================================================
    // Empty string tests - cover all four template instantiation paths
    // =========================================================================

    TEST(ConversionUtils, empty_char_to_wchar)
    {
        std::string  src;
        std::wstring result = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_TRUE(result.empty());
    }

    TEST(ConversionUtils, empty_wchar_to_char)
    {
        std::wstring src;
        std::string  result = ConversionUtils::convert_to<wchar_t, char>(src);
        EXPECT_TRUE(result.empty());
    }

    TEST(ConversionUtils, empty_char_to_char)
    {
        std::string src;
        std::string result = ConversionUtils::convert_to<char, char>(src);
        EXPECT_TRUE(result.empty());
    }

    TEST(ConversionUtils, empty_wchar_to_wchar)
    {
        std::wstring src;
        std::wstring result = ConversionUtils::convert_to<wchar_t, wchar_t>(src);
        EXPECT_TRUE(result.empty());
    }


    // =========================================================================
    // Single character tests
    // =========================================================================

    TEST(ConversionUtils, single_ascii_char_to_wchar)
    {
        std::string  src {"A"};
        std::wstring expected {L"A"};
        auto         result = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_EQ(expected, result);
    }

    TEST(ConversionUtils, single_ascii_wchar_to_char)
    {
        std::wstring src {L"A"};
        std::string  expected {"A"};
        auto         result = ConversionUtils::convert_to<wchar_t, char>(src);
        EXPECT_EQ(expected, result);
    }

    TEST(ConversionUtils, single_ascii_char_to_char)
    {
        std::string src {"Z"};
        auto        result = ConversionUtils::convert_to<char, char>(src);
        EXPECT_EQ(src, result);
    }

    TEST(ConversionUtils, single_ascii_wchar_to_wchar)
    {
        std::wstring src {L"Z"};
        auto         result = ConversionUtils::convert_to<wchar_t, wchar_t>(src);
        EXPECT_EQ(src, result);
    }


    // =========================================================================
    // Pure ASCII round-trip
    // =========================================================================

    TEST(ConversionUtils, ascii_roundtrip_narrow_wide_narrow)
    {
        std::string src {"The quick brown fox jumps over the lazy dog."};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    TEST(ConversionUtils, ascii_roundtrip_wide_narrow_wide)
    {
        std::wstring src {L"The quick brown fox jumps over the lazy dog."};
        auto         narrow = ConversionUtils::convert_to<wchar_t, char>(src);
        auto         wide   = ConversionUtils::convert_to<char, wchar_t>(narrow);
        EXPECT_EQ(src, wide);
    }


    // =========================================================================
    // Strings with whitespace / special ASCII characters
    // =========================================================================

    TEST(ConversionUtils, whitespace_chars_char_to_wchar)
    {
        std::string  src {"line1\nline2\ttab\r\n"};
        std::wstring expected {L"line1\nline2\ttab\r\n"};
        auto         result = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_EQ(expected, result);
    }

    TEST(ConversionUtils, whitespace_chars_wchar_to_char)
    {
        std::wstring src {L"line1\nline2\ttab\r\n"};
        std::string  expected {"line1\nline2\ttab\r\n"};
        auto         result = ConversionUtils::convert_to<wchar_t, char>(src);
        EXPECT_EQ(expected, result);
    }


    // =========================================================================
    // UTF-8 multi-byte sequences of varying lengths
    // =========================================================================

    // 2-byte UTF-8: Latin characters with diacritics (e-acute = U+00E9)
    TEST(ConversionUtils, utf8_2byte_roundtrip)
    {
        std::string src {"\xC3\xA9"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(1u, wide.size());
    }

    // 3-byte UTF-8: CJK character (U+4E16)
    TEST(ConversionUtils, utf8_3byte_roundtrip)
    {
        std::string src {"\xE4\xB8\x96"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(1u, wide.size());
    }

    // 4-byte UTF-8: Emoji (U+1F600)
    TEST(ConversionUtils, utf8_4byte_roundtrip)
    {
        std::string src {"\xF0\x9F\x98\x80"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Mixed multi-byte: 1-byte + 2-byte + 3-byte + 4-byte
    TEST(ConversionUtils, utf8_mixed_multibyte_roundtrip)
    {
        std::string src {"A\xC3\xA9\xE4\xB8\x96\xF0\x9F\x98\x80"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }


    // =========================================================================
    // Longer strings to exercise buffer handling
    // =========================================================================

    TEST(ConversionUtils, long_ascii_roundtrip)
    {
        std::string src(10000, 'x');
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(10000u, wide.size());
    }

    TEST(ConversionUtils, long_unicode_roundtrip)
    {
        std::string unit {"\xE4\xB8\x96"};
        std::string src;
        src.reserve(3000);
        for (int i = 0; i < 1000; ++i) src += unit;

        auto wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(1000u, wide.size());
    }


    // =========================================================================
    // Identity conversion preserves content exactly
    // =========================================================================

    TEST(ConversionUtils, identity_char_preserves_content)
    {
        std::string src {"Hello\xC3\xA9World"};
        auto        result = ConversionUtils::convert_to<char, char>(src);
        EXPECT_EQ(src, result);
        EXPECT_EQ(src.size(), result.size());
    }

    TEST(ConversionUtils, identity_wchar_preserves_content)
    {
        std::wstring src {L"Hello\x00E9World"};
        auto         result = ConversionUtils::convert_to<wchar_t, wchar_t>(src);
        EXPECT_EQ(src, result);
        EXPECT_EQ(src.size(), result.size());
    }


    // =========================================================================
    // Numeric and punctuation characters
    // =========================================================================

    TEST(ConversionUtils, digits_and_punctuation_roundtrip)
    {
        std::string src {"0123456789 !@#$%^&*()_+-=[]{}|;':\",./<>?"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }


    // =========================================================================
    // Various scripts round-trip
    // =========================================================================

    TEST(ConversionUtils, cyrillic_roundtrip)
    {
        std::string src {"\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    TEST(ConversionUtils, japanese_roundtrip)
    {
        std::string src {"\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    TEST(ConversionUtils, korean_roundtrip)
    {
        std::string src {"\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    TEST(ConversionUtils, thai_roundtrip)
    {
        std::string src {"\xE0\xB8\xAA\xE0\xB8\xA7\xE0\xB8\xB1\xE0\xB8\xAA\xE0\xB8\x94\xE0\xB8\xB5"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }


    // =========================================================================
    // Wide-originated conversions (start from wchar_t literals)
    // =========================================================================

    TEST(ConversionUtils, wide_latin_extended_to_narrow_roundtrip)
    {
        std::wstring src {L"\x00FC\x00F6\x00E4"};
        auto         narrow = ConversionUtils::convert_to<wchar_t, char>(src);
        auto         wide   = ConversionUtils::convert_to<char, wchar_t>(narrow);
        EXPECT_EQ(src, wide);
    }

    TEST(ConversionUtils, wide_cjk_to_narrow_roundtrip)
    {
        std::wstring src {L"\x4E16\x754C"};
        auto         narrow = ConversionUtils::convert_to<wchar_t, char>(src);
        auto         wide   = ConversionUtils::convert_to<char, wchar_t>(narrow);
        EXPECT_EQ(src, wide);
    }


    // =========================================================================
    // Strings with spaces only
    // =========================================================================

    TEST(ConversionUtils, spaces_only_roundtrip)
    {
        std::string src {"     "};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(5u, wide.size());
    }


    // =========================================================================
    // Default template parameters (char -> wchar_t)
    // =========================================================================

    TEST(ConversionUtils, default_template_params)
    {
        std::string src {"hello"};
        auto result = ConversionUtils::convert_to(src);
        EXPECT_EQ(L"hello", result);
    }

    // =========================================================================
    // Additional edge case and coverage tests
    // =========================================================================

    // Test with Hebrew script
    TEST(ConversionUtils, hebrew_roundtrip)
    {
        std::string src {"\xD7\x9E\xD7\x95\xD7\xA9\xD7\x94"};  // "משה" (Moses)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with Greek script
    TEST(ConversionUtils, greek_roundtrip)
    {
        std::string src {"\xCE\x95\xCE\xBB\xCE\xBB\xCE\xAC\xCE\xB4\xCE\xB1"};  // "Ελλάδα" (Greece)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with Devanagari script (Hindi)
    TEST(ConversionUtils, devanagari_roundtrip)
    {
        std::string src {"\xE0\xA4\xA8\xE0\xA4\xAE\xE0\xA4\xB8\xE0\xA5\x8D\xE0\xA4\xA4\xE0\xA5\x87"};  // "नमस्ते" (Hello in Hindi)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with Arabic script
    TEST(ConversionUtils, arabic_roundtrip)
    {
        std::string src {"\xD9\x85\xD8\xB1\xD8\xAD\xD8\xA8\xD8\xA7"};  // "مرحبا" (Hello in Arabic)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with Vietnamese (combining diacritics)
    TEST(ConversionUtils, vietnamese_roundtrip)
    {
        std::string src {"\xE1\xBB\x8F"};  // "ỏ" (Vietnamese character with combining marks)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test multiple consecutive conversions (char -> wide -> char -> wide)
    TEST(ConversionUtils, multiple_consecutive_conversions)
    {
        std::string original {"Hello, 世界"};
        auto wide1   = ConversionUtils::convert_to<char, wchar_t>(original);
        auto narrow1 = ConversionUtils::convert_to<wchar_t, char>(wide1);
        auto wide2   = ConversionUtils::convert_to<char, wchar_t>(narrow1);
        auto narrow2 = ConversionUtils::convert_to<wchar_t, char>(wide2);
        EXPECT_EQ(original, narrow2);
    }

    // Test with newlines and carriage returns mixed with Unicode
    TEST(ConversionUtils, unicode_with_line_endings)
    {
        std::string src {"Line1: \xE4\xB8\x96\r\nLine2: \xE7\x95\x8C"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with tabs and Unicode
    TEST(ConversionUtils, unicode_with_tabs)
    {
        std::string src {"\xE4\xB8\x96\t\xE7\x95\x8C\t\xE6\x97\xA5"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with leading and trailing spaces
    TEST(ConversionUtils, unicode_with_leading_trailing_spaces)
    {
        std::string src {"   \xE4\xB8\x96\xE7\x95\x8C   "};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with only whitespace characters
    TEST(ConversionUtils, only_whitespace_roundtrip)
    {
        std::string src {" \t\n\r\v\f"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with repeated Unicode characters
    TEST(ConversionUtils, repeated_unicode_chars)
    {
        std::string unit {"\xE4\xB8\x96"};  // Single CJK character
        std::string src;
        for (int i = 0; i < 100; ++i) src += unit;
        auto wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
        EXPECT_EQ(100u, wide.size());
    }

    // Test with alternating ASCII and Unicode
    TEST(ConversionUtils, alternating_ascii_unicode)
    {
        std::string src {"A\xE4\xB8\x96" "B\xE7\x95\x8C" "C\xE6\x97\xA5" "D"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with all ASCII printable characters
    TEST(ConversionUtils, all_ascii_printable)
    {
        std::string src;
        for (char c = 32; c < 127; ++c) src += c;
        auto wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with single byte UTF-8 (ASCII range)
    TEST(ConversionUtils, single_byte_utf8_range)
    {
        std::string src {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with mixed 2-byte UTF-8 sequences
    TEST(ConversionUtils, multiple_2byte_utf8)
    {
        std::string src {"\xC3\xA9\xC3\xA0\xC3\xBC\xC3\xB1"};  // é, à, ü, ñ
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with mixed 3-byte UTF-8 sequences
    TEST(ConversionUtils, multiple_3byte_utf8)
    {
        std::string src {"\xE4\xB8\x96\xE7\x95\x8C\xE6\x97\xA5\xE6\x9C\xAC"};  // Multiple CJK chars
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with mixed 4-byte UTF-8 sequences (emojis)
    TEST(ConversionUtils, multiple_4byte_utf8_emojis)
    {
        std::string src {"\xF0\x9F\x98\x80\xF0\x9F\x98\x81\xF0\x9F\x98\x82"};  // 😀😁😂
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with very long string (stress test)
    TEST(ConversionUtils, very_long_string)
    {
        std::string src;
        src.reserve(100000);
        for (int i = 0; i < 10000; ++i) {
            src += "Hello ";
            src += "\xE4\xB8\x96";  // Add CJK character
        }
        auto wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test wide string with various Unicode characters
    TEST(ConversionUtils, wide_string_mixed_unicode)
    {
        std::wstring src {L"Hello \x4E16\x754C \x00E9\x00E0\x00FC"};  // Hello 世界 éàü
        auto         narrow = ConversionUtils::convert_to<wchar_t, char>(src);
        auto         wide   = ConversionUtils::convert_to<char, wchar_t>(narrow);
        EXPECT_EQ(src, wide);
    }

    // Test single character conversions for various Unicode blocks
    TEST(ConversionUtils, single_char_various_blocks)
    {
        // Test single characters from different Unicode blocks
        std::string chars[] = {
            "\xC2\xA9",  // © (copyright sign)
            "\xE2\x82\xAC",  // € (euro sign)
            "\xE2\x98\x85",  // ★ (star)
            "\xF0\x9F\x8C\x8D"  // 🌍 (earth globe)
        };
        
        for (const auto& ch : chars) {
            auto wide   = ConversionUtils::convert_to<char, wchar_t>(ch);
            auto narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
            EXPECT_EQ(ch, narrow);
        }
    }

    // Test that size is preserved for ASCII
    TEST(ConversionUtils, ascii_size_preservation)
    {
        std::string src {"The quick brown fox jumps over the lazy dog. 1234567890!@#$%^&*()"};
        auto        wide = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_EQ(src.size(), wide.size());
    }

    // Test that wide string size is correct for Unicode
    TEST(ConversionUtils, wide_size_for_unicode)
    {
        std::string src {"\xE4\xB8\x96\xE7\x95\x8C"};  // 2 CJK characters (6 bytes in UTF-8)
        auto        wide = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_EQ(2u, wide.size());  // Should be 2 wide characters
    }

    // Test conversion preserves character boundaries
    TEST(ConversionUtils, character_boundary_preservation)
    {
        std::string src {"A\xC3\xA9" "B\xE4\xB8\x96" "C"};  // A + é + B + 世 + C
        auto        wide = ConversionUtils::convert_to<char, wchar_t>(src);
        EXPECT_EQ(5u, wide.size());  // A, é, B, 世, C = 5 characters
    }

    // Test with combining characters (if supported)
    TEST(ConversionUtils, combining_characters)
    {
        std::string src {"e\xCC\x81"};  // e + combining acute accent = é
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test round-trip with special mathematical symbols
    TEST(ConversionUtils, mathematical_symbols)
    {
        std::string src {"\xE2\x88\x80\xE2\x88\x83\xE2\x88\x88"};  // ∀∃∈ (for all, exists, element of)
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with currency symbols
    TEST(ConversionUtils, currency_symbols)
    {
        std::string src {"\xC2\xA2\xC2\xA3\xC2\xA4\xC2\xA5\xE2\x82\xAC"};  // ¢£¤¥€
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

    // Test with arrows and geometric shapes
    TEST(ConversionUtils, arrows_and_shapes)
    {
        std::string src {"\xE2\x86\x90\xE2\x86\x91\xE2\x86\x92\xE2\x86\x93"};  // ←↑→↓
        auto        wide   = ConversionUtils::convert_to<char, wchar_t>(src);
        auto        narrow = ConversionUtils::convert_to<wchar_t, char>(wide);
        EXPECT_EQ(src, narrow);
    }

} // namespace siddiqsoft
