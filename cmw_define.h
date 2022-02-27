/**
 * @file cmw_def.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __CMW_DEF_H__
#define __CMW_DEF_H__


#ifdef __cplusplus
extern "C" {
#endif
	
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

//#define CMW_USING_RTT 

/**************************************************************************
*                               模块裁剪                                 *
**************************************************************************/
// #define CMW_USING_CTRL                      // Ctrl 组件
// #define CMW_USING_SPI                       // SPI 组件
// #define CMW_USING_LAT                       // LAT 组件
// #define CMW_USING_SMD
// #define CMW_USING_BTN
#define CMW_USING_LAT
/**************************************************************************/


/**************************************************************************
*                              variable Typedef                           *
**************************************************************************/
typedef char                     cmw_s8_t; 
typedef short int                cmw_s16_t;
typedef int                      cmw_s32_t;
typedef unsigned           char  cmw_u8_t;
typedef unsigned short     int   cmw_u16_t;
typedef unsigned           int   cmw_u32_t;
typedef char*                    cmw_ps8_t;
typedef short int*               cmw_ps16_t;
typedef int*                     cmw_ps32_t;
typedef unsigned           char* cmw_pu8_t;
typedef unsigned short     int*  cmw_pu16_t;
typedef unsigned           int*  cmw_pu32_t;
typedef float                    cmw_f32_t;
typedef double                   cmw_d64_t;
#define  cmw_inline            __inline

typedef int                             cmw_bool_t;      /**< boolean type */
typedef long                            cmw_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   cmw_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef cmw_base_t                      cmw_err_t;       /**< Type for error number */
typedef cmw_u32_t                       cmw_time_t;      /**< Type for time stamp */
typedef cmw_u32_t                       cmw_tick_t;      /**< Type for tick count */
typedef cmw_base_t                      cmw_flag_t;      /**< Type for flags */
typedef cmw_ubase_t                     cmw_size_t;      /**< Type for size number */
typedef cmw_ubase_t                     cmw_dev_t;       /**< Type for device */
typedef cmw_base_t                      cmw_off_t;       /**< Type for offset */


/** @addtogroup 输出类型定义(Exported_types)
  * @{
  */ 
#define CMW_NULL NULL

#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

#define C_CMW_U32_MAXVAL    ((u32_t)0xffffffff)
#define C_CMW_U16_MAXVAL    ((u16_t)0xffff)
#define C_CMW_U8_MAXVAL     ((u8_t)0xff)

/*  error code definitions */
#define CMW_EOK                          0               /**< There is no error */
#define CMW_ERROR                        1               /**< A generic error happens */
#define CMW_ETIMEOUT                     2               /**< Timed out */
#define CMW_EFULL                        3               /**< The resource is full */
#define CMW_EEMPTY                       4               /**< The resource is empty */
#define CMW_ENOMEM                       5               /**< No memory */
#define CMW_ENOSYS                       6               /**< No system */
#define CMW_EBUSY                        7               /**< Busy */
#define CMW_EIO                          8               /**< IO error */
#define CMW_EINTR                        9               /**< Interrupted system call */
#define CMW_EINVAL                       10              /**< Invalid argument */

#ifdef __GNUC__
#  define CMW_NOINLINE __attribute__((noinline))
#  define CMW_SECTION(name) __attribute__ ((section (name)))
#  define CMW_UNUSED __attribute__((unused))
#else
#  define CMW_SECTION(name)
#  define CMW_NOINLINE
#  define CMW_UNUSED
#endif

/**
 * @brief uint32 bit define 
 * 
 */
