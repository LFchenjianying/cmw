/**
 * @file lat_core.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cmw_lat.h"
#include <math.h>
#include <stdarg.h>
#include <string.h>

#define LAT_POINT_SOLID     (1u)
#define LAT_POINT_EMPTY     (0u)

#define LAT_DRAW_UPPER_RIGHT  0x01
#define LAT_DRAW_UPPER_LEFT   0x02
#define LAT_DRAW_LOWER_LEFT   0x04
#define LAT_DRAW_LOWER_RIGHT  0x08
#define LAT_DRAW_ALL (LAT_DRAW_UPPER_RIGHT|LAT_DRAW_UPPER_LEFT|LAT_DRAW_LOWER_RIGHT|LAT_DRAW_LOWER_LEFT)

/**
 * @brief 画空心扇区
 *
 * @param p_window
 * @param x
 * @param y
 * @param x0
 * @param y0
 * @param option
 */
static void lat_draw_circle_section ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t x0, cmw_u32_t y0, cmw_u8_t option )
{
    /* upper right */
    if ( option & LAT_DRAW_UPPER_RIGHT )
    {
        p_window->in_method.draw_pixel_cb ( x0 + x, y0 - y, LAT_POINT_SOLID );
        p_window->in_method.draw_pixel_cb ( x0 + y, y0 - x, LAT_POINT_SOLID );
    }

    /* upper left */
    if ( option & LAT_DRAW_UPPER_LEFT )
    {
        p_window->in_method.draw_pixel_cb ( x0 - x, y0 - y, LAT_POINT_SOLID );
        p_window->in_method.draw_pixel_cb ( x0 - y, y0 - x, LAT_POINT_SOLID );
    }

    /* lower right */
    if ( option & LAT_DRAW_LOWER_RIGHT )
    {
        p_window->in_method.draw_pixel_cb ( x0 + x, y0 + y, LAT_POINT_SOLID );
        p_window->in_method.draw_pixel_cb ( x0 + y, y0 + x, LAT_POINT_SOLID );
    }

    /* lower left */
    if ( option & LAT_DRAW_LOWER_LEFT )
    {
        p_window->in_method.draw_pixel_cb ( x0 - x, y0 + y, LAT_POINT_SOLID );
        p_window->in_method.draw_pixel_cb ( x0 - y, y0 + x, LAT_POINT_SOLID );
    }
}

/**
 * @brief 画四分之一空心圆
 *
 * @param p_window
 * @param x0
 * @param y0
 * @param rad
 * @param option
 */
static void lat_draw_circle ( lat_window_t *p_window, cmw_u32_t x0, cmw_u32_t y0, cmw_u32_t rad, cmw_u8_t option )
{
    cmw_s32_t f;
    cmw_s32_t ddF_x;
    cmw_s32_t ddF_y;
    cmw_u32_t x;
    cmw_u32_t y;
    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;
    lat_draw_circle_section ( p_window, x, y, x0, y0, option );

    while ( x < y )
    {
        if ( f >= 0 )
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;
        lat_draw_circle_section ( p_window, x, y, x0, y0, option );
    }
}

/**
 * @brief This is the toplevel function for the hv line draw procedures.
 *        This function should be called by the user.
 *        "dir" may have 4 directions: 0 (left to right), 1, 2, 3 (down up)
 * @param p_window
 * @param x
 * @param y
 * @param len
 * @param dir
 * @param is_solid
 */
static void lat_draw_horizon_vertical_line ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t len, cmw_u8_t dir, cmw_u8_t is_solid )
{
    cmw_u16_t u16_cnt = 0;

    if ( len != 0 )
    {
        if ( dir ) /* vertical */
        {
            for ( u16_cnt = 0; u16_cnt < len; u16_cnt++ )
            {
                p_window->in_method.draw_pixel_cb ( x, y + u16_cnt, LAT_POINT_SOLID );
            }
        }
        else
        {
            for ( u16_cnt = 0; u16_cnt < len; u16_cnt++ )
            {
                p_window->in_method.draw_pixel_cb ( x + u16_cnt, y, LAT_POINT_SOLID );
            }
        }
    }
}

/**
 * @brief 画实心水平线
 *
 * @param p_window
 * @param x
 * @param y
 * @param len
 */
static void lat_draw_horizon_line ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t len )
{
    lat_draw_horizon_vertical_line ( p_window, x, y, len, 0, LAT_POINT_SOLID );
}

/**
 * @brief 绘制实体垂直线
 *
 * @param p_window
 * @param x
 * @param y
 * @param len
 */
static void lat_draw_vertical_line ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t len )
{
    lat_draw_horizon_vertical_line ( p_window, x, y, len, 1, LAT_POINT_SOLID );
}

/**
 * @brief 绘制水平空心线
 *
 * @param p_window
 * @param x
 * @param y
 * @param len
 */
static void lat_draw_empty_horizon_line ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t len )
{
    if ( p_window )
    {
        lat_draw_horizon_vertical_line ( p_window, x, y, len, 0, LAT_POINT_SOLID );
    }
}

/**
 * @brief 绘制垂直空心线
 *
 * @param p_window
 * @param x
 * @param y
 * @param len
 */
static void lat_draw_empty_vertical_line ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t len )
{
    if ( p_window )
    {
        lat_draw_horizon_vertical_line ( p_window, x, y, len, 1, LAT_POINT_SOLID );
    }
}

/**
 * @brief 画圆角矩形框
 *
 * @param p_window
 * @param x
 * @param y
 * @param w
 * @param h
 * @param r
 */
static void lat_draw_round_frame ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t w, cmw_u32_t h, cmw_u32_t r )
{
    cmw_u32_t xl, yu;
    xl = x;
    xl += r;
    yu = y;
    yu += r;
    {
        cmw_u32_t yl, xr;
        xr = x;
        xr += w;
        xr -= r;
        xr -= 1;
        yl = y;
        yl += h;
        yl -= r;
        yl -= 1;
        lat_draw_circle ( p_window, xl, yu, r, LAT_DRAW_UPPER_LEFT );
        lat_draw_circle ( p_window, xr, yu, r, LAT_DRAW_UPPER_RIGHT );
        lat_draw_circle ( p_window, xl, yl, r, LAT_DRAW_LOWER_LEFT );
        lat_draw_circle ( p_window, xr, yl, r, LAT_DRAW_LOWER_RIGHT );
    }
    {
        cmw_u32_t ww, hh;
        ww = w;
        ww -= r;
        ww -= r;
        hh = h;
        hh -= r;
        hh -= r;
        xl++;
        yu++;

        if ( ww >= 3 )
        {
            ww -= 2;
            h--;
            lat_draw_horizon_line ( p_window, xl, y, ww );
            lat_draw_horizon_line ( p_window, xl, y + h, ww );
        }

        if ( hh >= 3 )
        {
            hh -= 2;
            w--;
            lat_draw_vertical_line ( p_window, x, yu, hh );
            lat_draw_vertical_line ( p_window, x + w, yu, hh );
        }
    }
}

/**
 * @brief 画实心矩形
 *
 * @param p_window
 * @param x
 * @param y
 * @param w
 * @param h
 */
static void lat_draw_box ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t w, cmw_u32_t h )
{
    while ( h != 0 )
    {
        lat_draw_horizon_line ( p_window, x, y, w );
        y++;
        h--;
    }
}

/**
 * @brief 画矩形框
 *
 * @param p_window
 * @param x
 * @param y
 * @param w
 * @param h
 */
static void lat_draw_frame ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t w, cmw_u32_t h )
{
    cmw_u32_t xtmp = x;
    lat_draw_horizon_line ( p_window, x, y, w );
    lat_draw_vertical_line ( p_window, x, y, h );
    x += w;
    x--;
    lat_draw_vertical_line ( p_window, x, y, h );
    y += h;
    y--;
    lat_draw_horizon_line ( p_window, xtmp, y, w );
}

/**
 * @brief 画实心扇形
 *
 * @param p_window
 * @param x
 * @param y
 * @param x0
 * @param y0
 * @param option
 */
static void lat_draw_disc_section ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t x0, cmw_u32_t y0, cmw_u8_t option )
{
    /* upper right */
    if ( option & LAT_DRAW_UPPER_RIGHT )
    {
        lat_draw_vertical_line ( p_window, x0 + x, y0 - y, y + 1 );
        lat_draw_vertical_line ( p_window, x0 + y, y0 - x, x + 1 );
    }

    /* upper left */
    if ( option & LAT_DRAW_UPPER_LEFT )
    {
        lat_draw_vertical_line ( p_window, x0 - x, y0 - y, y + 1 );
        lat_draw_vertical_line ( p_window, x0 - y, y0 - x, x + 1 );
    }

    /* lower right */
    if ( option & LAT_DRAW_LOWER_RIGHT )
    {
        lat_draw_vertical_line ( p_window, x0 + x, y0, y + 1 );
        lat_draw_vertical_line ( p_window, x0 + y, y0, x + 1 );
    }

    /* lower left */
    if ( option & LAT_DRAW_LOWER_LEFT )
    {
        lat_draw_vertical_line ( p_window, x0 - x, y0, y + 1 );
        lat_draw_vertical_line ( p_window, x0 - y, y0, x + 1 );
    }
}

/**
 * @brief 画实心圆
 *
 * @param p_window
 * @param x0  圆心在X轴位置
 * @param y0  圆心在Y轴位置
 * @param rad 半径
 * @param option
 */
static void lat_draw_disc ( lat_window_t *p_window, cmw_u32_t x0, cmw_u32_t y0, cmw_u32_t rad, cmw_u8_t option )
{
    cmw_s32_t f;
    cmw_s32_t ddF_x;
    cmw_s32_t ddF_y;
    cmw_u32_t x;
    cmw_u32_t y;
    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;
    lat_draw_disc_section ( p_window, x, y, x0, y0, option );

    while ( x < y )
    {
        if ( f >= 0 )
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;
        lat_draw_disc_section ( p_window, x, y, x0, y0, option );
    }
}

/**
 * @brief 画实心圆角矩形
 *
 * @param p_window
 * @param x
 * @param y
 * @param w
 * @param h
 * @param r
 */
static void lat_draw_round_box ( lat_window_t *p_window, cmw_u32_t x, cmw_u32_t y, cmw_u32_t w, cmw_u32_t h, cmw_u32_t r )
{
    cmw_u32_t xl, yu;
    cmw_u32_t yl, xr;
    xl = x;
    xl += r;
    yu = y;
    yu += r;
    xr = x;
    xr += w;
    xr -= r;
    xr -= 1;
    yl = y;
    yl += h;
    yl -= r;
    yl -= 1;
    lat_draw_disc ( p_window, xl, yu, r, LAT_DRAW_UPPER_LEFT );
    lat_draw_disc ( p_window, xr, yu, r, LAT_DRAW_UPPER_RIGHT );
    lat_draw_disc ( p_window, xl, yl, r, LAT_DRAW_LOWER_LEFT );
    lat_draw_disc ( p_window, xr, yl, r, LAT_DRAW_LOWER_RIGHT );
    {
        cmw_u32_t ww, hh;
        ww = w;
        ww -= r;
        ww -= r;
        xl++;
        yu++;

        if ( ww >= 3 )
        {
            ww -= 2;
            lat_draw_box ( p_window, xl, y, ww, r + 1 );
            lat_draw_box ( p_window, xl, yl, ww, r + 1 );
        }

        hh = h;
        hh -= r;
        hh -= r;

        //h--;
        if ( hh >= 3 )
        {
            hh -= 2;
            lat_draw_box ( p_window, x, yu, w, hh );
        }
    }
}

/**
 * @brief 画实心线
 *
 * @param p_window
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param is_solid
 */
static void lat_draw_line ( lat_window_t *p_window, cmw_u32_t x1, cmw_u32_t y1, cmw_u32_t x2, cmw_u32_t y2, cmw_u8_t is_solid )
{
    cmw_u32_t tmp;
    cmw_u32_t x, y;
    cmw_u32_t dx, dy;
    cmw_s32_t err;
    cmw_s32_t ystep;
    cmw_u8_t swapxy = 0;

    /* no intersection check at the moment, should be added... */
    if ( x1 > x2 )
    {
        dx = x1 - x2;
    }
    else
    {
        dx = x2 - x1;
    }

    if ( y1 > y2 )
    {
        dy = y1 - y2;
    }
    else
    {
        dy = y2 - y1;
    }

    if ( dy > dx )
    {
        swapxy = 1;
        tmp = dx;
        dx = dy;
        dy = tmp;
        tmp = x1;
        x1 = y1;
        y1 = tmp;
        tmp = x2;
        x2 = y2;
        y2 = tmp;
    }

    if ( x1 > x2 )
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    err = dx >> 1;

    if ( y2 > y1 )
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    y = y1;

    if ( x2 == 0xffff )
    {
        x2--;
    }

    for ( x = x1; x <= x2; x++ )
    {
        if ( swapxy == 0 )
        {
            p_window->in_method.draw_pixel_cb ( x, y, is_solid );
        }
        else
        {
            p_window->in_method.draw_pixel_cb ( y, x, is_solid );
        }

        err -= ( cmw_u8_t ) dy;

        if ( err < 0 )
        {
            y += ( cmw_u32_t ) ystep;
            err += ( cmw_u32_t ) dx;
        }
    }
}

/**
 * @brief 指定宽度划实心线
 *
 * @param p_window
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param width
 * @param is_solid
 */
static void lat_draw_wline ( lat_window_t *p_window, cmw_u32_t x1, cmw_u32_t y1, cmw_u32_t x2, cmw_u32_t y2, cmw_u16_t width, cmw_u8_t is_solid )
{
    cmw_u32_t tmp;
    cmw_u32_t x, y;
    cmw_u32_t dx, dy;
    cmw_s32_t err;
    cmw_s32_t ystep;
    cmw_u16_t i = 0;
    cmw_u8_t swapxy = 0;

    /* no intersection check at the moment, should be added... */
    if ( x1 > x2 )
    {
        dx = x1 - x2;
    }
    else
    {
        dx = x2 - x1;
    }

    if ( y1 > y2 )
    {
        dy = y1 - y2;
    }
    else
    {
        dy = y2 - y1;
    }

    if ( dy > dx )
    {
        swapxy = 1;
        tmp = dx;
        dx = dy;
        dy = tmp;
        tmp = x1;
        x1 = y1;
        y1 = tmp;
        tmp = x2;
        x2 = y2;
        y2 = tmp;
    }

    if ( x1 > x2 )
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    err = dx >> 1;

    if ( y2 > y1 )
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    y = y1;

    if ( x2 == 0xffff )
    {
        x2--;
    }

    for ( x = x1; x <= x2; x++ )
    {
        if ( swapxy == 0 )
        {
            p_window->in_method.draw_pixel_cb ( x, y, is_solid );

            for ( i = 0; ( i < width ) && ( ( x + i ) <= x2 ); i++ )
            {
                p_window->in_method.draw_pixel_cb ( x + 1 + i, y, is_solid );
            }
        }
        else
        {
            cmw_u16_t y_temp = y;
            p_window->in_method.draw_pixel_cb ( y, x, is_solid );

            for ( i = 0; ( i < width ) && ( ( y + i ) <= ( y_temp + dy ) ); i++ )
            {
                p_window->in_method.draw_pixel_cb ( y + 1 + i, x, is_solid );
            }
        }

        err -= ( cmw_u8_t ) dy;

        if ( err < 0 )
        {
            y += ( cmw_u32_t ) ystep;
            err += ( cmw_u32_t ) dx;
        }
    }
}

/**
 * @brief  将数值转为特定格式字符串并返回字符串长度
 *
 * @param pu8_buf
 * @param value
 * @param integer_digits
 * @param decimal_digits
 * @return cmw_u8_t
 */
static cmw_u8_t lat_number_conver_to_string ( cmw_u8_t *pu8_buf, cmw_s32_t value, cmw_u8_t integer_digits, cmw_u8_t decimal_digits )
{
    cmw_u8_t u8_value_digits = 0;
    cmw_s32_t  s32_temp_value = value;

    if ( pu8_buf == NULL || integer_digits > 9 || decimal_digits > 9 || ( ( decimal_digits + integer_digits ) > 10 ) )
    {
        return 0;
    }

    if ( integer_digits == 0 && decimal_digits == 0 )
    {
        return 0;
    }

    // 取得Value的最大位数,最少一位
    {
        if ( s32_temp_value < 0 )
        {
            s32_temp_value = -s32_temp_value;
        }

        u8_value_digits = 1;
    }
    {
        cmw_u8_t i = 0;
        cmw_s32_t  s32_value = value;
        cmw_u8_t negative_flag = 0;
        cmw_u8_t remainder_digit = 0;

        for ( i = 0; i < ( decimal_digits + integer_digits + 2 ); i++ )
        {
            pu8_buf[i] = '0';
        }

        if ( value < 0 )
        {
            pu8_buf[0] = '-';
            negative_flag = 1;
            s32_value = -s32_value;
        }

        u8_value_digits = decimal_digits + integer_digits;

        for ( i = 0; i < u8_value_digits; i++ )
        {
            remainder_digit = s32_value % 10;
            s32_value /= 10;

            if ( negative_flag )
            {
                if ( decimal_digits == i )
                {
                    if ( decimal_digits )
                    {
                        pu8_buf[1 + integer_digits] = '.';
                    }

                    pu8_buf[1 + integer_digits - 1] = remainder_digit + '0';
                }
                else if ( decimal_digits < i )
                {
                    pu8_buf[decimal_digits + integer_digits - i] = remainder_digit + '0';
                }
                else
                {
                    pu8_buf[1 + decimal_digits + integer_digits - i] = remainder_digit + '0';
                }
            }
            else
            {
                if ( decimal_digits == i )
                {
                    if ( decimal_digits )
                    {
                        pu8_buf[integer_digits] = '.';
                    }

                    pu8_buf[integer_digits - 1] = remainder_digit + '0';
                }
                else if ( decimal_digits < i )
                {
                    pu8_buf[decimal_digits + integer_digits - 1 - i] = remainder_digit + '0';
                }
                else
                {
                    pu8_buf[decimal_digits + integer_digits - i] = remainder_digit + '0';
                }
            }
        }

        if ( negative_flag )
        {
            if ( decimal_digits )
            {
                return ( decimal_digits + integer_digits + 2 );
            }
            else
            {
                return ( decimal_digits + integer_digits + 1 );
            }
        }
        else
        {
            if ( decimal_digits )
            {
                return ( decimal_digits + integer_digits + 1 );
            }
            else
            {
                return ( decimal_digits + integer_digits );
            }
        }
    }
}

