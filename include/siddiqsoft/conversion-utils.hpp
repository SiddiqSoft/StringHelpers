/*
    StringHelpers : Simple wide, narrow, utf8 conversion functions

    BSD 3-Clause License

    Copyright (c) 2021, Siddiq Software LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, d_, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef CONVERSION_UTILS_HPP
#define CONVERSION_UTILS_HPP

#include <chrono>
#include <concepts>
#include <format>
#include <ranges>
#include <string>


#if defined(__linux__)
#include <iconv.h>
#elif defined(__unix__)
#include <iconv.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Conversion Functions for ascii to wide, utf8 to wide and vice-versa
    struct ConversionUtils
    {
        template<typename S, typename D>
        static auto convert_to(const S& src)
        {
#if defined(__linux__) || defined(__unix__)
iconv_t conv = iconv_open("WCHAR_T", "UTF-8");
    if (conv == (iconv_t)-1) {
        throw std::runtime_error("iconv_open");
    }
#else
#endif

    // Set up conversion buffers
    size_t in_bytes = src.size();
    char* in_buf = const_cast<char*>(src.c_str());

    std::vector<wchar_t> wide_buf(in_bytes + 1);
    char* out_buf
        = reinterpret_cast<char*>(wide_buf.data());
    size_t out_bytes = wide_buf.size() * sizeof(wchar_t);

    // Perform conversion
    if (iconv(conv, &in_buf, &in_bytes, &out_buf,
              &out_bytes)
        == (size_t)-1) {
        iconv_close(conv);
        throw std::runtime_error("Conversion failure");
    }

    // Close iconv descriptor
    iconv_close(conv);
    // Create wide string
    return std::wstring(wide_buf.data());
        }
        
        
        /// @brief Convert given wide string to ascii encoded string
        /// @param src std::wstring input
        /// @return std::string with ascii encoding
        static auto asciiFromWide(const std::wstring& src) -> std::string
        {
            if (src.empty()) return {};
#if defined(WIN32)
            if (auto destSize =
                        WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0, nullptr, nullptr);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::vector<char> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = WideCharToMultiByte(CP_ACP,
                                               0,
                                               src.c_str(),
                                               static_cast<int>(src.length()),
                                               destBuffer.data(),
                                               static_cast<DWORD>(destSize),
                                               nullptr,
                                               nullptr);
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }
#else
            std::mbstate_t state = std::mbstate_t();
            const wchar_t* wstr  = src.c_str();
            std::size_t    len   = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
            std::string    mbstr(len, '\0');

            auto convertedCount = std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
            if (convertedCount != static_cast<std::size_t>(-1)) {
                mbstr.resize(convertedCount);
                return mbstr;
            };
#endif
            // Fall-through error -> empty string
            return {};
        }

        /// @brief Convert given wide string to utf8 encoded string
        /// @param src std::wstring input
        /// @return std::string with utf-8 encoding
        static auto utf8FromWide(const std::wstring& src) -> std::string
        {
            if (src.empty()) return {};

#if defined(WIN32)
            if (auto destSize =
                        WideCharToMultiByte(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0, nullptr, nullptr);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::string destBuffer (static_cast<size_t>(destSize) + 1, '\0');
                destSize = WideCharToMultiByte(CP_UTF8,
                                               0,
                                               src.c_str(),
                                               static_cast<int>(src.length()),
                                               destBuffer.data(),
                                               static_cast<DWORD>(destSize),
                                               nullptr,
                                               nullptr);
                destBuffer.resize(destSize);
                return destBuffer;
            }
#else
            std::mbstate_t state = std::mbstate_t();
            const wchar_t* wstr  = src.c_str();
            std::size_t    len   = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
            std::string    mbstr(len, '\0');

            auto convertedCount = std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
            if (convertedCount != static_cast<std::size_t>(-1)) {
                mbstr.resize(convertedCount);
                return mbstr;
            };
#endif

            // Fall-through error -> empty string
            return {};
        }

        /// @brief Given a utf-8 encoded string returns a utf-16 in std::wstring
        /// @param src Utf-8 encoded string
        /// @return Utf-16 encoded wstring
        static auto wideFromUtf8(const std::string& src) -> std::wstring
        {
            if (src.empty()) return {};

#if defined(WIN32)
            if (auto destSize = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::wstring destBuffer(static_cast<size_t>(destSize) + 1, L'\0');
                destSize = MultiByteToWideChar(
                        CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
                destBuffer.resize(destSize);
                return destBuffer;
            }
#else
            std::mbstate_t state = std::mbstate_t();
            const char*    mbstr = src.c_str();
            std::size_t    len   = 1 + std::mbsrtowcs(nullptr, &mbstr, 0, &state);
            std::wstring   wstr(len, L'\0');

            auto convertedCount = std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
            if (convertedCount != static_cast<std::size_t>(-1)) {
                wstr.resize(convertedCount);
                return wstr;
            }
#endif

            // Fall-through error -> empty string
            return {};
        }

        /// @brief Given an ascii encoded string returns a utf-16 in std::wstring
        /// @param src ascii encoded string
        /// @return Utf-16 encoded wstring (Windows) and utf-8 encoded for UNIX
        static auto wideFromAscii(const std::string& src) -> std::wstring
        {
            if (src.empty()) return {};

#if defined(WIN32)
            if (auto destSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::vector<wchar_t> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = MultiByteToWideChar(
                        CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }
#else
            std::mbstate_t state = std::mbstate_t();
            const char*    mbstr = src.c_str();
            std::size_t    len   = 1 + std::mbsrtowcs(nullptr, &mbstr, 0, &state);
            std::wstring   wstr(len, L'\0');

            auto convertedCount = std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
            if (convertedCount != static_cast<std::size_t>(-1)) {
                wstr.resize(convertedCount);
                return wstr;
            }
#endif

            // Fall-through error -> empty string
            return {};
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
