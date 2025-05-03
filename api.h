//
// Created by hujiayucc on 25-5-2.
//

#pragma once
#include <windows.h>
#include <stdexcept>

class Api;

// 定义所有函数指针类型
typedef void (__stdcall*ApiOutFunc)(const char *, int);
typedef void (__stdcall*ApiLoadFunc)(const char *);
typedef char * (__stdcall*ApiSendAFunc)(const char *, const char *, const char *, const char *);
typedef char * (__stdcall*ApiAccountFunc)();
typedef char * (__stdcall*ApiSendBFunc)(const char *, const char *, const char *, int, const char *, const char *);
typedef char * (__stdcall*ApiSendCFunc)(const char *, const char *, const char *, const char *, const char *);
typedef char * (__stdcall*ApiSendDFunc)(const char *, const char *, const char *, const char *);
typedef char * (__stdcall*ApiSendEFunc)(const char *, const char *, const char *, const char *, const char *);
typedef char * (__stdcall*ApiSendFFunc)(const char *, const char *, const char *, int, const char *, const char *);

inline Api *api;

class Api {
public:
    Api() {
        hModule = LoadLibraryA("zzapi.dll");
        if (!hModule) throw std::runtime_error("Failed to load zzapi.dll");

        // 获取所有函数地址
        apiOut = reinterpret_cast<ApiOutFunc>(GetProcAddress(hModule, "\u8f93\u51fa\u65e5\u5fd7"));
        apiReLoad = reinterpret_cast<ApiLoadFunc>(GetProcAddress(hModule, "\u91cd\u8f7d\u81ea\u8eab"));
        apiSendA = reinterpret_cast<ApiSendAFunc>(GetProcAddress(hModule, "\u53d1\u9001\u6587\u5b57\u6d88\u606f"));
        apiAccount = reinterpret_cast<ApiAccountFunc>(GetProcAddress(hModule, "\u53d6\u8d26\u53f7\u4fe1\u606f"));
        apiSendB = reinterpret_cast<ApiSendBFunc>(GetProcAddress(hModule, "\u53d1\u9001\u56fe\u6587\u6d88\u606f"));
        apiSendC = reinterpret_cast<ApiSendCFunc>(GetProcAddress(hModule, "\u53d1\u9001MD\u6d88\u606f"));
        apiSendD = reinterpret_cast<ApiSendDFunc>(GetProcAddress(
            hModule, "\u53d1\u9001\u79c1\u804a\u6587\u5b57\u6d88\u606f"));
        apiSendE = reinterpret_cast<ApiSendEFunc>(GetProcAddress(hModule, "\u53d1\u9001\u79c1\u804aMD\u6d88\u606f"));
        apiSendF = reinterpret_cast<ApiSendFFunc>(GetProcAddress(
            hModule, "\u53d1\u9001\u79c1\u804a\u56fe\u6587\u6d88\u606f"));

        // 检查所有函数指针是否有效
        if (!apiOut || !apiReLoad || !apiSendA || !apiAccount ||
            !apiSendB || !apiSendC || !apiSendD || !apiSendE || !apiSendF) {
            FreeLibrary(hModule);
            throw std::runtime_error("Failed to get function pointers");
        }
    }

    ~Api() {
        if (hModule) {
            FreeLibrary(hModule);
            hModule = nullptr;
        }
    }

    /**
     * @brief 输出带颜色的日志信息
     * @param message 要输出的日志内容（UTF-8编码）
     * @param color 文本颜色标识（具体值参考DLL文档）
     */
    void Out(const char *message, const int color = 0x00FF00) const {
        apiOut(message, color);
    }

    /**
     * @brief 热重载插件
     * @param dir 新插件文件路径（需替换plugin目录下的文件）
     * @note 调用后会重新初始化Api实例
     */
    void Reload(const char *dir) const {
        apiReLoad(dir);
        delete api;
        api = new Api();
    }

