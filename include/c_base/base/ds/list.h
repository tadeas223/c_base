#ifndef LIST_H
#define LIST_H

#include "base/ds/ds.h"
#include "base/types.h"
#include "base/arena.h"
#include "base/errors.h"
#include "system.h"
#include "base/macros.h"

#define List(type) Concat(type, _List)
#define Node(type) Concat(type, _Node)

#define ListInit(type, list) Concat(type, _List_init)((list))
#define ListPushHead(type, arena, list, value) Concat(type, _List_push_head)((arena), (list), (value))
#define ListPush(type, arena, list, value) Concat(type, _List_push)((arena), (list), (value))
#define ListPopHead(type, list) Concat(type, _List_pop_head)((list))
#define ListPop(type, list) Concat(type, _List_pop)((list))
#define ListPeekHead(type, list) Concat(type, _List_peek_head)((list))
#define ListPeek(type, list) Concat(type, _List_peek)((list))
#define ListAdd(type, arena, list, index, value) Concat(type, _List_add)((arena), (list), (index), (value))
#define ListGet(type, list, index) Concat(type, _List_get)((list), (index))
#define ListRemove(type,list, index) Concat(type, _List_remove)((list), (index))

#define ListForeach(type, list, code...) do { \
    Node(type) *node = (list)->head;          \
    type value;                               \
    u32 iter = 0;                             \
    (void)sizeof(iter);                       \
    (void)sizeof(value);                      \
    while(node != null) {                     \
        value = node->value;                  \
        do code while(0);                     \
        node = node->next;                    \
        iter++;                               \
    }                                         \
} while(0)

#define ExternGenericImpl_List(type)\
    void Concat(type, _List_init)(List(type) *list);\
    void Concat(type, _List_push_head)(Arena *arena, List(type) *list, type value);\
    void Concat(type, _List_push)(Arena *arena, List(type) *list, type value);\
    type Concat(type, _List_pop_head)(List(type) *list);\
    type Concat(type, _List_pop)(List(type) *list);\
    type Concat(type, _List_peek_head)(List(type) *list);\
    type Concat(type, _List_peek)(List(type) *list);\
    type Concat(type, _List_get)(List(type) *list, u32 index);\
    void Concat(type, _List_add)(Arena *arena, List(type) *list, u32 index, type value);\
    type Concat(type, _List_remove)(List(type) *list, u32 index);\

#define GenericType_List(type)\
    TypeStruct(Node(type), {type value; Node(type) *next;});\
    TypeStruct(List(type), {Node(type) *head; Node(type) *tail; u32 len;});

