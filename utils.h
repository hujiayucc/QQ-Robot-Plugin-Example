//
// Created by hujiayucc on 25-5-3.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "web.h"

inline bool str_has_text(const char *str, const char *text) {
    return strcmp(str, text) == 0;
}

inline std::string weather(const std::string &city) {
    const std::string url = "http://api.xiaoyuyc.com/weather.php?city=" + std::string(city);
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
