#ifndef List_H
#define List_H

#include "types.h"

// `List` is a simple, dynamically allocated, array of contiguous memory.
//
// Before you use a new list, you should call `ListInit` to initialize the size
// of each element. Once you do, you can add, index, and remove elements of that
// size.
//
// As memory is dynamically allocated, it is a good idea to call `ListFree` when
// you are done with this list to free up it's memory. Note that after you call
// `ListFree`, you can still use the list like normal as it frees the data and
// zeroes the capacity but the list is still valid.
//
// See also the `DecList` and `DefList` macros for ways to create lists specific
// to a certain type.
typedef struct List {
    byte* const data;
    const int itemSize;
    const int len;
    const int cap;
} List;

// `ListInit` initializes a blank list with the given `itemSize`. You can
// specify the initial length and capacity of the list to preallocate memory for
// this list.
//
// This returns true if the allocation was successful, else it returns false.
bool ListInit(List* list, int itemSize, int len, int cap);

// `ListGet` retrieves an element from this list at the specified index. This
// returns nil if the index is out of range.
//
// Note: The pointer returned from `ListGet` is owned by this list. It may
// change, especially if you shift, unshift, pop, or grow this list.
void* ListGet(List* list, int index);

// `ListFree` discards all elements from this list and frees it's memory. Even
// though the memory is freed, the list is still valid to use without
// initializing again; it's capacity is simply set to zero (which may mean
// other operations may need to grow this list again).
//
// You should call this when you are finished using this list.
void ListFree(List* list);

// `ListGrow` tries to grow this list to fit the amount of elements specified by
// `newCap`. This does not modify the length of the list, rather it allocates
// more memory to fit more elements, copies the old data, then changes the lists
// capacity. This allows you to allocate extra memory to minimize calls to the
// allocator.
//
// This returns true if the allocation was successful, else it returns false. If
// `newCap` is smaller than the current capacity, no changes are made and this
// returns true.
bool ListGrow(List* list, int newCap);

// `ListPush` tries to append a new entry and copies the data located by `item`
// into this list. If the list's capacity is too small to accommodate a new
// entry, the list is automatically grown by a scale factor of `1.5 * list.cap`.
//
// This returns true if appending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListPush(List* list, void* item);

// `ListPop` removes an entry from the end of the list. if `dest` is not nil,
// the entry is copied to that location.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListPop(List* list, void* dest);

// `ListUnshift` tries to prepend a new entry and copies the data located by
// `item` into this list. If the list's capacity is too small to accommodate a
// new entry, the list is automatically grown by a scale factor of
// `1.5 * list.cap`. This is slightly more expensive than pushing as the older
// data needs to be copied over to accommodate the new entry.
//
// This returns true if prepending the entry was successful. If the list was
// unable to allocate a new entry then this returns false.
bool ListUnshift(List* list, void* item);

// `ListPop` removes an entry from the beginning of the list. if `dest` is not
// nil, the entry is copied to that location. This is slightly more expensive
// than removing from the beginning as the remaining data needs to be copied
// over.
//
// This returns true if it actually removed an element. It returns false if
// there are no elements to remove.
bool ListShift(List* list, void* dest);

// `DecList` declares a list with entries of type: `Type` and type name: `Name`.
// This defines the type but forward declares the functions without
// implementing or defining them yet (see `DefList`). This allows you to declare
// lists as a struct instead of a pointer as well as get access to list length
// and capacity but might necessitate the use of include guards. or
// `#pragma once`.
//
// This is intended to be placed in header files so as not to duplicate the
// definition of each function.
//
// The functions declared by this macro wrap the ordinary `List` type but ensure
// that the size of entries match the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DecList(Type, Name)                        \
    typedef struct Name {                          \
        Type* const data;                          \
        const int itemSize;                        \
        const int len;                             \
        const int cap;                             \
    } Name;                                        \
    bool Name##Init(Name* list, int len, int cap); \
    Type* Name##Get(struct Name* list, int index); \
    void Name##Free(Name* list);                   \
    bool Name##Grow(Name* list, int newCap);       \
    bool Name##Push(Name* list, Type item);        \
    bool Name##Pop(Name* list, Type* dest);        \
    bool Name##Unshift(Name* list, Type item);     \
    bool Name##Shift(Name* list, Type* dest);

// `DefList` defines the functions of a list declared with `DecList`. The
// functions defined by this macro wrap the ordinary list functions but ensure
// that the size of entries math the size of `Type` and provide some ergonomics
// such as passing entries by the entry type instead of by `void*`.
//
// This is intended to be placed in a C function to avoid duplicating the
// function definitions but it should come after you have declared the list with
// `DecList`.
//
// Make sure you call the init function before you use the list to ensure that
// the size of each entry is properly set.
#define DefList(Type, Name)                                       \
    extern inline bool Name##Init(Name* list, int len, int cap) { \
        return ListInit((List*)list, sizeof(Type), len, cap);     \
    }                                                             \
    extern inline Type* Name##Get(Name* list, int index) {        \
        return (Type*)ListGet((List*)list, index);                \
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