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


#ifndef __MDL_LAT_H__
#define __MDL_LAT_H__

#include "mdl_def.h"

#if defined(MDL_USING_LAT)

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
    mdl_u16_t up;
    mdl_u16_t down;
    mdl_u16_t left;
    mdl_u16_t right;
    mdl_u16_t enter;
    mdl_u16_t esc;
} lat_page_shift_t;

// 切换聚焦方向定义
typedef struct
{
    mdl_u16_t up;
    mdl_u16_t down;
    mdl_u16_t left;
    mdl_u16_t right;
    mdl_u16_t enter;
    mdl_u16_t esc;
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
        mdl_u16_t id; /* index of button */
        mdl_u16_t x; /* x position of button */
        mdl_u16_t y; /* y position of button */
        mdl_u16_t width; /* width of button */
        mdl_u16_t height; /* height of button */
        mdl_u16_t radius; /* radius of button */
        mdl_u16_t x_text_offset; /* x position of text offset */
        mdl_u16_t y_text_offset; /* y position of text offset */
        mdl_u16_t text_size; /* size of text */
        mdl_u8_t *p_text; /* pointer to text */
        mdl_u8_t **pp_deposit_text; /* deposit to text */

        mdl_u8_t is_focus; /* flag indicating if focus should be focused */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct button_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_button_control *p_button );
        void ( *out_ctrl_cb ) ( struct lat_button_control *p_button );
        void ( *key_cb ) ( struct lat_button_control *p_button, mdl_u16_t key_value );
    } in_method;
    struct button_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *set_text ) ( struct lat_button_control *p_button, mdl_u8_t *p_text );
    } out_method;
    mdl_slist_t list;
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
        mdl_u16_t id; /* index of button */
        mdl_u16_t x; /* x position of picture */
        mdl_u16_t y; /* y position of picture */
        mdl_u16_t width; /* width of picture */
        mdl_u16_t height; /* height of picture */
        mdl_u8_t *p_picture; /* pointer to picture */
        mdl_u8_t **pp_deposit_picture; /* deposit to text */

        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct picture_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_picture_control *p_picture );
        void ( *out_ctrl_cb ) ( struct lat_picture_control *p_picture );
        void ( *key_cb ) ( struct lat_picture_control *p_picture, mdl_u16_t key_value );
    } in_method;
    //  struct picture_out_method        /* 应用层可直接调用的方法 */
    //    {
    //        void (*config)(struct lat_picture_control p_picture, mdl_u16_t x, mdl_u16_t y, mdl_u16_t width, mdl_u16_t height, uint32_t * p_picture_addr);
    //        void (*change_picture)(struct lat_picture_control p_picture, uint32_t * p_picture_addr);
    //  } out_method;
    mdl_slist_t list;
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
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */
        mdl_u16_t size; /* size of label */
        mdl_u8_t *p_text; /* label text */
        mdl_u8_t **pp_deposit_text; /* deposit to text */

        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct label_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_label_control *p_label );
        void ( *out_ctrl_cb ) ( struct lat_label_control *p_label );

        void ( *key_cb ) ( struct lat_label_control *p_label, mdl_u16_t key_value );
    } in_method;
    struct label_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *set_text ) ( struct lat_label_control *p_label, mdl_u8_t *p_text );
    } out_method;

    mdl_slist_t list;
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
        mdl_u16_t id; /* index  of number */
        mdl_u16_t x; /* x position of number */
        mdl_u16_t y; /* y position of number */
        mdl_u16_t size; /* size of number */
        mdl_u8_t integer_digits;  /* integer of number */
        mdl_u8_t decimal_digits; /* decimal of number */
        mdl_s32_t value; /* value of number */
        mdl_s32_t *p_deposit_value; /* deposit to text */

        mdl_u16_t number_focus_position; /* focus of position in number */

        mdl_s32_t old_value;
        mdl_s32_t max_value;
        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        mdl_u8_t is_limit; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;

    } in_data;

    struct number_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_number_control *p_number );
        void ( *out_ctrl_cb ) ( struct lat_number_control *p_number );
        void ( *key_cb ) ( struct lat_number_control *p_number, mdl_u16_t key_value );
        void ( *value_change ) ( struct lat_number_control *p_number );
    } in_method;
    struct number_out_method        /* 应用层可直接调用的方法 */
    {
        void ( *number_focus_left_shift ) ( struct lat_number_control *p_number );
        void ( *number_focus_right_shift ) ( struct lat_number_control *p_number );

        void ( *number_focus_position_set ) ( struct lat_number_control *p_number, mdl_u16_t number_focus_position );

        void ( *value_add ) ( struct lat_number_control *p_number );
        void ( *value_sub ) ( struct lat_number_control *p_number );

        mdl_u16_t ( *get_number_focus_positions ) ( struct lat_number_control *p_number );

    } out_method;
    mdl_slist_t list;
};

