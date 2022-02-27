/**
 * @file mdl_iic.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __MDL_IIC_H__
#define __MDL_IIC_H__

#ifdef M_MDL_IIC_EXTERN_DEF
	#define M_MDL_IIC_EXTERN
#else 
	#define M_MDL_IIC_EXTERN extern
#endif /* M_MDL_IIC_EXTERN_DEF */

#include "mdl_def.h"



#ifdef __cplusplus
extern "C" {
#endif


#define MDL_IIC_WIRE_LOW  0  /* 表示IO状态为低电平 */
#define MDL_IIC_WIRE_HIGH 1  /* 表示IO状态为高电平 */


typedef enum {
	E_MDL_IIC_WIRE_INPUT,  /* 表示IO为输入状态 */
	E_MDL_IIC_WIRE_OUTPUT, /* 表示IO为输出状态 */
} mdl_iic_wire_dir_et;


#define MDL_I2C_WR                0x0000
#define MDL_I2C_RD               (1u << 0)
#define MDL_I2C_ADDR_10BIT       (1u << 2)  /* this is a ten bit chip address */
#define MDL_I2C_NO_START         (1u << 4)
#define MDL_I2C_IGNORE_NACK      (1u << 5)
#define MDL_I2C_NO_READ_ACK      (1u << 6)  /* when I2C reading, we do not ACK */
#define MDL_I2C_NO_STOP          (1u << 7)

struct mdl_i2c_msg
{
    mdl_u16_t addr;
    mdl_u16_t flags;
    mdl_u16_t len;
    mdl_u8_t  *buf;
};

/* iic 操作句柄定义 */
typedef struct mdl_iic_obj{
	struct iic_in_data{
        mdl_u32_t u32_sclk_delay;        /* 时钟延时 */
        mdl_u32_t u32_wait_ack_timeout;       /* 等待ack 超时时间 */
	} in_data;

	struct iic_in_method{
        void (*set_sda)(mdl_bool_t u32_sta);
        void (*set_scl)(mdl_bool_t et_sta);
        mdl_bool_t (*get_sda)(void);
        mdl_bool_t (*get_scl)(void);
        void (*delay)(mdl_u32_t u32_val);  /* delay 函数 */
        
	} in_method;
	struct iic_out_method{
	void (*write_byte)(struct mdl_iic_obj *pst_obj, mdl_u8_t u8_data);  /* IIC_写8位数据 */
        void (*write_bytes)(struct mdl_iic_obj *pst_obj, mdl_pu8_t pu8_data, mdl_u32_t u32_length); /* 写BUFF数据 */
        mdl_u8_t (*read_byte)(struct mdl_iic_obj *pst_obj, mdl_u8_t u8_data);   /* IIC_读8位数据 */
        void (*read_bytes)(struct mdl_iic_obj *pst_obj, mdl_pu8_t pu8_rx_data, mdl_u32_t u32_length);/* 读BUFF数据 */
	} out_method;
} mdl_iic_obj_st;

#ifdef __cplusplus
extern "C" {
#endif

#endif /* __MDL_IIC_H__ */	




