/**
 * @file lat_core.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef __CMW_LAT_H__
#define __CMW_LAT_H__

#include "cmw_def.h"

#if defined(CMW_USING_LAT)

#ifdef __cplusplus
extern "C" {
#endif

#define LAT_KEY_UP                 1
#define LAT_KEY_DOWN               2
#define LAT_KEY_LEFT               3
#define LAT_KEY_RIGHT              4
#define LAT_KEY_ENTER              5
#define LAT_KEY_ESC                6

// 切换页面定义
typedef struct
{
    cmw_u16_t up;
    cmw_u16_t down;
    cmw_u16_t left;
    cmw_u16_t right;
    cmw_u16_t enter;
    cmw_u16_t esc;
} lat_page_shift_t;

// 切换聚焦方向定义
typedef struct
{
    cmw_u16_t up;
    cmw_u16_t down;
    cmw_u16_t left;
    cmw_u16_t right;
    cmw_u16_t enter;
    cmw_u16_t esc;
} lat_control_focus_shift_t;
// 按键事件定义
typedef void ( *lat_key_cb_t ) ( void );
typedef struct
{
    lat_key_cb_t up;
    lat_key_cb_t down;
    lat_key_cb_t left;
    lat_key_cb_t right;
    lat_key_cb_t enter;
    lat_key_cb_t esc;
} lat_key_event_t;

/**
 * @brief 按钮 button class define
 *
 */
struct lat_button_control
{
    struct button_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index of button */
        cmw_u16_t x; /* x position of button */
        cmw_u16_t y; /* y position of button */
        cmw_u16_t width; /* width of button */
        cmw_u16_t height; /* height of button */
        cmw_u16_t radius; /* radius of button */
        cmw_u16_t x_text_offset; /* x position of text offset */
        cmw_u16_t y_text_offset; /* y position of text offset */
        cmw_u16_t text_size; /* size of text */
        cmw_u8_t *p_text; /* pointer to text */
        cmw_u8_t **pp_deposit_text; /* deposit to text */

        cmw_u8_t is_focus; /* flag indicating if focus should be focused */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct button_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_button_control *p_button );
        void ( *out_ctrl_cb ) ( struct lat_button_control *p_button );
        void ( *key_cb ) ( struct lat_button_control *p_button, cmw_u16_t key_value );
    } in_method;
    struct button_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *set_text ) ( struct lat_button_control *p_button, cmw_u8_t *p_text );
    } out_method;
    cmw_slist_t list;
};

typedef struct lat_button_control lat_button_control_t;

/**
 * @brief 图片 picture class define
 *
 */
struct lat_picture_control
{
    struct picture_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index of button */
        cmw_u16_t x; /* x position of picture */
        cmw_u16_t y; /* y position of picture */
        cmw_u16_t width; /* width of picture */
        cmw_u16_t height; /* height of picture */
        cmw_u8_t *p_picture; /* pointer to picture */
        cmw_u8_t **pp_deposit_picture; /* deposit to text */

        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct picture_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_picture_control *p_picture );
        void ( *out_ctrl_cb ) ( struct lat_picture_control *p_picture );
        void ( *key_cb ) ( struct lat_picture_control *p_picture, cmw_u16_t key_value );
    } in_method;
    //  struct picture_out_method        /* 应用层可直接调用的方法 */
    //    {
    //        void (*config)(struct lat_picture_control p_picture, cmw_u16_t x, cmw_u16_t y, cmw_u16_t width, cmw_u16_t height, uint32_t * p_picture_addr);
    //        void (*change_picture)(struct lat_picture_control p_picture, uint32_t * p_picture_addr);
    //  } out_method;
    cmw_slist_t list;
};

typedef struct lat_picture_control lat_picture_control_t;

/**
 * @brief 标签 label class define
 *
 */
