StringHelpers : C++ String Conversion Utilities
-------------------------------------------
<!-- badges -->
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.StringHelpers?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=26&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.StringHelpers)
![](https://img.shields.io/github/v/tag/SiddiqSoft/StringHelpers)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/26)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/26)
<!-- end badges -->

# Objective

- Collect useful helpers for Azure REST API in a single location.
- Header-only
  - Use the nuget package or CMakeLists to import into your project.
- Cross-platform support
  - Uses Win32 APIs on Windows for optimal performance
  - Uses standard C++ `codecvt` on Linux and macOS
  - Comprehensive test coverage across all platforms

## Usage
- Use the nuget [SiddiqSoft.StringHelpers](https://www.nuget.org/packages/SiddiqSoft.StringHelpers/)
- Use the CPM in your CMakeLists file.

```cmake
..
.. # import the CPM.cmake module and activate..
..
CPMAddModule("gh:siddiqsoft/StringHelpers#1.0.0")
..
..
target_link_libraries(your-project PRIVATE StringHelpers::StringHelpers)
```

# Features 

- ConversionUtils (`conversion-utils.hpp`)
  
  ```cpp
  template<typename S, typename D>
  auto convert_to<char,wchar_t>(const std::basic_string<T>& src) -> std::basic_string<D>
  {
     ...
    return std::basic_string<D>{};
  }
  ```

  - `convert_to<char,wchar_t>(const std::string& src)`
    - Convert from std::string to std::wstring (using UTF-8 encoding)
  - `convert_to<wchar_t,char>(const std::wstring& src)`
    - Convert from std::wstring to std::string (UTF-8 encodes the contents).

## Test Coverage

The library includes **63 comprehensive tests** covering:

- **Empty strings** - All four template instantiation paths
- **Single character conversions** - ASCII and Unicode
- **ASCII round-trips** - Narrow ↔ wide conversions
- **Whitespace handling** - Newlines, tabs, carriage returns, spaces
- **UTF-8 multi-byte sequences** - 2-byte, 3-byte, and 4-byte sequences
- **International scripts** - Hebrew, Greek, Devanagari (Hindi), Arabic, Vietnamese, Cyrillic, Japanese, Korean, Thai
- **Unicode blocks** - Mathematical symbols, currency symbols, arrows, geometric shapes, emojis
- **Edge cases** - Very long strings (stress tests), repeated characters, alternating ASCII/Unicode
- **Character boundaries** - Proper preservation of multi-byte character boundaries
- **Combining characters** - Diacritical marks and combining sequences
- **Size preservation** - Correct character counting for ASCII and Unicode

All tests pass on **Windows, Linux, and macOS** with 100% success rate.

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Windows  | ✅ Fully Supported | Uses Win32 APIs (WideCharToMultiByte, MultiByteToWideChar) |
| Linux    | ✅ Fully Supported | Uses standard C++ codecvt |
| macOS    | ✅ Fully Supported | Uses standard C++ codecvt |

> Use this library until C++26 is prevalent and the compiler vendors remove the `codecvt` library--not simply deprecation warnings.

<p align="right">
&copy; 2024 Siddiq Software LLC. All rights reserved.
</p>
