//
// Created by hujiayucc on 25-5-2.
//

#include "main.h"
#include "api.h"
#include "type.h"
#include "utils.h"

API char* ZZ_initialize() {
    api = new Api();
    api->Out("小宇云创初始化完成", 0x00FF00);
    return const_cast<char*>("{'name':'小宇云创','auth':'小宇','version':'1.0.0','explain':'小宇云创专属插件'}");
}

/**
 * @brief 消息
 * @param botId 机器人ID
 * @param event 消息类型/事件类型
 * @param msgFrom 消息来源
 * @param userId 触发人ID
 * @param msgId 消息ID
 * @param text 消息内容
 * @param file 附件 富媒体文件附件，文件类型："图片，语音，视频，文件"
 * @param json 原始JSON
 * @return 100（继续）
 */
API int ZZ_messages(char *botId, int event, char *msgFrom, char *userId, char *msgId, char *text, char *file, char *json) {
    std::string textStr = std::string(text);
    const std::string WEATHER_CMD = "/天气查询";
    const size_t WEATHER_CMD_LEN = WEATHER_CMD.length();

    if (event == EVENT_MSG_GROUP) {
        // 群消息处理
        if (textStr == "/菜单") {
            std::string reply = "\n" + std::string(ROBOT_MENU);
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr.find(WEATHER_CMD) == 0) {
            std::string city = textStr.substr(WEATHER_CMD_LEN);
            if (city.empty()) {
                api->Out(api->SendTextMessage(botId, msgFrom, "\n请输入城市!", msgId));
                return EVENT_MSG_CONTINUE;
            }
            std::string reply = "\n" + weather(city);
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/一言") {
            std::string reply = "\n" + yy();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/桌面端显卡排行") {
            std::string reply = "\n" + gpu1();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/移动端显卡排行") {
            std::string reply = "\n" + gpu2();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/电脑端CPU排行") {
            std::string reply = "\n" + cpu1();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/手机端CPU排行") {
            std::string reply = "\n" + cpu2();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
        else if (textStr == "/移动端CPU排行") {
            std::string reply = "\n" + cpu3();
            api->Out(api->SendTextMessage(botId, msgFrom, reply.c_str(), msgId));
        }
    }
    else if (event == EVENT_MSG_PRIVATE || event == EVENT_MSG_PRIVATE2) {
        // 好友消息处理
        if (textStr == "/菜单") {
            api->Out(api->SendPrivateText(botId, userId, ROBOT_MENU, msgId));
        }
        else if (textStr.find(WEATHER_CMD) == 0) {
            std::string city = textStr.substr(WEATHER_CMD_LEN);
            if (city.empty()) {
                api->Out(api->SendPrivateText(botId, userId, "请输入城市!", msgId));
                return EVENT_MSG_CONTINUE;
            }
            std::string reply = weather(city);
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/一言") {
            std::string reply = yy();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/桌面端显卡排行") {
            std::string reply = gpu1();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/移动端显卡排行") {
            std::string reply = gpu2();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/电脑端CPU排行") {
            std::string reply = cpu1();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/手机端CPU排行") {
            std::string reply = cpu2();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
        else if (textStr == "/移动端CPU排行") {
            std::string reply = cpu3();
            api->Out(api->SendPrivateText(botId, userId, reply.c_str(), msgId));
        }
    }

    return EVENT_MSG_CONTINUE;
}

API int ZZ_Deactivate() {
    return 1;
}

API void ZZ_uninstall() {
    delete api;
}

API int ZZ_set() {
    api->Out("窗口需要自行实现", 0x00FF00);
    return 1;
}