#ifndef LIST_H
#define LIST_H

/* singly linked list */
#define ListInit(type, list, pool) type##List_init(list, pool)
#define ListPush(type, list, data) type##List_push(list, data)
#define ListPop(type, list) type##List_pop(list)
#define ListAdd(type, list, data, index) type##List_add(list, datam, index)
#define ListRemove(type, list, index) type##List_remove(list, index)
#define ListGet(type, list, index) type##List_get(list, index)
#define ListLength(list) (list)->length

#define List(type) type##List

#define ListPrototype(type)\
typedef struct type##Node {\
    struct type##Node *next;\
    type data;\
} type##Node;\
typedef struct type##List {\
    type##Node *head;\
    u64 length;\
    m_Pool *pool;\
} type##List;\
void type##NodePool_init(m_Pool *pool);\
void List_init(type##List *list, m_Pool *pool);\
void List_push(type##List* list, type data);\
type* List_pop(type##List* list);\
type* List_remove(type##List* list, u64 index);\
void List_add(type##List* list, type data, u64 index);

#define ListImpl(type)\
void type##NodePool_init(m_Pool *pool) {\
    m_pool_init(pool, sizeof(type##Node));\
}\
void type##List_init(type##List *list, m_Pool *pool) {\
    type##NodePool_init(pool);\
    list->pool = pool;\
    list->head = null;\
    list->length = 0;\
}\
void type##List_cleanup(type##List *list) {\
    m_pool_cleanup((m_Pool*) list->pool);\
}\
void type##List_push(type##List *list, type data) {\
    type##Node *node = m_pool_alloc(list->pool);\
    type##Node *lastNode = list->head;\
    node->data = data;\
    node->next = null;\
    if(lastNode->next == null) {\
        list->head = node; \
        list->head = node;\
    } else {\
        while(lastNode->next != null) { lastNode = lastNode->next; }\
        lastNode->next = node;\
    }\
    list->length++;\
}\
type type##List_pop(type##List* list) {\
    type result;\
    type##Node *node = list->head;\
    if(node->next != null) {\
        while(node->next->next != null) { node = node->next; }\
        result = node->next->data;\
        m_pool_dealloc(list->pool, node->next);\
        node->next = null;\
    } else {\
        result = node->data;\
        m_pool_dealloc(list->pool, node);\
        list->head = null;\
    }\
    list->length--;\
    return result;\
}\
type type##List_remove(type##List *list, u64 index) {\
    type result;\
    type##Node *node = list->head;\
    u64 i;\
    for(i = 0; i < index - 1; i++) { node = node->next; }\
    result = node->next->data;\
    node->next = node->next->next;\
    m_pool_dealloc(list->pool, node->next);\
    list->length--;\
    return result;\
}\
void type##List_add(type##List* list, type data, u64 index) {\
    type##Node *node = list->head;\
    type##Node *new_node = m_pool_alloc(list->pool);\
    new_node->data = data;\
    u64 i;\
    for(i = 0; i < index - 1; i++) { node = node->next; }\
    new_node->next = node->next->next;\
    node->next = new_node;\
    list->length++;\
}\
type type##List_get(type##List *list, u64 index) {\
    type##Node *node = list->head;\
    u64 i;\
    for(i = 0; i < index; i++) { node = node->next; }\
    return node->data;\
}

#endif