typedef struct {
	cmw_u32_t u32_bit_0:1;
	cmw_u32_t u32_bit_1:1;
	cmw_u32_t u32_bit_2:1;
	cmw_u32_t u32_bit_3:1;
	cmw_u32_t u32_bit_4:1;
	cmw_u32_t u32_bit_5:1;
	cmw_u32_t u32_bit_6:1;
	cmw_u32_t u32_bit_7:1;
	cmw_u32_t u32_bit_8:1;
	cmw_u32_t u32_bit_9:1;
	cmw_u32_t u32_bit_10:1;
	cmw_u32_t u32_bit_11:1;
	cmw_u32_t u32_bit_12:1;
	cmw_u32_t u32_bit_13:1;
	cmw_u32_t u32_bit_14:1;
	cmw_u32_t u32_bit_15:1;
	cmw_u32_t u32_bit_16:1;
	cmw_u32_t u32_bit_17:1;
	cmw_u32_t u32_bit_18:1;
	cmw_u32_t u32_bit_19:1;
	cmw_u32_t u32_bit_20:1;
	cmw_u32_t u32_bit_21:1;
	cmw_u32_t u32_bit_22:1;
	cmw_u32_t u32_bit_23:1;
	cmw_u32_t u32_bit_24:1;
	cmw_u32_t u32_bit_25:1;
	cmw_u32_t u32_bit_26:1;
	cmw_u32_t u32_bit_27:1;
	cmw_u32_t u32_bit_28:1;
	cmw_u32_t u32_bit_29:1;
	cmw_u32_t u32_bit_30:1;
	cmw_u32_t u32_bit_31:1;
} cmw_uint32_bit_t;


/**
  * @}
  */

/**
 * @brief 通过结构体(tpye)中的成员(member)找到结构体指针(ptr)
 * 
 */
#define cmw_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/* 内存管理 */
void *cmw_malloc(cmw_size_t size);
void *cmw_realloc(void *rmem, cmw_size_t newsize);
void cmw_free(void *rmem);


/** @addtogroup 中间件 公用函数 结构体
  * @{
  */   


typedef void *cmw_rqueue_data_t;

/**
 * @brief 队列对象结构体
 * 
 */
struct cmw_ring_queue 
{
	struct queue_in_data
	{
		cmw_u32_t rear; /* position of queue rear */
		cmw_u32_t front; /* position of queue front */
		cmw_u32_t length; /* maximun length of queue */
		void **p_buffer;
       cmw_u32_t data_size; /* position of queue data size */
	} in_data;
	
	struct queue_out_method
	{
		cmw_bool_t (*reset)(struct cmw_ring_queue *p_queue);
		cmw_bool_t (*enqueue)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data);
		cmw_bool_t (*dequeue)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data);
		cmw_bool_t (*get_front)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data);
        cmw_bool_t (*get_rear)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data);
		cmw_bool_t (*get_front_addr)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t *queue_data);
        cmw_bool_t (*get_rear_addr)(struct cmw_ring_queue *p_queue, cmw_rqueue_data_t *queue_data);
        cmw_u32_t (*get_remain_length)(struct cmw_ring_queue *queue);
	} out_method;
};

typedef struct cmw_ring_queue  cmw_ring_queue_t;

/**
 * @brief 创建队列
 * 
 * @param queue 
 * @param queue_data 
 * @param queue_length 
 * @param data_size 
 * @return cmw_bool_t 
 */
cmw_bool_t cmw_ring_queue_create(cmw_ring_queue_t *queue, cmw_rqueue_data_t *queue_data, uint32_t queue_length, uint32_t data_size);



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
 * @brief 单向链表节点定义
 * 
 */
