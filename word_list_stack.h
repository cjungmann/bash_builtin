// #include <builtins.h>
#include <command.h>              // for COMMAND.flags, among other things
// #include <shell.h>
// #include <builtins/bashgetopt.h>  // for internal_getopt(), etc.
// #include <builtins/common.h>      // for no_options()

#include <alloca.h>

#ifndef WORD_LIST_STACK_H
#define WORD_LIST_STACK_H

#define WL_WORDSIZE(STR) sizeof((STR))
#define WL_SIZE(STR) sizeof(WORD_LIST) + sizeof(WORD_DESC) + strlen((STR)) + 1
#define WL_A_STR(BLOCK) (char*)((char*)(BLOCK)+sizeof(WORD_LIST)+sizeof(WORD_DESC))
#define WL_P_DESC(BLOCK) ((WORD_DESC*)((char*)(BLOCK))+sizeof(WORD_LIST))
#define WL_P_LIST(BLOCK) ((WORD_LIST*)(BLOCK))

#define WL_COPYWORD(BLOCK, STR) strcpy(WL_A_STR((BLOCK)),(char*)(STR))
#define WL_INITDESC(BLOCK, STR) WL_COPYWORD((BLOCK),(STR)); WL_P_DESC((BLOCK))->word=WL_A_STR((BLOCK))
#define WL_INITLIST(BLOCK, STR) WL_INITDESC((BLOCK),(STR)); WL_P_LIST((BLOCK))->word=WL_P_DESC((BLOCK))

#define WL_ALLOC(STR) (WORD_LIST*)memset(alloca(WL_SIZE((STR))),0,WL_SIZE((STR)))

#define WL_APPEND(HEAD, TAIL, ARREL) \
while (*(ARREL)) {               \
   WORD_LIST *stail=(TAIL);      \
   (TAIL) = WL_ALLOC(*(ARREL));  \
   if ((TAIL) == NULL)           \
      break;                     \
   WL_INITLIST((TAIL),*(ARREL)); \
   if (stail)                    \
      stail->next = (TAIL);      \
   else                          \
      (HEAD) = (TAIL);           \
   ++(ARREL);                    \
}


#endif
