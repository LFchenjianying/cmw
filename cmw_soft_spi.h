/**
 * @file cmw_spi.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __CMW_SOFT_SPI_H__
#define __CMW_SOFT_SPI_H__

#ifdef M_CMW_SPI_EXTERN_DEF
	#define M_CMW_SPI_EXTERN
#else 
	#define M_CMW_SPI_EXTERN extern
#endif /* M_BSP_SPI_EXTERN_DEF */
#include "cmw_define.h"

#if defined(CMW_USING_SPI)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	E_CMW_SPI_CPOL_LOW,     /* 表示当SCLK=0时处于空闲，有效状态为SCLK处于高电平时 */
	E_CMW_SPI_CPOL_HIGH,    /* 表示当SCLK=1时处于空闲，有效状态为SCLK处于低电平时 */
} cmw_spi_cpol_t;

typedef enum {
	E_CMW_SPI_CPHA_1EDGE, /* 表示数据采样是在第1个边沿，数据发送在第2个边沿 */
	E_CMW_SPI_CPHA_2EDGE, /* 表示数据采样是在第2个边沿，数据发送在第1个边沿 */
} cmw_spi_cpha_t;

typedef enum {
	E_CMW_SPI_WIRE_LOW,  /* 表示IO状态为低电平 */
	E_CMW_SPI_WIRE_HIGH, /* 表示IO状态为高电平 */
} cmw_spi_wire_state_t;

/* Spi操作句柄定义 */
typedef struct cmw_spi_obj{
	struct spi_in_data{
		cmw_spi_cpol_t et_cpol;       /* 极性 */
        cmw_spi_cpha_t et_cpha;       /* 相位 */
        cmw_u32_t u32_sclk_delay;        /* 时钟延时 */
	} in_data;

	struct spi_in_method{
		void (*sclk_op)(cmw_spi_wire_state_t et_sta);  /* sclk IO 操作函数 */
        cmw_spi_wire_state_t (*sdi_op)(void);  /* sdi IO 操作函数 */
        void (*sdo_op)(cmw_spi_wire_state_t et_sta);  /* sdo IO 操作函数 */
        void (*cs_op)(cmw_spi_wire_state_t et_sta);  /* cs IO 操作函数 */
        void (*delay)(cmw_u32_t u32_val);  /* delay 函数 */
        
	} in_method;
	struct spi_out_method{
		void (*write_byte)(struct cmw_spi_obj *pst_obj, cmw_u8_t u8_data);  /* SPI_写8位数据 */
        void (*write_bytes)(struct cmw_spi_obj *pst_obj, cmw_pu8_t pu8_data, cmw_u32_t u32_length); /* 写BUFF数据 */
        cmw_u8_t (*read_byte)(struct cmw_spi_obj *pst_obj, cmw_u8_t u8_data);   /* SPI_读8位数据 */
        void (*read_nbytes)(struct cmw_spi_obj *pst_obj, cmw_pu8_t pu8_tx_data, cmw_pu8_t pu8_rx_data, cmw_u32_t u32_length);/* SPI_读BUFF数据 */
	} out_method;
} cmw_spi_obj_st;


/**
 * @brief spi 对象配置
 * 
 * @param pst_obj 
 * @param et_cpol 
 * @param et_cpha 
 * @param u32_sclk_delay 
 * @param sclk_op 
 * @param sdi_op 
 * @param sdo_op 
 * @param cs_op 
 * @param delay 
 */
void cmw_spi_obj_config(cmw_spi_obj_st *pst_obj, cmw_spi_cpol_t et_cpol, cmw_spi_cpha_t et_cpha, cmw_u32_t u32_sclk_delay, 
    void (*sclk_op)(cmw_spi_wire_state_t et_sta), 
    cmw_spi_wire_state_t (*sdi_op)(void), 
    void (*sdo_op)(cmw_spi_wire_state_t et_sta), 
    void (*cs_op)(cmw_spi_wire_state_t et_sta), 
    void (*delay)(cmw_u32_t u32_val)
    );


#ifdef __cplusplus
}
#endif
#endif /* CMW_USING_SPI */

#endif /* __CMW_SPI_H__ */	

