/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#ifdef _WIN32
#include <windows.h>
#endif
module lysa.global;

namespace lysa {

    std::wstring sanitizeName(const std::wstring &name) {
        auto newName = name;
        std::ranges::replace(newName, '/', '_');
        std::ranges::replace(newName, ':', '_');
        return newName;
    }

    float getCurrentTimeMilliseconds() {
        using namespace std::chrono;
        return static_cast<float>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
    }

    std::wstring to_lower(const std::wstring& str) {
        auto s = str;
        // https://en.cppreference.com/w/cpp/string/byte/tolower
        std::ranges::transform(s, s.begin(),
                  [](const unsigned char c){ return std::tolower(c); }
                 );
        return s;
    }

    std::string to_lower(const std::string& str) {
        auto s = str;
        // https://en.cppreference.com/w/cpp/string/byte/tolower
        std::ranges::transform(s, s.begin(),
                  [](const unsigned char c){ return std::tolower(c); }
                 );
        return s;
    }

    std::string to_string(const wchar_t* wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(wstr);
    }

    std::string to_string(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(wstr);
    }

    std::wstring to_wstring(const std::string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.from_bytes(str);
    }

    std::vector<std::string_view> split(const std::string_view str, const char delimiter) {
        std::vector<std::string_view> result;
        size_t start = 0;
        size_t end = str.find(delimiter);
        while (end != std::string_view::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }
        result.push_back(str.substr(start)); // Add the last token
        return result;
    }

    float3 to_float3(const std::string& str) {
        std::stringstream ss(str);
        float3 result{};
        if (std::string token; std::getline(ss, token, ',')) {
            result.x = stof(token);
            if (getline(ss, token, ',')) {
                result.y = stof(token);
                if (getline(ss, token, ',')) {
                    result.z = stof(token);
                }
            }
        }
        return result;
    }

    float4 to_float4(const std::string& str) {
        std::stringstream ss(str);
        float4 result{};
        if (std::string token; std::getline(ss, token, ',')) {
            result.x = std::stof(token);
            if (getline(ss, token, ',')) {
                result.y = std::stof(token);
                if (getline(ss, token, ',')) {
                    result.z = std::stof(token);
                    if (getline(ss, token, ',')) {
                        result.w = std::stof(token);
                    }
                }
            }
        }
        return result;
    }

#ifdef _WIN32
    bool dirExists(const std::string& dirName) {
        const DWORD ftyp = GetFileAttributesA(dirName.c_str());
        return (ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY);
    }
#endif

    std::string to_hexstring(const void* ptr) {
        std::stringstream ss;
        ss << "0x" << std::hex << reinterpret_cast<uint64>(ptr);
        return ss.str();
    }

    std::string to_hexstring(const uint32 ptr) {
        std::stringstream ss;
        ss << "0x" << std::hex << ptr;
        return ss.str();
    }

    std::string to_string(const float3& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "}";
    }

    std::string to_string(const float2& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "}";
    }

   std::string to_string(const float4& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + std::to_string(vec.w) + "}";
    }

    std::string to_string(const quaternion& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," + std::to_string(vec.w) + "}";
    }

}