struct lat_label_control
{
    struct label_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */
        cmw_u16_t size; /* size of label */
        cmw_u8_t *p_text; /* label text */
        cmw_u8_t **pp_deposit_text; /* deposit to text */

        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct label_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_label_control *p_label );
        void ( *out_ctrl_cb ) ( struct lat_label_control *p_label );

        void ( *key_cb ) ( struct lat_label_control *p_label, cmw_u16_t key_value );
    } in_method;
    struct label_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *set_text ) ( struct lat_label_control *p_label, cmw_u8_t *p_text );
    } out_method;

    cmw_slist_t list;
};

typedef struct lat_label_control lat_label_control_t;

/**
 * @brief 数值输入控件 text box  class define
 *
 */
struct lat_number_control
{
    struct number_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index  of number */
        cmw_u16_t x; /* x position of number */
        cmw_u16_t y; /* y position of number */
        cmw_u16_t size; /* size of number */
        cmw_u8_t integer_digits;  /* integer of number */
        cmw_u8_t decimal_digits; /* decimal of number */
        cmw_s32_t value; /* value of number */
        cmw_s32_t *p_deposit_value; /* deposit to text */

        cmw_u16_t number_focus_position; /* focus of position in number */

        cmw_s32_t old_value;
        cmw_s32_t max_value;
        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        cmw_u8_t is_limit; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;

    } in_data;

    struct number_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_number_control *p_number );
        void ( *out_ctrl_cb ) ( struct lat_number_control *p_number );
        void ( *key_cb ) ( struct lat_number_control *p_number, cmw_u16_t key_value );
        void ( *value_change ) ( struct lat_number_control *p_number );
    } in_method;
    struct number_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *number_focus_left_shift ) ( struct lat_number_control *p_number );
        void ( *number_focus_right_shift ) ( struct lat_number_control *p_number );

        void ( *number_focus_position_set ) ( struct lat_number_control *p_number, cmw_u16_t number_focus_position );

        void ( *value_add ) ( struct lat_number_control *p_number );
        void ( *value_sub ) ( struct lat_number_control *p_number );

        cmw_u16_t ( *get_number_focus_positions ) ( struct lat_number_control *p_number );

    } out_method;
    cmw_slist_t list;
};

typedef struct lat_number_control lat_number_control_t;


// 检验栏 check box  class define
struct lat_checkbox_control
{
    struct checkbox_in_data
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */

        cmw_u8_t *p_text;
        cmw_u16_t size;
        cmw_u8_t is_select;
        cmw_u8_t *p_deposit_is_select;

        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct checkbox_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_checkbox_control *p_checkbox );
        void ( *out_ctrl_cb ) ( struct lat_checkbox_control *p_checkbox );
        void ( *key_cb ) ( struct lat_checkbox_control *p_checkbox, cmw_u16_t key_value );
    } in_method;


    cmw_slist_t list;
};

typedef struct lat_checkbox_control lat_checkbox_control_t;


// 下拉菜单组合框 combo box  class define
#define LAT_COMBO_DATA(p)  ((cmw_u8_t *)(p))
#define LAT_COMBO_DATA_LIST_MAX  100
struct lat_combo_control
{
    struct combo_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */

        cmw_u16_t width;
        cmw_u16_t height;
        cmw_u16_t box_width;
        cmw_u16_t x_text_offset; /* x position of text offset */
        cmw_u16_t y_text_offset; /* y position of text offset */

        cmw_u16_t size; /* size of label */
        cmw_u8_t *p_text[LAT_COMBO_DATA_LIST_MAX];
        cmw_u16_t current_index; /* current index */
        cmw_u16_t *p_deposit_current_index; /* current index */

        cmw_u16_t is_edit;
        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct combo_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_combo_control *p_combo );
        void ( *out_ctrl_cb ) ( struct lat_combo_control *p_combo );
        void ( *key_cb ) ( struct lat_combo_control *p_combo, cmw_u16_t key_value );
    } in_method;
    struct combo_out_method
    {
        void ( *combo_index_add ) ( struct lat_combo_control *p_combo );
        void ( *combo_index_sub ) ( struct lat_combo_control *p_combo );
    } out_method;

    cmw_slist_t list;
};