/*=================== 实心多边形绘制底层定义 ========================*/
struct pg_point_struct
{
    cmw_s16_t x;
    cmw_s16_t y;
};

typedef struct _pg_struct pg_struct;    /* forward declaration */

struct pg_edge_struct
{
    cmw_s16_t x_direction;    /* 1, if x2 is greater than x1, -1 otherwise */
    cmw_s16_t height;
    cmw_s16_t current_x_offset;
    cmw_s16_t error_offset;

    /* --- line loop --- */
    cmw_s16_t current_y;
    cmw_s16_t max_y;
    cmw_s16_t current_x;
    cmw_s16_t error;

    /* --- outer loop --- */
    uint8_t ( *next_idx_fn ) ( pg_struct *pg, uint8_t i );
    uint8_t curr_idx;
};
/* maximum number of points in the polygon */
/* can be redefined, but highest possible value is 254 */
#define PG_MAX_POINTS 6

/* index numbers for the pge structures below */
#define PG_LEFT 0
#define PG_RIGHT 1

struct _pg_struct
{
    struct pg_point_struct list[PG_MAX_POINTS];
    uint8_t cnt;
    uint8_t is_min_y_not_flat;
    cmw_s16_t total_scan_line_cnt;
    struct pg_edge_struct pge[2]; /* left and right line draw structures */
};

/*===========================================*/
/* line draw algorithm */

static uint8_t pge_Next ( struct pg_edge_struct *pge )
{
    if ( pge->current_y >= pge->max_y )
    {
        return 0;
    }

    pge->current_x += pge->current_x_offset;
    pge->error += pge->error_offset;

    if ( pge->error > 0 )
    {
        pge->current_x += pge->x_direction;
        pge->error -= pge->height;
    }

    pge->current_y++;
    return 1;
}

/* assumes y2 > y1 */
static void pge_Init ( struct pg_edge_struct *pge, cmw_s16_t x1, cmw_s16_t y1, cmw_s16_t x2, cmw_s16_t y2 )
{
    cmw_s16_t dx = x2 - x1;
    cmw_s16_t width;
    pge->height = y2 - y1;
    pge->max_y = y2;
    pge->current_y = y1;
    pge->current_x = x1;

    if ( dx >= 0 )
    {
        pge->x_direction = 1;
        width = dx;
        pge->error = 0;
    }
    else
    {
        pge->x_direction = -1;
        width = -dx;
        pge->error = 1 - pge->height;
    }

    pge->current_x_offset = dx / pge->height;
    pge->error_offset = width % pge->height;
}

/**
 * @brief convex polygon algorithm -- inc
 *
 * @param pg
 * @param i
 * @return uint8_t
 */
static uint8_t pg_inc ( pg_struct *pg, uint8_t i )
{
    i++;

    if ( i >= pg->cnt )
    {
        i = 0;
    }

    return i;
}

/**
 * @brief convex polygon algorithm -- dec
 *
 * @param pg
 * @param i
 * @return uint8_t
 */
static uint8_t pg_dec ( pg_struct *pg, uint8_t i )
{
    i--;

    if ( i >= pg->cnt )
    {
        i = pg->cnt - 1;
    }

    return i;
}
/**
 * @brief 扩展最小y值
 *
 * @param pg
 * @param min_y
 * @param pge_idx
 */
static void pg_expand_min_y ( pg_struct *pg, cmw_s16_t min_y, uint8_t pge_idx )
{
    uint8_t i = pg->pge[pge_idx].curr_idx;

    for ( ;; )
    {
        i = pg->pge[pge_idx].next_idx_fn ( pg, i );

        if ( pg->list[i].y != min_y )
        {
            break;
        }

        pg->pge[pge_idx].curr_idx = i;
    }
}

/**
 * @brief 调整多边形
 *
 * @param pg
 * @return uint8_t
 */
static uint8_t pg_prepare ( pg_struct *pg )
{
    cmw_s16_t max_y;
    cmw_s16_t min_y;
    uint8_t i;
    /* setup the next index procedures */
    pg->pge[PG_RIGHT].next_idx_fn = pg_inc;
    pg->pge[PG_LEFT].next_idx_fn = pg_dec;
    /* search for highest and lowest point */
    max_y = pg->list[0].y;
    min_y = pg->list[0].y;
    pg->pge[PG_LEFT].curr_idx = 0;

    for ( i = 1; i < pg->cnt; i++ )
    {
        if ( max_y < pg->list[i].y )
        {
            max_y = pg->list[i].y;
        }

        if ( min_y > pg->list[i].y )
        {
            pg->pge[PG_LEFT].curr_idx = i;
            min_y = pg->list[i].y;
        }
    }

    /* calculate total number of scan lines */
    pg->total_scan_line_cnt = max_y;
    pg->total_scan_line_cnt -= min_y;

    /* exit if polygon height is zero */
    if ( pg->total_scan_line_cnt == 0 )
    {
        return 0;
    }

    /* if the minimum y side is flat, try to find the lowest and highest x points */
    pg->pge[PG_RIGHT].curr_idx = pg->pge[PG_LEFT].curr_idx;
    pg_expand_min_y ( pg, min_y, PG_RIGHT );
    pg_expand_min_y ( pg, min_y, PG_LEFT );
    /* check if the min side is really flat (depends on the x values) */
    pg->is_min_y_not_flat = 1;

    if ( pg->list[pg->pge[PG_LEFT].curr_idx].x != pg->list[pg->pge[PG_RIGHT].curr_idx].x )
    {
        pg->is_min_y_not_flat = 0;
    }
    else
    {
        pg->total_scan_line_cnt--;

        if ( pg->total_scan_line_cnt == 0 )
        {
            return 0;
        }
    }

    return 1;
}
/**
 * @brief 按多边形要求绘制水平线
 *
 * @param pg
 * @param p_window
 */
static void pg_hline ( pg_struct *pg, lat_window_t *p_window )
{
    cmw_s16_t x1, x2, y;
    x1 = pg->pge[PG_LEFT].current_x;
    x2 = pg->pge[PG_RIGHT].current_x;
    y = pg->pge[PG_RIGHT].current_y;

    if ( y < 0 )
    {
        return;
    }

    //  if ( y >= u8g2_GetDisplayHeight(u8g2) )  // does not work for 256x64 display???
    //    return;
    if ( x1 < x2 )
    {
        if ( x2 < 0 )
        {
            return;
        }

        //    if ( x1 >= u8g2_GetDisplayWidth(u8g2) )
        //      return;
        if ( x1 < 0 )
        {
            x1 = 0;
        }

        //    if ( x2 >= u8g2_GetDisplayWidth(u8g2) )
        //      x2 = u8g2_GetDisplayWidth(u8g2);
        lat_draw_horizon_line ( p_window, x1, y, x2 - x1 );
    }
    else
    {
        if ( x1 < 0 )
        {
            return;
        }

        //    if ( x2 >= u8g2_GetDisplayWidth(u8g2) )
        //      return;
        if ( x2 < 0 )
        {
            x1 = 0;
        }

        //    if ( x1 >= u8g2_GetDisplayWidth(u8g2) )
        //      x1 = u8g2_GetDisplayWidth(u8g2);
        lat_draw_horizon_line ( p_window, x2, y, x1 - x2 );
    }
}

/**
 * @brief 多边形边线初始化
 *
 * @param pg
 * @param pge_index
 */
static void pg_line_init ( pg_struct *const pg, uint8_t pge_index )
{
    struct pg_edge_struct  *pge = pg->pge + pge_index;
    uint8_t idx;
    cmw_s16_t x1;
    cmw_s16_t y1;
    cmw_s16_t x2;
    cmw_s16_t y2;
    idx = pge->curr_idx;
    y1 = pg->list[idx].y;
    x1 = pg->list[idx].x;
    idx = pge->next_idx_fn ( pg, idx );
    y2 = pg->list[idx].y;
    x2 = pg->list[idx].x;
    pge->curr_idx = idx;
    pge_Init ( pge, x1, y1, x2, y2 );
}

/**
 * @brief 执行多边形绘制
 *
 * @param pg
 * @param p_window
 */
static void pg_exec ( pg_struct *pg, lat_window_t *p_window )
{
    cmw_s16_t i = pg->total_scan_line_cnt;
    /* first line is skipped if the min y line is not flat */
    pg_line_init ( pg, PG_LEFT );
    pg_line_init ( pg, PG_RIGHT );

    if ( pg->is_min_y_not_flat != 0 )
    {
        pge_Next ( & ( pg->pge[PG_LEFT] ) );
        pge_Next ( & ( pg->pge[PG_RIGHT] ) );
    }

    do
    {
        pg_hline ( pg, p_window );

        while ( pge_Next ( & ( pg->pge[PG_LEFT] ) ) == 0 )
        {
            pg_line_init ( pg, PG_LEFT );
        }

        while ( pge_Next ( & ( pg->pge[PG_RIGHT] ) ) == 0 )
        {
            pg_line_init ( pg, PG_RIGHT );
        }

        i--;
    } while ( i > 0 );
}

/*===========================================*/
/* API procedures */

/**
 * @brief 多边形(poligon)角点清零
 *
 * @param pg
 */
static void pg_xy_clear ( pg_struct *pg )
{
    pg->cnt = 0;
}

/**
 * @brief 添加多边形点
 *
 * @param pg
 * @param x
 * @param y
 */
static void pg_xy_add ( pg_struct *pg, int16_t x, int16_t y )
{
    if ( pg->cnt < PG_MAX_POINTS )
    {
        pg->list[pg->cnt].x = x;
        pg->list[pg->cnt].y = y;
        pg->cnt++;
    }
}

/**
 * @brief 绘制多边形
 *
 * @param pg
 * @param p_window
 */
static void pg_draw ( pg_struct *pg, lat_window_t *p_window )
{
    if ( pg_prepare ( pg ) == 0 )
    {
        return;
    }

    pg_exec ( pg, p_window );
}

/**
 * @brief 绘制实心三角形
 *
 * @param p_window
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void lat_draw_triangle ( lat_window_t *p_window, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2 )
{
    pg_struct u8g2_pg;
    pg_xy_clear ( &u8g2_pg );
    pg_xy_add ( &u8g2_pg, x0, y0 );
    pg_xy_add ( &u8g2_pg, x1, y1 );
    pg_xy_add ( &u8g2_pg, x2, y2 );
    pg_draw ( &u8g2_pg, p_window );
}



/**
 * @brief button 控件 绘制接口
 *
 * @param p_window
 * @param p_button
 */
static void lat_button_draw_port ( lat_window_t *p_window, lat_button_control_t *p_button )
{
    //   lat_draw_triangle(p_window, 10,10, 20,0,30,10);
    if ( p_button->in_data.is_focus  == TRUE )
    {
        lat_draw_round_box ( p_window, p_button->in_data.x,
                             p_button->in_data.y, p_button->in_data.width,
                             p_button->in_data.height, p_button->in_data.radius );
        p_window->in_method.draw_chinese_cb ( p_button->in_data.x + p_button->in_data.x_text_offset,
                                              p_button->in_data.y + p_button->in_data.y_text_offset,
                                              p_button->in_data.p_text, p_button->in_data.text_size,
                                              p_button->in_data.is_focus );
    }
    else
    {
        p_window->in_method.draw_chinese_cb ( p_button->in_data.x + p_button->in_data.x_text_offset,
                                              p_button->in_data.y + p_button->in_data.y_text_offset,
                                              p_button->in_data.p_text, p_button->in_data.text_size,
                                              p_button->in_data.is_focus );
        lat_draw_round_frame ( p_window, p_button->in_data.x,
                               p_button->in_data.y,
                               p_button->in_data.width,
                               p_button->in_data.height,
                               p_button->in_data.radius );
    }
}

/**
 * @brief picture 控件 绘制接口
 *
 * @param p_window
 * @param p_picture
 */
static void lat_picture_draw_port ( lat_window_t *p_window, lat_picture_control_t *p_picture )
{
    p_window->in_method.draw_picture_cb ( p_picture->in_data.x,
                                          p_picture->in_data.y,
                                          p_picture->in_data.width,
                                          p_picture->in_data.height,
                                          p_picture->in_data.is_focus,
                                          p_picture->in_data.p_picture );
}

/**
 * @brief label 控件 绘制接口
 *
 * @param p_window
 * @param p_label
 */
static void lat_label_draw_port ( lat_window_t *p_window, lat_label_control_t *p_label )
{
    p_window->in_method.draw_chinese_cb ( p_label->in_data.x,
                                          p_label->in_data.y,
                                          p_label->in_data.p_text,
                                          p_label->in_data.size,
                                          p_label->in_data.is_focus );
}

/**
 * @brief number 输入控件 绘制接口
 *
 * @param p_window
 * @param p_number
 */
static void lat_number_draw_port ( lat_window_t *p_window, lat_number_control_t *p_number )
{
    cmw_u8_t u8_value_buf[30];
    cmw_u8_t u8_value_string_length = 0;
    cmw_u8_t u8_cnt = 0;
    u8_value_string_length = lat_number_conver_to_string ( u8_value_buf,
                             p_number->in_data.value,
                             p_number->in_data.integer_digits,
                             p_number->in_data.decimal_digits );

    if ( u8_value_string_length != 0 )
    {
        if ( p_number->in_data.old_value != p_number->in_data.value )
        {
            if ( p_number->in_data.old_value < 0 && p_number->in_data.value >= 0 )
            {
                p_window->in_method.clear_part_display_cb ( p_number->in_data.x, p_number->in_data.y,
                        ( p_number->in_data.size / 2 ) * ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits + 2 ),
                        p_number->in_data.size );
            }

            p_number->in_data.old_value = p_number->in_data.value;
        }

        if ( p_number->in_data.number_focus_position > ( p_number->in_data.integer_digits + p_number->in_data.decimal_digits ) )
        {
            p_number->in_data.number_focus_position = p_number->in_data.integer_digits + p_number->in_data.decimal_digits;
        }

        if ( ( p_number->in_data.is_focus  == TRUE ) && ( p_number->in_data.number_focus_position ) )
        {
            for ( u8_cnt = 0; u8_cnt < u8_value_string_length; u8_cnt++ )
            {
                if ( ( p_number->in_data.number_focus_position ) >= ( p_number->in_data.decimal_digits + 1 ) && p_number->in_data.decimal_digits )
                {
                    if ( u8_cnt == ( u8_value_string_length - 1 - p_number->in_data.number_focus_position ) )
                    {
                        p_window->in_method.draw_char_cb ( p_number->in_data.x + ( u8_cnt * ( p_number->in_data.size / 2 ) ),
                                                           p_number->in_data.y,
                                                           u8_value_buf[u8_cnt],
                                                           p_number->in_data.size,
                                                           p_number->in_data.is_focus );
                    }
                    else
                    {
                        p_window->in_method.draw_char_cb ( p_number->in_data.x + ( u8_cnt * ( p_number->in_data.size / 2 ) ),
                                                           p_number->in_data.y,
                                                           u8_value_buf[u8_cnt],
                                                           p_number->in_data.size,
                                                           !p_number->in_data.is_focus );
                    }
                }
                else
                {
                    if ( u8_cnt == ( u8_value_string_length - p_number->in_data.number_focus_position ) )
                    {
                        p_window->in_method.draw_char_cb ( p_number->in_data.x + ( u8_cnt * ( p_number->in_data.size / 2 ) ),
                                                           p_number->in_data.y,
                                                           u8_value_buf[u8_cnt],
                                                           p_number->in_data.size,
                                                           p_number->in_data.is_focus );
                    }
                    else
                    {
                        p_window->in_method.draw_char_cb ( p_number->in_data.x + ( u8_cnt * ( p_number->in_data.size / 2 ) ),
                                                           p_number->in_data.y,
                                                           u8_value_buf[u8_cnt],
                                                           p_number->in_data.size,
                                                           !p_number->in_data.is_focus );
                    }
                }
            }
        }
        else
        {
            for ( u8_cnt = 0; u8_cnt < u8_value_string_length; u8_cnt++ )
            {
                p_window->in_method.draw_char_cb ( p_number->in_data.x + ( u8_cnt * ( p_number->in_data.size / 2 ) ),
                                                   p_number->in_data.y,
                                                   u8_value_buf[u8_cnt],
                                                   p_number->in_data.size,
                                                   p_number->in_data.is_focus );
            }
        }
    }
}

