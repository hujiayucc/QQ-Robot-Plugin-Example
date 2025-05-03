//
// Created by hujiayucc on 25-5-2.
//

#ifndef LIBRARY_H
#define LIBRARY_H
#ifdef _WIN32
#ifdef TEST_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#ifdef __cplusplus
extern "C" {
#endif
    /** 初始化 */
    API char* ZZ_initialize();
    /** 消息 */
    API int ZZ_messages(char *botId, int event, char *msgFrom, char *userId, char *msgId, char *text, char *file, char *json);
    /** 停用 */
    API int ZZ_Deactivate();
    /** 卸载 */
    API void ZZ_uninstall();
    /** 打开窗口 */
    API int ZZ_set();
#ifdef __cplusplus
}
#endif
#endif