typedef struct lat_combo_control lat_combo_control_t;


// 文本列表框 list box  class define
struct lat_listbox_control
{
    struct listbox_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */
        cmw_u16_t width;
        cmw_u16_t height;
        cmw_u16_t size; /* size of label */
        cmw_u16_t max_show_line;    /* 每页最大显示行数 */
        cmw_u8_t **p_text_list;    /* 由后台传递 */
        cmw_u16_t first_line_index; /* 首行索引 */
        cmw_u16_t current_index; /* current index */
        cmw_u16_t *p_deposit_current_index; /* current index */
        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct listbox_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_listbox_control *p_listbox );
        void ( *out_ctrl_cb ) ( struct lat_listbox_control *p_listbox );
        void ( *key_cb ) ( struct lat_listbox_control *p_listbox, cmw_u16_t key_value );
    } in_method;
    struct listbox_out_method
    {
        void ( *listbox_index_add ) ( struct lat_listbox_control *p_listbox );
        void ( *listbox_index_sub ) ( struct lat_listbox_control *p_listbox );
    } out_method;

    cmw_slist_t list;
};

typedef struct lat_listbox_control lat_listbox_control_t;


// 进度条 progressbar
struct lat_progressbar_control
{
    struct progressbar_in_data
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */
        cmw_u16_t width;
        cmw_u16_t height;
        cmw_u8_t value;
        cmw_u8_t *p_deposit_value;
        cmw_u8_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct progressbar_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_progressbar_control *p_checkbox );
        void ( *out_ctrl_cb ) ( struct lat_progressbar_control *p_checkbox );
        void ( *key_cb ) ( struct lat_progressbar_control *p_progressbar, cmw_u16_t key_value );
    } in_method;

    cmw_slist_t list;
};

typedef struct lat_progressbar_control lat_progressbar_control_t;


// 标题栏控件 title
struct lat_title_control
{
    struct title_in_data           /* 应用层不可直接调用的数据 */
    {
        cmw_u16_t id; /* index  of label */
        cmw_u16_t x; /* x position of label */
        cmw_u16_t y; /* y position of label */
        cmw_u16_t width;
        cmw_u16_t height;
        cmw_u16_t size; /* size of label */
        cmw_u8_t *p_text;
        cmw_u16_t **p_deposit_text;
        cmw_u16_t is_focus; /* is_focus flag */
        cmw_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct title_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_title_control *p_title );
        void ( *out_ctrl_cb ) ( struct lat_title_control *p_title );
        void ( *key_cb ) ( struct lat_title_control *p_title, cmw_u16_t key_value );
    } in_method;
    //  struct title_out_method
    //  {
    //     void (*listbox_index_add)(struct lat_listbox_control *p_listbox);
    //       void (*listbox_index_sub)(struct lat_listbox_control *p_listbox);
    //   } out_method;

    cmw_slist_t list;
};

/**
 * @brief 选项卡页面
 *
 */
struct lat_tabcontrol_page
{
    struct tabcontrol_page_in_data
    {
        cmw_u16_t id; /* id */
        cmw_slist_t button_head; /* button control hook */
        cmw_slist_t picture_head; /* picture control hook */
        cmw_slist_t label_head; /* label control hook */
        cmw_slist_t number_head; /* number control hook */
        cmw_slist_t checkbox_head;
        cmw_slist_t progressbar_head;
        cmw_slist_t listbox_head;
        cmw_slist_t combo_head;  /* combo control hook */
        cmw_u8_t is_show; /* is_show flag */
    } in_data;
    cmw_slist_t list;
};

typedef struct lat_tabcontrol_page lat_tabcontrol_page_t;

/**
 * @brief 选项卡：组织一个或者多个控件组
 *
 *
 */
