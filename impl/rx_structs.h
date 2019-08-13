#ifndef _rx_structs_H_
#define _rx_structs_H_

#include "rx/rx.h"
#include "utils/list.h"

//  Accel
struct rx_accel_impl {
    struct list_head nibrs;
    uintptr_t accel;
};
RX_STATIC_ASSERT(sizeof(struct rx_accel_impl) == sizeof(struct rx_accel), "确保内部实现和接口一致");

//  App
struct rx_app_impl {
    uint16_t size;
    uint16_t mode;
    uint32_t noused;
    uintptr_t inst; //  进程实例
    struct rx_accel_impl accels; //  加速键
    struct rx_panel* entry; //  主窗口
};
RX_STATIC_ASSERT(sizeof(struct rx_app_impl) == sizeof(struct rx_app), "确保内部实现和接口一致");

//  Elem
struct rx_elem_impl {
    struct list_head        nibrs;
    struct rx_elem_impl*    parent;
    struct list_head        childs;
    int                     childscount;
    struct rx_rect          rect;
    struct rx_ctrl_impl*    ctrl;
};
RX_STATIC_ASSERT(sizeof(struct rx_elem_impl) == sizeof(struct rx_elem), "确保内部实现和接口一致");

//  Ctrl
struct rx_ctrl_impl {
    struct rx_elem_impl* elem;
    RX_PAINT paint;
};
RX_STATIC_ASSERT(sizeof(struct rx_ctrl_impl) == sizeof(struct rx_ctrl), "确保内部实现和接口一致");

//  Layout
struct rx_layout_impl {
    RX_RELAYOUT relayout;
};
RX_STATIC_ASSERT(sizeof(struct rx_layout_impl) == sizeof(struct rx_layout), "确保内部实现和接口一致");

//  Cntr
struct rx_cntr_impl {
    struct rx_ctrl_impl super;
    struct rx_layout* layout;
};
RX_STATIC_ASSERT(sizeof(struct rx_cntr_impl) == sizeof(struct rx_cntr), "确保内部实现和接口一致");

//  Panel
#define RX_PANEL_CLASS_NAME (_T("rx.panel"))
struct rx_panel_impl {
    uintptr_t fd;
    RX_EVENT event;
    struct rx_cntr_impl* cntr;
};
RX_STATIC_ASSERT(sizeof(struct rx_panel_impl) == sizeof(struct rx_panel), "确保内部实现和接口一致");

//  Tree
struct rx_tree_impl {
    struct rx_ctrl_impl ctrl;
    uintptr_t fd;
};
RX_STATIC_ASSERT(sizeof(struct rx_tree_impl) == sizeof(struct rx_tree), "确保内部实现和接口一致");

#endif //_rx_structs_H_
