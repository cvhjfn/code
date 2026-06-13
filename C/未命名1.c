#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*
 * ===== 程序说明（初学者版） =====
 * 这是一个 Win32 桌面小程序，会创建很多“提示小弹窗”。
 *
 * 执行流程：
 * 1) 第一阶段：按心形曲线逐个弹出窗口。
 * 2) 短暂停留后，关闭第一阶段窗口。
 * 3) 第二阶段：在屏幕随机位置快速弹出更多窗口。
 * 4) 停留后再逐个关闭，形成退场动画。
 * 5) 按空格可以随时退出。
 *
 * 你能在这个文件里学到：
 * - Win32 窗口类注册与创建
 * - 窗口过程（WndProc）如何处理消息
 * - WM_PAINT 里如何绘制背景和文字
 * - 消息泵如何与“延时动画”共存
 * - 动态内存与 GDI 资源释放
 */

/* 单个提示窗口的宽高（像素） */
#define TIP_W 150
#define TIP_H 60

/* 心形阶段总共创建多少个窗口 */
#define HEART_N 100

/* 每个窗口绑定一份数据：文本 + 背景色 */
typedef struct TipData {
    const wchar_t* text;
    COLORREF bg;
} TipData;

/* 随机文案池（宽字符串，配合 Unicode API） */
static const wchar_t* kTips[] = {
    L"\u591A\u559D\u6C34",
    L"\u597D\u597D\u7231\u81EA\u5DF1",
    L"\u597D\u597D\u5403\u996D",
    L"\u4FDD\u6301\u597D\u5FC3\u60C5",
    L"\u6211\u60F3\u4F60\u4E86",
    L"\u987A\u987A\u5229\u5229",
    L"\u522B\u71AC\u591C",
    L"\u5929\u51C9\u4E86\u591A\u7A7F\u8863\u670D"
};

/* 随机背景色池 */
static const COLORREF kColors[] = {
    RGB(255, 192, 203),
    RGB(173, 216, 230),
    RGB(144, 238, 144),
    RGB(255, 250, 205),
    RGB(255, 105, 180),
    RGB(135, 206, 235)
};

/*
 * 全局状态（为了示例代码简化，使用全局变量）。
 * gHeartWins: 第一阶段（心形）窗口句柄数组
 * gAllWins:   全程创建过的所有窗口句柄数组（用于统一清理）
 */
static HWND* gHeartWins = NULL;
static int gHeartCount = 0;
static HWND* gAllWins = NULL;
static int gAllCount = 0;
static int gAllCap = 0;
static BOOL gQuit = FALSE;

/*
 * 把窗口句柄追加到 gAllWins 动态数组。
 * 典型“容量不足则扩容（翻倍）”写法。
 */
static void append_window(HWND hWnd) {
    if (gAllCount >= gAllCap) {
        int newCap = (gAllCap == 0) ? 256 : gAllCap * 2;
        HWND* newBuf = (HWND*)realloc(gAllWins, sizeof(HWND) * newCap);

        /* 扩容失败：不追加，但程序继续运行（降级处理） */
        if (!newBuf) return;

        gAllWins = newBuf;
        gAllCap = newCap;
    }

    gAllWins[gAllCount++] = hWnd;
}

/*
 * 销毁所有仍存在的窗口。
 * IsWindow 用于判断句柄当前是否有效，避免重复销毁。
 */
static void destroy_all_windows(void) {
    for (int i = 0; i < gAllCount; ++i) {
        if (gAllWins[i] && IsWindow(gAllWins[i])) {
            DestroyWindow(gAllWins[i]);
        }
    }
    gAllCount = 0;
}

/*
 * 非阻塞消息泵。
 * 为什么不用 GetMessage：
 * - GetMessage 在无消息时会阻塞，动画会卡住。
 * - PeekMessage 可“有消息就处理，没消息就继续下一步”。
 */
