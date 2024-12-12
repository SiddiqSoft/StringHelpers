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
- Use Win32 functions on Windows
  - No support for Linux/Darwin!

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

> Use this library until C++26 is prevalent and the compiler vendors remove the `codecvt` library--not simply deprecation warnings.

<p align="right">
&copy; 2024 Siddiq Software LLC. All rights reserved.
</p>