struct lat_tabcontrol
{
    struct tabcontrol_in_data
    {
        cmw_u16_t id; /* id */
        cmw_u16_t x; /* x position */
        cmw_u16_t y; /* y position */
        cmw_u16_t width; /* width */
        cmw_u16_t height; /* height */
        cmw_slist_t page_head;
    } in_data;
    cmw_slist_t list;
};

typedef struct lat_tabcontrol lat_tabcontrol_t;

/**
 * @brief 页面对象
 *
 */
struct lat_page
{
    struct page_in_data
    {
        cmw_u16_t page_id; /* page id */
        cmw_u16_t focus_control_id; /* focus id of control*/
        cmw_u16_t first_control_id; /* first id of control*/
        cmw_u16_t last_focus_control_id; /* 最后一次聚焦的ID */
        cmw_slist_t button_head; /* button control hook */
        cmw_slist_t picture_head; /* picture control hook */
        cmw_slist_t label_head; /* label control hook */
        cmw_slist_t number_head; /* number control hook */
        cmw_slist_t checkbox_head;
        cmw_slist_t progressbar_head;
        cmw_slist_t listbox_head;
        cmw_slist_t combo_head;  /* combo control hook */
        cmw_slist_t tabcontrol_head; /* group control hook */
    } in_data;
    struct page_in_method
    {
        void ( *loop_exec ) ( struct lat_page *p_page );
        void ( *in_page_cb ) ( struct lat_page *p_page );
        void ( *out_page_cb ) ( struct lat_page *p_page );
    } in_method;
    cmw_slist_t list;
};

typedef struct lat_page lat_page_t;


/**
 * @brief  窗口对象
 *
 */
struct lat_window
{
    struct window_in_data
    {
        cmw_slist_t page_head; /* button_head */
        lat_page_t *p_focus_page; /* focus page */
        cmw_u16_t last_focus_page_id; /* last_focus_page_id */
        cmw_u16_t key_value; /* u16_key_value */
        cmw_u16_t last_key_value; /* u16_key_value */
    } in_data;

    struct window_in_method
    {
        void ( *clear_display_cb ) ( void );
        void ( *clear_part_display_cb ) ( cmw_u16_t x, cmw_u16_t y, cmw_u16_t width, cmw_u16_t high );
        void ( *draw_pixel_cb ) ( cmw_u16_t x, cmw_u16_t y, cmw_u8_t is_solid );
        void ( *draw_str_cb ) ( cmw_u16_t x, cmw_u16_t y, uint8_t *p_str );
        void ( *draw_chinese_cb ) ( cmw_u16_t x, cmw_u16_t y, uint8_t *p_chinese, cmw_u8_t size, cmw_u8_t is_focus );
        void ( *draw_picture_cb ) ( cmw_u16_t x, cmw_u16_t y, cmw_u16_t width, cmw_u16_t high, cmw_u16_t mode, cmw_u8_t *p_picture );
        void ( *draw_char_cb ) ( cmw_u16_t x, cmw_u16_t y, cmw_u8_t u8_char, cmw_u16_t size, cmw_u16_t mode );
        cmw_u16_t ( *key_value_recv_cb ) ( struct lat_window *p_window );
    } in_method;