typedef struct lat_number_control lat_number_control_t;


// 检验栏 check box  class define
struct lat_checkbox_control
{
    struct checkbox_in_data
    {
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */

        mdl_u8_t *p_text;
        mdl_u16_t size;
        mdl_u8_t is_select;
        mdl_u8_t *p_deposit_is_select;

        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct checkbox_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_checkbox_control *p_checkbox );
        void ( *out_ctrl_cb ) ( struct lat_checkbox_control *p_checkbox );
        void ( *key_cb ) ( struct lat_checkbox_control *p_checkbox, mdl_u16_t key_value );
    } in_method;


    mdl_slist_t list;
};

typedef struct lat_checkbox_control lat_checkbox_control_t;


// 下拉菜单组合框 combo box  class define
#define LAT_COMBO_DATA(p)  ((mdl_u8_t *)(p))
#define LAT_COMBO_DATA_LIST_MAX  100
struct lat_combo_control
{
    struct combo_in_data           /* 应用层不可直接调用的数据 */
    {
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */

        mdl_u16_t width;
        mdl_u16_t height;
        mdl_u16_t box_width;
        mdl_u16_t x_text_offset; /* x position of text offset */
        mdl_u16_t y_text_offset; /* y position of text offset */

        mdl_u16_t size; /* size of label */
        mdl_u8_t *p_text[LAT_COMBO_DATA_LIST_MAX];
        mdl_u16_t current_index; /* current index */
        mdl_u16_t *p_deposit_current_index; /* current index */

        mdl_u16_t is_edit;
        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct combo_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_combo_control *p_combo );
        void ( *out_ctrl_cb ) ( struct lat_combo_control *p_combo );
        void ( *key_cb ) ( struct lat_combo_control *p_combo, mdl_u16_t key_value );
    } in_method;
    struct combo_out_method
    {
        void ( *combo_index_add ) ( struct lat_combo_control *p_combo );
        void ( *combo_index_sub ) ( struct lat_combo_control *p_combo );
    } out_method;

    mdl_slist_t list;
};

typedef struct lat_combo_control lat_combo_control_t;


// 文本列表框 list box  class define
struct lat_listbox_control
{
    struct listbox_in_data           /* 应用层不可直接调用的数据 */
    {
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */
        mdl_u16_t width;
        mdl_u16_t height;
        mdl_u16_t size; /* size of label */
        mdl_u16_t max_show_line;    /* 每页最大显示行数 */
        mdl_u8_t **p_text_list;    /* 由后台传递 */
        mdl_u16_t first_line_index; /* 首行索引 */
        mdl_u16_t current_index; /* current index */
        mdl_u16_t *p_deposit_current_index; /* current index */
        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct listbox_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_listbox_control *p_listbox );
        void ( *out_ctrl_cb ) ( struct lat_listbox_control *p_listbox );
        void ( *key_cb ) ( struct lat_listbox_control *p_listbox, mdl_u16_t key_value );
    } in_method;
    struct listbox_out_method
    {
        void ( *listbox_index_add ) ( struct lat_listbox_control *p_listbox );
        void ( *listbox_index_sub ) ( struct lat_listbox_control *p_listbox );
    } out_method;

    mdl_slist_t list;
};

typedef struct lat_listbox_control lat_listbox_control_t;


// 进度条 progressbar
struct lat_progressbar_control
{
    struct progressbar_in_data
    {
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */
        mdl_u16_t width;
        mdl_u16_t height;
        mdl_u8_t value;
        mdl_u8_t *p_deposit_value;
        mdl_u8_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;
    struct progressbar_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_progressbar_control *p_checkbox );
        void ( *out_ctrl_cb ) ( struct lat_progressbar_control *p_checkbox );
        void ( *key_cb ) ( struct lat_progressbar_control *p_progressbar, mdl_u16_t key_value );
    } in_method;

    mdl_slist_t list;
};

typedef struct lat_progressbar_control lat_progressbar_control_t;


