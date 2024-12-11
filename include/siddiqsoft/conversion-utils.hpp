﻿/*
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

#include <codecvt>
#include <locale>
#include <memory>
#include <type_traits>
#ifndef CONVERSION_UTILS_HPP
#define CONVERSION_UTILS_HPP

#include <chrono>
#include <concepts>
#include <format>
#include <ranges>
#include <string>
#include <vector>
#include <exception>
#include <utility>

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
        template <typename S = char, typename D = wchar_t>
            requires(
                    requires { std::same_as<S, char>&& std::same_as<D, wchar_t>; } ||
                    requires { std::same_as<S, wchar_t>&& std::same_as<D, char>; })
        static auto convert_to(const std::basic_string<S>& src) -> std::basic_string<D>
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            if constexpr (std::is_same_v<S, wchar_t>) {
                return converter.to_bytes(src);
            }
            else if constexpr (std::is_same_v<S, char>) {
                return converter.from_bytes(src);
            }
            // We should not end up here!
            return std::basic_string<D> {};
        }
    };
} // namespace siddiqsoft

#endif