/**
 * @brief combo 控件 绘制接口
 *
 * @param p_window
 * @param p_combo
 */
static void lat_combo_draw_port ( lat_window_t *p_window, lat_combo_control_t *p_combo )
{
    lat_draw_frame ( p_window, p_combo->in_data.x, p_combo->in_data.y,
                     p_combo->in_data.width, p_combo->in_data.height );

    if ( p_combo->in_data.is_focus == TRUE )
    {
        if ( p_combo->in_data.is_edit == TRUE )
        {
            cmw_u16_t x0, y0, x1, y1, x2, y2 = 0;

            if ( ( p_combo->in_data.width - p_combo->in_data.box_width ) > 0 )
            {
                x0 = p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width;
                y0 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
                x1 = p_combo->in_data.x + p_combo->in_data.width - ( p_combo->in_data.box_width / 2 ) - 1;
                y1 = p_combo->in_data.y + p_combo->in_data.height - 1;
                x2 = p_combo->in_data.x + p_combo->in_data.width - 1;
                y2 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
                lat_draw_frame ( p_window, ( p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width ),
                                 p_combo->in_data.y, p_combo->in_data.box_width, p_combo->in_data.height );
                lat_draw_triangle ( p_window, x0, y0, x1, y1, x2, y2 );
            }

            if ( p_combo->in_data.p_text[p_combo->in_data.current_index] )
            {
                p_window->in_method.draw_chinese_cb ( p_combo->in_data.x + p_combo->in_data.x_text_offset,
                                                      p_combo->in_data.y + p_combo->in_data.y_text_offset,
                                                      p_combo->in_data.p_text[p_combo->in_data.current_index],
                                                      p_combo->in_data.size, p_combo->in_data.is_focus );
            }
        }
        else
        {
            if ( ( p_combo->in_data.width - p_combo->in_data.box_width ) > 0 )
            {
                cmw_u16_t x0, y0, x1, y1, x2, y2 = 0;
                lat_draw_box ( p_window, ( p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width ),
                               p_combo->in_data.y, p_combo->in_data.box_width, p_combo->in_data.height );
                x0 = p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width;
                y0 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
                x1 = p_combo->in_data.x + p_combo->in_data.width - ( p_combo->in_data.box_width / 2 ) - 1;
                y1 = p_combo->in_data.y + p_combo->in_data.height - 1;
                x2 = p_combo->in_data.x + p_combo->in_data.width - 1;
                y2 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
                lat_draw_frame ( p_window, ( p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width ),
                                 p_combo->in_data.y, p_combo->in_data.box_width, p_combo->in_data.height );
                lat_draw_line ( p_window, x0, y0, x1, y1, LAT_POINT_EMPTY );
                lat_draw_line ( p_window, x1, y1, x2, y2, LAT_POINT_EMPTY );
            }

            if ( p_combo->in_data.p_text[p_combo->in_data.current_index] )
            {
                p_window->in_method.draw_chinese_cb ( p_combo->in_data.x + p_combo->in_data.x_text_offset,
                                                      p_combo->in_data.y + p_combo->in_data.y_text_offset,
                                                      p_combo->in_data.p_text[p_combo->in_data.current_index],
                                                      p_combo->in_data.size, !p_combo->in_data.is_focus );
            }
        }
    }
    else
    {
        cmw_u16_t x0, y0, x1, y1, x2, y2 = 0;

        if ( ( p_combo->in_data.width - p_combo->in_data.box_width ) > 0 )
        {
            x0 = p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width;
            y0 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
            x1 = p_combo->in_data.x + p_combo->in_data.width - ( p_combo->in_data.box_width / 2 ) - 1;
            y1 = p_combo->in_data.y + p_combo->in_data.height - 1;
            x2 = p_combo->in_data.x + p_combo->in_data.width - 1;
            y2 = p_combo->in_data.y + p_combo->in_data.height / 2 - 1;
            lat_draw_frame ( p_window, ( p_combo->in_data.x + p_combo->in_data.width - p_combo->in_data.box_width ),
                             p_combo->in_data.y, p_combo->in_data.box_width, p_combo->in_data.height );
            lat_draw_triangle ( p_window, x0, y0, x1, y1, x2, y2 );

            if ( p_combo->in_data.p_text[p_combo->in_data.current_index] )
            {
                p_window->in_method.draw_chinese_cb ( p_combo->in_data.x + p_combo->in_data.x_text_offset,
                                                      p_combo->in_data.y + p_combo->in_data.y_text_offset,
                                                      p_combo->in_data.p_text[p_combo->in_data.current_index],
                                                      p_combo->in_data.size, p_combo->in_data.is_focus );
            }
        }
    }
}

/**
 * @brief checkbox 控件 绘制接口
 *
 * @param p_window
 * @param p_checkbox
 */
static void lat_checkbox_draw_port ( lat_window_t *p_window, lat_checkbox_control_t *p_checkbox )
{
    cmw_u16_t x0, y0, x1, y1, x2, y2 = 0;

    if ( p_checkbox->in_data.is_focus == FALSE && p_checkbox->in_data.is_select == TRUE )
    {
        lat_draw_frame ( p_window, p_checkbox->in_data.x, p_checkbox->in_data.y,
                         p_checkbox->in_data.size, p_checkbox->in_data.size );
        x0 = p_checkbox->in_data.x;
        y0 = p_checkbox->in_data.y + p_checkbox->in_data.size / 2 - 1;
        x1 = p_checkbox->in_data.x + p_checkbox->in_data.size / 2 - 1;
        y1 = p_checkbox->in_data.y + p_checkbox->in_data.size - 1;
        x2 = p_checkbox->in_data.x + p_checkbox->in_data.size - 2;
        y2 = p_checkbox->in_data.y;
        lat_draw_wline ( p_window, x0, y0, x1, y1, 1, LAT_POINT_SOLID );
        lat_draw_wline ( p_window, x1, y1, x2, y2, 1, LAT_POINT_SOLID );
    }
    else if ( p_checkbox->in_data.is_focus == TRUE && p_checkbox->in_data.is_select == FALSE )
    {
        lat_draw_box ( p_window, p_checkbox->in_data.x, p_checkbox->in_data.y,
                       p_checkbox->in_data.size, p_checkbox->in_data.size );
    }
    else if ( p_checkbox->in_data.is_focus == TRUE && p_checkbox->in_data.is_select == TRUE )
    {
        lat_draw_box ( p_window, p_checkbox->in_data.x, p_checkbox->in_data.y,
                       p_checkbox->in_data.size, p_checkbox->in_data.size );
        x0 = p_checkbox->in_data.x;
        y0 = p_checkbox->in_data.y + p_checkbox->in_data.size / 2 - 1;
        x1 = p_checkbox->in_data.x + p_checkbox->in_data.size / 2 - 1;
        y1 = p_checkbox->in_data.y + p_checkbox->in_data.size - 1;
        x2 = p_checkbox->in_data.x + p_checkbox->in_data.size - 2;
        y2 = p_checkbox->in_data.y;
        lat_draw_wline ( p_window, x0, y0, x1, y1, 1, LAT_POINT_EMPTY );
        lat_draw_wline ( p_window, x1, y1, x2, y2, 1, LAT_POINT_EMPTY );
    }
    else
    {
        lat_draw_frame ( p_window, p_checkbox->in_data.x, p_checkbox->in_data.y,
                         p_checkbox->in_data.size, p_checkbox->in_data.size );
    }

    p_window->in_method.draw_chinese_cb ( p_checkbox->in_data.x + ( p_checkbox->in_data.size * 1.5 ),
                                          p_checkbox->in_data.y, p_checkbox->in_data.p_text,
                                          p_checkbox->in_data.size, p_checkbox->in_data.is_focus );
}

/**
 * @brief progressbar 控件 绘制接口
 *
 * @param p_window
 * @param p_progressbar
 */
static void lat_progressbar_draw_port ( lat_window_t *p_window, lat_progressbar_control_t *p_progressbar )
{
    if ( p_progressbar->in_data.value > 100 )
    {
        p_progressbar->in_data.value = 100;
    }

    if ( p_progressbar->in_data.height > 2 && ( p_progressbar->in_data.width > 2 ) )
    {
        lat_draw_frame ( p_window, p_progressbar->in_data.x,
                         p_progressbar->in_data.y,
                         p_progressbar->in_data.width,
                         p_progressbar->in_data.height );
        lat_draw_box ( p_window, p_progressbar->in_data.x + 1,
                       p_progressbar->in_data.y + 1,
                       ( ( ( p_progressbar->in_data.width - 2 ) * p_progressbar->in_data.value ) / 100 ),
                       p_progressbar->in_data.height - 2 );
        p_window->in_method.clear_part_display_cb ( p_progressbar->in_data.x + 1 + ( ( ( p_progressbar->in_data.width - 2 ) * p_progressbar->in_data.value ) / 100 ),
                p_progressbar->in_data.y + 1,
                p_progressbar->in_data.width - 3 - ( ( ( p_progressbar->in_data.width - 2 ) * p_progressbar->in_data.value ) / 100 ),
                p_progressbar->in_data.height - 3 );
    }

    if ( p_progressbar->in_data.is_focus == TRUE )
    {
        cmw_u16_t u16_cnt = 0;
        cmw_u16_t u16_len = 0;
        u16_len = p_progressbar->in_data.height + 1;

        for ( u16_cnt = 0; u16_cnt < u16_len; u16_cnt++ )
        {
            if ( u16_cnt % 2 )
            {
                p_window->in_method.draw_pixel_cb ( p_progressbar->in_data.x + p_progressbar->in_data.width,
                                                    p_progressbar->in_data.y + u16_cnt, LAT_POINT_EMPTY );
            }
            else
            {
                p_window->in_method.draw_pixel_cb ( p_progressbar->in_data.x + p_progressbar->in_data.width,
                                                    p_progressbar->in_data.y + u16_cnt, LAT_POINT_SOLID );
            }
        }

        u16_len = p_progressbar->in_data.width + 1;

        for ( u16_cnt = 0; u16_cnt < u16_len; u16_cnt++ )
        {
            if ( u16_cnt % 2 )
            {
                p_window->in_method.draw_pixel_cb ( p_progressbar->in_data.x + u16_cnt,
                                                    p_progressbar->in_data.y  + p_progressbar->in_data.height, LAT_POINT_EMPTY );
            }
            else
            {
                p_window->in_method.draw_pixel_cb ( p_progressbar->in_data.x + u16_cnt,
                                                    p_progressbar->in_data.y  + p_progressbar->in_data.height, LAT_POINT_SOLID );
            }
        }

        // 选中后实线框
        // lat_draw_vertical_line(p_window, p_progressbar->in_data.x + p_progressbar->in_data.width,
        //                                           p_progressbar->in_data.y,
        //                                           p_progressbar->in_data.height + 1);
        // lat_draw_horizon_line(p_window, p_progressbar->in_data.x,
        //                                           p_progressbar->in_data.y  + p_progressbar->in_data.height,
        //                                           p_progressbar->in_data.width + 1);
    }
}

/**
 * @brief listbox 控件 绘制接口
 *
 * @param p_window
 * @param p_listbox
 */
static void lat_listbox_draw_port ( lat_window_t *p_window, lat_listbox_control_t *p_listbox )
{
    cmw_u16_t i = 0;

    for ( i = 0; i < p_listbox->in_data.max_show_line; i++ )
    {
        if ( p_listbox->in_data.current_index == ( p_listbox->in_data.first_line_index + i ) && ( p_listbox->in_data.is_focus == TRUE ) )
        {
            lat_draw_disc ( p_window, p_listbox->in_data.x + p_listbox->in_data.size / 2,
                            p_listbox->in_data.y + ( i * p_listbox->in_data.size ) + p_listbox->in_data.size / 2 + 1,
                            p_listbox->in_data.size / 4,
                            LAT_DRAW_ALL );
            p_window->in_method.draw_chinese_cb ( p_listbox->in_data.x + p_listbox->in_data.size,
                                                  p_listbox->in_data.y + ( i * p_listbox->in_data.size )  + 1,
                                                  p_listbox->in_data.p_text_list[p_listbox->in_data.first_line_index + i],
                                                  p_listbox->in_data.size, 1 );
        }
        else
        {
            lat_draw_circle ( p_window, p_listbox->in_data.x + p_listbox->in_data.size / 2,
                              p_listbox->in_data.y + ( i * p_listbox->in_data.size ) + p_listbox->in_data.size / 2 + 1,
                              p_listbox->in_data.size / 4,
                              LAT_DRAW_ALL );
            p_window->in_method.draw_chinese_cb ( p_listbox->in_data.x + p_listbox->in_data.size,
                                                  p_listbox->in_data.y + ( i * p_listbox->in_data.size ) + 1,
                                                  p_listbox->in_data.p_text_list[p_listbox->in_data.first_line_index + i],
                                                  p_listbox->in_data.size, 0 );
        }

        if ( strlen ( p_listbox->in_data.p_text_list[p_listbox->in_data.first_line_index + i] ) == 0 )
        {
            p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x + p_listbox->in_data.size,
                    p_listbox->in_data.y + ( i * p_listbox->in_data.size ) + 1,
                    p_listbox->in_data.width - 2 * p_listbox->in_data.size,
                    p_listbox->in_data.size );
        }
    }

    if ( p_listbox->in_data.is_focus == TRUE )
    {
        lat_draw_frame ( p_window, p_listbox->in_data.x, p_listbox->in_data.y, p_listbox->in_data.width, p_listbox->in_data.height );
    }
}

/**
 * @brief 根据id 查找页面下的 button 控件句柄
 *
 * @param page
 * @param id
 * @return lat_button_control_t*
 */
static lat_button_control_t *lat_search_button ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_button_control_t *p_button = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.button_head )
    {
        p_button = cmw_container_of ( node, lat_button_control_t, list );

        if ( p_button->in_data.id == id )
        {
            return p_button;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.button_head )
            {
                p_button = cmw_container_of ( node3, lat_button_control_t, list );

                if ( p_button->in_data.id == id )
                {
                    return p_button;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的可以显示的 button 控件句柄
 *
 * @param page
 * @param id
 * @return lat_button_control_t*
 */
static lat_button_control_t *lat_search_button_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_button_control_t *p_button = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.button_head )
    {
        p_button = cmw_container_of ( node, lat_button_control_t, list );

        if ( p_button->in_data.id == id )
        {
            return p_button;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.button_head )
                {
                    p_button = cmw_container_of ( node3, lat_button_control_t, list );

                    if ( p_button->in_data.id == id )
                    {
                        return p_button;
                    }
                }
            }
        }
    }
    return NULL;
}


/**
 * @brief 根据id 查找页面下的 label 控件句柄
 *
 * @param page
 * @param id
 * @return lat_label_control_t*
 */
