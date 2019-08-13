#include "impl/rx_structs.h"
#include "rx/rx.h"
#include "impl/rx_error.h"


//static int rx_tree_class_register(uintptr_t hInstance)
//{
//    return 0;
//}
//struct rx_tree_impl {
//    struct rx_ctrl_impl ctrl;
//    uintptr_t fd;
//};
RX_API(int, rx_tree_init(struct rx_tree* tree, struct rx_panel* p))
{
    RX_ASSERT((NULL != tree), "");
    RX_ASSERT((NULL != p), "");

    struct rx_panel_impl* panel = (struct rx_panel_impl*)p;

    struct rx_tree_impl* impl = (struct rx_tree_impl*)tree;
    HWND hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_TREEVIEW,
        _T(""),
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0,
        0,
        200,
        200,
        (HWND)(panel->fd),
        NULL,
        NULL,
        NULL);
    if (NULL == hWnd) {
        return RX_FAIL;
    }

    //  关联窗口句柄与用户态对象
    SetLastError(0);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, impl);
    if (0 != GetLastError()) {
        goto ERROR_EXIT;
    }

    impl->fd = hWnd;

    return RX_OK;

ERROR_EXIT:
    DestroyWindow(hWnd);
    return RX_FAIL;
}