#define GenericImpl_List(type)\
    void Concat(type, _List_init)(List(type) *list) {\
        list->head = null;\
        list->tail = null;\
        list->len = 0;\
    }\
    \
    void Concat(type, _List_push_head)(Arena *arena, List(type) *list, type value) {\
        Node(type) *node = arena_alloc(arena, sizeof(*node));\
        node->value = value;\
        node->next = null;\
        \
        switch (list->len) {\
            case 0:\
                list->head = node;\
                break;\
            default:\
                node->next = list->head;\
                list->head = node;\
                break;\
        }\
        list->len++;\
    }\
    \
    void Concat(type, _List_push)(Arena *arena, List(type) *list, type value) {\
        Node(type) *node = arena_alloc(arena, sizeof(*node));\
        node->value = value;\
        node->next = null;\
    \
        switch (list->len) {\
            case 0:\
                list->head = node;\
                break;\
            case 1:\
                list->tail = node;\
                list->head->next = list->tail;\
                break;\
            default:\
                list->tail->next = node;\
                list->tail = node;\
        }\
    \
        list->len++;\
    }\
    \
    type Concat(type, _List_pop_head)(List(type) *list) {\
        type value = list->head->value;\
        switch(list->len) {\
            case 0:\
                crash(E(EG_DataStructures, E_InvalidArgument, S("cannot pop from an empty list")));\
            default:\
                list->head = list->head->next;\
        }\
        list->len--;\
        return value;\
    }\
    \
    type Concat(type, _List_pop)(List(type) *list) {\
        Node(type) *node = list->head;\
        \
        type value = {0};\
    \
        switch(list->len) {\
            case 0:\
                crash(E(EG_DataStructures, E_InvalidArgument, S("cannot pop from an empty list")));\
            case 1:\
                value = list->head->value;\
                list->head = null;\
                break;\
            default:\
                while(node->next->next != null) {\
                    node = node->next; \
                }\
                value = list->tail->value;\
                list->tail = node;\
                break;\
    \
        }\
    \
    \
        list->len--;\
        return value;\
    }\
    \
    type Concat(type, _List_peek_head)(List(type) *list) {\
        if(list->len == 0) {\
            crash(E(EG_DataStructures, E_OutOfBounds, S("cannot peek on a list")));\
        }\
        return list->head->value;\
    }\
    \
    type Concat(type, _List_peek)(List(type) *list) {\
        if(list->len == 0) {\
            crash(E(EG_DataStructures, E_OutOfBounds, S("cannot peek on a list")));\
        }\
        return list->tail->value;\
    }\
    \
    type Concat(type, _List_get)(List(type) *list, u32 index) {\
        if(index >= list->len) {\
            crash(E(EG_DataStructures, E_OutOfBounds, S("index is outside of the list")));\
        }\
    \
        Node(type) *node = list->head;\
        for(u32 i = 0; i < index; i++) {\
            node = node->next;\
        }\
    \
        return node->value;\
    }\
    \
    void Concat(type, _List_add)(Arena *arena, List(type) *list, u32 index, type value) {\
        Node(type) *node = arena_alloc(arena, sizeof(*node));\
        node->value = value;\
        node->next = null;\
    \
        switch(list->len) {\
            case 0:\
                return ListPush(type, arena, list, value);\
            default:\
                Node(type) *prev_node = list->head;\
                for(u32 i = 0; i < index-1; i++) {\
                    prev_node = prev_node->next; \
                }\
    \
                node->next = prev_node->next;\
                prev_node->next = node;\
        }\
    \
        list->len++;\
    }\
    \
    type Concat(type, _List_remove)(List(type) *list, u32 index) {\
        if(index >= list->len) {\
            crash(E(EG_DataStructures, E_OutOfBounds, S("index is outside of the list")));\
        }\
        \
        type value = {0};\
        if(index == list->len-1) {\
            return ListPop(type, list);\
        } else if(index == 0) {\
            value = list->head->value;\
            list->head = list->head->next;\
        } else {\
            Node(type) *prev_node = list->head;\
            for(u32 i = 0; i < index-1; i++) {\
                prev_node = prev_node->next; \
            }\
            \
            value = prev_node->next->value;\
            prev_node->next = prev_node->next->next;\
            \
        }\
        list->len--;\
        return value;\
    }

GenericType_List(ptr);

GenericType_List(u8);
GenericType_List(u16);
GenericType_List(u32);
GenericType_List(u64);

GenericType_List(s8);
GenericType_List(s16);
GenericType_List(s32);
GenericType_List(s64);

GenericType_List(b8);
GenericType_List(b16);
GenericType_List(b32);
GenericType_List(b64);

ExternGenericImpl_List(ptr);

ExternGenericImpl_List(u8);
ExternGenericImpl_List(u16);
ExternGenericImpl_List(u32);
ExternGenericImpl_List(u64);

ExternGenericImpl_List(s8);
ExternGenericImpl_List(s16);
ExternGenericImpl_List(s32);
ExternGenericImpl_List(s64);

ExternGenericImpl_List(b8);
ExternGenericImpl_List(b16);
ExternGenericImpl_List(b32);
ExternGenericImpl_List(b64);

#endif
