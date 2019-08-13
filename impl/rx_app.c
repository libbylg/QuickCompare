#include "rx/rx.h"
#include "impl/rx_structs.h"
#include "impl/utils/list.h"
#include "impl/rx_error.h"

RX_API(int, rx_app_init(struct rx_app* app, uint32_t version, uintptr_t inst))
{
    RX_ASSERT((NULL != app), "");
    RX_ASSERT((NULL != inst), "");

    struct rx_app_impl* impl = (struct rx_app_impl*)app;
    memset(impl, 0, sizeof(struct rx_app_impl));

    impl->size = sizeof(struct rx_app_impl);
    impl->mode = 0;
    impl->noused = 0;
    impl->inst = inst;
    rx_accel_init((struct rx_accel*)(&(impl->accels)), inst, 0);
    impl->entry = NULL;

    return 0;
}

RX_API(int, rx_app_entry_bind(struct rx_app* app, struct rx_panel* panel, int cmd))
{
    RX_ASSERT((NULL != app), "");
    RX_ASSERT((NULL != panel), "");

    HWND hWnd = (HWND)rx_panel_fd(panel);
    RX_ASSERT((NULL != hWnd), "");
    SetLastError(0);
    (void)ShowWindow(hWnd, cmd); //  该接口返回值只是你会修改窗口显示状态之前的值，不需要检查返回值
    BOOL ret = UpdateWindow(hWnd);
    RX_ASSERT(ret, "不应该失败");
    if (!ret) {
        return RX_FAIL;
    }
    return RX_OK;
}

RX_API(uintptr_t, rx_app_inst(struct rx_app* app))
{
    RX_ASSERT((NULL != app), "");

    struct rx_app_impl* impl = (struct rx_app_impl*)app;
    return impl->inst;
}

RX_API(int, rx_app_accels_push(struct rx_app* app, struct rx_accel* accel))
{
    RX_ASSERT(NULL != app, "");
    RX_ASSERT(NULL != accel, "");

    struct rx_app_impl* impl = (struct rx_app_impl*)app;
    list_add(&(impl->accels.nibrs), (struct list_head*)(accel));
    return RX_OK;
}

static rx_bool rx_app_accels_process(struct rx_app* app, uintptr_t pmsg)
{
    RX_ASSERT(NULL != app, "");

    struct rx_app_impl* impl = (struct rx_app_impl*)app;
    struct list_head* pos = NULL;
    struct list_head* n = NULL;

    LPMSG msg = (LPMSG)pmsg;

    list_for_each_safe(pos, n, &(impl->accels.nibrs))
    {
        struct rx_accel_impl* accel = (struct rx_accel_impl*)pos;

        //  如果设置了加速键，那么先通过加速键过滤下
        if ((NULL != accel) && TranslateAccelerator(msg->hwnd, (HACCEL)accel, msg)) {
            return rx_true;
        }
    }

    return rx_false;
}

RX_API(int, rx_app_run(struct rx_app* app))
{
    RX_ASSERT(NULL != app, "");

    struct rx_app_impl* impl = (struct rx_app_impl*)app;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {

        //  先通过加速键过滤下
        if (rx_app_accels_process(app, (uintptr_t)&msg)) {
            continue;
        }

        //  没有被加速键处理的事件，依次通过 TranslateMessage 和 DispatchMessage 向窗口发送
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
