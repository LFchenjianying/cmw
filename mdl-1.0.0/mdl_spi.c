/**
 * @file mdl_spi.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define  M_MDL_SPI_EXTERN_DEF
#include "mdl_spi.h"


/**
 * @brief SPI_模式0；发送读取8位长度数据
 * 
 * @param pst_obj 
 * @param u8_data 发送读取8位长度数据
 * @return mdl_u8_t 接收到的数据
 */
static mdl_u8_t spi_rw_mode_0(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	/*
	 * CPOL = 0, CPHA = 0; 此时空闲态时SCLK处于低电平，数据采样是在第1个边沿，
	 * 也就是SCLK由低电平到高电平的跳变，所以数据采样是在上升沿，数据在SCK上升沿采样，下降沿变化 
	 */
	mdl_u8_t u8_rx = 0;
	mdl_u8_t u8_cnt = 0;

	for (u8_cnt = 0; u8_cnt < 8; u8_cnt++)						
	{	
		if ((u8_data&0x80)==0x80)
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_HIGH);
		}
		else
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_LOW);
		}
		u8_data <<= 1;	
		u8_rx <<= 1;

        pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);

        pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_HIGH);

		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);

		if (pst_obj->in_method.sdi_op() == E_MDL_SPI_WIRE_HIGH)
		{
			u8_rx |= 0x01;
		}
		
		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_LOW);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
	}	

	return u8_rx;
}

/**
 * @brief SPI_模式1；发送读取8位长度数据
 * 
 * @param pst_obj 
 * @param u8_data 发送读取8位长度数据
 * @return mdl_u8_t 接收到的数据
 */
static mdl_u8_t spi_rw_mode_1(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	/*
	 * CPOL = 0, CPHA = 1; 此时空闲态时SCLK处于低电平，数据发送是在第1个边沿，
	 * 也就是SCLK由低电平到高电平的跳变，所以数据采样是在下降沿
	 */
	mdl_u8_t u8_rx = 0;
	mdl_u8_t u8_cnt = 0;

	for (u8_cnt = 0; u8_cnt < 8; u8_cnt++)						
	{	
		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_HIGH);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
		
		if ((u8_data&0x80)==0x80)
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_HIGH);
		}
		else
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_LOW);
		}
		u8_data <<= 1;	
		u8_rx <<= 1;
		
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);

		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_LOW);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
		
		if (pst_obj->in_method.sdi_op() == E_MDL_SPI_WIRE_HIGH)
		{
			u8_rx |= 0x01;
		}
	}	

	return u8_rx;
}


/**
 * @brief SPI_模式2；发送读取8位长度数据
 * 
 * @param pst_obj 
 * @param u8_data 发送读取8位长度数据
 * @return mdl_u8_t 接收到的数据
 */
static mdl_u8_t spi_rw_mode_2(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	/*
	 * CPOL = 1, CPHA = 0; 此时空闲态时SCLK处于高电平，数据采集是在第1个边沿，
	 * 也就是SCLK由高电平到低电平的跳变，所以数据采样是在下降沿 
	 */
	mdl_u8_t u8_rx = 0;
	mdl_u8_t u8_cnt = 0;

	for (u8_cnt = 0; u8_cnt < 8; u8_cnt++)						
	{			
		if ((u8_data&0x80)==0x80)
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_HIGH);
		}
		else
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_LOW);
		}
		u8_data <<= 1;	
		u8_rx <<= 1;
		
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);

		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_LOW);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
		
		if (pst_obj->in_method.sdi_op() == E_MDL_SPI_WIRE_HIGH)
		{
			u8_rx |= 0x01;
		}
		
		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_HIGH);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
	}	

	return u8_rx;
}



/**
 * @brief SPI_模式3；发送读取8位长度数据
 * 
 * @param pst_obj 
 * @param u8_data 发送读取8位长度数据
 * @return mdl_u8_t 接收到的数据
 */
static mdl_u8_t spi_rw_mode_3(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	/*
	 * CPOL = 1, CPHA = 1; 此时空闲态时SCLK处于高电平，数据发送是在第1个边沿，
	 * 也就是SCLK由高电平到低电平的跳变，所以数据采样是在上升沿 
	 */
	mdl_u8_t u8_rx = 0;
	mdl_u8_t u8_cnt = 0;

	for (u8_cnt = 0; u8_cnt < 8; u8_cnt++)						
	{			

		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_LOW);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
		
		if ((u8_data&0x80)==0x80)
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_HIGH);
		}
		else
		{
            pst_obj->in_method.sdo_op(E_MDL_SPI_WIRE_LOW);
		}
		u8_data <<= 1;	
		u8_rx <<= 1;
		
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);

		pst_obj->in_method.sclk_op(E_MDL_SPI_WIRE_HIGH);
		pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay);
		
		if (pst_obj->in_method.sdi_op() == E_MDL_SPI_WIRE_HIGH)
		{
			u8_rx |= 0x01;
		}
	}	

	return u8_rx;
}



