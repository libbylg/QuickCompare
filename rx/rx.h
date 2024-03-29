#ifndef __rx_H_
#define __rx_H_

#include "rx/rx_types.h"

//  用于定义接口的宏
#define RX_DEFINE(name, pname, size) \
    name { uint8_t unknown[sizeof(pname) + size]; }

struct rx_rect {
    int left;
    int top;
    int right;
    int bottom;
};
RX_STATIC_ASSERT(sizeof(int) == sizeof(long), "int 和 long 的长度一致");

struct rx_notify {
    struct rx_ctrl* target;
    struct rx_ctrl* source;
    uintptr_t name;
    uintptr_t param1;
    uintptr_t param2;
    uintptr_t result;
};

#define RX_UNIT_AUTO    (0x00010000)     //  自动，无类型
#define RX_UNIT_PX      (0x00020001)     //  px，整数
#define RX_UNIT_FR      (0x00030001)     //  fr，整数
#define RX_UNIT_PERCENT (0x00040002)     //  百分比，浮点数
typedef uint64_t rx_metric;
#define rx_metric_of(t, v)  ((((uint64_t)(t)) << 32) | ((uint32_t)(v)))
#define rx_metric_type(m)   ((uint32_t)((((uint64_t)(m)) >> 32) & 0x00000000FFFFFFFFULL))
#define rx_metric_int32(m)  ((int32_t)(((uint64_t)(m)) & 0x00000000FFFFFFFFULL))
#define rx_metric_uint32(m) ((uint32_t)(((uint64_t)(m)) & 0x00000000FFFFFFFFULL))
#define rx_metric_float(m)  ((float)(((uint64_t)(m)) & 0x00000000FFFFFFFFULL))


struct rx_value {
    uint8_t unknown[16];
};

struct rx_app;
struct rx_elem;

struct rx_object {
    uint8_t unknown[16];
};
RX_API(int, rx_object_init(struct rx_object* obj));

RX_DEFINE(struct rx_app, struct rx_object, 44);
RX_API(int,             rx_app_init(struct rx_app* app, uint32_t version, uintptr_t inst));
RX_API(uintptr_t,       rx_app_inst(struct rx_app* app));
RX_API(int,             rx_app_entry_bind(struct rx_app* app, struct rx_panel* panel, int cmd));
RX_API(int,             rx_app_accels_push(struct rx_app* app, struct rx_accel* accel));
RX_API(int,             rx_app_run(struct rx_app* app));

RX_DEFINE(struct rx_elem, struct rx_object, 44);
RX_API(int,             rx_elem_init(struct rx_elem* elem, struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_elem_ctrl(struct rx_elem* elem));
RX_API(struct rx_ctrl*, rx_elem_ctrl_set(struct rx_elem* elem, struct rx_ctrl* ctrl));
RX_API(struct rx_elem*, rx_elem_nextnibr(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_prevnibr(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_parent(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_firstchild(struct rx_elem* elem));
RX_API(int,             rx_elem_childscount(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_lastchild(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_root(struct rx_elem* elem));
//
RX_API(void,            rx_elem_nibr_addbefore(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_nibr_addbehind(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_child_addtail(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_child_addhead(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(struct rx_elem*, rx_elem_split(struct rx_elem* elem));

RX_DEFINE(struct rx_ctrl, struct rx_object, 12);
typedef uintptr_t(RX_CALL* RX_PAINT)(struct rx_ctrl* ctrl, uintptr_t canvas, uintptr_t context);
typedef int(RX_CALL* RX_NOTIFY)(struct rx_ctrl* ctrl, struct rx_notify* notify);
RX_API(int,             rx_ctrl_init(struct rx_ctrl* ctrl, RX_PAINT paint, RX_NOTIFY notify));
RX_API(struct rx_ctrl*, rx_ctrl_nextnibr(struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_ctrl_prevnibr(struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_ctrl_parent(struct rx_ctrl* ctrl));
RX_API(void,            rx_ctrl_nibr_addbefore(struct rx_ctrl* ctrl, struct rx_ctrl* newitem));
RX_API(void,            rx_ctrl_nibr_addbehind(struct rx_ctrl* ctrl, struct rx_ctrl* newitem));
RX_API(struct rx_elem*, rx_ctrl_elem(struct rx_ctrl* ctrl));
RX_API(int,             rx_ctrl_pos(struct rx_ctrl* ctrl, struct rx_rect* rect));

RX_DEFINE(struct rx_cntr, struct rx_ctrl, 4);
RX_API(int,                 rx_cntr_init(struct rx_cntr* cntr, struct rx_panel* panel));
RX_API(struct rx_ctrl*,     rx_cntr_firstchild(struct rx_cntr* cntr));
RX_API(int,                 rx_cntr_childscount(struct rx_cntr* cntr));
RX_API(struct rx_ctrl*,     rx_cntr_lastchild(struct rx_cntr* cntr));
RX_API(void,                rx_cntr_child_addtail(struct rx_cntr* cntr, struct rx_ctrl* newitem));
RX_API(void,                rx_cntr_child_addhead(struct rx_cntr* cntr, struct rx_ctrl* newitem));
RX_API(struct rx_layout*,   rx_cntr_layout(struct rx_cntr* cntr));
RX_API(struct rx_layout*,   rx_cntr_layout_set(struct rx_cntr* cntr, struct rx_layout* layout));

RX_DEFINE(struct rx_layout, struct rx_object, 4);
typedef uintptr_t(RX_CALL* RX_RELAYOUT)(struct rx_layout* layout, struct rx_cntr* cntr, uintptr_t context);
RX_API(int,     rx_layout_init(struct rx_layout* layout, RX_RELAYOUT relayout));

RX_DEFINE(struct rx_panel, struct rx_object, 12);
typedef uintptr_t(RX_CALL* RX_EVENT)(void* target, uint32_t message, uintptr_t wParam, uintptr_t lParam);
RX_API(int,             rx_panel_init(struct rx_panel* p, uintptr_t inst, RX_EVENT event, const wchar_t* title));
RX_API(uintptr_t,       rx_panel_fd(struct rx_panel* p));
RX_API(struct rx_cntr*, rx_panel_cntr(struct rx_panel* p));

RX_DEFINE(struct rx_list, struct rx_ctrl, 24);
RX_API(struct rx_list*, rx_list_init());

RX_DEFINE(struct rx_tree, struct rx_ctrl, 4);
RX_API(int, rx_tree_init(struct rx_tree* tree, struct rx_panel* panel));

RX_DEFINE(struct rx_accel, struct rx_object, 12);
RX_API(int, rx_accel_init(struct rx_accel* accel, uintptr_t inst, int id));

RX_DEFINE(struct rx_menu, struct rx_ctrl, 16);
RX_API(int, rx_menu_init(struct rx_menu* menu, uintptr_t inst, int id));

RX_DEFINE(struct rx_gridlayout, struct rx_layout, 0);
RX_API(int, rx_gridlayout_init(struct rx_gridlayout* layout));
RX_API(int, rx_gridlayout_columns_set(struct rx_gridlayout* layout, struct rx_metric* m, int count));
RX_API(int, rx_gridlayout_rows_set(struct rx_gridlayout* layout, struct rx_metric* m, int count));

//RX_DEFINE()

#endif //__rx_H_
