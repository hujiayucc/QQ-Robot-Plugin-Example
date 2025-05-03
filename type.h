//
// Created by hujiayucc on 25-5-2.
//

#ifndef TYPE_H
#define TYPE_H
constexpr const char* ROBOT_MENU = R"(菜单：
/一言
/电脑端CPU排行
/手机端CPU排行
/移动端CPU排行
/桌面端显卡排行
/移动端显卡排行
/天气查询 + 城市)";
/** 登录成功 */
constexpr int EVENT_SUCCESS = 1;
/** 群消息 */
constexpr int EVENT_MSG_GROUP = 2;
/** 机器人加入群 */
constexpr int EVENT_ADD_GROUP = 3;
/** 机器人被移出群 */
constexpr int EVENT_REMOVE_GROUP = 4;

/** 被允许接受消息 */
constexpr int EVENT_ALLOW_MSG = 5;
/** 被拒绝接受消息 */
constexpr int EVENT_REFUSE_GROUP = 6;
/** 插件被载入 */
constexpr int EVENT_PLUGIN_LOAD = 7;
/** 插件被启用 */
constexpr int EVENT_PLUGIN_OPEN = 8;
/** 插件被卸载 */
constexpr int EVENT_PLUGIN_UNINSTALL = 9;
/** 好友消息 */
constexpr int EVENT_MSG_PRIVATE = 10;
/** 好友消息 */
constexpr int EVENT_MSG_PRIVATE2 = 11;

/** 继续 */
constexpr int EVENT_MSG_CONTINUE = 100;
/** 拦截 */
constexpr int EVENT_MSG_INTERCEPT = 110;
#endif
