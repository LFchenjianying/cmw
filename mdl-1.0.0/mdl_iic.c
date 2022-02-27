/**
 * @file mdl_iic.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#define  M_MDL_IIC_EXTERN_DEF
#include "mdl_iic.h"


#define SET_SDA(p_obj,val)   p_obj->in_method.set_sda(val)
#define SET_SCL(p_obj,val)   p_obj->in_method.set_scl(val)
#define GET_SDA(p_obj)        p_obj->in_method.get_sda()
#define GET_SCL(p_obj)        p_obj->in_method.get_scl()

mdl_inline void i2c_delay ( struct mdl_iic_obj *p_obj )
{
    p_obj->in_method.delay ( ( p_obj->in_data.u32_sclk_delay + 1 ) >> 1 );
}

mdl_inline void i2c_delay2 ( struct mdl_iic_obj *p_obj )
{
    p_obj->in_method.delay ( p_obj->in_data.u32_sclk_delay );
}

#define SDA_L(p_obj)          SET_SDA(p_obj, MDL_IIC_WIRE_LOW)
#define SDA_H(p_obj)          SET_SDA(p_obj, MDL_IIC_WIRE_HIGH)
#define SCL_L(p_obj)          SET_SCL(p_obj, MDL_IIC_WIRE_LOW)
#define SCL_H(p_obj)          SET_SCL(p_obj, MDL_IIC_WIRE_HIGH)

static void i2c_start ( struct mdl_iic_obj *p_obj )
{
    SDA_L ( p_obj );
    i2c_delay ( p_obj );
    SCL_L ( p_obj );
}

static void i2c_restart ( struct mdl_iic_obj *p_obj )
{
    SDA_H ( p_obj );
    SCL_H ( p_obj );
    i2c_delay ( p_obj );
    SDA_L ( p_obj );
    i2c_delay ( p_obj );
    SCL_L ( p_obj );
}

static void i2c_stop ( struct mdl_iic_obj *p_obj )
{
    SDA_L ( p_obj );
    i2c_delay ( p_obj );
    SCL_H ( p_obj );
    i2c_delay ( p_obj );
    SDA_H ( p_obj );
    i2c_delay2 ( p_obj );
}

mdl_inline mdl_bool_t i2c_waitack ( struct mdl_iic_obj *p_obj )
{
    mdl_bool_t ack;
    SDA_H ( p_obj );
    i2c_delay ( p_obj );
    SCL_H ( p_obj );
    ack = !GET_SDA ( p_obj ); /* ACK : SDA pin is pulled low */
    SCL_L ( p_obj );
    return ack;
}

/**
 * @brief 写入 8位 比特数据
 *
 * @param p_obj
 * @param u8_data
 * @return mdl_s32_t
 */
static mdl_s32_t i2c_writeb ( struct mdl_iic_obj *p_obj, mdl_u8_t u8_data )
{
    mdl_s32_t i;
    mdl_u8_t bit;

    for ( i = 7; i >= 0; i-- )
    {
        SCL_L ( p_obj );
        bit = ( u8_data >> i ) & 1;
        SET_SDA ( p_obj, bit );
        i2c_delay ( p_obj );
        SCL_H ( p_obj );
    }

    SCL_L ( p_obj );
    i2c_delay ( p_obj );
    return i2c_waitack ( p_obj );
}


/**
 * @brief 读取 8位 比特数据
 *
 * @param p_obj
 * @return mdl_s32_t
 */
static mdl_s32_t i2c_readb ( struct mdl_iic_obj *p_obj )
{
    mdl_u8_t i;
    mdl_u8_t data = 0;
    SDA_H ( p_obj );
    i2c_delay ( p_obj );

    for ( i = 0; i < 8; i++ )
    {
        data <<= 1;
        SCL_H ( p_obj );

        if ( GET_SDA ( p_obj ) )
        { data |= 1; }

        SCL_L ( p_obj );
        i2c_delay2 ( p_obj );
    }

    return data;
}


/**
 * @brief 发送一则消息中的一个或者多个字节 数据
 *
 * @param p_obj
 * @param msg
 * @return mdl_size_t
 */
static mdl_size_t i2c_send_bytes ( struct mdl_iic_obj *p_obj,
                                   struct mdl_i2c_msg *msg )
{
    mdl_s32_t ret;
    mdl_size_t bytes = 0;
    const mdl_s8_t *ptr = msg->buf;
    mdl_s32_t count = msg->len;
    mdl_s16_t ignore_nack = msg->flags & MDL_I2C_IGNORE_NACK;

    while ( count > 0 )
    {
        ret = i2c_writeb ( p_obj, *ptr );

        if ( ( ret > 0 ) || ( ignore_nack && ( ret == 0 ) ) )
        {
            count --;
            ptr ++;
            bytes ++;
        }
        else if ( ret == 0 )
        {
            return 0;
        }
        else
        {
            return ret;
        }
    }

    return bytes;
}

/**
 * @brief 发送 nack或者ack
 *
 * @param p_obj
 * @param ack
 * @return mdl_err_t
 */
static mdl_err_t i2c_send_ack_or_nack ( struct mdl_iic_obj *p_obj, int ack )
{
    if ( ack )
    { SET_SDA ( p_obj, 0 ); }

    i2c_delay ( p_obj );
    SCL_H ( p_obj );
    SCL_L ( p_obj );
    return MDL_EOK;
}

/**
 * @brief 接收一个或多个字节放置与msg中
 *
 * @param p_obj
 * @param msg
 * @return mdl_s32_t
 */
