
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
#include <libgen.h>

#include "log.h"

/*
typedef struct simpleLinkedListNode {
  struct simpleLinkedListNode * pHead;
  struct simpleLinkedListNode * pPrev;
  struct simpleLinkedListNode * pNext;
  char c;
} simpleLinkedListNodeStructType;

typedef struct simpleLinkedList {
  const char * name;
  simpleLinkedListNodeStructType * pHead;
  simpleLinkedListNodeStructType * pTail;
  uint8_t u8NodesCount;
  const uint16_t u16NodeDataSize;
} simpleLinkedListStructType;


#define CoreMem_NewSimpleLinkedList(var, name, size) \
  static simpleLinkedListStructType CoreMem_##name_List = {name, NULL, NULL, 0, size}; \
  void * var = &CoreMem_##name_List


void simpleLinkedListInsertAtTail(void * handle, void * data, uint16_t sz) {
  assert(handle);
  assert(data);
  simpleLinkedListStructType * pList = (simpleLinkedListStructType *) data;

  assert(pList->u16NodeDataSize == sz);

  simpleLinkedListNodeStructType * pNode = (simpleLinkedListNodeStructType * ) malloc(sizeof(simpleLinkedListNodeStructType) + sz);
  assert(pNode);
  memset(pNode, 0x00, sizeof(simpleLinkedListNodeStructType) + sz);
  memcpy(&pNode->c, data, sz);

  if(!(pList->pHead)) {
    pList->pHead = pNode;
    pList->pTail = pNode;
  } else {
    pList->pTail
    while
  }

  simpleLinkedListNodeStructType * pHead = pList->pHead;
  while()

}




typedef struct integListTest {
  int x;
} integListTestStructType;

CoreMem_NewSimpleLinkedList(test_simple_list, "test_simple_list", sizeof(integListTestStructType));

*/


#define SIZEOF_ARRARY(buff) (sizeof(buff) / sizeof(buff[0]))

typedef bool (*CliCmdHAndlerCb)(int argc, char * argv[]);

typedef struct CliCmdNode {
  const char * dir;
  const char * cmd;
  CliCmdHAndlerCb cb;
} CliCmdNodeStructType;


static CliCmdNodeStructType s_pstCliCmdNodeList[256];
static uint16_t s_u16CliCmdNodeListSize = 0;


void Cli_AddNewCmNode(const char * dir, const char * cmd, CliCmdHAndlerCb cb) {
  assert(dir && *dir);
  assert(cmd && *cmd);
  assert(cb);

  assert(s_u16CliCmdNodeListSize < SIZEOF_ARRARY(s_pstCliCmdNodeList));

  s_pstCliCmdNodeList[s_u16CliCmdNodeListSize].dir = dir;
  s_pstCliCmdNodeList[s_u16CliCmdNodeListSize].cmd = cmd;
  s_pstCliCmdNodeList[s_u16CliCmdNodeListSize].cb = cb;
  s_u16CliCmdNodeListSize++;
}

CliCmdHAndlerCb Cli_FindCmdNode(const char * path) {
  assert(path && *path);

  char * pPath = strdup(path);
  assert(pPath && *pPath);

  char * dir = dirname(pPath);
  char * cmd = basename(pPath);

  if(dir && cmd && *dir && *cmd) {
    for(uint8_t i = 0; i < s_u16CliCmdNodeListSize; i++) {
      CliCmdNodeStructType * p = &s_pstCliCmdNodeList[i];
      if(p->cb && !strcmp(p->cmd, cmd) && !strcmp(p->dir, dir)) {
        return p->cb;
      }
    }
  }
  return NULL;
}

const char * Cli_ListCmds(const char * path) {
  assert(path && *path);

  static char cmdList[256];
  cmdList[0] = 0;

  char * pPath = strdup(path);
  assert(pPath && *pPath);

  char * dir = dirname(pPath);
  char * subdir = basename(pPath);
  bool subdirIsAppended=false;

  for(uint8_t i = 0; i < s_u16CliCmdNodeListSize; i++) {
    CliCmdNodeStructType * p = &s_pstCliCmdNodeList[i];
    if(!strcmp(p->dir, path)) {
      //Append all commands here
    } else if(!subdirIsAppended)

    }
  }
  return cmdList;
}

int main(void)
{
  return 0;
}