static lat_label_control_t *lat_search_label ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_label_control_t *p_lable = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.label_head )
    {
        p_lable = cmw_container_of ( node, lat_label_control_t, list );

        if ( p_lable->in_data.id == id )
        {
            return p_lable;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.label_head )
            {
                p_lable = cmw_container_of ( node3, lat_label_control_t, list );

                if ( p_lable->in_data.id == id )
                {
                    return p_lable;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下可以显示的 label 控件句柄
 *
 * @param page
 * @param id
 * @return lat_label_control_t*
 */
static lat_label_control_t *lat_search_label_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_label_control_t *p_lable = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.label_head )
    {
        p_lable = cmw_container_of ( node, lat_label_control_t, list );

        if ( p_lable->in_data.id == id )
        {
            return p_lable;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.label_head )
                {
                    p_lable = cmw_container_of ( node3, lat_label_control_t, list );

                    if ( p_lable->in_data.id == id )
                    {
                        return p_lable;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 picture 控件句柄
 *
 * @param page
 * @param id
 * @return lat_picture_control_t*
 */
static lat_picture_control_t *lat_search_picture ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_picture_control_t *p_picture = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.picture_head )
    {
        p_picture = cmw_container_of ( node, lat_picture_control_t, list );

        if ( p_picture->in_data.id == id )
        {
            return p_picture;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.picture_head )
            {
                p_picture = cmw_container_of ( node3, lat_picture_control_t, list );

                if ( p_picture->in_data.id == id )
                {
                    return p_picture;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 picture 控件句柄
 *
 * @param page
 * @param id
 * @return lat_picture_control_t*
 */
static lat_picture_control_t *lat_search_picture_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_picture_control_t *p_picture = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.picture_head )
    {
        p_picture = cmw_container_of ( node, lat_picture_control_t, list );

        if ( p_picture->in_data.id == id )
        {
            return p_picture;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.picture_head )
                {
                    p_picture = cmw_container_of ( node3, lat_picture_control_t, list );

                    if ( p_picture->in_data.id == id )
                    {
                        return p_picture;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 number 控件句柄
 *
 * @param page
 * @param id
 * @return lat_number_control_t*
 */
static lat_number_control_t *lat_search_number ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_number_control_t *p_number = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.number_head )
    {
        p_number = cmw_container_of ( node, lat_number_control_t, list );

        if ( p_number->in_data.id == id )
        {
            return p_number;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.number_head )
            {
                p_number = cmw_container_of ( node3, lat_number_control_t, list );

                if ( p_number->in_data.id == id )
                {
                    return p_number;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 number 控件句柄
 *
 * @param page
 * @param id
 * @return lat_number_control_t*
 */
static lat_number_control_t *lat_search_number_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_number_control_t *p_number = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.number_head )
    {
        p_number = cmw_container_of ( node, lat_number_control_t, list );

        if ( p_number->in_data.id == id )
        {
            return p_number;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.number_head )
                {
                    p_number = cmw_container_of ( node3, lat_number_control_t, list );

                    if ( p_number->in_data.id == id )
                    {
                        return p_number;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 combo 控件句柄
 *
 * @param page
 * @param id
 * @return lat_combo_control_t*
 */
static lat_combo_control_t *lat_search_combo ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_combo_control_t *p_combo = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.combo_head )
    {
        p_combo = cmw_container_of ( node, lat_combo_control_t, list );

        if ( p_combo->in_data.id == id )
        {
            return p_combo;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.combo_head )
            {
                p_combo = cmw_container_of ( node3, lat_combo_control_t, list );

                if ( p_combo->in_data.id == id )
                {
                    return p_combo;
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 combo 控件句柄
 *
 * @param page
 * @param id
 * @return lat_combo_control_t*
 */
static lat_combo_control_t *lat_search_combo_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_combo_control_t *p_combo = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.combo_head )
    {
        p_combo = cmw_container_of ( node, lat_combo_control_t, list );

        if ( p_combo->in_data.id == id )
        {
            return p_combo;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.combo_head )
                {
                    p_combo = cmw_container_of ( node3, lat_combo_control_t, list );

                    if ( p_combo->in_data.id == id )
                    {
                        return p_combo;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 checkbox 控件句柄
 *
 * @param page
 * @param id
 * @return lat_checkbox_control_t*
 */
static lat_checkbox_control_t *lat_search_checkbox ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.checkbox_head )
    {
        p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

        if ( p_checkbox->in_data.id == id )
        {
            return p_checkbox;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.checkbox_head )
            {
                p_checkbox = cmw_container_of ( node3, lat_checkbox_control_t, list );

                if ( p_checkbox->in_data.id == id )
                {
                    return p_checkbox;
                }
            }
        }
    }
    return NULL;
}


/**
 * @brief 根据id 查找页面下的 checkbox 控件句柄
 *
 * @param page
 * @param id
 * @return lat_checkbox_control_t*
 */
static lat_checkbox_control_t *lat_search_checkbox_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.checkbox_head )
    {
        p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

        if ( p_checkbox->in_data.id == id )
        {
            return p_checkbox;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.checkbox_head )
                {
                    p_checkbox = cmw_container_of ( node3, lat_checkbox_control_t, list );

                    if ( p_checkbox->in_data.id == id )
                    {
                        return p_checkbox;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据id 查找页面下的 progressbar 控件句柄
 *
 * @param page
 * @param id
 * @return lat_progressbar_control_t*
 */
static lat_progressbar_control_t *lat_search_progressbar ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.progressbar_head )
    {
        p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

        if ( p_progressbar->in_data.id == id )
        {
            return p_progressbar;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.progressbar_head )
            {
                p_progressbar = cmw_container_of ( node3, lat_progressbar_control_t, list );

                if ( p_progressbar->in_data.id == id )
                {
                    return p_progressbar;
                }
            }
        }
    }
    return NULL;
}


/**
 * @brief 根据id 查找页面下的 progressbar 控件句柄
 *
 * @param page
 * @param id
 * @return lat_progressbar_control_t*
 */
static lat_progressbar_control_t *lat_search_progressbar_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.progressbar_head )
    {
        p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

        if ( p_progressbar->in_data.id == id )
        {
            return p_progressbar;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.progressbar_head )
                {
                    p_progressbar = cmw_container_of ( node3, lat_progressbar_control_t, list );

                    if ( p_progressbar->in_data.id == id )
                    {
                        return p_progressbar;
                    }
                }
            }
        }
    }
    return NULL;
}


/**
 * @brief 根据id 查找页面下的 listbox 控件句柄
 *
 * @param page
 * @param id
 * @return lat_listbox_control_t*
 */
static lat_listbox_control_t *lat_search_listbox ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_listbox_control_t *p_listbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.listbox_head )
    {
        p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

        if ( p_listbox->in_data.id == id )
        {
            return p_listbox;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.listbox_head )
            {
                p_listbox = cmw_container_of ( node3, lat_listbox_control_t, list );

                if ( p_listbox->in_data.id == id )
                {
                    return p_listbox;
                }
            }
        }
    }
    return NULL;
}


/**
 * @brief 根据id 查找页面下的 listbox 控件句柄
 *
 * @param page
 * @param id
 * @return lat_listbox_control_t*
 */
static lat_listbox_control_t *lat_search_listbox_show ( lat_page_t *page, uint16_t id )
{
    cmw_slist_t *node = NULL;
    lat_listbox_control_t *p_listbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( page == NULL || id == 0 )
    {
        return NULL;
    }

    cmw_slist_for_each ( node, &page->in_data.listbox_head )
    {
        p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

        if ( p_listbox->in_data.id == id )
        {
            return p_listbox;
        }
    }
    // 从选项卡中查找指定ID的控件
    cmw_slist_for_each ( node, &page->in_data.tabcontrol_head )
    {
        p_tabcontrol = cmw_container_of ( node, lat_tabcontrol_t, list );
        cmw_slist_for_each ( node2, &p_tabcontrol->in_data.page_head )
        {
            p_tabcontrol_page = cmw_container_of ( node2, lat_tabcontrol_page_t, list );

            if ( p_tabcontrol_page->in_data.is_show == TRUE )
            {
                cmw_slist_for_each ( node3, &p_tabcontrol_page->in_data.listbox_head )
                {
                    p_listbox = cmw_container_of ( node3, lat_listbox_control_t, list );

                    if ( p_listbox->in_data.id == id )
                    {
                        return p_listbox;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief 绘制 button
 *
 * @param p_window
 */
static void lat_draw_button ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_button_control_t *p_button = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.button_head )
        {
            p_button = cmw_container_of ( node, lat_button_control_t, list );

            if ( p_button && p_button->in_data.pp_deposit_text )
            {
                p_button->in_data.p_text = *p_button->in_data.pp_deposit_text;
            }

            if ( p_button->in_data.is_show == TRUE )
            {
                // 调用底层绘制函数
                lat_button_draw_port ( p_window, p_button );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.button_head )
                    {
                        p_button = cmw_container_of ( node, lat_button_control_t, list );

                        if ( p_button && p_button->in_data.pp_deposit_text )
                        {
                            p_button->in_data.p_text = *p_button->in_data.pp_deposit_text;
                        }

                        if ( p_button->in_data.is_show == TRUE )
                        {
                            // 调用底层绘制函数
                            lat_button_draw_port ( p_window, p_button );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 label
 *
 * @param p_window
 */
static void lat_draw_label ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_label_control_t *p_label = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.label_head )
        {
            p_label = cmw_container_of ( node, lat_label_control_t, list );

            if ( p_label && p_label->in_data.pp_deposit_text )
            {
                p_label->in_data.p_text = *p_label->in_data.pp_deposit_text;
            }

            if ( p_label->in_data.is_show == TRUE )
            {
                lat_label_draw_port ( p_window, p_label );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.label_head )
                    {
                        p_label = cmw_container_of ( node, lat_label_control_t, list );

                        if ( p_label && p_label->in_data.pp_deposit_text )
                        {
                            p_label->in_data.p_text = *p_label->in_data.pp_deposit_text;
                        }

                        if ( p_label->in_data.is_show == TRUE )
                        {
                            lat_label_draw_port ( p_window, p_label );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 picture
 *
 * @param p_window
 */
static void lat_draw_picture ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_picture_control_t *p_picture = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.picture_head )
        {
            p_picture = cmw_container_of ( node, lat_picture_control_t, list );

            if ( p_picture && p_picture->in_data.pp_deposit_picture )
            {
                p_picture->in_data.p_picture = *p_picture->in_data.pp_deposit_picture;
            }

            if ( p_picture->in_data.is_show == TRUE )
            {
                lat_picture_draw_port ( p_window, p_picture );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.picture_head )
                    {
                        p_picture = cmw_container_of ( node, lat_picture_control_t, list );

                        if ( p_picture && p_picture->in_data.pp_deposit_picture )
                        {
                            p_picture->in_data.p_picture = *p_picture->in_data.pp_deposit_picture;
                        }

                        if ( p_picture->in_data.is_show == TRUE )
                        {
                            lat_picture_draw_port ( p_window, p_picture );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 number
 *
 * @param p_window
 */
static void lat_draw_number ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_number_control_t *p_number = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.number_head )
        {
            p_number = cmw_container_of ( node, lat_number_control_t, list );

            // 更新托管数据
            if ( p_number &&
                    ( p_number->in_data.number_focus_position == 0 ) &&
                    p_number->in_data.p_deposit_value )
            {
                p_number->in_data.value = *p_number->in_data.p_deposit_value;
            }

            if ( p_number->in_data.is_show == TRUE )
            {
                lat_number_draw_port ( p_window, p_number );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.number_head )
                    {
                        p_number = cmw_container_of ( node, lat_number_control_t, list );

                        // 更新托管数据
                        if ( p_number &&
                                ( p_number->in_data.number_focus_position == 0 ) &&
                                p_number->in_data.p_deposit_value )
                        {
                            p_number->in_data.value = *p_number->in_data.p_deposit_value;
                        }

                        if ( p_number->in_data.is_show == TRUE )
                        {
                            lat_number_draw_port ( p_window, p_number );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 combo
 *
 * @param p_window
 */
static void lat_draw_combo ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_combo_control_t *p_combo = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.combo_head )
        {
            p_combo = cmw_container_of ( node, lat_combo_control_t, list );

            if ( p_combo->in_data.p_deposit_current_index )
            {
                p_combo->in_data.current_index = *p_combo->in_data.p_deposit_current_index;
            }

            if ( p_combo->in_data.is_show == TRUE )
            {
                lat_combo_draw_port ( p_window, p_combo );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.combo_head )
                    {
                        p_combo = cmw_container_of ( node, lat_combo_control_t, list );

                        if ( p_combo->in_data.p_deposit_current_index )
                        {
                            p_combo->in_data.current_index = *p_combo->in_data.p_deposit_current_index;
                        }

                        if ( p_combo->in_data.is_show == TRUE )
                        {
                            lat_combo_draw_port ( p_window, p_combo );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 checkbox
 *
 * @param p_window
 */
static void lat_draw_checkbox ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.checkbox_head )
        {
            p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

            if ( p_checkbox->in_data.p_deposit_is_select )
            {
                p_checkbox->in_data.is_select = *p_checkbox->in_data.p_deposit_is_select;
            }

            if ( p_checkbox->in_data.is_show == TRUE )
            {
                lat_checkbox_draw_port ( p_window, p_checkbox );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.checkbox_head )
                    {
                        p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

                        if ( p_checkbox->in_data.p_deposit_is_select )
                        {
                            p_checkbox->in_data.is_select = *p_checkbox->in_data.p_deposit_is_select;
                        }

                        if ( p_checkbox->in_data.is_show == TRUE )
                        {
                            lat_checkbox_draw_port ( p_window, p_checkbox );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 progressbar
 *
 * @param p_window
 */
static void lat_draw_progressbar ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.progressbar_head )
        {
            p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

            if ( p_progressbar->in_data.p_deposit_value )
            {
                p_progressbar->in_data.value = *p_progressbar->in_data.p_deposit_value;
            }

            if ( p_progressbar->in_data.is_show == TRUE )
            {
                lat_progressbar_draw_port ( p_window, p_progressbar );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.progressbar_head )
                    {
                        p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

                        if ( p_progressbar->in_data.p_deposit_value )
                        {
                            p_progressbar->in_data.value = *p_progressbar->in_data.p_deposit_value;
                        }

                        if ( p_progressbar->in_data.is_show == TRUE )
                        {
                            lat_progressbar_draw_port ( p_window, p_progressbar );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 绘制 listbox
 *
 * @param p_window
 */
static void lat_draw_listbox ( lat_window_t *p_window )
{
    cmw_slist_t *node = NULL;
    lat_listbox_control_t *p_listbox = NULL;
    cmw_slist_t *node2 = NULL;
    cmw_slist_t *node3 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;
    lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

    if ( p_window != NULL && p_window->in_data.p_focus_page != NULL )
    {
        cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.listbox_head )
        {
            p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

            if ( p_listbox->in_data.p_deposit_current_index )
            {
                p_listbox->in_data.current_index = *p_listbox->in_data.p_deposit_current_index;
            }

            if ( p_listbox->in_data.is_show == TRUE )
            {
                lat_listbox_draw_port ( p_window, p_listbox );
            }
        }
        cmw_slist_for_each ( node2, &p_window->in_data.p_focus_page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_window->in_data.p_focus_page->in_data.listbox_head )
                    {
                        p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

                        if ( p_listbox->in_data.p_deposit_current_index )
                        {
                            p_listbox->in_data.current_index = *p_listbox->in_data.p_deposit_current_index;
                        }

                        if ( p_listbox->in_data.is_show == TRUE )
                        {
                            lat_listbox_draw_port ( p_window, p_listbox );
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief button 控件聚焦状态下托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_button
 * @param key_value
 */
static void lat_button_focus_deposit_process ( lat_window_t *p_window,
        struct lat_button_control *p_button,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_button->in_data.key_event.up )
            {
                p_button->in_data.key_event.up();
            }

            if ( p_button->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
                
            }
            else
            {
                if ( p_button->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_button->in_data.key_event.down )
            {
                p_button->in_data.key_event.down();
            }

            if ( p_button->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_button->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_button->in_data.key_event.left )
            {
                p_button->in_data.key_event.left();
            }

            if ( p_button->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_button->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_button->in_data.key_event.right )
            {
                p_button->in_data.key_event.right();
            }

            if ( p_button->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_button->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_button->in_data.key_event.enter )
            {
                p_button->in_data.key_event.enter();
            }

            if ( p_button->in_data.page_shift.enter )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.enter );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_button->in_data.focus_shift.enter )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.enter );
                }
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_button->in_data.key_event.esc )
            {
                p_button->in_data.key_event.esc();
            }

            if ( p_button->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_button->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_button->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_button->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief number 控件聚焦状态下托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_number
 * @param key_value
 */
static void lat_number_focus_key_deposit_process ( lat_window_t *p_window,
        struct lat_number_control *p_number,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_number->in_data.key_event.up )
            {
                p_number->in_data.key_event.up();
            }

            if ( p_number->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_number->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_number->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_number->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_number->in_data.key_event.down )
            {
                p_number->in_data.key_event.down();
            }

            if ( p_number->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_number->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_number->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_number->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_number->in_data.key_event.left )
            {
                p_number->in_data.key_event.left();
            }

            if ( p_number->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_number->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_number->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_number->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_number->in_data.key_event.right )
            {
                p_number->in_data.key_event.right();
            }

            if ( p_number->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_number->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_number->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_number->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            p_number->in_data.number_focus_position = 1;
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_number->in_data.key_event.esc )
            {
                p_number->in_data.key_event.esc();
            }

            if ( p_number->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_number->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_number->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_number->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief number 控件编辑状态下托管处理(数值编辑处理)
 *
 * @param p_window
 * @param p_number
 * @param key_value
 */
static void lat_number_edit_key_deposit_process ( lat_window_t *p_window,
        struct lat_number_control *p_number,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            p_number->out_method.value_add ( p_number );
        }
        break;

        case LAT_KEY_DOWN:
        {
            p_number->out_method.value_sub ( p_number );
        }
        break;

        case LAT_KEY_LEFT:
        {
            p_number->out_method.number_focus_left_shift ( p_number );
            if ( p_number->in_data.number_focus_position > ( p_number->in_data.integer_digits + p_number->in_data.decimal_digits ) )
            {
                p_number->in_data.number_focus_position = ( p_number->in_data.integer_digits + p_number->in_data.decimal_digits );
                if (p_number->in_data.focus_shift.left)
                {
                    p_window->out_method.control_focus_shift(p_window, p_number->in_data.focus_shift.left);
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            p_number->out_method.number_focus_right_shift ( p_number );
            if (p_number->in_data.number_focus_position == 0)
            {
                if (p_number->in_data.focus_shift.right)
                {
                    p_window->out_method.control_focus_shift(p_window, p_number->in_data.focus_shift.right);
                }
                else
                {
                    p_number->in_data.number_focus_position = 1;
                }
            }

        }
        break;

        case LAT_KEY_ENTER:
        {
            p_number->out_method.number_focus_position_set ( p_number, 0 );

            if ( p_number->in_data.p_deposit_value )
            {
                *p_number->in_data.p_deposit_value = p_number->in_data.value;
            }

            if ( p_number->in_data.key_event.enter )
            {
                p_number->in_data.key_event.enter();
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            p_number->out_method.number_focus_position_set ( p_number, 0 );

            if ( p_number->in_data.p_deposit_value )
            {
                p_number->in_data.value = *p_number->in_data.p_deposit_value;
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief picture 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_picture
 * @param key_value
 */
static void lat_picture_key_deposit_process ( lat_window_t *p_window,
        struct lat_picture_control *p_picture,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_picture->in_data.key_event.up )
            {
                p_picture->in_data.key_event.up();
            }

            if ( p_picture->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_picture->in_data.key_event.down )
            {
                p_picture->in_data.key_event.down();
            }

            if ( p_picture->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_picture->in_data.key_event.left )
            {
                p_picture->in_data.key_event.left();
            }

            if ( p_picture->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_picture->in_data.key_event.right )
            {
                p_picture->in_data.key_event.right();
            }

            if ( p_picture->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_picture->in_data.key_event.enter )
            {
                p_picture->in_data.key_event.enter();
            }

            if ( p_picture->in_data.page_shift.enter )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.enter );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.enter )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.enter );
                }
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_picture->in_data.key_event.esc )
            {
                p_picture->in_data.key_event.esc();
            }

            if ( p_picture->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_picture->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_picture->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_picture->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief label 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_label
 * @param key_value
 */
static void lat_label_key_deposit_process ( lat_window_t *p_window,
        struct lat_label_control *p_label,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_label->in_data.key_event.up )
            {
                p_label->in_data.key_event.up();
            }

            if ( p_label->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_label->in_data.key_event.down )
            {
                p_label->in_data.key_event.down();
            }

            if ( p_label->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_label->in_data.key_event.left )
            {
                p_label->in_data.key_event.left();
            }

            if ( p_label->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_label->in_data.key_event.right )
            {
                p_label->in_data.key_event.right();
            }

            if ( p_label->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_label->in_data.key_event.enter )
            {
                p_label->in_data.key_event.enter();
            }

            if ( p_label->in_data.page_shift.enter )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.enter );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.enter )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.enter );
                }
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_label->in_data.key_event.esc )
            {
                p_label->in_data.key_event.esc();
            }

            if ( p_label->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_label->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_label->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_label->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief combo 聚焦模式下 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_combo
 * @param key_value
 */
static void lat_combo_focus_key_deposit_process ( lat_window_t *p_window,
        struct lat_combo_control *p_combo,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_combo->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_combo->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_combo->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_combo->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_combo->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_combo->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_combo->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_combo->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_combo->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_combo->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_combo->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_combo->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_combo->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_combo->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_combo->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_combo->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            p_combo->in_data.is_edit = TRUE;
            p_window->in_method.clear_part_display_cb ( p_combo->in_data.x, p_combo->in_data.y,
                    p_combo->in_data.width, p_combo->in_data.height );
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_combo->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_combo->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_combo->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_combo->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief combo 编辑模式下 托管处理(选择项处理)
 *
 * @param p_window
 * @param p_combo
 * @param key_value
 */
static void lat_combo_edit_key_deposit_process ( lat_window_t *p_window,
        struct lat_combo_control *p_combo,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            p_combo->out_method.combo_index_sub ( p_combo );
            p_window->in_method.clear_part_display_cb ( p_combo->in_data.x, p_combo->in_data.y,
                    p_combo->in_data.width, p_combo->in_data.height );

            if ( p_combo->in_data.p_deposit_current_index )
            {
                *p_combo->in_data.p_deposit_current_index =  p_combo->in_data.current_index ;
            }

            if ( p_combo->in_data.key_event.down )
            {
                p_combo->in_data.key_event.down();
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            p_combo->out_method.combo_index_add ( p_combo );
            p_window->in_method.clear_part_display_cb ( p_combo->in_data.x, p_combo->in_data.y,
                    p_combo->in_data.width, p_combo->in_data.height );

            if ( p_combo->in_data.p_deposit_current_index )
            {
                *p_combo->in_data.p_deposit_current_index =  p_combo->in_data.current_index ;
            }

            if ( p_combo->in_data.key_event.up )
            {
                p_combo->in_data.key_event.up();
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_combo->in_data.key_event.left )
            {
                p_combo->in_data.key_event.left();
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_combo->in_data.key_event.right )
            {
                p_combo->in_data.key_event.right();
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            *p_combo->in_data.p_deposit_current_index = p_combo->in_data.current_index;
            p_combo->in_data.is_edit = FALSE;

            if ( p_combo->in_data.key_event.enter )
            {
                p_combo->in_data.key_event.enter();
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_combo->in_data.p_deposit_current_index )
            {
                p_combo->in_data.current_index = *p_combo->in_data.p_deposit_current_index;
            }

            p_combo->in_data.is_edit = FALSE;

            if ( p_combo->in_data.key_event.esc )
            {
                p_combo->in_data.key_event.esc();
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief checkbox 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_checkbox
 * @param key_value
 */
static void lat_checkbox_key_deposit_process ( lat_window_t *p_window,
        struct lat_checkbox_control *p_checkbox,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_checkbox->in_data.key_event.up )
            {
                p_checkbox->in_data.key_event.up();
            }

            if ( p_checkbox->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_checkbox->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_checkbox->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_checkbox->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_checkbox->in_data.key_event.down )
            {
                p_checkbox->in_data.key_event.down();
            }

            if ( p_checkbox->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_checkbox->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }

            if ( p_checkbox->in_data.focus_shift.down )
            {
                p_window->out_method.control_focus_shift ( p_window, p_checkbox->in_data.focus_shift.down );
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_checkbox->in_data.key_event.left )
            {
                p_checkbox->in_data.key_event.left();
            }

            if ( p_checkbox->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_checkbox->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_checkbox->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_checkbox->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_checkbox->in_data.key_event.right )
            {
                p_checkbox->in_data.key_event.right();
            }

            if ( p_checkbox->in_data.focus_shift.right )
            {
                p_window->out_method.control_focus_shift ( p_window, p_checkbox->in_data.focus_shift.right );
            }
            else if ( p_checkbox->in_data.key_event.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_checkbox->in_data.page_shift.right );
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_checkbox->in_data.key_event.enter )
            {
                p_checkbox->in_data.key_event.enter();
            }

            if ( p_checkbox->in_data.is_select == TRUE )
            {
                p_checkbox->in_data.is_select = FALSE;
            }
            else
            {
                p_checkbox->in_data.is_select = TRUE;
            }

            p_window->in_method.clear_part_display_cb ( p_checkbox->in_data.x, p_checkbox->in_data.y,
                    p_checkbox->in_data.size, p_checkbox->in_data.size );

            if ( p_checkbox->in_data.p_deposit_is_select )
            {
                *p_checkbox->in_data.p_deposit_is_select = p_checkbox->in_data.is_select;
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_checkbox->in_data.key_event.esc )
            {
                p_checkbox->in_data.key_event.esc();
            }

            if ( p_checkbox->in_data.focus_shift.esc )
            {
                p_window->out_method.control_focus_shift ( p_window, p_checkbox->in_data.focus_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_checkbox->in_data.page_shift.esc )
                {
                    p_window->out_method.page_focus_shift ( p_window, p_checkbox->in_data.page_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief progressbar 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_progressbar
 * @param key_value
 */
static void lat_progressbar_key_deposit_process ( lat_window_t *p_window,
        struct lat_progressbar_control *p_progressbar,
        cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_progressbar->in_data.key_event.up )
            {
                p_progressbar->in_data.key_event.up();
            }

            if ( p_progressbar->in_data.page_shift.up )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.up );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.up )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.up );
                }
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_progressbar->in_data.key_event.down )
            {
                p_progressbar->in_data.key_event.down();
            }

            if ( p_progressbar->in_data.page_shift.down )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.down );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.down )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.down );
                }
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_progressbar->in_data.key_event.left )
            {
                p_progressbar->in_data.key_event.left();
            }

            if ( p_progressbar->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_progressbar->in_data.key_event.right )
            {
                p_progressbar->in_data.key_event.right();
            }

            if ( p_progressbar->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_progressbar->in_data.key_event.enter )
            {
                p_progressbar->in_data.key_event.enter();
            }

            if ( p_progressbar->in_data.page_shift.enter )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.enter );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.enter )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.enter );
                }
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_progressbar->in_data.key_event.esc )
            {
                p_progressbar->in_data.key_event.esc();
            }

            if ( p_progressbar->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_progressbar->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_progressbar->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_progressbar->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief listbox 托管处理(聚焦切换和按键事件响应)
 *
 * @param p_window
 * @param p_listbox
 * @param key_value
 */
static void lat_listbox_key_deposit_process ( lat_window_t *p_window, struct lat_listbox_control *p_listbox, cmw_u16_t key_value )
{
    switch ( key_value )
    {
        case LAT_KEY_UP:
        {
            if ( p_listbox->in_data.key_event.up )
            {
                p_listbox->in_data.key_event.up();
            }

            if ( p_listbox->in_data.current_index )
            {
                p_listbox->in_data.current_index--;
            }
            else
            {
                p_listbox->in_data.current_index =  p_listbox->in_data.max_show_line - 1;
                p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x, p_listbox->in_data.y,
                        p_listbox->in_data.width + 1, p_listbox->in_data.height + 1 );
            }

            if ( p_listbox->in_data.p_deposit_current_index )
            {
                *p_listbox->in_data.p_deposit_current_index = p_listbox->in_data.current_index;
            }
        }
        break;

        case LAT_KEY_DOWN:
        {
            if ( p_listbox->in_data.key_event.down )
            {
                p_listbox->in_data.key_event.down();
            }

            if ( ( p_listbox->in_data.current_index + 1 ) <  p_listbox->in_data.max_show_line ) // 当数据连续时,这个位置会出现bug,待优化
            {
                if ( p_listbox->in_data.p_text_list[p_listbox->in_data.current_index + 1] )
                {
                    p_listbox->in_data.current_index++;
                }
            }
            else
            {
                p_listbox->in_data.current_index =  0;
                p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x, p_listbox->in_data.y,
                        p_listbox->in_data.width + 1, p_listbox->in_data.height + 1 );
            }

            if ( p_listbox->in_data.p_deposit_current_index )
            {
                *p_listbox->in_data.p_deposit_current_index = p_listbox->in_data.current_index;
            }
        }
        break;

        case LAT_KEY_LEFT:
        {
            if ( p_listbox->in_data.key_event.left )
            {
                p_listbox->in_data.key_event.left();
            }

            if ( p_listbox->in_data.page_shift.left )
            {
                p_window->out_method.page_focus_shift ( p_window, p_listbox->in_data.page_shift.left );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_listbox->in_data.focus_shift.left )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_listbox->in_data.focus_shift.left );
                }
            }
        }
        break;

        case LAT_KEY_RIGHT:
        {
            if ( p_listbox->in_data.key_event.right )
            {
                p_listbox->in_data.key_event.right();
            }

            if ( p_listbox->in_data.page_shift.right )
            {
                p_window->out_method.page_focus_shift ( p_window, p_listbox->in_data.page_shift.right );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_listbox->in_data.focus_shift.right )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_listbox->in_data.focus_shift.right );
                }
            }
        }
        break;

        case LAT_KEY_ENTER:
        {
            if ( p_listbox->in_data.key_event.enter )
            {
                p_listbox->in_data.key_event.enter();
            }

            if ( p_listbox->in_data.page_shift.enter && strlen ( p_listbox->in_data.p_text_list[p_listbox->in_data.current_index] ) )
            {
                p_window->out_method.page_focus_shift ( p_window, p_listbox->in_data.page_shift.enter );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_listbox->in_data.focus_shift.enter )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_listbox->in_data.focus_shift.enter );
                }
            }
        }
        break;

        case LAT_KEY_ESC:
        {
            if ( p_listbox->in_data.key_event.esc )
            {
                p_listbox->in_data.key_event.esc();
            }

            if ( p_listbox->in_data.page_shift.esc )
            {
                p_window->out_method.page_focus_shift ( p_window, p_listbox->in_data.page_shift.esc );
                if (p_window->in_data.p_focus_page->in_data.last_focus_control_id)
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.last_focus_control_id );
                }
                else
                {
                    p_window->out_method.control_focus_shift ( p_window, p_window->in_data.p_focus_page->in_data.first_control_id );
                }
            }
            else
            {
                if ( p_listbox->in_data.focus_shift.esc )
                {
                    p_window->out_method.control_focus_shift ( p_window, p_listbox->in_data.focus_shift.esc );
                }
            }
        }
        break;

        default:
        {
        }
    }
}

/**
 * @brief 根据聚焦的类型 调用 按键托管处理函数(控件聚焦切换和按键事件处理)
 *
 * @param p_window
 * @param page
 * @param key_value
 * @return cmw_bool_t
 */
static cmw_bool_t lat_key_deposit_process ( lat_window_t *p_window, lat_page_t *page, cmw_u16_t key_value )
{
    cmw_slist_t *node = NULL;
    lat_button_control_t *p_button      = NULL;
    lat_picture_control_t *p_picture    = NULL;
    lat_label_control_t *p_label        = NULL;
    lat_number_control_t *p_number      = NULL;
    lat_combo_control_t *p_combo        = NULL;
    lat_checkbox_control_t *p_checkbox  = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    lat_listbox_control_t *p_listbox = NULL;

    if ( page )
    {
        cmw_slist_t *node2 = NULL;
        cmw_slist_t *node3 = NULL;
        lat_tabcontrol_t *p_tabcontrol = NULL;
        lat_tabcontrol_page_t *p_tabcontrol_page = NULL;
        cmw_slist_for_each ( node, &page->in_data.number_head )
        {
            p_number = cmw_container_of ( node, lat_number_control_t, list );

            if ( p_number->in_data.number_focus_position && p_number->in_data.is_focus  == TRUE )
            {
                lat_number_edit_key_deposit_process ( p_window, p_number, key_value );
                return TRUE;
            }
            else if ( p_number->in_data.is_focus == TRUE )
            {
                lat_number_focus_key_deposit_process ( p_window, p_number, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.button_head )
        {
            p_button = cmw_container_of ( node, lat_button_control_t, list );

            if ( p_button->in_data.is_focus == TRUE )
            {
                lat_button_focus_deposit_process ( p_window, p_button, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.picture_head )
        {
            p_picture = cmw_container_of ( node, lat_picture_control_t, list );

            if ( p_picture->in_data.is_focus == TRUE )
            {
                lat_picture_key_deposit_process ( p_window, p_picture, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.label_head )
        {
            p_label = cmw_container_of ( node, lat_label_control_t, list );

            if ( p_label->in_data.is_focus == TRUE )
            {
                lat_label_key_deposit_process ( p_window, p_label, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.combo_head )
        {
            p_combo = cmw_container_of ( node, lat_combo_control_t, list );

            if ( p_combo->in_data.is_focus == TRUE )
            {
                if ( p_combo->in_data.is_focus == TRUE && p_combo->in_data.is_edit  == TRUE )
                {
                    lat_combo_edit_key_deposit_process ( p_window, p_combo, key_value );
                }
                else
                {
                    lat_combo_focus_key_deposit_process ( p_window, p_combo, key_value );
                }

                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.checkbox_head )
        {
            p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

            if ( p_checkbox->in_data.is_focus == TRUE )
            {
                lat_checkbox_key_deposit_process ( p_window, p_checkbox, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.progressbar_head )
        {
            p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

            if ( p_progressbar->in_data.is_focus == TRUE )
            {
                lat_progressbar_key_deposit_process ( p_window, p_progressbar, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.listbox_head )
        {
            p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

            if ( p_listbox->in_data.is_focus == TRUE )
            {
                lat_listbox_key_deposit_process ( p_window, p_listbox, key_value );
                p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x, p_listbox->in_data.y,
                        p_listbox->in_data.width + 1, p_listbox->in_data.height + 1 );
                return TRUE;
            }
        }
        // 查询选项卡中是否有需要键值响应的控件
        cmw_slist_for_each ( node2, &page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.number_head )
                    {
                        p_number = cmw_container_of ( node, lat_number_control_t, list );

                        if ( p_number->in_data.number_focus_position && p_number->in_data.is_focus  == TRUE )
                        {
                            lat_number_edit_key_deposit_process ( p_window, p_number, key_value );
                            return TRUE;
                        }
                        else if ( p_number->in_data.is_focus == TRUE )
                        {
                            lat_number_focus_key_deposit_process ( p_window, p_number, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.button_head )
                    {
                        p_button = cmw_container_of ( node, lat_button_control_t, list );

                        if ( p_button->in_data.is_focus == TRUE )
                        {
                            lat_button_focus_deposit_process ( p_window, p_button, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.picture_head )
                    {
                        p_picture = cmw_container_of ( node, lat_picture_control_t, list );

                        if ( p_picture->in_data.is_focus == TRUE )
                        {
                            lat_picture_key_deposit_process ( p_window, p_picture, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.label_head )
                    {
                        p_label = cmw_container_of ( node, lat_label_control_t, list );

                        if ( p_label->in_data.is_focus == TRUE )
                        {
                            lat_label_key_deposit_process ( p_window, p_label, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.combo_head )
                    {
                        p_combo = cmw_container_of ( node, lat_combo_control_t, list );

                        if ( p_combo->in_data.is_focus == TRUE )
                        {
                            if ( p_combo->in_data.is_focus == TRUE && p_combo->in_data.is_edit  == TRUE )
                            {
                                lat_combo_edit_key_deposit_process ( p_window, p_combo, key_value );
                            }
                            else
                            {
                                lat_combo_focus_key_deposit_process ( p_window, p_combo, key_value );
                            }

                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.checkbox_head )
                    {
                        p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

                        if ( p_checkbox->in_data.is_focus == TRUE )
                        {
                            lat_checkbox_key_deposit_process ( p_window, p_checkbox, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.progressbar_head )
                    {
                        p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

                        if ( p_progressbar->in_data.is_focus == TRUE )
                        {
                            lat_progressbar_key_deposit_process ( p_window, p_progressbar, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.listbox_head )
                    {
                        p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

                        if ( p_listbox->in_data.is_focus == TRUE )
                        {
                            lat_listbox_key_deposit_process ( p_window, p_listbox, key_value );
                            p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x, p_listbox->in_data.y,
                                    p_listbox->in_data.width + 1, p_listbox->in_data.height + 1 );
                            return TRUE;
                        }
                    }
                }
            }
        }
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief 根据聚焦的对象 调用 对应的自定义按键回调函数
 *
 * @param page
 * @param key_value
 * @return cmw_bool_t
 */
static cmw_bool_t lat_key_process ( lat_page_t *page, cmw_u16_t key_value )
{
    cmw_slist_t *node = NULL;
    lat_button_control_t *p_button   = NULL;
    lat_picture_control_t *p_picture = NULL;
    lat_label_control_t *p_label    = NULL;
    lat_number_control_t *p_number  = NULL;
    lat_combo_control_t *p_combo    = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    lat_progressbar_control_t *p_progressbar   = NULL;
    lat_listbox_control_t *p_listbox    = NULL;

    if ( page )
    {
        cmw_slist_t *node2 = NULL;
        cmw_slist_t *node3 = NULL;
        lat_tabcontrol_t *p_tabcontrol = NULL;
        lat_tabcontrol_page_t *p_tabcontrol_page = NULL;
        cmw_slist_for_each ( node, &page->in_data.number_head )
        {
            p_number = cmw_container_of ( node, lat_number_control_t, list );

            if ( p_number->in_data.is_focus == TRUE &&  p_number->in_method.key_cb )
            {
                p_number->in_method.key_cb ( p_number, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.button_head )
        {
            p_button = cmw_container_of ( node, lat_button_control_t, list );

            if ( p_button->in_data.is_focus == TRUE && p_button->in_method.key_cb )
            {
                p_button->in_method.key_cb ( p_button, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.picture_head )
        {
            p_picture = cmw_container_of ( node, lat_picture_control_t, list );

            if ( p_picture->in_data.is_focus == TRUE && p_picture->in_method.key_cb )
            {
                p_picture->in_method.key_cb ( p_picture, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.label_head )
        {
            p_label = cmw_container_of ( node, lat_label_control_t, list );

            if ( p_label->in_data.is_focus == TRUE && p_label->in_method.key_cb )
            {
                p_label->in_method.key_cb ( p_label, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.combo_head )
        {
            p_combo = cmw_container_of ( node, lat_combo_control_t, list );

            if ( p_combo->in_data.is_focus == TRUE && p_combo->in_method.key_cb )
            {
                p_combo->in_method.key_cb ( p_combo, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.checkbox_head )
        {
            p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

            if ( p_checkbox->in_data.is_focus == TRUE && p_checkbox->in_method.key_cb )
            {
                p_checkbox->in_method.key_cb ( p_checkbox, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.progressbar_head )
        {
            p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

            if ( p_progressbar->in_data.is_focus == TRUE && p_progressbar->in_method.key_cb )
            {
                p_progressbar->in_method.key_cb ( p_progressbar, key_value );
                return TRUE;
            }
        }
        cmw_slist_for_each ( node, &page->in_data.listbox_head )
        {
            p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

            if ( p_listbox->in_data.is_focus == TRUE && p_listbox->in_method.key_cb )
            {
                p_listbox->in_method.key_cb ( p_listbox, key_value );
                return TRUE;
            }
        }
        // 查询选项卡中是否有需要键值响应的控件
        cmw_slist_for_each ( node2, &page->in_data.tabcontrol_head )
        {
            p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );
            cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
            {
                p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                if ( p_tabcontrol_page->in_data.is_show == TRUE )
                {
                    // button 响应
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.button_head )
                    {
                        p_button = cmw_container_of ( node, lat_button_control_t, list );

                        if ( p_button->in_data.is_focus == TRUE && p_button->in_method.key_cb )
                        {
                            p_button->in_method.key_cb ( p_button, key_value );
                            return TRUE;
                        }
                    }
                    // number响应
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.number_head )
                    {
                        p_number = cmw_container_of ( node, lat_number_control_t, list );

                        if ( p_number->in_data.is_focus == TRUE &&  p_number->in_method.key_cb )
                        {
                            p_number->in_method.key_cb ( p_number, key_value );
                            return TRUE;
                        }
                    }
                    // picture
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.picture_head )
                    {
                        p_picture = cmw_container_of ( node, lat_picture_control_t, list );

                        if ( p_picture->in_data.is_focus == TRUE && p_picture->in_method.key_cb )
                        {
                            p_picture->in_method.key_cb ( p_picture, key_value );
                            return TRUE;
                        }
                    }
                    // label
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.label_head )
                    {
                        p_label = cmw_container_of ( node, lat_label_control_t, list );

                        if ( p_label->in_data.is_focus == TRUE && p_label->in_method.key_cb )
                        {
                            p_label->in_method.key_cb ( p_label, key_value );
                            return TRUE;
                        }
                    }
                    // combo
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.combo_head )
                    {
                        p_combo = cmw_container_of ( node, lat_combo_control_t, list );

                        if ( p_combo->in_data.is_focus == TRUE && p_combo->in_method.key_cb )
                        {
                            p_combo->in_method.key_cb ( p_combo, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.checkbox_head )
                    {
                        p_checkbox = cmw_container_of ( node, lat_checkbox_control_t, list );

                        if ( p_checkbox->in_data.is_focus == TRUE && p_checkbox->in_method.key_cb )
                        {
                            p_checkbox->in_method.key_cb ( p_checkbox, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.progressbar_head )
                    {
                        p_progressbar = cmw_container_of ( node, lat_progressbar_control_t, list );

                        if ( p_progressbar->in_data.is_focus == TRUE && p_progressbar->in_method.key_cb )
                        {
                            p_progressbar->in_method.key_cb ( p_progressbar, key_value );
                            return TRUE;
                        }
                    }
                    cmw_slist_for_each ( node, &p_tabcontrol_page->in_data.listbox_head )
                    {
                        p_listbox = cmw_container_of ( node, lat_listbox_control_t, list );

                        if ( p_listbox->in_data.is_focus == TRUE && p_listbox->in_method.key_cb )
                        {
                            p_listbox->in_method.key_cb ( p_listbox, key_value );
                            return TRUE;
                        }
                    }
                }
            }
        }
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief 刷新 window
 *
 * @param p_window
 * @return cmw_bool_t
 */
static cmw_bool_t lat_window_refresh ( lat_window_t *p_window )
{
    uint16_t u16_key_value = 0;

    if ( p_window->in_data.p_focus_page == NULL )
    {
        return FALSE;
    }

    if ( p_window->in_data.p_focus_page->in_method.loop_exec )
    {
        p_window->in_data.p_focus_page->in_method.loop_exec ( p_window->in_data.p_focus_page );
    }

    u16_key_value = p_window->in_method.key_value_recv_cb ( p_window );

    // 优先响应键值
    if ( u16_key_value )
    {
        if ( p_window->in_data.p_focus_page->in_data.focus_control_id )
        {
            // 调用对应控件的按键回调或者页面的按键回调
            lat_key_process ( p_window->in_data.p_focus_page, u16_key_value );
            // 调用对应控件下按键的聚集切换和事件响应托管处理
            lat_key_deposit_process ( p_window, p_window->in_data.p_focus_page, u16_key_value );
        }
    }

    // 控件绘制
    lat_draw_number ( p_window );
    lat_draw_picture ( p_window );
    lat_draw_label ( p_window );
    lat_draw_button ( p_window );
    lat_draw_combo ( p_window );
    lat_draw_checkbox ( p_window );
    lat_draw_progressbar ( p_window );
    lat_draw_listbox ( p_window );
    return TRUE;
}

/**
 * @brief 获取指定页面中指定的控件ID的控件句柄
 *
 * @param p_window
 * @param page_id
 * @param control_id
 * @return void*
 */
static void *lat_get_control_handle ( struct lat_window *p_window,
                                      cmw_u16_t page_id,
                                      cmw_u16_t control_id )
{
    lat_button_control_t *p_button = NULL;
    lat_label_control_t *p_label = NULL;
    lat_picture_control_t *p_picture = NULL;
    lat_number_control_t *p_number = NULL;
    lat_combo_control_t *p_combo = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    lat_listbox_control_t *p_listbox = NULL;
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL )
    {
        return FALSE;
    }

    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            break;
        }
    }
    p_button  = lat_search_button ( p_page, control_id );

    if ( p_button )
    {
        return p_button;
    }

    p_label  = lat_search_label ( p_page, control_id );

    if ( p_label )
    {
        return p_label;
    }

    p_picture  = lat_search_picture ( p_page, control_id );

    if ( p_picture && p_picture->in_method.out_ctrl_cb )
    {
        return p_picture;
    }

    p_number  = lat_search_number ( p_page, control_id );

    if ( p_number )
    {
        return p_number;
    }

    p_combo  = lat_search_combo ( p_page, control_id );

    if ( p_combo )
    {
        return p_combo;
    }

    p_checkbox = lat_search_checkbox ( p_page, control_id );

    if ( p_checkbox )
    {
        return p_checkbox;
    }

    p_progressbar = lat_search_progressbar ( p_page, control_id );

    if ( p_progressbar )
    {
        return p_progressbar;
    }

    p_listbox = lat_search_listbox ( p_page, control_id );

    if ( p_listbox )
    {
        return p_listbox;
    }

    return NULL;
}

/**
 * @brief 设置 button 控件text内容
 *
 * @param p_button
 * @param p_text
 */
static void lat_button_set_text ( struct lat_button_control *p_button, cmw_u8_t *p_text )
{
    if ( p_button )
    {
        p_button->in_data.p_text = p_text;
    }
}

/**
 * @brief 添加 button 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_button
 * @param button_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_button_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_button_control_t *p_button,
        cmw_u16_t button_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_button == NULL || button_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < button_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_button[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_button[u16_cnt].in_data.is_show = TRUE;
                p_button[u16_cnt].out_method.set_text = lat_button_set_text;
                cmw_slist_append ( &p_page->in_data.button_head, &p_button[u16_cnt].list );
            }
        }
    }
    return TRUE;
}

/**
 * @brief 自动计算标签卡的尺寸和位置
 *
 * @param p_tabcontro
 * @param x
 * @param y
 * @param w
 * @param h
 * @return void
 */
static void lat_tabcontrol_size_auto_calibration ( lat_tabcontrol_t *p_tabcontrol,
        cmw_u16_t x, cmw_u16_t y, cmw_u16_t w, cmw_u16_t h )
{
    if ( p_tabcontrol->in_data.x >  x )
    {
        p_tabcontrol->in_data.x = x;
    }

    if ( p_tabcontrol->in_data.y >  y )
    {
        p_tabcontrol->in_data.y = y;
    }

    if ( p_tabcontrol->in_data.width <  x + w )
    {
        p_tabcontrol->in_data.width = x + w;
    }

    if ( p_tabcontrol->in_data.height <  y + h )
    {
        p_tabcontrol->in_data.height = y + h;
    }
}

/**
 * @brief  添加 button 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_button
 * @param button_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_button_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_button_control_t *p_button,
        cmw_u16_t button_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_button == NULL || button_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < button_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_button[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_button[u16_cnt].in_data.x, p_button[u16_cnt].in_data.y,
                                                                       p_button[u16_cnt].in_data.width, p_button[u16_cnt].in_data.height );
                                p_button[u16_cnt].in_data.is_show = TRUE;
                                p_button[u16_cnt].out_method.set_text = lat_button_set_text;
                                cmw_slist_append ( &p_tabcontrol_page->in_data.button_head, &p_button[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

/**
 * @brief 显示指定ID的选项卡中指定ID的页面
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @return cmw_bool_t
 */
static cmw_bool_t lat_show_tabcontrol_page ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;
    cmw_bool_t ret = FALSE;

    if ( p_window == NULL || tabcontrol_id == 0 || tabcontrolpage_id == 0 )
    {
        return ret;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;
            cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
            {
                p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                if ( p_tabcontrol->in_data.id == tabcontrol_id )
                {
                    cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                    {
                        p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                        if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                        {
                            // 清除上一选项页面
                            p_window->in_method.clear_part_display_cb ( p_tabcontrol->in_data.x, p_tabcontrol->in_data.y,
                                    p_tabcontrol->in_data.width, p_tabcontrol->in_data.height );
                            p_tabcontrol_page->in_data.is_show = TRUE;
                            ret = TRUE;
                        }
                        else
                        {
                            p_tabcontrol_page->in_data.is_show = FALSE;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

/**
 * @brief 设置 label 控件的 字符显示内容
 *
 * @param p_label
 * @param p_text
 */
static void lat_label_set_text ( struct lat_label_control *p_label, cmw_u8_t *p_text )
{
    if ( p_label )
    {
        p_label->in_data.p_text = p_text;
    }
}

/**
 * @brief 添加 label 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_label
 * @param label_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_label_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_label_control_t *p_label,
        cmw_u16_t label_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_label == NULL || label_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < label_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_label[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_label[u16_cnt].in_data.is_show = TRUE;
                p_label[u16_cnt].out_method.set_text = lat_label_set_text;
                cmw_slist_append ( &p_page->in_data.label_head, &p_label[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 label 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_label
 * @param label_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_label_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_label_control_t *p_label,
        cmw_u16_t label_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_label == NULL || label_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < label_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_label[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_label[u16_cnt].in_data.x, p_label[u16_cnt].in_data.y,
                                                                       p_label[u16_cnt].in_data.size * strlen ( ( const char * ) p_label[u16_cnt].in_data.p_text ),
                                                                       p_label[u16_cnt].in_data.size );
                                p_label[u16_cnt].in_data.is_show = TRUE;
                                p_label[u16_cnt].out_method.set_text = lat_label_set_text;
                                cmw_slist_append ( &p_tabcontrol_page->in_data.label_head, &p_label[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

/**
 * @brief 添加 picture 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_picture
 * @param picture_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_picture_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_picture_control_t *p_picture,
        cmw_u16_t picture_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_picture == NULL || picture_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < picture_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_picture[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_picture[u16_cnt].in_data.is_show = TRUE;
                cmw_slist_append ( &p_page->in_data.picture_head, &p_picture[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 picture 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_picture
 * @param picture_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_picture_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_picture_control_t *p_picture,
        cmw_u16_t picture_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_picture == NULL || picture_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < picture_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_picture[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_picture[u16_cnt].in_data.x, p_picture[u16_cnt].in_data.y,
                                                                       p_picture[u16_cnt].in_data.width, p_picture[u16_cnt].in_data.height );
                                p_picture[u16_cnt].in_data.is_show = TRUE;
                                cmw_slist_append ( &p_tabcontrol_page->in_data.picture_head, &p_picture[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}


/**
 * @brief 设置 number 控件数值聚焦位置
 *
 * @param p_number
 * @param number_focus_position
 */
static void lat_number_focus_position_set ( struct lat_number_control *p_number, cmw_u16_t number_focus_position )
{
    if ( p_number )
    {
        p_number->in_data.number_focus_position = number_focus_position;
    }
}

/**
 * @brief  number 控件加加
 *
 * @param p_number
 */
static void lat_number_value_add ( struct lat_number_control *p_number )
{
    cmw_s32_t value_temp = p_number->in_data.value;

    if ( p_number && p_number->in_data.number_focus_position )
    {
        if ( p_number->in_data.value >= 0 )
        {
            p_number->in_data.value += pow ( 10, ( p_number->in_data.number_focus_position - 1 ) );
        }
        else
        {
            value_temp = -value_temp;
            value_temp += pow ( 10, ( p_number->in_data.number_focus_position - 1 ) );
            p_number->in_data.value = - value_temp;
        }

        if ( p_number->in_data.value >= pow ( 10, ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits ) ) )
        {
            p_number->in_data.value = pow ( 10, ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits ) - 1 );
        }

        if ( p_number->in_method.value_change )
        {
            p_number->in_method.value_change ( p_number );
        }

        if (p_number->in_data.is_limit)
        {
            if (p_number->in_data.max_value < p_number->in_data.value)
            {
                p_number->in_data.value = p_number->in_data.max_value;
            }
        }
    }
}

/**
 * @brief number 控件减减
 *
 * @param p_number
 */
static void lat_number_value_sub ( struct lat_number_control *p_number )
{
    cmw_s32_t value_temp = p_number->in_data.value;

    if ( p_number && p_number->in_data.number_focus_position )
    {
        if ( p_number->in_data.value >= 0 )
        {
            if ( p_number->in_data.value >= ( pow ( 10, ( p_number->in_data.number_focus_position - 1 ) ) ) )
            {
                p_number->in_data.value -= pow ( 10, ( p_number->in_data.number_focus_position - 1 ) );
            }
            else
            {
                p_number->in_data.value += ( pow ( 10, ( p_number->in_data.number_focus_position ) ) -
                                             pow ( 10, ( p_number->in_data.number_focus_position - 1 ) ) );
            }
        }
        else
        {
            value_temp = -value_temp;

            if ( value_temp >= ( pow ( 10, ( p_number->in_data.number_focus_position - 1 ) ) ) )
            {
                value_temp -= pow ( 10, ( p_number->in_data.number_focus_position - 1 ) );
            }
            else
            {
                value_temp += ( pow ( 10, ( p_number->in_data.number_focus_position ) ) -
                                pow ( 10, ( p_number->in_data.number_focus_position - 1 ) ) );
            }

            p_number->in_data.value = - value_temp;
        }

        if ( p_number->in_data.value <= - pow ( 10, ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits ) ) )
        {
            p_number->in_data.value = 1 - pow ( 10, ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits ) );
        }

        if ( p_number->in_method.value_change )
        {
            p_number->in_method.value_change ( p_number );
        }
    }
}

/**
 * @brief number 控件数字聚焦左移
 *
 * @param p_number
 */
static void lat_number_focus_left_shift ( struct lat_number_control *p_number )
{
    if ( p_number )
    {
        p_number->in_data.number_focus_position++;
    }
}

/**
 * @brief number 控件数字聚焦右移
 *
 * @param p_number
 */
static void lat_number_focus_right_shift ( struct lat_number_control *p_number )
{
    if ( p_number )
    {
        if ( p_number->in_data.number_focus_position )
        {
            p_number->in_data.number_focus_position--;
        }
    }
}

/**
 * @brief 返回当前聚焦 数字的位置
 *
 * @param p_number
 * @return cmw_u16_t
 */
static cmw_u16_t lat_number_get_focus_positions ( struct lat_number_control *p_number )
{
    if ( p_number )
    {
        return p_number->in_data.number_focus_position;
    }

    return 0;
}

/**
 * @brief 添加 number 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_number
 * @param number_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_number_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_number_control_t *p_number,
        cmw_u16_t number_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_number == NULL || number_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < number_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_number[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_number[u16_cnt].in_data.is_show = TRUE;
                p_number[u16_cnt].out_method.get_number_focus_positions = lat_number_get_focus_positions;
                p_number[u16_cnt].out_method.number_focus_left_shift    = lat_number_focus_left_shift;
                p_number[u16_cnt].out_method.number_focus_right_shift   = lat_number_focus_right_shift;
                p_number[u16_cnt].out_method.value_add                  = lat_number_value_add;
                p_number[u16_cnt].out_method.value_sub                  = lat_number_value_sub;
                p_number[u16_cnt].out_method.number_focus_position_set  = lat_number_focus_position_set;
                cmw_slist_append ( &p_page->in_data.number_head, &p_number[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 number 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_number
 * @param number_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_number_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_number_control_t *p_number,
        cmw_u16_t number_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_number == NULL || number_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < number_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_number[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_number[u16_cnt].in_data.x, p_number[u16_cnt].in_data.y,
                                                                       p_number[u16_cnt].in_data.size * ( p_number[u16_cnt].in_data.integer_digits + p_number[u16_cnt].in_data.decimal_digits ),
                                                                       p_number[u16_cnt].in_data.size );
                                p_number[u16_cnt].in_data.is_show = TRUE;
                                p_number[u16_cnt].out_method.get_number_focus_positions = lat_number_get_focus_positions;
                                p_number[u16_cnt].out_method.number_focus_left_shift    = lat_number_focus_left_shift;
                                p_number[u16_cnt].out_method.number_focus_right_shift   = lat_number_focus_right_shift;
                                p_number[u16_cnt].out_method.value_add                  = lat_number_value_add;
                                p_number[u16_cnt].out_method.value_sub                  = lat_number_value_sub;
                                p_number[u16_cnt].out_method.number_focus_position_set  = lat_number_focus_position_set;
                                cmw_slist_append ( &p_tabcontrol_page->in_data.number_head, &p_number[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}


/**
 * @brief combo 索引加加
 *
 * @param p_combo
 */
static void lat_combo_index_add ( lat_combo_control_t *p_combo )
{
    if ( p_combo )
    {
        if ( p_combo->in_data.current_index < ( LAT_COMBO_DATA_LIST_MAX - 1 ) &&
                p_combo->in_data.p_text[p_combo->in_data.current_index + 1] != NULL )
        {
            p_combo->in_data.current_index++;
        }
        else
        {
            p_combo->in_data.current_index = 0;
        }
    }
}

/**
 * @brief combo 索引减减
 *
 * @param p_combo
 */
static void lat_combo_index_sub ( lat_combo_control_t *p_combo )
{
    if ( p_combo )
    {
        if ( p_combo->in_data.current_index )
        {
            p_combo->in_data.current_index--;
        }
    }
}

/**
 * @brief 添加 combo 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_combo
 * @param combo_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_combo_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_combo_control_t *p_combo,
        cmw_u16_t combo_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_combo == NULL || combo_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < combo_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_combo[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_init ( &p_combo[u16_cnt].list );
                p_combo[u16_cnt].in_data.is_show = TRUE;
                p_combo[u16_cnt].out_method.combo_index_add = lat_combo_index_add;
                p_combo[u16_cnt].out_method.combo_index_sub = lat_combo_index_sub;
                cmw_slist_append ( &p_page->in_data.combo_head, &p_combo[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 combo 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_combo
 * @param combo_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_combo_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_combo_control_t *p_combo,
        cmw_u16_t combo_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_combo == NULL || combo_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < combo_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_combo[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_combo[u16_cnt].in_data.x, p_combo[u16_cnt].in_data.y,
                                                                       p_combo[u16_cnt].in_data.width, p_combo[u16_cnt].in_data.height );
                                cmw_slist_init ( &p_combo[u16_cnt].list );
                                p_combo[u16_cnt].in_data.is_show = TRUE;
                                p_combo[u16_cnt].out_method.combo_index_add = lat_combo_index_add;
                                p_combo[u16_cnt].out_method.combo_index_sub = lat_combo_index_sub;
                                cmw_slist_append ( &p_tabcontrol_page->in_data.combo_head, &p_combo[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}



/**
 * @brief 添加 checkbox 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_checkbox
 * @param checkbox_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_checkbox_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_checkbox_control_t *p_checkbox,
        cmw_u16_t checkbox_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_checkbox == NULL || checkbox_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < checkbox_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_checkbox[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_checkbox[u16_cnt].in_data.is_show = TRUE;
                cmw_slist_init ( &p_checkbox[u16_cnt].list );
                cmw_slist_append ( &p_page->in_data.checkbox_head, &p_checkbox[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 checkbox 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_checkbox
 * @param checkbox_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_checkbox_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_checkbox_control_t *p_checkbox,
        cmw_u16_t checkbox_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_checkbox == NULL || checkbox_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < checkbox_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_checkbox[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_checkbox[u16_cnt].in_data.x, p_checkbox[u16_cnt].in_data.y,
                                                                       p_checkbox[u16_cnt].in_data.size, p_checkbox[u16_cnt].in_data.size );
                                p_checkbox[u16_cnt].in_data.is_show = TRUE;
                                cmw_slist_init ( &p_checkbox[u16_cnt].list );
                                cmw_slist_append ( &p_tabcontrol_page->in_data.checkbox_head, &p_checkbox[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}


/**
 * @brief 添加 progressbar 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_progressbar
 * @param progressbar_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_progressbar_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_progressbar_control_t *p_progressbar,
        cmw_u16_t progressbar_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_progressbar == NULL || progressbar_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < progressbar_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_progressbar[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_progressbar[u16_cnt].in_data.is_show = TRUE;
                cmw_slist_init ( &p_progressbar[u16_cnt].list );
                cmw_slist_append ( &p_page->in_data.progressbar_head, &p_progressbar[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 progressbar 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_progressbar
 * @param progressbar_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_progressbar_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_progressbar_control_t *p_progressbar,
        cmw_u16_t progressbar_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_progressbar == NULL || progressbar_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < progressbar_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_progressbar[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_progressbar[u16_cnt].in_data.x, p_progressbar[u16_cnt].in_data.y,
                                                                       p_progressbar[u16_cnt].in_data.width, p_progressbar[u16_cnt].in_data.height );
                                p_progressbar[u16_cnt].in_data.is_show = TRUE;
                                cmw_slist_init ( &p_progressbar[u16_cnt].list );
                                cmw_slist_append ( &p_tabcontrol_page->in_data.progressbar_head, &p_progressbar[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

/**
 * @brief 添加 listbox 控件列表
 *
 * @param p_window
 * @param page_id
 * @param p_listbox
 * @param listbox_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_listbox_control ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_listbox_control_t *p_listbox,
        cmw_u16_t listbox_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_listbox == NULL || listbox_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;

            for ( u16_cnt = 0; u16_cnt < listbox_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_listbox[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                p_listbox[u16_cnt].in_data.is_show = TRUE;
                cmw_slist_init ( &p_listbox[u16_cnt].list );
                cmw_slist_append ( &p_page->in_data.listbox_head, &p_listbox[u16_cnt].list );
            }
        }
    }
    return FALSE;
}

/**
 * @brief  添加 listbox 控件列表到tabcontrol的指定页面中
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param tabcontrolpage_id
 * @param p_listbox
 * @param listbox_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_listbox_tabcontrolpage ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id,
        lat_listbox_control_t *p_listbox,
        cmw_u16_t listbox_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_listbox == NULL || listbox_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            cmw_slist_t *node3 = NULL;
            lat_tabcontrol_t *p_tabcontrol = NULL;
            lat_tabcontrol_page_t *p_tabcontrol_page = NULL;

            // 将 button 添加到页面下的 button 钩子上
            for ( u16_cnt = 0; u16_cnt < listbox_num; u16_cnt++ )
            {
                // 发现有重复ID的控件就停止添加
                if ( lat_get_control_handle ( p_window, page_id, p_listbox[u16_cnt].in_data.id ) )
                {
                    return FALSE;
                }

                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol->in_data.id == tabcontrol_id )
                    {
                        cmw_slist_for_each ( node3, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page = cmw_container_of ( node3, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page->in_data.id == tabcontrolpage_id )
                            {
                                lat_tabcontrol_size_auto_calibration ( p_tabcontrol,
                                                                       p_listbox[u16_cnt].in_data.x, p_listbox[u16_cnt].in_data.y,
                                                                       p_listbox[u16_cnt].in_data.width, p_listbox[u16_cnt].in_data.height );
                                p_listbox[u16_cnt].in_data.is_show = TRUE;
                                cmw_slist_init ( &p_listbox[u16_cnt].list );
                                cmw_slist_append ( &p_tabcontrol_page->in_data.listbox_head, &p_listbox[u16_cnt].list );
                            }
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

/**
 * @brief 往选项卡中添加一个或者多个选项页
 *
 * @param p_window
 * @param page_id
 * @param tabcontrol_id
 * @param p_tabcontrol_page
 * @param tabcontrol_page_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_tabcontrol_page ( lat_window_t *p_window,
        cmw_u16_t page_id, cmw_u16_t tabcontrol_id,
        lat_tabcontrol_page_t *p_tabcontrol_page,
        cmw_u16_t tabcontrol_page_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;
    cmw_slist_t *node2 = NULL;
    lat_tabcontrol_t *p_tabcontrol = NULL;

    if ( p_window == NULL || p_tabcontrol_page == NULL || tabcontrol_page_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
            {
                p_tabcontrol = cmw_container_of ( node2, lat_tabcontrol_t, list );

                if ( p_tabcontrol->in_data.id == tabcontrol_id )
                {
                    cmw_u16_t u16_cnt = 0;
                    cmw_slist_t *page_node = NULL;
                    lat_tabcontrol_page_t *p_tabcontrol_page_temp = NULL;

                    for ( u16_cnt = 0; u16_cnt < tabcontrol_page_num; u16_cnt++ )
                    {
                        // 发现有重复ID的选项卡页面就停止添加
                        cmw_slist_for_each ( page_node, &p_tabcontrol->in_data.page_head )
                        {
                            p_tabcontrol_page_temp = cmw_container_of ( page_node, lat_tabcontrol_page_t, list );

                            if ( p_tabcontrol_page_temp->in_data.id == p_tabcontrol_page[u16_cnt].in_data.id )
                            {
                                return FALSE;
                            }
                        }
                        cmw_slist_init ( &p_tabcontrol_page[u16_cnt].list );
                        cmw_slist_append ( &p_tabcontrol->in_data.page_head, &p_tabcontrol_page[u16_cnt].list );
                    }
                }
            }
        }
    }
    return TRUE;
}
/**
 * @brief 添加一个或多个选项卡子项
 *
 * @param p_window
 * @param page_id
 * @param p_tabcontrol
 * @param tabcontrol_num
 * @return cmw_bool_t
 */
static cmw_bool_t lat_add_tabcontrol_container ( lat_window_t *p_window,
        cmw_u16_t page_id,
        lat_tabcontrol_t *p_tabcontrol,
        cmw_u16_t tabcontrol_num )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || p_tabcontrol == NULL || tabcontrol_num == 0 )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            cmw_u16_t u16_cnt = 0;
            cmw_slist_t *node2 = NULL;
            lat_tabcontrol_t *p_tabcontrol_2 = NULL;

            for ( u16_cnt = 0; u16_cnt < tabcontrol_num; u16_cnt++ )
            {
                // 发现有重复ID的选项卡就停止添加
                cmw_slist_for_each ( node2, &p_page->in_data.tabcontrol_head )
                {
                    p_tabcontrol_2 = cmw_container_of ( node2, lat_tabcontrol_t, list );

                    if ( p_tabcontrol_2->in_data.id == p_tabcontrol[u16_cnt].in_data.id )
                    {
                        return FALSE;
                    }
                }
                cmw_slist_init ( &p_tabcontrol[u16_cnt].list );
                cmw_slist_append ( &p_page->in_data.tabcontrol_head, &p_tabcontrol[u16_cnt].list );
            }
        }
    }
    return TRUE;
}


/**
 * @brief 创建一个菜单页
 *
 * @param p_window
 * @param page_id
 * @return cmw_bool_t
 */
static lat_page_t *lat_page_create ( lat_window_t *p_window, cmw_u16_t page_id, cmw_u16_t first_control_id )
{
    lat_page_t *page = NULL;
    lat_page_t *page_temp = NULL;

    if ( p_window == NULL )
    {
        return NULL;
    }
    else
    {
        cmw_slist_t *node = NULL;
        lat_page_t *p_page = NULL;
        page = ( lat_page_t * ) cmw_malloc ( sizeof ( lat_page_t ) );

        if ( page == NULL )
        {
            cmw_free ( page );
            return FALSE;
        }

        memset ( page, 0, sizeof ( lat_page_t ) );
        page->in_data.first_control_id = first_control_id;
        page->in_data.last_focus_control_id = first_control_id;
        /* 查找当前window中挂载页面最大ID值 */
        cmw_slist_for_each ( node, &p_window->in_data.page_head )
        {
            page_temp = cmw_container_of ( node, lat_page_t, list );

            /* 如果已经存在该ID的页面，则直接释放以上申请的内存 */
            if ( page_temp->in_data.page_id == page_id )
            {
                cmw_free ( page );
                return NULL;
            }
        }
        page->in_data.page_id = page_id;
        page->in_data.focus_control_id     = 0;
        page->in_data.button_head.next     = CMW_NULL;
        page->in_data.label_head.next      = CMW_NULL;
        page->in_data.picture_head.next    = CMW_NULL;
        page->in_data.number_head.next     = CMW_NULL;
        page->in_data.combo_head.next      = CMW_NULL;
        page->in_data.tabcontrol_head.next   = CMW_NULL;
        /* 往window 的 page链表中挂接page */
        cmw_slist_append ( &p_window->in_data.page_head, &page->list );

        // 指向第一页
        if ( p_window->in_data.p_focus_page == NULL )
        {
            p_page = cmw_container_of ( p_window->in_data.page_head.next, lat_page_t, list );

            if ( p_page )
            {
                p_window->in_data.p_focus_page = p_page;
            }
        }

        return page;
    }
}

/**
 * @brief 为页面添加循环调用函数
 *
 * @param p_window
 * @param page_id
 * @param loop_exec
 * @return cmw_bool_t
 */
static cmw_bool_t lat_page_add_loop_cb ( lat_window_t *p_window,
        cmw_u16_t page_id,
        void ( *loop_exec ) ( struct lat_page *p_page ) )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || loop_exec == NULL )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            p_page->in_method.loop_exec = loop_exec;
        }
    }
    return TRUE;
}


/**
 * @brief 为页面添加进入函数
 *
 * @param p_window
 * @param page_id
 * @param in_page_cb
 * @return cmw_bool_t
 */
static cmw_bool_t lat_page_add_in_cb ( lat_window_t *p_window,
                                       cmw_u16_t page_id,
                                       void ( *in_page_cb ) ( struct lat_page *p_page ) )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || in_page_cb == NULL )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            p_page->in_method.in_page_cb = in_page_cb;
        }
    }
    return TRUE;
}
/**
 * @brief 为页面添加退出函数
 *
 * @param p_window
 * @param page_id
 * @param out_page_cb
 * @return cmw_bool_t
 */
static cmw_bool_t lat_page_add_out_cb ( lat_window_t *p_window,
                                        cmw_u16_t page_id,
                                        void ( *out_page_cb ) ( struct lat_page *p_page ) )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL || out_page_cb == NULL )
    {
        return FALSE;
    }

    // 从window中查找 Page ID
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            p_page->in_method.out_page_cb = out_page_cb;
        }
    }
    return TRUE;
}

/**
 * @brief 切换 聚焦的页面
 *
 * @param p_window
 * @param page_id
 * @return cmw_bool_t
 */
static cmw_bool_t lat_shift_page ( lat_window_t *p_window, cmw_u16_t page_id )
{
    cmw_slist_t *node = NULL;
    lat_page_t *p_page = NULL;

    if ( p_window == NULL )
    {
        return FALSE;
    }

    // 调用退出 Page 函数
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page == p_window->in_data.p_focus_page && p_window->in_data.p_focus_page->in_method.out_page_cb )
        {
            p_window->in_data.p_focus_page->in_method.out_page_cb ( p_page );
        }
    }
    // 切换 page
    cmw_slist_for_each ( node, &p_window->in_data.page_head )
    {
        p_page = cmw_container_of ( node, lat_page_t, list );

        if ( p_page->in_data.page_id == page_id )
        {
            // 记录上一页面ID
            if ( p_window->in_data.p_focus_page )
            {
                p_window->in_data.last_focus_page_id = p_window->in_data.p_focus_page->in_data.page_id;
            }

            p_window->in_method.clear_display_cb();
            p_window->in_data.p_focus_page = p_page;
            //        p_window->out_method.control_focus_shift ( p_window, 1 ); // 需要改进

            // 调用切换后 Page 的进入函数
            if ( p_window->in_data.p_focus_page->in_method.in_page_cb )
            {
                p_window->in_data.p_focus_page->in_method.in_page_cb ( p_page );
            }
        }
    }
    return TRUE;
}

/**
 * @brief 键值接收
 *
 * @param window
 * @return uint16_t
 */
static uint16_t lat_key_recv ( lat_window_t *window )
{
    uint16_t key;
    key = window->in_data.key_value;
    
    window->in_data.last_key_value = window->in_data.key_value;

    window->in_data.key_value = 0;
    return key;
}

/**
 * @brief 当按下按键时发送键值给UI
 *
 * @param window
 * @param key_value
 */
static void lat_key_send ( lat_window_t *window, cmw_u16_t key_value )
{
    window->in_data.key_value = key_value;
}

/**
 * @brief 调用当前控件的退出聚焦函数
 *
 * @param p_window
 * @return cmw_bool_t
 */
static cmw_bool_t lat_ctrl_out ( lat_window_t *p_window )
{
    lat_button_control_t *p_button = NULL;
    lat_label_control_t *p_label = NULL;
    lat_picture_control_t *p_picture = NULL;
    lat_number_control_t *p_number = NULL;
    lat_combo_control_t *p_combo = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    lat_listbox_control_t *p_listbox = NULL;

    if ( p_window == NULL || p_window->in_data.p_focus_page == NULL )
    {
        return FALSE;
    }

    p_button  = lat_search_button ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_button )
    {
        p_button->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_button->in_data.x, p_button->in_data.y,
                p_button->in_data.width, p_button->in_data.height );

        if ( p_button->in_method.out_ctrl_cb )
        {
            p_button->in_method.out_ctrl_cb ( p_button );
        }

        return TRUE;
    }

    p_label  = lat_search_label ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_label )
    {
        p_label->in_data.is_focus = FALSE;

        if ( p_label->in_method.out_ctrl_cb )
        {
            p_label->in_method.out_ctrl_cb ( p_label );
        }

        return TRUE;
    }

    p_picture  = lat_search_picture ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_picture )
    {
        p_picture->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_picture->in_data.x, p_picture->in_data.y,
                p_picture->in_data.width, p_picture->in_data.height );

        if ( p_picture->in_method.out_ctrl_cb )
        {
            p_picture->in_method.out_ctrl_cb ( p_picture );
        }

        return TRUE;
    }

    p_number  = lat_search_number ( p_window->in_data.p_focus_page,
                                    p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_number )
    {
        p_number->in_data.is_focus = FALSE;
        p_number->in_data.number_focus_position = 0;

        if ( p_number->in_data.value < 0 )
        {
            p_window->in_method.clear_part_display_cb ( p_number->in_data.x, p_number->in_data.y,
                    ( p_number->in_data.size / 2 ) * ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits + 2 ),
                    p_number->in_data.size );
        }
        else
        {
            p_window->in_method.clear_part_display_cb ( p_number->in_data.x, p_number->in_data.y,
                    ( p_number->in_data.size / 2 ) * ( p_number->in_data.decimal_digits + p_number->in_data.integer_digits + 1 ),
                    p_number->in_data.size );
        }

        if ( p_number->in_method.out_ctrl_cb )
        {
            p_number->in_method.out_ctrl_cb ( p_number );
        }

        return TRUE;
    }

    p_combo  = lat_search_combo ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_combo )
    {
        p_combo->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_combo->in_data.x, p_combo->in_data.y,
                p_combo->in_data.width, p_combo->in_data.height );

        if ( p_combo->in_method.out_ctrl_cb )
        {
            p_combo->in_method.out_ctrl_cb ( p_combo );
        }

        return TRUE;
    }

    p_checkbox = lat_search_checkbox ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_checkbox )
    {
        p_checkbox->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_checkbox->in_data.x, p_checkbox->in_data.y,
                p_checkbox->in_data.size, p_checkbox->in_data.size );

        if ( p_checkbox->in_method.out_ctrl_cb )
        {
            p_checkbox->in_method.out_ctrl_cb ( p_checkbox );
        }

        return TRUE;
    }

    p_progressbar = lat_search_progressbar ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_progressbar )
    {
        p_progressbar->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_progressbar->in_data.x, p_progressbar->in_data.y,
                p_progressbar->in_data.width + 1, p_progressbar->in_data.height + 1 );

        if ( p_progressbar->in_method.out_ctrl_cb )
        {
            p_progressbar->in_method.out_ctrl_cb ( p_progressbar );
        }

        return TRUE;
    }

    p_listbox = lat_search_listbox ( p_window->in_data.p_focus_page, p_window->in_data.p_focus_page->in_data.focus_control_id );

    if ( p_listbox )
    {
        p_listbox->in_data.is_focus = FALSE;
        p_window->in_method.clear_part_display_cb ( p_listbox->in_data.x, p_listbox->in_data.y,
                p_listbox->in_data.width + 1, p_listbox->in_data.height + 1 );

        if ( p_listbox->in_method.out_ctrl_cb )
        {
            p_listbox->in_method.out_ctrl_cb ( p_listbox );
        }

        return TRUE;
    }

    return FALSE;
}

/**
 * @brief 调用当前控件的进入聚焦函数
 *
 * @param p_window
 * @param control_id
 * @return cmw_bool_t
 */
static cmw_bool_t lat_ctrl_in ( lat_window_t *p_window, uint16_t control_id )
{
    lat_button_control_t *p_button = NULL;
    lat_label_control_t *p_label = NULL;
    lat_picture_control_t *p_picture = NULL;
    lat_number_control_t *p_number = NULL;
    lat_combo_control_t *p_combo = NULL;
    lat_checkbox_control_t *p_checkbox = NULL;
    lat_progressbar_control_t *p_progressbar = NULL;
    lat_listbox_control_t *p_listbox = NULL;

    if ( p_window == NULL || p_window->in_data.p_focus_page == NULL )
    {
        return FALSE;
    }

    p_button  = lat_search_button ( p_window->in_data.p_focus_page, control_id );

    if ( p_button )
    {
        p_button->in_data.is_focus = TRUE;

        if ( p_button->in_method.in_ctrl_cb )
        {
            p_button->in_method.in_ctrl_cb ( p_button );
        }

        return TRUE;
    }

    p_label  = lat_search_label ( p_window->in_data.p_focus_page, control_id );

    if ( p_label )
    {
        p_label->in_data.is_focus = TRUE;

        if ( p_label->in_method.in_ctrl_cb )
        {
            p_label->in_method.in_ctrl_cb ( p_label );
        }

        return TRUE;
    }

    p_picture  = lat_search_picture ( p_window->in_data.p_focus_page, control_id );

    if ( p_picture )
    {
        p_picture->in_data.is_focus = TRUE;

        if ( p_picture->in_method.in_ctrl_cb )
        {
            p_picture->in_method.in_ctrl_cb ( p_picture );
        }

        return TRUE;
    }

    p_number  = lat_search_number ( p_window->in_data.p_focus_page, control_id );

    if ( p_number )
    {
        p_number->in_data.is_focus = TRUE;

        if (p_window->in_data.last_key_value == LAT_KEY_LEFT)
        {
            p_number->in_data.number_focus_position = 1;
        }
        else
        {
            p_number->in_data.number_focus_position = p_number->in_data.integer_digits + p_number->in_data.decimal_digits;
        }
        

        if ( p_number->in_method.in_ctrl_cb )
        {
            p_number->in_method.in_ctrl_cb ( p_number );
        }

        return TRUE;
    }

    p_combo  = lat_search_combo ( p_window->in_data.p_focus_page, control_id );

    if ( p_combo )
    {
        p_combo->in_data.is_focus = TRUE;

        if ( p_combo->in_method.in_ctrl_cb )
        {
            p_combo->in_method.in_ctrl_cb ( p_combo );
        }

        return TRUE;
    }

    p_checkbox  = lat_search_checkbox ( p_window->in_data.p_focus_page, control_id );

    if ( p_checkbox )
    {
        p_checkbox->in_data.is_focus = TRUE;

        if ( p_checkbox->in_method.in_ctrl_cb )
        {
            p_checkbox->in_method.in_ctrl_cb ( p_checkbox );
        }

        return TRUE;
    }

    p_progressbar  = lat_search_progressbar ( p_window->in_data.p_focus_page, control_id );

    if ( p_progressbar )
    {
        p_progressbar->in_data.is_focus = TRUE;

        if ( p_progressbar->in_method.in_ctrl_cb )
        {
            p_progressbar->in_method.in_ctrl_cb ( p_progressbar );
        }

        return TRUE;
    }

    p_listbox  = lat_search_listbox ( p_window->in_data.p_focus_page, control_id );

    if ( p_listbox )
    {
        p_listbox->in_data.is_focus = TRUE;

        if ( p_listbox->in_method.in_ctrl_cb )
        {
            p_listbox->in_method.in_ctrl_cb ( p_listbox );
        }

        return TRUE;
    }

    return FALSE;
}

/**
 * @brief 页面内设定聚焦的控件
 *
 * @param p_window
 * @param control_id
 * @return cmw_bool_t
 */
static cmw_bool_t lat_shift_control ( lat_window_t *p_window, uint16_t control_id )
{
    if ( p_window == NULL || ( p_window->in_data.p_focus_page == NULL ) || ( control_id == 0 ) )
    {
        return FALSE;
    }

    // 判断当前的控件id 是否在列表中存在
    if ( ( lat_search_button_show ( p_window->in_data.p_focus_page, control_id ) == NULL )           &&
            ( lat_search_label_show ( p_window->in_data.p_focus_page, control_id ) == NULL )         &&
            ( lat_search_number_show ( p_window->in_data.p_focus_page, control_id ) == NULL )        &&
            ( lat_search_picture_show ( p_window->in_data.p_focus_page, control_id ) == NULL )       &&
            ( lat_search_combo_show ( p_window->in_data.p_focus_page, control_id ) == NULL )         &&
            ( lat_search_checkbox_show ( p_window->in_data.p_focus_page, control_id ) == NULL )      &&
            ( lat_search_progressbar_show ( p_window->in_data.p_focus_page, control_id ) == NULL )   &&
            ( lat_search_listbox_show ( p_window->in_data.p_focus_page, control_id ) == NULL )
       )
    {
        return FALSE;
    }

    // 查找所切换的控件是否显示

    // if ( p_window->in_data.p_focus_page->in_data.focus_control_id != control_id )
    // {
    //     // 切换聚焦的控件
    //     p_window->in_data.p_focus_page->in_data.focus_control_id   = control_id;
    //     // 调用下一聚焦控件的进入聚焦函数
    //     lat_ctrl_in ( p_window, control_id );
    // }

    if ( p_window->in_data.p_focus_page->in_data.focus_control_id == control_id )
    {
        return FALSE;
    }
    else
    {
        // 调用当前控件的退出聚焦函数并将控件设置为不聚焦状态
        lat_ctrl_out ( p_window );
        // 切换聚焦的控件
        p_window->in_data.p_focus_page->in_data.focus_control_id   = control_id;
        // 调用下一聚焦控件的进入聚焦函数并将控件设置为聚焦状态
        lat_ctrl_in ( p_window, control_id );

        if (p_window->in_data.p_focus_page->in_data.focus_control_id != p_window->in_data.p_focus_page->in_data.last_focus_control_id)
        {
            p_window->in_data.p_focus_page->in_data.last_focus_control_id = p_window->in_data.p_focus_page->in_data.focus_control_id;
        }
    }

    return TRUE;
}

/**
 * @brief littice window initializing and config
 *
 * @param p_window
 * @param p_lat_config
 * @return cmw_bool_t
 */
lat_window_t *cmw_lattice_window_create ( lat_config_t *p_lat_config )
{
    lat_window_t *p_window = NULL;
    p_window = ( lat_window_t * ) cmw_malloc ( sizeof ( lat_window_t ) );

    if ( p_window == NULL && p_lat_config == NULL )
    {
        // 此处只为不报警
        lat_draw_empty_horizon_line ( p_window, 0, 0, 1 );
        lat_draw_empty_vertical_line ( p_window, 0, 0, 1 );
        return NULL;
    }

    if ( p_lat_config->draw_chinese_cb          == NULL ||
            p_lat_config->draw_picture_cb       == NULL ||
            p_lat_config->draw_str_cb           == NULL ||
            p_lat_config->draw_pixel_cb         == NULL ||
            p_lat_config->clear_part_display_cb == NULL ||
            p_lat_config->clear_display_cb      == NULL )
    {
        return NULL;
    }

    cmw_slist_init ( &p_window->in_data.page_head );
    p_window->in_data.p_focus_page = NULL;
    p_window->in_method = *p_lat_config;
    p_window->in_method.key_value_recv_cb               = lat_key_recv;
    p_window->out_method.window_refresh                 = lat_window_refresh;
    p_window->out_method.page_create                    = lat_page_create;
    p_window->out_method.add_button_control             = lat_add_button_control;
    p_window->out_method.add_label_control              = lat_add_label_control;
    p_window->out_method.add_picture_control            = lat_add_picture_control;
    p_window->out_method.add_number_control             = lat_add_number_control;
    p_window->out_method.add_combo_control              = lat_add_combo_control;
    p_window->out_method.key_value_send_cb              = lat_key_send;
    p_window->out_method.add_checkbox_control           = lat_add_checkbox_control;
    p_window->out_method.add_progressbar_control        = lat_add_progressbar_control;
    p_window->out_method.add_listbox_control            = lat_add_listbox_control;
    p_window->out_method.add_tabcontrol_container       = lat_add_tabcontrol_container;
    p_window->out_method.add_tabcontrol_page            = lat_add_tabcontrol_page;
    p_window->out_method.add_button_tabcontrolpage      = lat_add_button_tabcontrolpage;
    p_window->out_method.add_label_tabcontrolpage       = lat_add_label_tabcontrolpage;
    p_window->out_method.add_picture_tabcontrolpage     = lat_add_picture_tabcontrolpage;
    p_window->out_method.add_number_tabcontrolpage      = lat_add_number_tabcontrolpage;
    p_window->out_method.add_combo_tabcontrolpage       = lat_add_combo_tabcontrolpage;
    p_window->out_method.add_checkbox_tabcontrolpage    = lat_add_checkbox_tabcontrolpage;
    p_window->out_method.add_progressbar_tabcontrolpage = lat_add_progressbar_tabcontrolpage;
    p_window->out_method.add_listbox_tabcontrolpage     = lat_add_listbox_tabcontrolpage;
    p_window->out_method.control_focus_shift            = lat_shift_control;
    p_window->out_method.page_focus_shift               = lat_shift_page;
    p_window->out_method.get_control_handle             = lat_get_control_handle;
    p_window->out_method.show_tabcontrol_page           = lat_show_tabcontrol_page;
    p_window->out_method.page_add_loop_cb               = lat_page_add_loop_cb;
    p_window->out_method.page_add_in_cb                 = lat_page_add_in_cb;
    p_window->out_method.page_add_out_cb                = lat_page_add_out_cb;
    return p_window;
}




