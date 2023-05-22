#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "list.h"

typedef struct List {
    byte* data;
    const int itemSize;
    int len;
    int cap;
} List;

bool ListInit(List* list, int itemSize, int len, int cap) {
    cap = cap < len ? len : cap;
    List init = {
        .itemSize = itemSize,
        .cap = cap,
        .len = len,
    };
    memcpy(list, &init, sizeof(List));
    if (cap > 0) {
        list->data = (byte*)malloc(itemSize * cap);
        if (list->data == nil) return false;
        memset(list->data, 0, itemSize * cap);
    }
    return true;
}

void* ListGet(List* list, int index) {
    if (index >= list->cap || index < 0) return nil;
    return &list->data[index * list->itemSize];
}

void ListFree(List* list) {
    if (list->data == nil) return;
    free(list->data);
    list->data = nil;
    list->len = list->cap = 0;
}

bool ListGrow(List* list, int newCap) {
    if (newCap < list->cap) return true;
    byte* newPtr = (byte*)malloc(newCap * list->itemSize);
    if (newPtr == nil) return false;
    memset(newPtr, 0, newCap * list->itemSize);
    memcpy(newPtr, list->data, list->cap * list->itemSize);
    free(list->data);
    list->data = newPtr;
    list->cap = newCap;
    return true;
}

bool ListPush(List* list, void* item) {
    const int index = list->len * list->itemSize;
    if (list->len + 1 > list->cap) {
        int newCap = (list->len * 3) / 2 + 1;
        if (ListGrow(list, newCap) == false) return false;
    }
    list->len++;
    memcpy(&list->data[index], item, list->itemSize);
    return true;
}

bool ListPop(List* list, void* dest) {
    if (list->len == 0) return false;
    const int index = list->len * list->itemSize;
    list->len--;
    if (dest != nil) memcpy(dest, &list->data[index], list->itemSize);
    // zero out removed element
    memset(&list->data[index], 0, list->itemSize);
    return true;
}

bool ListUnshift(List* list, void* item) {
    if (list->len + 1 > list->cap) {
        int newCap;
        if (list->cap > 0)
            newCap = (list->len * 3) / 2;
        else
            newCap = 1;
        if (ListGrow(list, newCap) == false) return false;
    }
    memmove(&list->data[list->itemSize], list->data, list->itemSize * list->len);
    memcpy(list->data, item, list->itemSize);
    return true;
}

bool ListShift(List* list, void* dest) {
    if (list->len == 0) return false;
    if (dest != nil) memcpy(dest, list->data, list->itemSize);
    memmove(list->data, &list->data[list->itemSize], list->itemSize * list->len);
    list->len--;
    // clean end of list
    memset(&list->data[list->len * list->itemSize], 0, list->itemSize);
    return true;
}