static void pump_messages(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            gQuit = TRUE;
            return;
        }

        /* 捕获全局热键（空格）后立刻退出 */
        if (msg.message == WM_HOTKEY) {
            gQuit = TRUE;
            destroy_all_windows();
            PostQuitMessage(0);
            return;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/*
 * 代替直接 Sleep(ms)：
 * 每 1ms 醒来一次并处理消息，这样窗口不会“未响应”。
 */
static void sleep_with_pump(int ms) {
    DWORD start = GetTickCount();
    while (!gQuit && (int)(GetTickCount() - start) < ms) {
        pump_messages();
        Sleep(1);
    }
}

/*
 * 窗口过程：系统把窗口事件都发到这里。
 * 常见消息：
 * - WM_NCCREATE: 窗口刚创建，适合绑定初始化数据
 * - WM_PAINT:    重绘窗口
 * - WM_DESTROY:  窗口销毁前清理资源
 */
static LRESULT CALLBACK TipWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    TipData* data;

    switch (msg) {
    case WM_NCCREATE: {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;

        /*
         * CreateWindowExW 的最后一个参数（lpParam）会进到这里，
         * 我们把它存到窗口私有数据槽（GWLP_USERDATA）里。
         */
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        return TRUE;
    }

    case WM_PAINT: {
        data = (TipData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);

        /* 1) 先画背景色 */
        HBRUSH brush = CreateSolidBrush(data ? data->bg : RGB(255, 192, 203));
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);  /* GDI 对象必须释放 */

        /* 2) 创建字体并选入 DC */
        HFONT font = CreateFontW(
            24, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, L"Microsoft YaHei"
        );
        HFONT old = (HFONT)SelectObject(hdc, font);

        /* 3) 画文字（居中） */
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(30, 30, 30));
        DrawTextW(
            hdc,
            data && data->text ? data->text : L"\u52A0\u6CB9",
            -1,
            &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE
        );

        /* 4) 恢复现场并清理 */
        SelectObject(hdc, old);
        DeleteObject(font);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            /* 即使窗口有焦点时按空格，也能触发退出 */
            gQuit = TRUE;
            destroy_all_windows();
            PostQuitMessage(0);
        }
        return 0;

    case WM_DESTROY:
        data = (TipData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

        /* 每个窗口对应一份 TipData，销毁时释放 */
        if (data) free(data);
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

/*
 * 创建一个提示窗口，并记录到全局数组。
 * tip 传 NULL 时，自动随机选一句文案。
 */
static HWND create_tip_window(HINSTANCE hInst, int x, int y, const wchar_t* tip) {
    TipData* data = (TipData*)malloc(sizeof(TipData));
    if (!data) return NULL;

    data->text = tip ? tip : kTips[rand() % (sizeof(kTips) / sizeof(kTips[0]))];
    data->bg = kColors[rand() % (sizeof(kColors) / sizeof(kColors[0]))];

    HWND hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"HeartTipClass",
        L"\u63D0\u793A",
        WS_POPUP | WS_BORDER,
        x, y, TIP_W, TIP_H,
        NULL, NULL, hInst, data
    );

    if (!hWnd) {
        free(data);
        return NULL;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    append_window(hWnd);
    return hWnd;
}

/*
 * 主逻辑函数：让 WinMain 和 wWinMain 共用同一套流程。
 * 这样可以同时兼容不同编译参数（ANSI / Unicode 入口）。
 */
static int run_app(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    srand((unsigned int)time(NULL));  /* 初始化随机种子 */

    /* 注册窗口类（告诉系统这个类由哪个 WndProc 处理） */
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = TipWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"HeartTipClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!RegisterClassW(&wc)) return 1;

    /* 注册全局热键：空格退出 */
    RegisterHotKey(NULL, 1, 0, VK_SPACE);

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    /* 存放心形阶段窗口句柄 */
    gHeartWins = (HWND*)malloc(sizeof(HWND) * HEART_N);
    if (!gHeartWins) return 1;

    /*
     * 第一阶段：按心形曲线依次生成窗口。
     * 心形参数方程：
     * x = 16 sin^3(t)
     * y = 13 cos(t) - 5 cos(2t) - 2 cos(3t) - cos(4t)
     */
    for (int i = 0; i < HEART_N && !gQuit; ++i) {
        double th = ((double)i / HEART_N) * 2.0 * 3.14159265358979323846;
        double x = 16.0 * pow(sin(th), 3.0);
        double y = 13.0 * cos(th) - 5.0 * cos(2.0 * th) - 2.0 * cos(3.0 * th) - cos(4.0 * th);

        /* 把数学坐标映射到屏幕坐标，并调整到屏幕中心附近 */
        int sx = (int)(sw / 2 + x * 20 - 50);
        int sy = (int)(sh / 2 - y * 20 - 80);

        /* 边界裁剪，避免窗口跑出屏幕 */
        if (sx < 0) sx = 0;
        if (sy < 0) sy = 0;
        if (sx > sw - TIP_W) sx = sw - TIP_W;
        if (sy > sh - TIP_H) sy = sh - TIP_H;

        gHeartWins[gHeartCount] = create_tip_window(
            hInstance,
            sx,
            sy,
            (i == HEART_N - 1) ? L"\u5145\u5B9E\u81EA\u5DF1" : NULL
        );

        if (gHeartWins[gHeartCount]) gHeartCount++;

        /* 间隔 30ms，形成逐帧出现效果 */
        sleep_with_pump(30);
    }

    sleep_with_pump(1000);  /* 心形完整展示 1 秒 */

    /* 关闭第一阶段窗口 */
    for (int i = 0; i < gHeartCount; ++i) {
        if (gHeartWins[i] && IsWindow(gHeartWins[i])) DestroyWindow(gHeartWins[i]);
    }

    /*
     * 第二阶段随机弹窗数量。
     * 这是经验公式：与屏幕大小相关，再加一个基础值。
     */
    int randomCount = (sw / TIP_W) * (sh / 40) + 50;

    for (int i = 0; i < randomCount && !gQuit; ++i) {
        int x = rand() % (sw - TIP_W + 1);
        int y = rand() % (sh - TIP_H + 1);
        create_tip_window(hInstance, x, y, NULL);
        sleep_with_pump(5);
    }

    sleep_with_pump(10000);  /* 保持 10 秒 */

    /* 最后逐个关闭，形成退场动画 */
    if (!gQuit && gAllCount > 0) {
        int interval = 1000 / gAllCount;
        if (interval < 1) interval = 1;

        for (int i = 0; i < gAllCount && !gQuit; ++i) {
            if (gAllWins[i] && IsWindow(gAllWins[i])) DestroyWindow(gAllWins[i]);
            sleep_with_pump(interval);
        }
    }

    /* 兜底清理 */
    destroy_all_windows();
    UnregisterHotKey(NULL, 1);
    free(gHeartWins);
    free(gAllWins);
    return 0;
}

/* Unicode 入口（常见于 Windows GUI 工程） */
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    return run_app(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/* ANSI 入口（兼容某些未启用 -municode 的 GCC 配置） */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)lpCmdLine;
    return run_app(hInstance, hPrevInstance, GetCommandLineW(), nCmdShow);
}

/*
 * Dev-C++ 里如果项目被创建成“Console Application”，链接器会找 main。
 * 增加 main 作为兼容入口，这样即使没设成 Windows 子系统也能运行。
 */
int main(void) {
    return run_app(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}