    /**
     * @brief 发送群组文字消息
     * @param appid 机器人APPID（由消息平台分配）
     * @param group 目标群组ID
     * @param msg 消息内容（建议使用UTF-8编码）
     * @param msgid 消息唯一标识
     * @return 包含执行结果的字符串（JSON格式或简单状态）
     */
    char *SendTextMessage(const char *appid, const char *group,
                          const char *msg, const char *msgid) const {
        return apiSendA(appid, group, msg, msgid);
    }

    /**
     * @brief 发送群组富媒体消息
     * @param appid 机器人APPID
     * @param group 目标群组ID
     * @param msg 文本描述内容
     * @param msg_type 媒体类型：1-图片，2-视频，3-语音，0-无媒体
     * @param msg_url 媒体资源URL地址
     * @param msgid 消息唯一标识
     * @return 执行结果字符串
     * @warning 当msg_type非0时msg_url必须有效
     */
    char *SendRichMessage(const char *appid, const char *group,
                          const char *msg, const int msg_type,
                          const char *msg_url, const char *msgid) const {
        return apiSendB(appid, group, msg, msg_type, msg_url, msgid);
    }

    /**
     * @brief 发送群组Markdown格式消息
     * @param appid 机器人APPID
     * @param group 目标群组ID
     * @param msg_md Markdown源码内容
     * @param msgid 消息唯一标识
     * @param MD_button 附加按钮的JSON格式描述（可选）
     * @return 执行结果字符串
     */
    char *SendMDMessage(const char *appid, const char *group,
                        const char *msg_md, const char *msgid,
                        const char *MD_button = "") const {
        return apiSendC(appid, group, msg_md, msgid, MD_button);
    }

    /**
     * @brief 发送私聊文字消息
     * @param appid 机器人APPID
     * @param userid 目标用户ID
     * @param msg 消息内容
     * @param msgid 消息唯一标识
     * @return 执行结果字符串
     */
    char *SendPrivateText(const char *appid, const char *userid,
                          const char *msg, const char *msgid) const {
        return apiSendD(appid, userid, msg, msgid);
    }

    /**
     * @brief 发送私聊Markdown消息
     * @param appid 机器人APPID
     * @param userid 目标用户ID
     * @param msg_md Markdown源码内容
     * @param msgid 消息唯一标识
     * @param MD_button 附加按钮的JSON描述（可选）
     * @return 执行结果字符串
     */
    char *SendPrivateMD(const char *appid, const char *userid,
                        const char *msg_md, const char *msgid,
                        const char *MD_button = "") const {
        return apiSendE(appid, userid, msg_md, msgid, MD_button);
    }

    /**
     * @brief 发送私聊富媒体消息
     * @param appid 机器人APPID
     * @param userid 目标用户ID
     * @param msg 文本描述内容
     * @param msg_type 媒体类型：1-图片，2-视频，3-语音，0-无媒体
     * @param msg_url 媒体资源URL地址
     * @param msgid 消息唯一标识
     * @return 执行结果字符串
     */
    char *SendPrivateRichMsg(const char *appid, const char *userid,
                             const char *msg, const int msg_type,
                             const char *msg_url, const char *msgid) const {
        return apiSendF(appid, userid, msg, msg_type, msg_url, msgid);
    }

    /**
     * @brief 获取当前账号信息
     * @return 包含账号详情的JSON字符串
     * @retval 格式示例：{"appid":"123","nickname":"测试账号"}
     */
    [[nodiscard]] char *GetAccountInfo() const {
        return apiAccount();
    }

private:
    HMODULE hModule = nullptr;
    ApiOutFunc apiOut = nullptr;
    ApiLoadFunc apiReLoad = nullptr;
    ApiSendAFunc apiSendA = nullptr;
    ApiAccountFunc apiAccount = nullptr;
    ApiSendBFunc apiSendB = nullptr;
    ApiSendCFunc apiSendC = nullptr;
    ApiSendDFunc apiSendD = nullptr;
    ApiSendEFunc apiSendE = nullptr;
    ApiSendFFunc apiSendF = nullptr;
};
