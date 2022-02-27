/**
 * @file cmw_iic.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __CMW_SOFT_IIC_H__
#define __CMW_SOFT_IIC_H__

#ifdef M_CMW_IIC_EXTERN_DEF
	#define M_CMW_IIC_EXTERN
#else 
	#define M_CMW_IIC_EXTERN extern
#endif /* M_CMW_IIC_EXTERN_DEF */

#include "cmw_define.h"



#ifdef __cplusplus
extern "C" {
#endif

#if defined(CMW_USING_SOFT_IIC)

#define CMW_IIC_WIRE_LOW  0  /* 表示IO状态为低电平 */
#define CMW_IIC_WIRE_HIGH 1  /* 表示IO状态为高电平 */


typedef enum {
	E_CMW_IIC_WIRE_INPUT,  /* 表示IO为输入状态 */
	E_CMW_IIC_WIRE_OUTPUT, /* 表示IO为输出状态 */
} cmw_iic_wire_dir_et;


#define CMW_I2C_WR                0x0000
#define CMW_I2C_RD               (1u << 0)
#define CMW_I2C_ADDR_10BIT       (1u << 2)  /* this is a ten bit chip address */
#define CMW_I2C_NO_START         (1u << 4)
#define CMW_I2C_IGNORE_NACK      (1u << 5)
#define CMW_I2C_NO_READ_ACK      (1u << 6)  /* when I2C reading, we do not ACK */
#define CMW_I2C_NO_STOP          (1u << 7)

struct cmw_i2c_msg
{
    cmw_u16_t addr;
    cmw_u16_t flags;
    cmw_u16_t len;
    cmw_u8_t  *buf;
};

/* iic 操作句柄定义 */
typedef struct cmw_iic_obj{
	struct iic_in_data{
        cmw_u32_t u32_sclk_delay;        /* 时钟延时 */
        cmw_u32_t u32_wait_ack_timeout;       /* 等待ack 超时时间 */
	} in_data;

	struct iic_in_method{
        void (*set_sda)(cmw_bool_t u32_sta);
        void (*set_scl)(cmw_bool_t et_sta);
        cmw_bool_t (*get_sda)(void);
        cmw_bool_t (*get_scl)(void);
        void (*delay)(cmw_u32_t u32_val);  /* delay 函数 */
        
	} in_method;
	struct iic_out_method{
	void (*write_byte)(struct cmw_iic_obj *pst_obj, cmw_u8_t u8_data);  /* IIC_写8位数据 */
        void (*write_bytes)(struct cmw_iic_obj *pst_obj, cmw_pu8_t pu8_data, cmw_u32_t u32_length); /* 写BUFF数据 */
        cmw_u8_t (*read_byte)(struct cmw_iic_obj *pst_obj, cmw_u8_t u8_data);   /* IIC_读8位数据 */
        void (*read_bytes)(struct cmw_iic_obj *pst_obj, cmw_pu8_t pu8_rx_data, cmw_u32_t u32_length);/* 读BUFF数据 */
	} out_method;
} cmw_iic_obj_st;

#endif

#ifdef __cplusplus
extern "C" {
#endif

#endif /* __CMW_IIC_H__ */	




