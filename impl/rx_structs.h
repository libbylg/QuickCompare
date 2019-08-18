#ifndef _rx_structs_H_
#define _rx_structs_H_

#include "rx/rx.h"
#include "utils/array.h"
#include "utils/list.h"
#include "utils/map.h"

//  Object
struct rx_object_impl {
    uint32_t _type;
    uint16_t _size;
    uint16_t _noused1;
    void* _class;
    uint8_t _noused2[4];
};
RX_STATIC_ASSERT(sizeof(struct rx_object_impl) == sizeof(struct rx_object), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Accel
struct rx_accel_impl {
    struct rx_object super;
    struct list_head nibrs;
    uintptr_t accel;
};
RX_STATIC_ASSERT(sizeof(struct rx_accel_impl) == sizeof(struct rx_accel), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  App
struct rx_app_impl {
    struct rx_object super;
    uint16_t size;
    uint16_t mode;
    uint32_t noused;
    uintptr_t inst; //  ����ʵ��
    struct rx_accel_impl accels; //  ���ټ�
    struct rx_panel* entry; //  ������
};
RX_STATIC_ASSERT(sizeof(struct rx_app_impl) == sizeof(struct rx_app), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Elem
struct rx_elem_impl {
    struct rx_object super;
    struct rx_elem_impl* parent;
    struct list_head nibrs;
    struct list_head childs;
    int childscount;
    struct rx_rect rect;
    struct rx_ctrl_impl* ctrl;
};
RX_STATIC_ASSERT(sizeof(struct rx_elem_impl) == sizeof(struct rx_elem), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Ctrl
struct rx_ctrl_impl {
    struct rx_object super;
    struct rx_elem_impl* elem;
    RX_PAINT paint;
    RX_NOTIFY notify;
};
RX_STATIC_ASSERT(sizeof(struct rx_ctrl_impl) == sizeof(struct rx_ctrl), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Layout
struct rx_layout_impl {
    struct rx_object super;
    RX_RELAYOUT relayout;
};
RX_STATIC_ASSERT(sizeof(struct rx_layout_impl) == sizeof(struct rx_layout), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Cntr
struct rx_cntr_impl {
    struct rx_ctrl_impl super;
    struct rx_layout* layout;
};
RX_STATIC_ASSERT(sizeof(struct rx_cntr_impl) == sizeof(struct rx_cntr), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Panel
#define RX_PANEL_CLASS_NAME (_T("rx.panel"))
struct rx_panel_impl {
    struct rx_object super;
    uintptr_t fd;
    RX_EVENT event;
    struct rx_cntr_impl* cntr;
};
RX_STATIC_ASSERT(sizeof(struct rx_panel_impl) == sizeof(struct rx_panel), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Tree
struct rx_tree_impl {
    struct rx_ctrl_impl super;
    uintptr_t fd;
};
RX_STATIC_ASSERT(sizeof(struct rx_tree_impl) == sizeof(struct rx_tree), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

struct rx_str {
    uint32_t hash;
    uint32_t size;
    wchar_t* name;
};

struct rx_value_impl {
    uintptr_t type;
    union {
        int64_t vint64;
        uint64_t vuint64;
        int32_t vint32;
        uint32_t vuint32;
        int16_t vint16;
        uint16_t vuint16;
        int8_t vint8;
        uint8_t vuint8;
        boot_t vbool;
        uintptr_t vany;
        rx_metric vmetric;
        struct array* varray;
        struct map* vmap;
        struct rx_value* ref;
    };
};
RX_STATIC_ASSERT(sizeof(struct rx_value_impl) == sizeof(struct rx_value), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

struct rx_attr {
    uint8_t unknown[36];
};

struct rx_attr_impl {
    struct list_head head;
    struct rx_str name;
    struct rx_value value;
};
RX_STATIC_ASSERT(sizeof(struct rx_attr_impl) == sizeof(struct rx_attr), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

//  Gridlayout
struct rx_gridlayout_impl {
    struct rx_layout_impl super;
    struct array grid_template_columns;
    struct array grid_template_rows;
    rx_metric grid_column_gap;
    rx_metric grid_row_gap;
    int grid_auto_flow;
    int justify_items;
    int align_items;
    int justify_content;
    int align_content;
};
RX_STATIC_ASSERT(sizeof(struct rx_gridlayout_impl) == sizeof(struct rx_gridlayout), "ȷ���ڲ�ʵ�ֺͽӿ�һ��");

#endif //_rx_structs_H_
