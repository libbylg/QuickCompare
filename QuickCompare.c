// QuickCompare.cpp : 定义应用程序的入口点。
//

#include "QuickCompare.h"

#include "rx/rx_os.h"
#include "rx/rx.h"

#define MAX_LOADSTRING 100

// 全局变量:
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名
struct rx_app app;

struct QCMainFrame {
    struct rx_panel super;
    struct rx_menu  menu;
    struct rx_accel accel;
    struct rx_tree  trees[2];
};

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(struct rx_panel* panel, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKCOMPARE, szWindowClass, MAX_LOADSTRING);
    

    //  初始化app
    int ret = rx_app_init(&app, RX_VERSION, (uintptr_t)hInstance);
    if (RX_OK != ret) {
        return FALSE;
    }

    //  分配用户态对象内存
    struct QCMainFrame mainframe;
    ;

    //  为 app 创建一个主窗体
    ret = rx_panel_init((struct rx_panel*)&mainframe, (uintptr_t)hInstance, WndProc, _T("Hello"));
    if (RX_OK != ret) {
        return FALSE;
    }

    //  加速表初始化
    ret = rx_accel_init(&(mainframe.accel), (uintptr_t)hInstance, IDC_QUICKCOMPARE);
    if (RX_OK != ret) {
        return FALSE;
    }

    //  将窗体绑定为 app 的入口
    ret = rx_app_entry_bind(&app, (struct rx_panel*)&mainframe, nCmdShow);
    if (RX_OK != ret) {
        return FALSE;
    }

    //  推入一个快捷键映射表
    ret = rx_app_accels_push(&app, &(mainframe.accel));
    if (RX_OK != ret) {
        return FALSE;
    }

    // 主消息循环
    return rx_app_run(&app);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = sizeof(uintptr_t);
//    wcex.hInstance = hInstance;
//    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKCOMPARE));
//    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKCOMPARE);
//    wcex.lpszClassName = szWindowClass;
//    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}




//函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//目标: 处理主窗口的消息。
//
//WM_COMMAND  - 处理应用程序菜单
//WM_PAINT    - 绘制主窗口
//WM_DESTROY  - 发送退出消息并返回
LRESULT CALLBACK WndProc(struct rx_panel* panel, UINT message, WPARAM wParam, LPARAM lParam)
{
    struct QCMainFrame* mainframe = (struct QCMainFrame*)panel;
    HWND hWnd = (HWND)rx_panel_fd(panel);
    HINSTANCE hInst = (HINSTANCE)rx_app_inst(&app);

    int ret = 0;
    switch (message) {
    case WM_CREATE: {
        ret = rx_tree_init(mainframe->trees + 0, panel);
        RX_ASSERT(0 == ret, "");
        ret = rx_tree_init(mainframe->trees + 1, panel);
        RX_ASSERT(0 == ret, "");
        DefWindowProc(hWnd, message, wParam, lParam);
    }break;
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