/**
 * @brief SPI_写8位数据
 * 
 * @param pst_obj 
 * @param u8_data 
 */
static void spi_write_byte(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	
	pst_obj->in_method.cs_op(E_MDL_SPI_WIRE_LOW);
	pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay*2);

	if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		spi_rw_mode_0(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		spi_rw_mode_1(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		spi_rw_mode_2(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		spi_rw_mode_3(pst_obj, u8_data);
	}

	pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay*2);
	pst_obj->in_method.cs_op(E_MDL_SPI_WIRE_HIGH);
}

/**
 * @brief SPI_写8位数据 连续调用不重复开关cs 
 * 
 * @param pst_obj 
 * @param u8_data 
 */
static void spi_continue_write_byte(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
	if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		spi_rw_mode_0(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		spi_rw_mode_1(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		spi_rw_mode_2(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		spi_rw_mode_3(pst_obj, u8_data);
	}
}

/**
 * @brief 写入多个字节
 * 
 * @param pu8_data 
 * @param u32_length 
 */
static void spi_write_nbyte(mdl_spi_obj_st *pst_obj, mdl_pu8_t pu8_data, mdl_u32_t u32_length) 
{
	mdl_u32_t u32_cnt = 0;
	
	pst_obj->in_method.cs_op(E_MDL_SPI_WIRE_LOW);
	pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay*2);

	for (u32_cnt = 0; u32_cnt < u32_length; u32_cnt++)
	{
		spi_continue_write_byte(pst_obj, *(pu8_data + u32_cnt));
 	}
	
	pst_obj->in_method.delay(pst_obj->in_data.u32_sclk_delay*2);
	pst_obj->in_method.cs_op(E_MDL_SPI_WIRE_HIGH);
}

/**
 * @brief SPI_读8位数据 
 * 
 * @return mdl_u8_t 
 */
static mdl_u8_t spi_read_byte(mdl_spi_obj_st *pst_obj, mdl_u8_t u8_data)
{
    mdl_u8_t u8_ret_data = 0;

	if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		u8_ret_data = spi_rw_mode_0(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_LOW && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		u8_ret_data = spi_rw_mode_1(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_1EDGE)
	{
		u8_ret_data = spi_rw_mode_2(pst_obj, u8_data);
	}
	else if (pst_obj->in_data.et_cpol == E_MDL_SPI_CPOL_HIGH && pst_obj->in_data.et_cpha == E_MDL_SPI_CPHA_2EDGE)
	{
		u8_ret_data = spi_rw_mode_3(pst_obj, u8_data);
	}
    return u8_ret_data;
}

/**
 * @brief 读取多个byte
 * 
 * @param pu8_data 
 * @param u32_length 
 */
static void spi_read_nbyte(mdl_spi_obj_st *pst_obj, mdl_pu8_t pu8_tx_data, mdl_pu8_t pu8_rx_data, mdl_u32_t u32_length) 
{
	mdl_u32_t u32_cnt = 0;
	
	for (u32_cnt = 0; u32_cnt < u32_length; u32_cnt++)
	{
		pu8_rx_data[u32_cnt] = spi_read_byte(pst_obj, *(pu8_tx_data + u32_cnt));
    }
}



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
void mdl_spi_obj_config(mdl_spi_obj_st *pst_obj, mdl_spi_cpol_t et_cpol, mdl_spi_cpha_t et_cpha, mdl_u32_t u32_sclk_delay,
    void (*sclk_op)(mdl_spi_wire_state_t et_sta), 
    mdl_spi_wire_state_t (*sdi_op)(void), 
    void (*sdo_op)(mdl_spi_wire_state_t et_sta), 
    void (*cs_op)(mdl_spi_wire_state_t et_sta), 
    void (*delay)(mdl_u32_t u32_val)
    )
{
    MDL_ASSERT(pst_obj);
    MDL_ASSERT(sclk_op);
    MDL_ASSERT(sdi_op);
    MDL_ASSERT(sdo_op);
    MDL_ASSERT(cs_op);
    MDL_ASSERT(delay);

    pst_obj->in_data.u32_sclk_delay = u32_sclk_delay;
    pst_obj->in_data.et_cpha = et_cpha;
    pst_obj->in_data.et_cpol = et_cpol;
    pst_obj->in_method.delay = delay;
    pst_obj->in_method.cs_op = cs_op;
    pst_obj->in_method.sclk_op = sclk_op;
    pst_obj->in_method.sdo_op = sdo_op;
    pst_obj->in_method.sdi_op = sdi_op;
	
	pst_obj->out_method.read_byte = spi_read_byte;
	pst_obj->out_method.read_nbytes = spi_read_nbyte;
    pst_obj->out_method.write_byte = spi_write_byte;
    pst_obj->out_method.write_bytes = spi_write_nbyte;

}


/* config */