    struct window_out_method
    {
        cmw_bool_t ( *window_refresh ) ( struct lat_window *p_window );
        lat_page_t * ( *page_create ) ( struct lat_window *p_window, cmw_u16_t page_id, cmw_u16_t first_control_id );
        cmw_bool_t ( *add_button_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_button_control_t *p_button, cmw_u16_t button_num );
        cmw_bool_t ( *add_label_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_label_control_t *p_label, cmw_u16_t label_num );
        cmw_bool_t ( *add_picture_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_picture_control_t *p_picture, cmw_u16_t picture_num );
        cmw_bool_t ( *add_number_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_number_control_t *p_number, cmw_u16_t number_num );
        cmw_bool_t ( *add_combo_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_combo_control_t *p_combo, cmw_u16_t combo_num );
        cmw_bool_t ( *add_checkbox_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_checkbox_control_t *p_checkbox, cmw_u16_t checkbox_num );
        cmw_bool_t ( *add_progressbar_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_progressbar_control_t *p_progressbar, cmw_u16_t progressbar_num );
        cmw_bool_t ( *add_listbox_control ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_listbox_control_t *p_listbox, cmw_u16_t listbox_num );
        cmw_bool_t ( *add_tabcontrol_container ) ( struct lat_window *p_window, cmw_u16_t page_id, lat_tabcontrol_t *p_tabcontrol, cmw_u16_t tabcontrol_num );
        cmw_bool_t ( *add_tabcontrol_page ) ( struct lat_window *p_window, cmw_u16_t page_id, cmw_u16_t tabcontrol_id, lat_tabcontrol_page_t *p_tabcontrol_page, cmw_u16_t tabcontrol_page_num );
        cmw_bool_t ( *add_button_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_button_control_t *p_button, cmw_u16_t button_num );
        cmw_bool_t ( *add_label_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_label_control_t *p_label, cmw_u16_t label_num );
        cmw_bool_t ( *add_picture_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_picture_control_t *p_picture, cmw_u16_t picture_num );
        cmw_bool_t ( *add_number_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_number_control_t *p_number, cmw_u16_t number_num );
        cmw_bool_t ( *add_combo_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_combo_control_t *p_combo, cmw_u16_t combo_num );
        cmw_bool_t ( *add_checkbox_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_checkbox_control_t *p_checkbox, cmw_u16_t checkbox_num );
        cmw_bool_t ( *add_progressbar_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_progressbar_control_t *p_progressbar, cmw_u16_t progressbar_num );
        cmw_bool_t ( *add_listbox_tabcontrolpage ) ( struct lat_window *p_window, cmw_u16_t page_id,
                cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id, lat_listbox_control_t *p_listbox, cmw_u16_t listbox_num );

        void ( *key_value_send_cb ) ( struct lat_window *p_window, cmw_u16_t key_value );
        // 切换 聚焦的控件
        cmw_bool_t ( *control_focus_shift ) ( struct lat_window *p_window, cmw_u16_t control_id );
        // 切换 聚焦的页面
        cmw_bool_t ( *page_focus_shift ) ( struct lat_window *p_window, cmw_u16_t page_id );
        // 获取指定页面中指定的控件ID的控件句柄
        void * ( *get_control_handle ) ( struct lat_window *p_window, cmw_u16_t page_id, cmw_u16_t control_id );
        // 显示指定ID页面的指定ID选项卡中指定ID的选项卡页面
        cmw_bool_t ( *show_tabcontrol_page ) ( struct lat_window  *p_window, cmw_u16_t page_id, cmw_u16_t tabcontrol_id, cmw_u16_t tabcontrolpage_id );
        // 为指定页面添加循环调用函数
        cmw_bool_t (*page_add_loop_cb) ( struct lat_window *p_window,cmw_u16_t page_id,void ( *loop_exec ) ( struct lat_page *p_page ) );
        // 为指定页面添加进入调用函数
        cmw_bool_t (*page_add_in_cb) ( struct lat_window *p_window,cmw_u16_t page_id,void ( *in_exec ) ( struct lat_page *p_page ) );
        // 为指定页面添加退出调用函数
        cmw_bool_t (*page_add_out_cb) ( struct lat_window *p_window,cmw_u16_t page_id,void ( *out_exec ) ( struct lat_page *p_page ) );
    } out_method;
};

typedef struct lat_window lat_window_t;
/**
 * @brief lattice port and key port
 *
 */
typedef struct window_in_method lat_config_t;

/**
 * @brief littice window initializing and config
 *
 * @param p_window
 * @param p_lat_config
 * @return cmw_bool_t
 */
lat_window_t *cmw_lattice_window_create ( lat_config_t *p_lat_config );



#ifdef __cplusplus
}
#endif

#endif /* CMW_USING_LAT */

#endif /* __CMW_LAT_H__ */




