/**
 * @file mdl_def.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __MDL_DEF_H__
#define __MDL_DEF_H__


#ifdef __cplusplus
extern "C" {
#endif
	
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

//#define MDL_USING_RTT 

/**************************************************************************
*                               模块裁剪                                 *
**************************************************************************/
// #define MDL_USING_CTRL                      // Ctrl 组件
// #define MDL_USING_SPI                       // SPI 组件
// #define MDL_USING_LAT                       // LAT 组件
// #define MDL_USING_SMD
// #define MDL_USING_BTN
#define MDL_USING_LAT
/**************************************************************************/


/**************************************************************************
*                              variable Typedef                           *
**************************************************************************/
typedef char                     mdl_s8_t; 
typedef short int                mdl_s16_t;
typedef int                      mdl_s32_t;
typedef unsigned           char  mdl_u8_t;
typedef unsigned short     int   mdl_u16_t;
typedef unsigned           int   mdl_u32_t;
typedef char*                    mdl_ps8_t;
typedef short int*               mdl_ps16_t;
typedef int*                     mdl_ps32_t;
typedef unsigned           char* mdl_pu8_t;
typedef unsigned short     int*  mdl_pu16_t;
typedef unsigned           int*  mdl_pu32_t;
typedef float                    mdl_f32_t;
typedef double                   mdl_d64_t;
#define  mdl_inline            __inline

typedef int                             mdl_bool_t;      /**< boolean type */
typedef long                            mdl_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   mdl_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef mdl_base_t                      mdl_err_t;       /**< Type for error number */
typedef mdl_u32_t                       mdl_time_t;      /**< Type for time stamp */
typedef mdl_u32_t                       mdl_tick_t;      /**< Type for tick count */
typedef mdl_base_t                      mdl_flag_t;      /**< Type for flags */
typedef mdl_ubase_t                     mdl_size_t;      /**< Type for size number */
typedef mdl_ubase_t                     mdl_dev_t;       /**< Type for device */
typedef mdl_base_t                      mdl_off_t;       /**< Type for offset */


/** @addtogroup 输出类型定义(Exported_types)
  * @{
  */ 
#define MDL_NULL NULL

#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

#define C_MDL_U32_MAXVAL    ((u32_t)0xffffffff)
#define C_MDL_U16_MAXVAL    ((u16_t)0xffff)
#define C_MDL_U8_MAXVAL     ((u8_t)0xff)

/*  error code definitions */
#define MDL_EOK                          0               /**< There is no error */
#define MDL_ERROR                        1               /**< A generic error happens */
#define MDL_ETIMEOUT                     2               /**< Timed out */
#define MDL_EFULL                        3               /**< The resource is full */
#define MDL_EEMPTY                       4               /**< The resource is empty */
#define MDL_ENOMEM                       5               /**< No memory */
#define MDL_ENOSYS                       6               /**< No system */
#define MDL_EBUSY                        7               /**< Busy */
#define MDL_EIO                          8               /**< IO error */
#define MDL_EINTR                        9               /**< Interrupted system call */
#define MDL_EINVAL                       10              /**< Invalid argument */

#ifdef __GNUC__
#  define MDL_NOINLINE __attribute__((noinline))
#  define MDL_SECTION(name) __attribute__ ((section (name)))
#  define MDL_UNUSED __attribute__((unused))
#else
#  define MDL_SECTION(name)
#  define MDL_NOINLINE
#  define MDL_UNUSED
#endif

/**
 * @brief uint32 bit define 
 * 
 */
typedef struct {
	mdl_u32_t u32_bit_0:1;
	mdl_u32_t u32_bit_1:1;
	mdl_u32_t u32_bit_2:1;
	mdl_u32_t u32_bit_3:1;
	mdl_u32_t u32_bit_4:1;
	mdl_u32_t u32_bit_5:1;
	mdl_u32_t u32_bit_6:1;
	mdl_u32_t u32_bit_7:1;
	mdl_u32_t u32_bit_8:1;
	mdl_u32_t u32_bit_9:1;
	mdl_u32_t u32_bit_10:1;
	mdl_u32_t u32_bit_11:1;
	mdl_u32_t u32_bit_12:1;
	mdl_u32_t u32_bit_13:1;
	mdl_u32_t u32_bit_14:1;
	mdl_u32_t u32_bit_15:1;
	mdl_u32_t u32_bit_16:1;
	mdl_u32_t u32_bit_17:1;
	mdl_u32_t u32_bit_18:1;
	mdl_u32_t u32_bit_19:1;
	mdl_u32_t u32_bit_20:1;
	mdl_u32_t u32_bit_21:1;
	mdl_u32_t u32_bit_22:1;
	mdl_u32_t u32_bit_23:1;
	mdl_u32_t u32_bit_24:1;
	mdl_u32_t u32_bit_25:1;
	mdl_u32_t u32_bit_26:1;
	mdl_u32_t u32_bit_27:1;
	mdl_u32_t u32_bit_28:1;
	mdl_u32_t u32_bit_29:1;
	mdl_u32_t u32_bit_30:1;
	mdl_u32_t u32_bit_31:1;
} mdl_uint32_bit_t;