static mdl_s32_t i2c_recv_bytes ( struct mdl_iic_obj *p_obj,
                                  struct mdl_i2c_msg        *msg )
{
    mdl_s32_t val;
    mdl_s32_t bytes = 0;   /* actual bytes */
    mdl_s8_t *ptr = msg->buf;
    mdl_s32_t count = msg->len;
    const mdl_u32_t flags = msg->flags;

    while ( count > 0 )
    {
        val = i2c_readb ( p_obj );

        if ( val >= 0 )
        {
            *ptr = val;
            bytes ++;
        }
        else
        {
            break;
        }

        ptr ++;
        count --;

        if ( ! ( flags & MDL_I2C_NO_READ_ACK ) )
        {
            val = i2c_send_ack_or_nack ( p_obj, count );

            if ( val < 0 )
            { return val; }
        }
    }

    return bytes;
}



/**
 * @brief 发送地址，应答失败可以重发
 *
 * @param p_obj
 * @param addr
 * @param retries 重发次数
 * @return mdl_s32_t
 */
static mdl_s32_t i2c_send_address ( struct mdl_iic_obj *p_obj,
                                    mdl_s8_t            addr,
                                    mdl_s32_t        retries )
{
    mdl_s32_t i;
    mdl_err_t ret = 0;

    for ( i = 0; i <= retries; i++ )
    {
        ret = i2c_writeb ( p_obj, addr );

        if ( ret == 1 || i == retries )
        { break; }

        i2c_stop ( p_obj );
        i2c_delay2 ( p_obj );
        i2c_start ( p_obj );
    }

    return ret;
}

/**
 * @brief 根据配置的不同位数（8位或10位地址）发送地址数据
 *
 * @param p_obj
 * @param msg
 * @param retries
 * @return mdl_err_t
 */
static mdl_err_t i2c_bit_send_address ( struct mdl_iic_obj *p_obj,
                                        struct mdl_i2c_msg    *msg,
                                        mdl_s32_t retries )
{
    mdl_s16_t flags = msg->flags;
    mdl_s16_t ignore_nack = msg->flags & MDL_I2C_IGNORE_NACK;
    mdl_s8_t addr1, addr2;
    mdl_err_t ret;

    if ( retries <= 0 ) { retries = 1; }

    retries = ignore_nack ? 0 : retries;

    if ( flags & MDL_I2C_ADDR_10BIT )
    {
        addr1 = 0xf0 | ( ( msg->addr >> 7 ) & 0x06 );
        addr2 = msg->addr & 0xff;
        ret = i2c_send_address ( p_obj, addr1, retries );

        if ( ( ret != 1 ) && !ignore_nack )
        {
            return -MDL_EIO;
        }

        ret = i2c_writeb ( p_obj, addr2 );

        if ( ( ret != 1 ) && !ignore_nack )
        {
            return -MDL_EIO;
        }

        if ( flags & MDL_I2C_RD )
        {
            i2c_restart ( p_obj );
            addr1 |= 0x01;
            ret = i2c_send_address ( p_obj, addr1, retries );

            if ( ( ret != 1 ) && !ignore_nack )
            {
                return -MDL_EIO;
            }
        }
    }
    else
    {
        /* 7-bit addr */
        addr1 = msg->addr << 1;

        if ( flags & MDL_I2C_RD )
        { addr1 |= 1; }

        ret = i2c_send_address ( p_obj, addr1, retries );

        if ( ( ret != 1 ) && !ignore_nack )
        { return -MDL_EIO; }
    }

    return MDL_EOK;
}

/**
 * @brief 发送或读取一组消息
 *
 * @param p_obj
 * @param msgs
 * @param num
 * @return mdl_size_t
 */
static mdl_size_t i2c_bit_xfer ( struct mdl_iic_obj *p_obj,
                                 struct mdl_i2c_msg   msgs[],
                                 mdl_u32_t             num )
{
    struct mdl_i2c_msg *msg;
    mdl_s32_t i, ret;
    mdl_u16_t ignore_nack;

    for ( i = 0; i < num; i++ )
    {
        msg = &msgs[i];
        ignore_nack = msg->flags & MDL_I2C_IGNORE_NACK;

        if ( ! ( msg->flags & MDL_I2C_NO_START ) )
        {
            if ( i )
            {
                i2c_restart ( p_obj );
            }
            else
            {
                i2c_start ( p_obj );
            }

            ret = i2c_bit_send_address ( p_obj, msg, 10 );

            if ( ( ret != MDL_EOK ) && !ignore_nack )
            {
                goto out;
            }
        }

        if ( msg->flags & MDL_I2C_RD )
        {
            ret = i2c_recv_bytes ( p_obj, msg );

            if ( ret < msg->len )
            {
                if ( ret >= 0 )
                { ret = -MDL_EIO; }

                goto out;
            }
        }
        else
        {
            ret = i2c_send_bytes ( p_obj, msg );

            if ( ret < msg->len )
            {
                if ( ret >= 0 )
                { ret = -MDL_ERROR; }

                goto out;
            }
        }
    }

    ret = i;
out:

    if ( ! ( msg->flags & MDL_I2C_NO_STOP ) )
    {
        i2c_stop ( p_obj );
    }

    return ret;
}



//  配置iic句柄




// static const struct mdl_i2c_bus_device_ops i2c_bit_bus_ops =
// {
//     i2c_bit_xfer,
//     RT_NULL,
//     RT_NULL
// };

// mdl_err_t rt_i2c_bit_add_bus(struct mdl_iic_obj *p_obj,
//                             const char               *bus_name)
// {
//     bus->ops = &i2c_bit_bus_ops;

//     return mdl_i2c_bus_device_register(bus, bus_name);
// }











