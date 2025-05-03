//
// Created by hujiayucc on 25-5-3.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "web.h"

inline std::string wstring_to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    const int size_needed = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), static_cast<int>(wstr.size()),
        nullptr, 0, nullptr, nullptr
    );
    std::string utf8_str(size_needed, 0);
    WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), static_cast<int>(wstr.size()),
        utf8_str.data(), size_needed, nullptr, nullptr
    );
    return utf8_str;
}

inline std::string ansi_to_utf8(const std::string& ansi) {
    if (ansi.empty()) return {};
    // 先转换为宽字符（UTF-16）
    const int wsize = MultiByteToWideChar(
        CP_ACP, 0,
        ansi.data(), static_cast<int>(ansi.size()),
        nullptr, 0
    );
    std::wstring wstr(wsize, 0);
    MultiByteToWideChar(
        CP_ACP, 0,
        ansi.data(), static_cast<int>(ansi.size()),
        wstr.data(), wsize
    );
    // 再转换为 UTF-8
    return wstring_to_utf8(wstr);
}

inline std::string url_encode(const std::string &value) {
    static constexpr char hex_chars[] = "0123456789ABCDEF";
    std::string escaped;
    escaped.reserve(value.size() * 3);

    for (const unsigned char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped.push_back(c);
        } else {
            escaped.push_back('%');
            escaped.push_back(hex_chars[(c >> 4) & 0x0F]);
            escaped.push_back(hex_chars[c & 0x0F]);
        }
    }
    return escaped;
}

inline std::string weather(const std::string &city) {
    const std::string url = "http://api.xiaoyuyc.com/weather.php?city=" + url_encode(ansi_to_utf8(std::string(city)));
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string yy() {
    const std::string url = "http://api.xiaoyuyc.com/yiyan.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string gpu1() {
    const std::string url = "http://api.xiaoyuyc.com/xianka1.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string gpu2() {
    const std::string url = "http://api.xiaoyuyc.com/xianka2.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string cpu1() {
    const std::string url = "http://api.xiaoyuyc.com/cpu1.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string cpu2() {
    const std::string url = "http://api.xiaoyuyc.com/cpu2.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}

inline std::string cpu3() {
    const std::string url = "http://api.xiaoyuyc.com/cpu3.php";
    auto [statusCode, content, error] = web_response(url);
    if (statusCode != 200) return error;
    return content;
}
#endif