/**
  * @}
  */

/**
 * @brief 通过结构体(tpye)中的成员(member)找到结构体指针(ptr)
 * 
 */
#define mdl_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/* 内存管理 */
void *mdl_malloc(mdl_size_t size);
void *mdl_realloc(void *rmem, mdl_size_t newsize);
void mdl_free(void *rmem);


/** @addtogroup 中间件 公用函数 结构体
  * @{
  */   


typedef void *mdl_rqueue_data_t;

/**
 * @brief 队列对象结构体
 * 
 */
struct mdl_ring_queue 
{
	struct queue_in_data
	{
		mdl_u32_t rear; /* position of queue rear */
		mdl_u32_t front; /* position of queue front */
		mdl_u32_t length; /* maximun length of queue */
		void **p_buffer;
       mdl_u32_t data_size; /* position of queue data size */
	} in_data;
	
	struct queue_out_method
	{
		mdl_bool_t (*reset)(struct mdl_ring_queue *p_queue);
		mdl_bool_t (*enqueue)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t queue_data);
		mdl_bool_t (*dequeue)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t queue_data);
		mdl_bool_t (*get_front)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t queue_data);
        mdl_bool_t (*get_rear)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t queue_data);
		mdl_bool_t (*get_front_addr)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t *queue_data);
        mdl_bool_t (*get_rear_addr)(struct mdl_ring_queue *p_queue, mdl_rqueue_data_t *queue_data);
        mdl_u32_t (*get_remain_length)(struct mdl_ring_queue *queue);
	} out_method;
};

typedef struct mdl_ring_queue  mdl_ring_queue_t;

/**
 * @brief 创建队列
 * 
 * @param queue 
 * @param queue_data 
 * @param queue_length 
 * @param data_size 
 * @return mdl_bool_t 
 */
mdl_bool_t mdl_ring_queue_create(mdl_ring_queue_t *queue, mdl_rqueue_data_t *queue_data, uint32_t queue_length, uint32_t data_size);



/**
 * @brief 双向链表 节点定义
 * 
 */
struct mdl_list_node
{
    struct mdl_list_node *next;                          /**< point to next node. */
    struct mdl_list_node *prev;                          /**< point to prev node. */
};
typedef struct mdl_list_node mdl_list_t;                  /**< Type for lists. */

/**
 * @brief 单向链表节点定义
 * 
 */
struct mdl_slist_node
{
    struct mdl_slist_node *next;                         /**< point to next node. */
};
typedef struct mdl_slist_node mdl_slist_t;                /**< Type for single list. */

 /**
  * @}
  */

/** @addtogroup 双向链表 内联函数声明
  * @{
  */ 

/**
 * @brief initialize a list object
 */
#define MDL_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief 定义一个链表并初始化
 * 
 */