// 标题栏控件 title
struct lat_title_control
{
    struct title_in_data           /* 应用层不可直接调用的数据 */
    {
        mdl_u16_t id; /* index  of label */
        mdl_u16_t x; /* x position of label */
        mdl_u16_t y; /* y position of label */
        mdl_u16_t width;
        mdl_u16_t height;
        mdl_u16_t size; /* size of label */
        mdl_u8_t *p_text;
        mdl_u16_t **p_deposit_text;
        mdl_u16_t is_focus; /* is_focus flag */
        mdl_u8_t is_show; /* is_show flag */
        lat_control_focus_shift_t focus_shift;
        lat_page_shift_t page_shift;
        lat_key_event_t key_event;
    } in_data;

    struct title_in_method         /* 应用层不可直接调用的方法 */
    {
        void ( *in_ctrl_cb ) ( struct lat_title_control *p_title );
        void ( *out_ctrl_cb ) ( struct lat_title_control *p_title );
        void ( *key_cb ) ( struct lat_title_control *p_title, mdl_u16_t key_value );
    } in_method;
    //  struct title_out_method
    //  {
    //     void (*listbox_index_add)(struct lat_listbox_control *p_listbox);
    //       void (*listbox_index_sub)(struct lat_listbox_control *p_listbox);
    //   } out_method;

    mdl_slist_t list;
};

/**
 * @brief 选项卡页面
 *
 */
