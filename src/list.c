#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "list.h"

bool ListInit(List* list, int itemSize, int len, int cap) {
    cap = cap < len ? len : cap;
    void* ptr = nil;
    if (cap > 0) {
        ptr = (byte*)malloc(itemSize * cap);
        if (ptr == nil) return false;
        memset(ptr, 0, itemSize * cap);
    }
    memcpy(list,
        &(List){
            .data = ptr,
            .itemSize = itemSize,
            .cap = cap,
            .len = len,
        },
        sizeof(List));
    return true;
}

void* ListGet(List* list, int index) {
    if (index >= list->cap || index < 0) return nil;
    return &list->data[index * list->itemSize];
}

void ListFree(List* list) {
    if (list->data == nil) return;
    free(list->data);

    memcpy(list,
        &(List){
            .data = nil,
            .itemSize = list->itemSize,
            .cap = 0,
            .len = 0,
        },
        sizeof(List));
}

bool ListGrow(List* list, int newCap) {
    if (newCap < list->cap) return true;
    byte* newPtr = (byte*)malloc(newCap * list->itemSize);
    if (newPtr == nil) return false;
    memset(newPtr, 0, newCap * list->itemSize);
    memcpy(newPtr, list->data, list->cap * list->itemSize);
    free(list->data);
    memcpy(list,
        &(List){
            .data = newPtr,
            .itemSize = list->itemSize,
            .cap = newCap,
            .len = list->len,
        },
        sizeof(List));
    return true;
}

bool ListPush(List* list, void* item) {
    const int index = list->len * list->itemSize;
    if (list->len + 1 > list->cap) {
        int newCap = (list->len * 3) / 2 + 1;
        if (ListGrow(list, newCap) == false) return false;
    }
    memcpy(&list->data[index], item, list->itemSize);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len + 1,
        },
        sizeof(List));
    return true;
}

bool ListPop(List* list, void* dest) {
    if (list->len == 0) return false;
    const int index = list->len * list->itemSize;
    if (dest != nil) memcpy(dest, &list->data[index], list->itemSize);
    // zero out removed element
    memset(&list->data[index], 0, list->itemSize);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len - 1,
        },
        sizeof(List));
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
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len + 1,
        },
        sizeof(List));
    return true;
}

bool ListShift(List* list, void* dest) {
    if (list->len == 0) return false;
    if (dest != nil) memcpy(dest, list->data, list->itemSize);
    memmove(list->data, &list->data[list->itemSize], list->itemSize * list->len);
    memcpy(list,
        &(List){
            .data = list->data,
            .itemSize = list->itemSize,
            .cap = list->cap,
            .len = list->len - 1,
        },
        sizeof(List));
    // clean end of list
    memset(&list->data[list->len * list->itemSize], 0, list->itemSize);
    return true;
}