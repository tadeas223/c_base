#ifndef DS_H
#define DS_H

#include "base/types.h"
#include "base/defines.h"

#define DListVars(node_type) node_type *head, *tail; u64 count
#define DNodeVars(node_name) node_name *next, *prev
#define DListCount(list) (list)->count

#define DListPush(list, node)                                                                  \
    Stmt(                                                                                      \
        (node)->next = null;                                                                   \
        (node)->prev = null;                                                                   \
        if((list)->head == null) {                                                             \
            (list)->head = (node);                                                             \
        } else if((list)->tail == null) {                                                      \
            (list)->tail = (node);                                                             \
            (list)->tail->prev = (list)->head;                                                 \
            (list)->head->next = (list)->tail;                                                 \
        } else {                                                                               \
            (list)->tail->next = (node);                                                       \
            (node)->prev = (list)->tail;                                                       \
            (list)->tail = (node);                                                             \
        }                                                                                      \
        (list)->count++;                                                                       \
    )

#define DListPushHead(list, node)                                                              \
    Stmt(                                                                                      \
        (node)->next = null;                                                                   \
        (node)->prev = null;                                                                   \
        if((list)->head != null) {                                                             \
            (node)->next = (list)->head;                                                       \
            (list)->head->prev = (node);                                                       \
        }                                                                                      \
        (list)->head = (node);                                                                 \
        (list)->count++;                                                                       \
    )

#define DListPop(list)                                                                         \
    Stmt(                                                                                      \
        (list)->tail = (list)->tail->prev;                                                     \
        (list)->tail->next = null;                                                             \
        (list)->count--;                                                                       \
    )

#define DListPopHead(list)                                                                     \
    Stmt(                                                                                      \
        (list)->head = (list)->head->hext;                                                     \
        (list)->head->prev = null;                                                             \
        (list)->count--;                                                                       \
    )

#define DListPeek(list, node_var)                                                              \
    Stmt(                                                                                      \
        if((list)->head == null) {                                                             \
            (node_var) = null;                                                                 \
        } else if((list)->tail == null) {                                                      \
            (node_var) = (list)->head;                                                         \
        } else {                                                                               \
            (node_var) = (list)->tail;                                                         \
        }                                                                                      \
    )

#define DListPeekHead(list, node_var)                                                         \
    Stmt(                                                                                     \
        if((list)->head == null) {                                                            \
            (node_var) = null;                                                                \
        } else {                                                                              \
            (node_var) = (list)->head;                                                        \
        }                                                                                     \
    )

#define DListAdd(list, index, node_var)                                                       \
    Stmt(                                                                                     \
        (node_var)->prev = (list)->head;                                                       \
        u64 _list_node_current;                                                               \
        for(_list_node_current = 0; _list_node_current < index-1; _list_node_current++) {       \
            (node_var)->prev = (node_var)->prev->next;                                        \
        }                                                                                     \
        (node_var)->prev->next->prev = (node_var);                                            \
        (node_var)->next = (node_var)->prev->next;                                            \
        (node_var)->prev->next = (node_var);                                                  \
        (list)->count++;                                                                      \
    )

#define DListRemove(list, index, calc_var)                                                    \
    Stmt(                                                                                     \
        (calc_var) = (list)->head;                                                            \
        u64 _list_node_current;                                                               \
        for(_list_node_current = 0; _list_node_current < index; _list_node_current++) {       \
            (calc_var) = (calc_var)->next;                                                    \
        }                                                                                     \
        (calc_var)->prev->next = (calc_var)->next;                                            \
        (calc_var)->next->prev = (calc_var)->prev;                                            \
        (list)->count--;                                                                      \
    )

#define DListGet(list, index, node_var)                                                       \
    Stmt(                                                                                     \
        (node_var) = (list)->head;                                                            \
        u64 _list_node_current;                                                               \
        for(_list_node_current = 0; _list_node_current < index; _list_node_current++) {       \
            (node_var) = (node_var)->next;                                                    \
        }                                                                                     \
    )

#define DListIter(list, node_var, code)                                                       \
    Stmt(                                                                                     \
        (node_var) = (list)->head;                                                              \
        while((node_var) != null) {                                                           \
            Stmt(code);                                                                       \
            (node_var) = (node_var)->next;                                                    \
        }                                                                                     \
    )
#endif
