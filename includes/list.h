#ifndef List_H
#define List_H

#include "types.h"

typedef struct List List;

bool ListInit(List* list, int itemSize, int len, int cap);
void* ListGet(struct List* list, int index);
void ListFree(List* list);
bool ListGrow(List* list, int newCap);
bool ListPush(List* list, void* item);
bool ListPop(List* list, void* dest);
bool ListUnshift(List* list, void* item);
bool ListShift(List* list, void* dest);

#define DecList(Type, Name)                        \
    typedef struct Name {                          \
        Type* data;                                \
        const int itemSize;                        \
        int len;                                   \
        int cap;                                   \
    } Name;                                        \
    bool Name##Init(Name* list, int len, int cap); \
    Type* Name##Get(struct Name* list, int index); \
    void Name##Free(Name* list);                   \
    bool Name##Grow(Name* list, int newCap);       \
    bool Name##Push(Name* list, Type item);        \
    bool Name##Pop(Name* list, Type* dest);        \
    bool Name##Unshift(Name* list, Type item);     \
    bool Name##Shift(Name* list, Type* dest);

#define DefList(Type, Name)                                       \
    extern inline bool Name##Init(Name* list, int len, int cap) { \
        return ListInit((List*)list, sizeof(Type), len, cap);     \
    }                                                             \
    extern inline Type* Name##Get(struct Name* list, int index) { \
        return &list->data[index];                                \
    }                                                             \
    extern inline void Name##Free(Name* list) {                   \
        ListFree((List*)list);                                    \
    }                                                             \
    extern inline bool Name##Grow(Name* list, int newCap) {       \
        return ListGrow((List*)list, newCap);                     \
    }                                                             \
    extern inline bool Name##Push(Name* list, Type item) {        \
        return ListPush((List*)list, &item);                      \
    }                                                             \
    extern inline bool Name##Pop(Name* list, Type* dest) {        \
        return ListPop((List*)list, dest);                        \
    }                                                             \
    extern inline bool Name##Unshift(Name* list, Type item) {     \
        return ListUnshift((List*)list, &item);                   \
    }                                                             \
    extern inline bool Name##Shift(Name* list, Type* dest) {      \
        return ListShift((List*)list, dest);                      \
    }

#endif  // List_H