
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#include "log.h"


static vCoreLog_PrintMsgCbType s_pfOveloadPrintMsgCb = NULL;
static vCoreLog_PrintFatalCbType s_pfOveloadPrintFatalCb = NULL;
static vCoreLog_PrintCliCbType s_pfOveloadPrintCliCb = NULL;

void vCoreLog_PrintCli (const char * msg) {
  assert(msg != NULL);
  if(s_pfOveloadPrintCliCb) {
    s_pfOveloadPrintCliCb(msg);
  } else {
    write(STDOUT_FILENO, msg, strlen(msg));
  }
}

void vCoreLog_PrintFatal (unsigned line,
  const char * file, const char * func,
  const char * msg) {
  assert(NULL != func);
  assert(NULL != file);
  assert(NULL != msg);

  if(s_pfOveloadPrintFatalCb) {
    s_pfOveloadPrintFatalCb(line, file, func, msg);
  }

  time_t ts = 0;
  char errorMsg[512];
  char thName[32];

  pthread_getname_np(pthread_self(), thName, sizeof(thName));
  time(&ts);

  int nb = snprintf(errorMsg,
      sizeof(errorMsg),
      "Error Date: %s"
      "Build Date: " __DATE__ " " __TIME__ ENDL
      "Thread    : %s" ENDL
      "File      : %s" ENDL
      "Function  : %s" ENDL
      "Line      : %d" ENDL
      "Errno     : %s" ENDL
      "Message   : %s" ENDL,
      ctime(&ts),
      thName,
      file,
      func,
      line,
      strerror(errno),
      msg);
  write(STDERR_FILENO, errorMsg, nb);
  //abort();
}

void vCoreLog_PrintMsg (CoreLogHandleSructType * handle,
  CoreLog_PrintLevelEnumType eLevel, uint32_t line,
  const char * file, const char * func,
  const char * msg) {
  assert(NULL != func);
  assert(NULL != file);
  assert(NULL != msg);
  assert(NULL != handle);

  if(s_pfOveloadPrintMsgCb) {
    s_pfOveloadPrintMsgCb(handle, eLevel, line, file, func, msg);
  } else {
    char hdr[64];
    int nb = snprintf(hdr, sizeof(hdr), "%-10s: %-10s: ", handle->name, CoreLog_PrintLevelToString(eLevel));
    if(eLevel >= handle->level) {
      write(STDOUT_FILENO, hdr, nb);
      write(STDOUT_FILENO, msg, strlen(msg));
    }
  }
}

void vCoreLog_SetVerbosity (CoreLogHandleSructType * handle,
  CoreLog_PrintLevelEnumType eLevel) {
  assert(NULL != handle);
  assert(CoreLog_IsValidPrintLevel(eLevel));
  handle->level = eLevel;
}

void vCoreLog_SetPrintMsgCb (vCoreLog_PrintMsgCbType pftLogHandler) {
  s_pfOveloadPrintMsgCb = pftLogHandler;
}

void vCoreLog_SetPrintFatalCb (vCoreLog_PrintFatalCbType pftLogHandler) {
  s_pfOveloadPrintFatalCb = pftLogHandler;
}

void vCoreLog_SetPrintCliCb (vCoreLog_PrintCliCbType pftLogHandler) {
  s_pfOveloadPrintCliCb = pftLogHandler;
}

CoreLog_NewLogHandle(mainLogHandle, "main_program");

int main(void)
{
  vCoreLog_SetVerbosity(mainLogHandle, CoreLog_PrintLevel_Warning);

  LogDebug(mainLogHandle, "Hello LogDebug" ENDL);
  errno = EIDRM;
  LogDebugWithArgs(mainLogHandle, "Hello world: %s" ENDL, strerror(errno));

  LogInfo(mainLogHandle, "Hello LogInfo" ENDL);
  errno = EINTR;
  LogInfoWithArgs(mainLogHandle, "Hello world: %s" ENDL, strerror(errno));

  LogWarning(mainLogHandle, "Hello LogWarning" ENDL);
  errno = EINTR;
  LogWarningWithArgs(mainLogHandle, "Hello world: %s" ENDL, strerror(errno));

  LogError(mainLogHandle, "Hello LogError" ENDL);
  errno = EINTR;
  LogErrorWithArgs(mainLogHandle, "Hello world: %s" ENDL, strerror(errno));
/*

  Assert(5 > 6);

  AssertWithArgs(5 > 6, "Invalid write value (%d > %d)", 5, 6);

  LogFatal("Invalid write argument");

  LogFatalWithArgs("Invalid write value (%d > %d)", 5, 6);

  Assert
  AssertWithArgs

  LogFatal
  LogFatalWithArgs*/
  return 0;
}