#ifndef __rx_H_
#define __rx_H_

#include "rx/rx_types.h"

//  用于定义接口的宏
#define RX_DEFINE(name, size) \
    name { uint8_t unknown[size]; }

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

struct rx_app;
struct rx_elem;

RX_DEFINE(struct rx_app, 28);
RX_API(int,             rx_app_init(struct rx_app* app, uint32_t version, uintptr_t inst));
RX_API(uintptr_t,       rx_app_inst(struct rx_app* app));
RX_API(int,             rx_app_entry_bind(struct rx_app* app, struct rx_panel* panel, int cmd));
RX_API(int,             rx_app_accels_push(struct rx_app* app, struct rx_accel* accel));
RX_API(int,             rx_app_run(struct rx_app* app));

RX_DEFINE(struct rx_elem, 44);
RX_API(int,             rx_elem_init(struct rx_elem* elem, struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_elem_ctrl(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_nextnibr(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_prevnibr(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_parent(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_firstchild(struct rx_elem* elem));
RX_API(int,             rx_elem_childscount(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_lastchild(struct rx_elem* elem));
RX_API(struct rx_elem*, rx_elem_root(struct rx_elem* elem));
//
RX_API(void,            rx_elem_nibr_addbreore(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_nibr_addbehind(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_child_addtail(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(void,            rx_elem_child_addhead(struct rx_elem* elem, struct rx_elem* newitem));
RX_API(struct rx_elem*, rx_elem_split(struct rx_elem* elem));

RX_DEFINE(struct rx_ctrl, 8);
typedef uintptr_t(RX_CALL* RX_PAINT)(struct rx_ctrl* ctrl, uintptr_t canvas, uintptr_t context);
typedef int(RX_CALL* RX_NOTIFY)(struct rx_ctrl* ctrl, struct rx_notify* notify);
RX_API(int,             rx_ctrl_init(struct rx_ctrl* ctrl, RX_PAINT paint, RX_NOTIFY notify));
RX_API(struct rx_ctrl*, rx_ctrl_nextnibr(struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_ctrl_prevnibr(struct rx_ctrl* ctrl));
RX_API(struct rx_ctrl*, rx_ctrl_parent(struct rx_ctrl* ctrl));
RX_API(void,            rx_ctrl_nibr_addbreore(struct rx_ctrl* ctrl, struct rx_ctrl* newitem));
RX_API(void,            rx_ctrl_nibr_addbehind(struct rx_ctrl* ctrl, struct rx_ctrl* newitem));
RX_API(struct rx_elem*, rx_ctrl_elem(struct rx_ctrl* ctrl));
RX_API(int,             rx_ctrl_pos(struct rx_ctrl* ctrl, struct rx_rect* rect));

RX_DEFINE(struct rx_cntr, 12);
RX_API(int,                 rx_cntr_init(struct rx_cntr* cntr, struct rx_panel* panel));
RX_API(struct rx_ctrl*,     rx_cntr_firstchild(struct rx_cntr* cntr));
RX_API(int,                 rx_cntr_childscount(struct rx_cntr* cntr));
RX_API(struct rx_ctrl*,     rx_cntr_lastchild(struct rx_cntr* cntr));
RX_API(void,                rx_cntr_child_addtail(struct rx_cntr* cntr, struct rx_ctrl* newitem));
RX_API(void,                rx_cntr_child_addhead(struct rx_cntr* cntr, struct rx_ctrl* newitem));
RX_API(struct rx_layout*,   rx_cntr_layout(struct rx_cntr* cntr));
RX_API(struct rx_layout*,   rx_cntr_layout_set(struct rx_cntr* cntr, struct rx_layout* layout));

RX_DEFINE(struct rx_layout, 4);
typedef uintptr_t(RX_CALL* RX_RELAYOUT)(struct rx_layout* layout, struct rx_cntr* cntr, uintptr_t context);
RX_API(int,     rx_layout_init(struct rx_layout* layout, RX_RELAYOUT relayout));

RX_DEFINE(struct rx_panel, 12);
typedef uintptr_t(RX_CALL* RX_EVENT)(void* target, uint32_t message, uintptr_t wParam, uintptr_t lParam);
RX_API(int,             rx_panel_init(struct rx_panel* p, uintptr_t inst, RX_EVENT event, const wchar_t* title));
RX_API(uintptr_t,       rx_panel_fd(struct rx_panel* p));
RX_API(struct rx_cntr*, rx_panel_cntr(struct rx_panel* p));

RX_DEFINE(struct rx_list, 20);
RX_API(struct rx_list*, rx_list_init());

RX_DEFINE(struct rx_tree, 12);
RX_API(int, rx_tree_init(struct rx_tree* tree, struct rx_panel* panel));

RX_DEFINE(struct rx_accel, 12);
RX_API(int, rx_accel_init(struct rx_accel* accel, uintptr_t inst, int id));

RX_DEFINE(struct rx_menu, 12);
RX_API(int, rx_menu_init(struct rx_menu* menu, uintptr_t inst, int id));

#endif //__rx_H_