#define MDL_LIST_HEAD(name) mdl_list_t name = MDL_LIST_OBJECT_INIT(name)

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
mdl_inline void __mdl_list_init(mdl_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
mdl_inline void __mdl_list_insert_after(mdl_list_t *l, mdl_list_t *n)
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
mdl_inline void __mdl_list_insert_before(mdl_list_t *l, mdl_list_t *n)
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
mdl_inline void __mdl_list_remove(mdl_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
mdl_inline int __mdl_list_isempty(const mdl_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
mdl_inline mdl_u32_t __mdl_list_len(const mdl_list_t *l)
{
    mdl_u32_t len = 0;
    const mdl_list_t *p = l;
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
void mdl_list_init(mdl_list_t *l);

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
void mdl_list_insert_after(mdl_list_t *l, mdl_list_t *n);
/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
void mdl_list_insert_before(mdl_list_t *l, mdl_list_t *n);

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
void mdl_list_remove(mdl_list_t *n);

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
int mdl_list_isempty(const mdl_list_t *l);

/**
 * @brief get the list length
 * @param l the list to get.
 */
mdl_u32_t mdl_list_len(const mdl_list_t *l);

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
#define mdl_list_entry(node, type, member) \
    mdl_container_of(node, type, member)

/**
 * mdl_list_for_each - iterate over a list
 * @pos:	the mdl_list_t * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define mdl_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * mdl_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the mdl_list_t * to use as a loop cursor.
 * @n:		another mdl_list_t * to use as temporary storage
 * @head:	the head for your list.
 */
#define mdl_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * mdl_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define mdl_list_for_each_entry(pos, head, member) \
    for (pos = mdl_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = mdl_list_entry(pos->member.next, typeof(*pos), member))

/**
 * mdl_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define mdl_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = mdl_list_entry((head)->next, typeof(*pos), member), \
         n = mdl_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = mdl_list_entry(n->member.next, typeof(*n), member))



/** @addtogroup 单向链表 内联函数声明
  * @{
  */ 
#define MDL_SLIST_OBJECT_INIT(object) { MDL_NULL }

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
mdl_inline void __mdl_slist_init(mdl_slist_t *l)
{
    l->next = MDL_NULL;
}

mdl_inline void __mdl_slist_append(mdl_slist_t *l, mdl_slist_t *n)
{
    struct mdl_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = MDL_NULL;
}

mdl_inline void __mdl_slist_insert(mdl_slist_t *l, mdl_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

mdl_inline mdl_u32_t __mdl_slist_len(const mdl_slist_t *l)
{
    mdl_u32_t len = 0;
    const mdl_slist_t *list = l->next;
    while (list != MDL_NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

mdl_inline mdl_slist_t *__mdl_slist_remove(mdl_slist_t *l, mdl_slist_t *n)
{
    /* remove slist head */
    struct mdl_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (mdl_slist_t *)0) node->next = node->next->next;

    return l;
}

mdl_inline mdl_slist_t *__mdl_slist_first(mdl_slist_t *l)
{
    return l->next;
}

mdl_inline mdl_slist_t *__mdl_slist_tail(mdl_slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}

mdl_inline mdl_slist_t *__mdl_slist_next(mdl_slist_t *n)
{
    return n->next;
}

mdl_inline mdl_s32_t __mdl_slist_isempty(mdl_slist_t *l)
{
    return l->next == MDL_NULL;
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
void mdl_slist_init(mdl_slist_t *l);

/**
 * @brief 单向链表节点添加
 * 
 * @param l 
 * @param n 
 */
void mdl_slist_append(mdl_slist_t *l, mdl_slist_t *n);

/**
 * @brief 单向链表插入节点
 * 
 * @param l 
 * @param n 
 */
void mdl_slist_insert(mdl_slist_t *l, mdl_slist_t *n);

/**
 * @brief 获取单向链表长度
 * 
 * @param l 
 * @return mdl_u32_t 
 */
mdl_u32_t mdl_slist_len(const mdl_slist_t *l);

/**
 * @brief 单向链表移除节点
 * 
 * @param l 
 * @param n 
 * @return mdl_slist_t* 
 */
mdl_slist_t *mdl_slist_remove(mdl_slist_t *l, mdl_slist_t *n);

/**
 * @brief 获取第一个节点
 * 
 * @param l 
 * @return mdl_slist_t* 
 */
mdl_slist_t *mdl_slist_first(mdl_slist_t *l);

/**
 * @brief 单向链表获取最后一个节点
 * 
 * @param l 
 * @return mdl_slist_t* 
 */
mdl_slist_t *mdl_slist_tail(mdl_slist_t *l);

/**
 * @brief 单向链表获取下一个节点
 * 
 * @param n 
 * @return mdl_slist_t* 
 */
mdl_slist_t *mdl_slist_next(mdl_slist_t *n);

/**
 * @brief 判断单向链表下一个节点是否存在
 * 
 * @param l 
 * @return int 
 */
mdl_s32_t mdl_slist_isempty(mdl_slist_t *l);
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
#define mdl_slist_entry(node, type, member) \
    mdl_container_of(node, type, member)

/**
 * mdl_slist_for_each - iterate over a single list
 * @pos:    the mdl_slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define mdl_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != MDL_NULL; pos = pos->next)

/**
 * mdl_slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define mdl_slist_for_each_entry(pos, head, member) \
    for (pos = rt_slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (MDL_NULL); \
         pos = rt_slist_entry(pos->member.next, typeof(*pos), member))

/**
 * mdl_slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define mdl_slist_first_entry(ptr, type, member) \
    mdl_slist_entry((ptr)->next, type, member)

/**
 * mdl_slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.df
 *
 * Note, that slist is expected to be not empty.
 */
#define mdl_slist_tail_entry(ptr, type, member) \
    mdl_slist_entry(mdl_slist_tail(ptr), type, member)

 /**
  * @}
  */




void mdl_assert_handler(const char *ex_string, const char *func, mdl_u32_t line);
#define MDL_ASSERT(EX)                                                        \
if (!(EX))                                                                    \
{                                                                             \
    mdl_assert_handler(#EX, __FUNCTION__, __LINE__);                          \
}

#ifdef __cplusplus
}
#endif

#endif /* __MDL_DEF_H__ */