struct cmw_slist_node
{
    struct cmw_slist_node *next;                         /**< point to next node. */
};
typedef struct cmw_slist_node cmw_slist_t;                /**< Type for single list. */

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
cmw_inline void __cmw_list_init(cmw_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
cmw_inline void __cmw_list_insert_after(cmw_list_t *l, cmw_list_t *n)
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
cmw_inline void __cmw_list_insert_before(cmw_list_t *l, cmw_list_t *n)
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
cmw_inline void __cmw_list_remove(cmw_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
cmw_inline int __cmw_list_isempty(const cmw_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_inline cmw_u32_t __cmw_list_len(const cmw_list_t *l)
{
    cmw_u32_t len = 0;
    const cmw_list_t *p = l;
    while (p->next != l)
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
void cmw_list_init(cmw_list_t *l);

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
void cmw_list_insert_after(cmw_list_t *l, cmw_list_t *n);
/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
void cmw_list_insert_before(cmw_list_t *l, cmw_list_t *n);

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
void cmw_list_remove(cmw_list_t *n);

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
int cmw_list_isempty(const cmw_list_t *l);

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_u32_t cmw_list_len(const cmw_list_t *l);

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
 * @pos:	the cmw_list_t * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define cmw_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * cmw_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the cmw_list_t * to use as a loop cursor.
 * @n:		another cmw_list_t * to use as temporary storage
 * @head:	the head for your list.
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



/** @addtogroup 单向链表 内联函数声明
  * @{
  */ 
#define CMW_SLIST_OBJECT_INIT(object) { CMW_NULL }

 /**
  * @}
  */


/** @addtogroup 单向链表内联函数
  * @{
  */ 

/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
cmw_inline void __cmw_slist_init(cmw_slist_t *l)
{
    l->next = CMW_NULL;
}

cmw_inline void __cmw_slist_append(cmw_slist_t *l, cmw_slist_t *n)
{
    struct cmw_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = CMW_NULL;
}

cmw_inline void __cmw_slist_insert(cmw_slist_t *l, cmw_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

cmw_inline cmw_u32_t __cmw_slist_len(const cmw_slist_t *l)
{
    cmw_u32_t len = 0;
    const cmw_slist_t *list = l->next;
    while (list != CMW_NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

cmw_inline cmw_slist_t *__cmw_slist_remove(cmw_slist_t *l, cmw_slist_t *n)
{
    /* remove slist head */
    struct cmw_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (cmw_slist_t *)0) node->next = node->next->next;

    return l;
}

cmw_inline cmw_slist_t *__cmw_slist_first(cmw_slist_t *l)
{
    return l->next;
}

cmw_inline cmw_slist_t *__cmw_slist_tail(cmw_slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}

cmw_inline cmw_slist_t *__cmw_slist_next(cmw_slist_t *n)
{
    return n->next;
}

cmw_inline cmw_s32_t __cmw_slist_isempty(cmw_slist_t *l)
{
    return l->next == CMW_NULL;
}

 /**
  * @}
  */


/** @addtogroup 单向链表函数声明
  * @{
  */ 

/**
 * @brief 单向链表初始化
 * 
 * @param l 
 */
void cmw_slist_init(cmw_slist_t *l);

/**
 * @brief 单向链表节点添加
 * 
 * @param l 
 * @param n 
 */
void cmw_slist_append(cmw_slist_t *l, cmw_slist_t *n);

/**
 * @brief 单向链表插入节点
 * 
 * @param l 
 * @param n 
 */
void cmw_slist_insert(cmw_slist_t *l, cmw_slist_t *n);

/**
 * @brief 获取单向链表长度
 * 
 * @param l 
 * @return cmw_u32_t 
 */
cmw_u32_t cmw_slist_len(const cmw_slist_t *l);

/**
 * @brief 单向链表移除节点
 * 
 * @param l 
 * @param n 
 * @return cmw_slist_t* 
 */
cmw_slist_t *cmw_slist_remove(cmw_slist_t *l, cmw_slist_t *n);

/**
 * @brief 获取第一个节点
 * 
 * @param l 
 * @return cmw_slist_t* 
 */
cmw_slist_t *cmw_slist_first(cmw_slist_t *l);

/**
 * @brief 单向链表获取最后一个节点
 * 
 * @param l 
 * @return cmw_slist_t* 
 */
cmw_slist_t *cmw_slist_tail(cmw_slist_t *l);

/**
 * @brief 单向链表获取下一个节点
 * 
 * @param n 
 * @return cmw_slist_t* 
 */
cmw_slist_t *cmw_slist_next(cmw_slist_t *n);

/**
 * @brief 判断单向链表下一个节点是否存在
 * 
 * @param l 
 * @return int 
 */
cmw_s32_t cmw_slist_isempty(cmw_slist_t *l);
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




void cmw_assert_handler(const char *ex_string, const char *func, cmw_u32_t line);
#define CMW_ASSERT(EX)                                                        \
if (!(EX))                                                                    \
{                                                                             \
    cmw_assert_handler(#EX, __FUNCTION__, __LINE__);                          \
}

#ifdef __cplusplus
}
#endif

#endif /* __CMW_DEF_H__ */