struct lat_tabcontrol_page
{
    struct tabcontrol_page_in_data
    {
        mdl_u16_t id; /* id */
        mdl_slist_t button_head; /* button control hook */
        mdl_slist_t picture_head; /* picture control hook */
        mdl_slist_t label_head; /* label control hook */
        mdl_slist_t number_head; /* number control hook */
        mdl_slist_t checkbox_head;
        mdl_slist_t progressbar_head;
        mdl_slist_t listbox_head;
        mdl_slist_t combo_head;  /* combo control hook */
        mdl_u8_t is_show; /* is_show flag */
    } in_data;
    mdl_slist_t list;
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
        mdl_u16_t id; /* id */
        mdl_u16_t x; /* x position */
        mdl_u16_t y; /* y position */
        mdl_u16_t width; /* width */
        mdl_u16_t height; /* height */
        mdl_slist_t page_head;
    } in_data;
    mdl_slist_t list;
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
        mdl_u16_t page_id; /* page id */
        mdl_u16_t focus_control_id; /* focus id of control*/
        mdl_u16_t first_control_id; /* first id of control*/
        mdl_u16_t last_focus_control_id; /* 最后一次聚焦的ID */
        mdl_slist_t button_head; /* button control hook */
        mdl_slist_t picture_head; /* picture control hook */
        mdl_slist_t label_head; /* label control hook */
        mdl_slist_t number_head; /* number control hook */
        mdl_slist_t checkbox_head;
        mdl_slist_t progressbar_head;
        mdl_slist_t listbox_head;
        mdl_slist_t combo_head;  /* combo control hook */
        mdl_slist_t tabcontrol_head; /* group control hook */
    } in_data;
    struct page_in_method
    {
        void ( *loop_exec ) ( struct lat_page *p_page );
        void ( *in_page_cb ) ( struct lat_page *p_page );
        void ( *out_page_cb ) ( struct lat_page *p_page );
    } in_method;
    mdl_slist_t list;
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
        mdl_slist_t page_head; /* button_head */
        lat_page_t *p_focus_page; /* focus page */
        mdl_u16_t last_focus_page_id; /* last_focus_page_id */
        mdl_u16_t key_value; /* u16_key_value */
        mdl_u16_t last_key_value; /* u16_key_value */
    } in_data;

    struct window_in_method
    {
        void ( *clear_display_cb ) ( void );
        void ( *clear_part_display_cb ) ( mdl_u16_t x, mdl_u16_t y, mdl_u16_t width, mdl_u16_t high );
        void ( *draw_pixel_cb ) ( mdl_u16_t x, mdl_u16_t y, mdl_u8_t is_solid );
        void ( *draw_str_cb ) ( mdl_u16_t x, mdl_u16_t y, uint8_t *p_str );
        void ( *draw_chinese_cb ) ( mdl_u16_t x, mdl_u16_t y, uint8_t *p_chinese, mdl_u8_t size, mdl_u8_t is_focus );
        void ( *draw_picture_cb ) ( mdl_u16_t x, mdl_u16_t y, mdl_u16_t width, mdl_u16_t high, mdl_u16_t mode, mdl_u8_t *p_picture );
        void ( *draw_char_cb ) ( mdl_u16_t x, mdl_u16_t y, mdl_u8_t u8_char, mdl_u16_t size, mdl_u16_t mode );
        mdl_u16_t ( *key_value_recv_cb ) ( struct lat_window *p_window );
    } in_method;

    struct window_out_method
    {
        mdl_bool_t ( *window_refresh ) ( struct lat_window *p_window );
        lat_page_t * ( *page_create ) ( struct lat_window *p_window, mdl_u16_t page_id, mdl_u16_t first_control_id );
        mdl_bool_t ( *add_button_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_button_control_t *p_button, mdl_u16_t button_num );
        mdl_bool_t ( *add_label_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_label_control_t *p_label, mdl_u16_t label_num );
        mdl_bool_t ( *add_picture_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_picture_control_t *p_picture, mdl_u16_t picture_num );
        mdl_bool_t ( *add_number_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_number_control_t *p_number, mdl_u16_t number_num );
        mdl_bool_t ( *add_combo_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_combo_control_t *p_combo, mdl_u16_t combo_num );
        mdl_bool_t ( *add_checkbox_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_checkbox_control_t *p_checkbox, mdl_u16_t checkbox_num );
        mdl_bool_t ( *add_progressbar_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_progressbar_control_t *p_progressbar, mdl_u16_t progressbar_num );
        mdl_bool_t ( *add_listbox_control ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_listbox_control_t *p_listbox, mdl_u16_t listbox_num );
        mdl_bool_t ( *add_tabcontrol_container ) ( struct lat_window *p_window, mdl_u16_t page_id, lat_tabcontrol_t *p_tabcontrol, mdl_u16_t tabcontrol_num );
        mdl_bool_t ( *add_tabcontrol_page ) ( struct lat_window *p_window, mdl_u16_t page_id, mdl_u16_t tabcontrol_id, lat_tabcontrol_page_t *p_tabcontrol_page, mdl_u16_t tabcontrol_page_num );
        mdl_bool_t ( *add_button_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_button_control_t *p_button, mdl_u16_t button_num );
        mdl_bool_t ( *add_label_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_label_control_t *p_label, mdl_u16_t label_num );
        mdl_bool_t ( *add_picture_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_picture_control_t *p_picture, mdl_u16_t picture_num );
        mdl_bool_t ( *add_number_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_number_control_t *p_number, mdl_u16_t number_num );
        mdl_bool_t ( *add_combo_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_combo_control_t *p_combo, mdl_u16_t combo_num );
        mdl_bool_t ( *add_checkbox_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_checkbox_control_t *p_checkbox, mdl_u16_t checkbox_num );
        mdl_bool_t ( *add_progressbar_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_progressbar_control_t *p_progressbar, mdl_u16_t progressbar_num );
        mdl_bool_t ( *add_listbox_tabcontrolpage ) ( struct lat_window *p_window, mdl_u16_t page_id,
                mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id, lat_listbox_control_t *p_listbox, mdl_u16_t listbox_num );

        void ( *key_value_send_cb ) ( struct lat_window *p_window, mdl_u16_t key_value );
        // 切换 聚焦的控件
        mdl_bool_t ( *control_focus_shift ) ( struct lat_window *p_window, mdl_u16_t control_id );
        // 切换 聚焦的页面
        mdl_bool_t ( *page_focus_shift ) ( struct lat_window *p_window, mdl_u16_t page_id );
        // 获取指定页面中指定的控件ID的控件句柄
        void * ( *get_control_handle ) ( struct lat_window *p_window, mdl_u16_t page_id, mdl_u16_t control_id );
        // 显示指定ID页面的指定ID选项卡中指定ID的选项卡页面
        mdl_bool_t ( *show_tabcontrol_page ) ( struct lat_window  *p_window, mdl_u16_t page_id, mdl_u16_t tabcontrol_id, mdl_u16_t tabcontrolpage_id );
        // 为指定页面添加循环调用函数
        mdl_bool_t (*page_add_loop_cb) ( struct lat_window *p_window,mdl_u16_t page_id,void ( *loop_exec ) ( struct lat_page *p_page ) );
        // 为指定页面添加进入调用函数
        mdl_bool_t (*page_add_in_cb) ( struct lat_window *p_window,mdl_u16_t page_id,void ( *in_exec ) ( struct lat_page *p_page ) );
        // 为指定页面添加退出调用函数
        mdl_bool_t (*page_add_out_cb) ( struct lat_window *p_window,mdl_u16_t page_id,void ( *out_exec ) ( struct lat_page *p_page ) );
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
 * @return mdl_bool_t
 */
lat_window_t *mdl_lattice_window_create ( lat_config_t *p_lat_config );



#ifdef __cplusplus
}
#endif

#endif /* MDL_USING_LAT */

#endif /* __MDL_LAT_H__ */




