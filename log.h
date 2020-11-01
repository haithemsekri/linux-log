
#ifndef CORE_LOG_H
#define CORE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef enum {
  CoreLog_PrintLevel_Debug,
  CoreLog_PrintLevel_Info,
  CoreLog_PrintLevel_Warning,
  CoreLog_PrintLevel_Error,
  CoreLogPrintLevel_NumberOfLevel
} CoreLog_PrintLevelEnumType;

#define CoreLog_IsValidPrintLevel(eLevel) \
  (eLevel < CoreLogPrintLevel_NumberOfLevel)

#define CoreLog_PrintLevelToString(eLevel) \
    (CoreLog_PrintLevel_Debug == eLevel)   ? "debug" : \
    (CoreLog_PrintLevel_Info == eLevel)    ? "info" : \
    (CoreLog_PrintLevel_Warning == eLevel) ? "warning" : \
    (CoreLog_PrintLevel_Error == eLevel)   ? "error" : \
    "PrintLevel_Unknown"

typedef struct {
  uint32_t level;
  const char * const name;
} CoreLogHandleSructType;

#define CoreLog_NewLogHandle(var, name) \
  static CoreLogHandleSructType CoreLog_##name_handle = {CoreLog_PrintLevel_Warning, name}; \
  CoreLogHandleSructType * var = &CoreLog_##name_handle

void vCoreLog_PrintCli (const char * msg);
typedef void (*vCoreLog_PrintCliCbType) (const char * msg);

void vCoreLog_PrintFatal (unsigned line,
  const char * file, const char * func,
  const char * msg);

typedef void (*vCoreLog_PrintFatalCbType) (unsigned line,
  const char * file, const char * func,
  const char * msg);

void vCoreLog_PrintMsg (CoreLogHandleSructType * handle,
  CoreLog_PrintLevelEnumType eLevel, uint32_t line,
  const char * file, const char * func,
  const char * msg);

typedef void (*vCoreLog_PrintMsgCbType) (CoreLogHandleSructType * handle,
  CoreLog_PrintLevelEnumType eLevel,
  uint32_t line, const char * file, const char * func,
  const char * msg);

void vCoreLog_SetPrintMsgCb (vCoreLog_PrintMsgCbType pftLogHandler);
void vCoreLog_SetPrintFatalCb (vCoreLog_PrintFatalCbType pftLogHandler);
void vCoreLog_SetPrintCliCb (vCoreLog_PrintCliCbType pftLogHandler);

/****************************** Printouts ********************************/

#define ENDL "\r\n"

#define LogMsg(handle, level, fmt, ...) do { \
    char msg[512]; \
    (void) snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    vCoreLog_PrintMsg(handle, level, __LINE__, __FILE__, __func__, msg);} while(0)

#define LogDebugWithArgs(handle, fmt, ...) \
    LogMsg(handle, CoreLog_PrintLevel_Debug, fmt, ##__VA_ARGS__)

#define LogDebug(handle, msg) \
    vCoreLog_PrintMsg(handle, CoreLog_PrintLevel_Debug, __LINE__, __FILE__, __func__, msg)

#define LogInfoWithArgs(handle, fmt, ...) \
    LogMsg(handle, CoreLog_PrintLevel_Info, fmt, ##__VA_ARGS__)

#define LogInfo(handle, msg) \
    vCoreLog_PrintMsg(handle, CoreLog_PrintLevel_Info, __LINE__, __FILE__, __func__, msg)

#define LogWarningWithArgs(handle, fmt, ...) \
    LogMsg(handle, CoreLog_PrintLevel_Warning, fmt, ##__VA_ARGS__)

#define LogWarning(handle, msg) \
    vCoreLog_PrintMsg(handle, CoreLog_PrintLevel_Warning, __LINE__, __FILE__, __func__, msg)

#define LogErrorWithArgs(handle, fmt, ...) \
    LogMsg(handle, CoreLog_PrintLevel_Error, fmt, ##__VA_ARGS__)

#define LogError(handle, msg) \
    vCoreLog_PrintMsg(handle, CoreLog_PrintLevel_Error, __LINE__, __FILE__, __func__, msg)



/****************************** Fatal ********************************/
#define LogFatal(msg) \
    vCoreLog_PrintFatal(__LINE__, __FILE__, __func__, msg)

#define LogFatalWithArgs(fmt, ...) do { \
    char msg[512]; \
    (void) snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    vCoreLog_PrintFatal(__LINE__, __FILE__, __func__, msg);} while(0)

#define Assert(cond) do { \
  if (!(cond)) { \
    LogFatalWithArgs("Assertion failure (%s)", #cond);}} while(0)

#define AssertWithArgs(cond, fmt, ...) do { \
  if (!(cond)) { \
    LogFatalWithArgs("Assertion failure (%s): " fmt, #cond, ##__VA_ARGS__);}} while(0)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif //CORE_LOG_H
