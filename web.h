//
// Created by hujiayucc on 25-5-3.
//

#pragma once
#include <map>
#include <string>
#include <vector>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

struct response {
    unsigned long statusCode{};
    std::string content;
    std::string error;
};

inline response web_response(
    const std::string& url,
    const std::string& method = "GET",
    const std::string& postData = "",
    const std::map<std::string, std::string>& headers = {},
    const int timeout = 10)
{
    response res;
    HINTERNET hSession = nullptr, hConnect = nullptr, hRequest = nullptr;

    // 获取系统当前代码页
    const UINT sysCodePage = GetACP();

    // 转换URL为宽字符
    const int urlLen = MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, nullptr, 0);
    std::wstring wUrl(urlLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, &wUrl[0], urlLen);

    // 解析URL组件
    URL_COMPONENTSW urlComp = { sizeof(urlComp) };
    wchar_t hostName[256] = {};
    wchar_t urlPath[1024] = {};
    urlComp.lpszHostName = hostName;
    urlComp.dwHostNameLength = ARRAYSIZE(hostName);
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = ARRAYSIZE(urlPath);

    if (!InternetCrackUrlW(wUrl.c_str(), 0, 0, &urlComp)) {
        res.error = "URL解析失败 (错误码: " + std::to_string(GetLastError()) + ")";
        return res;
    }

    // 自动补全端口
    INTERNET_PORT port = urlComp.nPort;
    if (port == 0) {
        port = urlComp.nScheme == INTERNET_SCHEME_HTTPS
            ? INTERNET_DEFAULT_HTTPS_PORT
            : INTERNET_DEFAULT_HTTP_PORT;
    }

    // 创建会话
    hSession = InternetOpenW(L"WebClient/1.0", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!hSession) {
        res.error = "创建会话失败 (错误码: " + std::to_string(GetLastError()) + ")";
        return res;
    }

    // 设置超时
    DWORD timeout_ms = timeout * 1000;
    InternetSetOptionW(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout_ms, sizeof(timeout_ms));
    InternetSetOptionW(hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout_ms, sizeof(timeout_ms));

    // 建立连接
    hConnect = InternetConnectW(hSession, hostName, port, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        res.error = "连接失败 (错误码: " + std::to_string(GetLastError()) + ")";
        InternetCloseHandle(hSession);
        return res;
    }

    // 创建请求标志
    DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
    if (urlComp.nScheme == INTERNET_SCHEME_HTTPS) {
        flags |= INTERNET_FLAG_SECURE;
    }

    // 打开请求
    std::wstring wMethod(method.begin(), method.end());
    hRequest = HttpOpenRequestW(hConnect, wMethod.c_str(), urlPath, nullptr, nullptr, nullptr, flags, 0);
    if (!hRequest) {
        res.error = "打开请求失败 (错误码: " + std::to_string(GetLastError()) + ")";
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return res;
    }

    // 设置请求头
    std::wstring headersStr;
    for (const auto& [key, value] : headers) {
        headersStr += std::wstring(key.begin(), key.end()) + L": "
                     + std::wstring(value.begin(), value.end()) + L"\r\n";
    }
    if (!headersStr.empty()) {
        HttpAddRequestHeadersW(hRequest, headersStr.c_str(), headersStr.length(), HTTP_ADDREQ_FLAG_ADD);
    }

    // 发送请求
    LPVOID postDataPtr = !postData.empty() ? LPVOID(postData.data()) : nullptr;
    if (DWORD postDataLen = !postData.empty() ? static_cast<DWORD>(postData.size()) : 0; !HttpSendRequestW(hRequest, nullptr, 0, postDataPtr, postDataLen)) {
        res.error = "发送请求失败 (错误码: " + std::to_string(GetLastError()) + ")";
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return res;
    }

    // 获取状态码
    DWORD statusCode = 0;
    DWORD bufSize = sizeof(statusCode);
    HttpQueryInfoW(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &bufSize, nullptr);
    res.statusCode = statusCode;

    // 读取响应数据并转换编码
    std::vector<char> rawData;
    char buffer[4096] = {};
    DWORD bytesRead = 0;

    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        rawData.insert(rawData.end(), buffer, buffer + bytesRead);
    }

    // 转换到系统编码
    if (!rawData.empty()) {
        // 先尝试转换为宽字符（使用UTF-8或其他编码）
        const int wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
            rawData.data(), static_cast<int>(rawData.size()), nullptr, 0);

        if (wlen > 0) {
            std::wstring wstr(wlen, 0);
            MultiByteToWideChar(CP_UTF8, 0, rawData.data(), static_cast<int>(rawData.size()), &wstr[0], wlen);

            // 转换到系统编码
            const int len = WideCharToMultiByte(sysCodePage, 0,
                wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

            res.content.resize(len);
            WideCharToMultiByte(sysCodePage, 0, wstr.c_str(), -1,
                &res.content[0], len, nullptr, nullptr);
            res.content.resize(len - 1);
        } else {
            // 如果UTF-8转换失败，直接使用原始数据
            res.content.assign(rawData.begin(), rawData.end());
        }
    }

    // 清理资源
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return res;
}