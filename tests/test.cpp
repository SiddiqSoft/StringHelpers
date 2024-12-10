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
            auto intermediate = ConversionUtils::wideFromUtf8(sample);
            // EXPECT_EQ(sample_w, intermediate);
            std::string roundTrip {ConversionUtils::utf8FromWide(intermediate)};
            std::clog << "sample    : " << sample << " - size: " << sample.size() << std::endl;
            std::clog << "roundTrip : " << roundTrip << " - size: " << roundTrip.size() << std::endl;
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
            auto intermediate = ConversionUtils::utf8FromWide(sample);
            EXPECT_EQ(sample_n, intermediate);
            std::wstring roundTrip {ConversionUtils::wideFromUtf8(intermediate)};
            std::wcerr << L"sample    : .." << sample << L".. - size: " << sample.size() << std::endl;
            std::cerr << "sample_n  : .." << sample_n << ".. - size: " << sample_n.size() << std::endl;
            std::wcerr << L"roundTrip : .." << roundTrip << L".. - size: " << roundTrip.size() << std::endl;
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
            std::string roundTrip {ConversionUtils::asciiFromWide(ConversionUtils::wideFromAscii(sample))};
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(ConversionUtils, test2a_w)
    {
        std::wstring sample {L"صديق"};
        std::wstring sample_w {L"\x635\x62F\x64A\x642"};
        std::string  sample_n {"صديق"};
        std::wstring  sample_8 {L"\u0635\u062f\u064a\u0642"};

        try {
            auto intermediate = ConversionUtils::asciiFromWide(sample);
            EXPECT_EQ(sample_n, intermediate);
            std::wstring roundTrip {ConversionUtils::wideFromAscii(intermediate)};
            EXPECT_EQ(sample_8, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(ConversionUtils, test3)
    {
#if defined(WIN32)
        std::string sample_0 {"Banc\xE9"};
#elif defined(OSX)
        std::string sample_0 {"Banc\xC3\xA9"};
#else
        std::string sample_0 {"Banc\xEF\xBF\xBD"};
#endif
        EXPECT_EQ(sample_0, ConversionUtils::utf8FromWide(ConversionUtils::wideFromUtf8(sample_0)));
        auto intermediate0 = ConversionUtils::wideFromUtf8(sample_0);
        std::cerr << "2: " << sample_0 << " -- ";
        std::wcerr << intermediate0 << std::endl;
    }
} // namespace siddiqsoft