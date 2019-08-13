#include "impl/rx_structs.h"
#include "rx/rx.h"
#include "rx/rx_os.h"
#include "impl/rx_error.h"

static uintptr_t RX_CALL rx_panel_message_handle_default(struct rx_panel* panel, uint32_t message, uintptr_t param1, uintptr_t param2)
{
    struct rx_panel_impl* impl = (struct rx_panel_impl*)panel;
    return DefWindowProc((HWND)(impl->fd), message, (WPARAM)param1, (LPARAM)param2);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK rx_panel_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //  从 HWND 中获取 panel 对象
    struct rx_panel_impl* panel = NULL;
    if (WM_NCCREATE == message) {
        LPCREATESTRUCT cs = (LPCREATESTRUCT)(lParam);
        RX_ASSERT(NULL != (cs->lpCreateParams), "");
        SetLastError(0);
        LONG_PTR ret = SetWindowLongPtr(hWnd, GWLP_USERDATA, cs->lpCreateParams);
        if (0 != GetLastError()) {
            goto ERROR_EXIT;
        }

        panel = (struct rx_panel_impl*)(cs->lpCreateParams);
        panel->fd = (uintptr_t)hWnd;
    } else {
        panel = (struct rx_panel_impl*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (NULL == panel) {
            goto ERROR_EXIT;
        }
    }

    if (NULL == panel->event) {
        goto ERROR_EXIT;
    }

    return panel->event(panel, message, (uintptr_t)wParam, (uintptr_t)lParam);

ERROR_EXIT:
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
int rx_panel_class_register(uintptr_t hInstance)
{
    //  检查一下是否已经注册了，如果没有注册过才需要重新注册
    WNDCLASSEXW wcex;
    BOOL ret = GetClassInfoEx((HINSTANCE)hInstance, RX_PANEL_CLASS_NAME, &wcex);
    if (ret) {
        return RX_OK;
    }

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = rx_panel_proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(uintptr_t);
    wcex.hInstance = (HINSTANCE)(hInstance);
    wcex.hIcon = NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKCOMPARE));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_QUICKCOMPARE);
    wcex.lpszClassName = RX_PANEL_CLASS_NAME;
    wcex.hIconSm = NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    ATOM atom = RegisterClassEx(&wcex);
    if (0 == atom) {
        return RX_FAIL;
    }

    return RX_OK;
}

RX_API(uintptr_t, rx_panel_fd(struct rx_panel* p))
{
    RX_ASSERT(NULL != p, "外部保证有效性");
    struct rx_panel_impl* panel = (struct rx_panel_impl*)p;
    return panel->fd;
}

RX_API(int, rx_panel_init(struct rx_panel* p, uintptr_t inst, RX_EVENT event, const wchar_t* title))
{
    RX_ASSERT(NULL != p, "");

    struct rx_panel_impl* panel = (struct rx_panel_impl*)p;

    //  如果没有注册窗口类就先注册一下
    int ret = rx_panel_class_register(inst);
    if (RX_OK != ret) {
        return RX_FAIL;
    }

    panel->fd = NULL;
    panel->event = event;

    //  创建窗口句柄：fd 将在 WM_NCCREATE 的时候赋值
    HWND hWnd = CreateWindow(RX_PANEL_CLASS_NAME, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, (HINSTANCE)inst, panel);
    if (!hWnd) {
        return RX_FAIL;
    }

    return RX_OK;
}


