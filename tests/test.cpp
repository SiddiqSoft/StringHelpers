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
        std::string  sample_0rt {"Hello, ä¸–ç•Œ"};
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
} // namespace siddiqsoft