/**
 * @file cmw_list.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cmw.h"


/******************************double list****************************************/
#ifdef CMW_USING_LIST

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
void cmw_list_init ( cmw_list_t *l )
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
void cmw_list_insert_after ( cmw_list_t *l, cmw_list_t *n )
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
void cmw_list_insert_before ( cmw_list_t *l, cmw_list_t *n )
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
void cmw_list_remove ( cmw_list_t *n )
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
cmw_s32_t cmw_list_isempty ( const cmw_list_t *l )
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_u32_t cmw_list_len ( const cmw_list_t *l )
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

#endif

/******************************single list****************************************/
#ifdef CMW_USING_SLIST
/**
 * @brief 单向链表初始化
 *
 * @param l
 */
void cmw_slist_init ( cmw_slist_t *l )
{
    l->next = CMW_NULL;
}

/**
 * @brief 单向链表节点添加
 *
 * @param l
 * @param n
 */
void cmw_slist_append ( cmw_slist_t *l, cmw_slist_t *n )
{
    struct cmw_slist_node *node;
    node = l;

    while ( node->next ) { node = node->next; }

    /* append the node to the tail */
    node->next = n;
    n->next = CMW_NULL;
}

/**
 * @brief 单向链表插入节点
 *
 * @param l
 * @param n
 */
void cmw_slist_insert ( cmw_slist_t *l, cmw_slist_t *n )
{
    n->next = l->next;
    l->next = n;
}

/**
 * @brief 获取单向链表长度
 *
 * @param l
 * @return cmw_u32_t
 */
cmw_u32_t cmw_slist_len ( const cmw_slist_t *l )
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

/**
 * @brief 单向链表移除节点
 *
 * @param l
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_remove ( cmw_slist_t *l, cmw_slist_t *n )
{
    /* remove slist head */
    struct cmw_slist_node *node = l;

    while ( node->next && node->next != n ) { node = node->next; }

    /* remove node */
    if ( node->next != ( cmw_slist_t * ) 0 ) { node->next = node->next->next; }

    return l;
}

/**
 * @brief 获取第一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_first ( cmw_slist_t *l )
{
    return l->next;
}

/**
 * @brief 单向链表获取最后一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_tail ( cmw_slist_t *l )
{
    while ( l->next ) { l = l->next; }

    return l;
}

/**
 * @brief 单向链表获取下一个节点
 *
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_next ( cmw_slist_t *n )
{
    return n->next;
}

/**
 * @brief 判断单向链表下一个节点是否存在
 *
 * @param l
 * @return int
 */
cmw_s32_t cmw_slist_isempty ( cmw_slist_t *l )
{
    return l->next == CMW_NULL;
}

#endif

