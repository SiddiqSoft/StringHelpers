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
        // Using Unicode code points to ensure consistent behavior across platforms
        // صديق = U+0635 U+062F U+064A U+0642
        std::string  sample {"صديق"};
        std::wstring sample_w {L"\x0635\x062F\x064A\x0642"};
        std::string sample_n {"\xD8\xB5\xD8\xAF\xD9\x8A\xD9\x82"};

        try {
            auto intermediate = ConversionUtils::convert_to<wchar_t, char>(sample_w);
            EXPECT_EQ(sample_n, intermediate);
            auto roundTrip = ConversionUtils::convert_to<char, wchar_t>(intermediate);
            EXPECT_EQ(sample_w, roundTrip);
            EXPECT_EQ(sample, intermediate);
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
        std::string  sample_0rt {"Hello, \xE4\xB8\x96\xE7\x95\x8C"};
        // Using Unicode code points for wide string: 世 = U+4E16, 界 = U+754C
        std::wstring sample_w {L"Hello, \x4E16\x754C"};
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

} // namespace siddiqsoft
