/**
 * @file cmw_list.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef __CMW_LIST_H__
#define __CMW_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "cmw_define.h"


#ifdef CMW_USING_LIST

/**
 * @brief 双向链表 节点定义
 *
 */
struct cmw_list_node
{
    struct cmw_list_node *next;                          /**< point to next node. */
    struct cmw_list_node *prev;                          /**< point to prev node. */
};
typedef struct cmw_list_node cmw_list_t;                  /**< Type for lists. */



/**
 * @}
 */

/** @addtogroup 双向链表 内联函数声明
  * @{
  */

/**
 * @brief initialize a list object
 */
#define CMW_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief 定义一个链表并初始化
 *
 */
#define CMW_LIST_HEAD(name) cmw_list_t name = CMW_LIST_OBJECT_INIT(name)

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
cmw_inline void __cmw_list_init ( cmw_list_t *l )
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
cmw_inline void __cmw_list_insert_after ( cmw_list_t *l, cmw_list_t *n )
{
    l->next->prev = n;
    n->next = l->next;
    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
cmw_inline void __cmw_list_insert_before ( cmw_list_t *l, cmw_list_t *n )
{
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
cmw_inline void __cmw_list_remove ( cmw_list_t *n )
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
cmw_inline int __cmw_list_isempty ( const cmw_list_t *l )
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_inline cmw_u32_t __cmw_list_len ( const cmw_list_t *l )
{
    cmw_u32_t len = 0;
    const cmw_list_t *p = l;

    while ( p->next != l )
    {
        p = p->next;
        len ++;
    }

    return len;
}
/**
 * @}
 */

/** @addtogroup 双向链表函数声明
  * @{
  */

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
void cmw_list_init ( cmw_list_t *l );

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
void cmw_list_insert_after ( cmw_list_t *l, cmw_list_t *n );
/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
void cmw_list_insert_before ( cmw_list_t *l, cmw_list_t *n );

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
void cmw_list_remove ( cmw_list_t *n );

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
int cmw_list_isempty ( const cmw_list_t *l );

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_u32_t cmw_list_len ( const cmw_list_t *l );

/**
 * @}
 */

/** @addtogroup 双向链表 宏定义函数
  * @{
  */

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define cmw_list_entry(node, type, member) \
    cmw_container_of(node, type, member)

/**
 * cmw_list_for_each - iterate over a list
 * @pos:    the cmw_list_t * to use as a loop cursor.
 * @head:   the head for your list.
 */
#define cmw_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * cmw_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the cmw_list_t * to use as a loop cursor.
 * @n:      another cmw_list_t * to use as temporary storage
 * @head:   the head for your list.
 */
#define cmw_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
            pos = n, n = pos->next)

/**
 * cmw_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define cmw_list_for_each_entry(pos, head, member) \
    for (pos = cmw_list_entry((head)->next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = cmw_list_entry(pos->member.next, typeof(*pos), member))

/**
 * cmw_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define cmw_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = cmw_list_entry((head)->next, typeof(*pos), member), \
            n = cmw_list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = n, n = cmw_list_entry(n->member.next, typeof(*n), member))

#endif


#ifdef CMW_USING_SLIST

/** @addtogroup 单向链表 内联函数声明
  * @{
  */
#define CMW_SLIST_OBJECT_INIT(object) { CMW_NULL }

/**
 * @}
 */

/**
 * @brief 单向链表节点定义
 *
 */
struct cmw_slist_node
{
    struct cmw_slist_node *next;                         /**< point to next node. */
};
typedef struct cmw_slist_node cmw_slist_t;                /**< Type for single list. */


/** @addtogroup 单向链表内联函数
  * @{
  */

/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
cmw_inline void __cmw_slist_init ( cmw_slist_t *l )
{
    l->next = CMW_NULL;
}

cmw_inline void __cmw_slist_append ( cmw_slist_t *l, cmw_slist_t *n )
{
    struct cmw_slist_node *node;
    node = l;

    while ( node->next ) { node = node->next; }

    /* append the node to the tail */
    node->next = n;
    n->next = CMW_NULL;
}

cmw_inline void __cmw_slist_insert ( cmw_slist_t *l, cmw_slist_t *n )
{
    n->next = l->next;
    l->next = n;
}

cmw_inline cmw_u32_t __cmw_slist_len ( const cmw_slist_t *l )
{
    cmw_u32_t len = 0;
    const cmw_slist_t *list = l->next;

    while ( list != CMW_NULL )
    {
        list = list->next;
        len ++;
    }

    return len;
}

cmw_inline cmw_slist_t *__cmw_slist_remove ( cmw_slist_t *l, cmw_slist_t *n )
{
    /* remove slist head */
    struct cmw_slist_node *node = l;

    while ( node->next && node->next != n ) { node = node->next; }

    /* remove node */
    if ( node->next != ( cmw_slist_t * ) 0 ) { node->next = node->next->next; }

    return l;
}

cmw_inline cmw_slist_t *__cmw_slist_first ( cmw_slist_t *l )
{
    return l->next;
}

cmw_inline cmw_slist_t *__cmw_slist_tail ( cmw_slist_t *l )
{
    while ( l->next ) { l = l->next; }

    return l;
}

cmw_inline cmw_slist_t *__cmw_slist_next ( cmw_slist_t *n )
{
    return n->next;
}

cmw_inline cmw_s32_t __cmw_slist_isempty ( cmw_slist_t *l )
{
    return l->next == CMW_NULL;
}



/** @addtogroup 单向链表函数声明
  * @{
  */

/**
 * @brief 单向链表初始化
 *
 * @param l
 */
void cmw_slist_init ( cmw_slist_t *l );

/**
 * @brief 单向链表节点添加
 *
 * @param l
 * @param n
 */
void cmw_slist_append ( cmw_slist_t *l, cmw_slist_t *n );

/**
 * @brief 单向链表插入节点
 *
 * @param l
 * @param n
 */
void cmw_slist_insert ( cmw_slist_t *l, cmw_slist_t *n );

/**
 * @brief 获取单向链表长度
 *
 * @param l
 * @return cmw_u32_t
 */
cmw_u32_t cmw_slist_len ( const cmw_slist_t *l );

/**
 * @brief 单向链表移除节点
 *
 * @param l
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_remove ( cmw_slist_t *l, cmw_slist_t *n );

/**
 * @brief 获取第一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_first ( cmw_slist_t *l );

/**
 * @brief 单向链表获取最后一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_tail ( cmw_slist_t *l );

/**
 * @brief 单向链表获取下一个节点
 *
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_next ( cmw_slist_t *n );

/**
 * @brief 判断单向链表下一个节点是否存在
 *
 * @param l
 * @return int
 */
cmw_s32_t cmw_slist_isempty ( cmw_slist_t *l );
/**
 * @}
 */


/** @addtogroup 单向链表 宏定义函数
  * @{
  */

/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define cmw_slist_entry(node, type, member) \
    cmw_container_of(node, type, member)

/**
 * cmw_slist_for_each - iterate over a single list
 * @pos:    the cmw_slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define cmw_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != CMW_NULL; pos = pos->next)

/**
 * cmw_slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define cmw_slist_for_each_entry(pos, head, member) \
    for (pos = rt_slist_entry((head)->next, typeof(*pos), member); \
            &pos->member != (CMW_NULL); \
            pos = rt_slist_entry(pos->member.next, typeof(*pos), member))

/**
 * cmw_slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define cmw_slist_first_entry(ptr, type, member) \
    cmw_slist_entry((ptr)->next, type, member)

/**
 * cmw_slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.df
 *
 * Note, that slist is expected to be not empty.
 */
#define cmw_slist_tail_entry(ptr, type, member) \
    cmw_slist_entry(cmw_slist_tail(ptr), type, member)

/**
 * @}
 */

#endif


#ifdef __cplusplus
}
#endif


#endif /* __CMW_H__ */


