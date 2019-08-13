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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK rx_panel_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //  �� HWND �л�ȡ panel ����
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
int rx_panel_class_register(uintptr_t hInstance)
{
    //  ���һ���Ƿ��Ѿ�ע���ˣ����û��ע�������Ҫ����ע��
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
    RX_ASSERT(NULL != p, "�ⲿ��֤��Ч��");
    struct rx_panel_impl* panel = (struct rx_panel_impl*)p;
    return panel->fd;
}

RX_API(int, rx_panel_init(struct rx_panel* p, uintptr_t inst, RX_EVENT event, const wchar_t* title))
{
    RX_ASSERT(NULL != p, "");

    struct rx_panel_impl* panel = (struct rx_panel_impl*)p;

    //  ���û��ע�ᴰ�������ע��һ��
    int ret = rx_panel_class_register(inst);
    if (RX_OK != ret) {
        return RX_FAIL;
    }

    panel->fd = NULL;
    panel->event = event;

    //  �������ھ����fd ���� WM_NCCREATE ��ʱ��ֵ
    HWND hWnd = CreateWindow(RX_PANEL_CLASS_NAME, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, (HINSTANCE)inst, panel);
    if (!hWnd) {
        return RX_FAIL;
    }

    return RX_OK;
}


