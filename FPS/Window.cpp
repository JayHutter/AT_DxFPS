#include "Window.h"
#include <sstream>
#include "WindowThrowMacro.h"
#include "Mouse.h"

Window::WindowClass Window::WindowClass::wndCls;

//WINDOW EXCEPTION HANDLER

Window::WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept :
    Exception(line, file), hr(hr)
{
}

const char* Window::WindowException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::WindowException::GetType() const noexcept
{
    return "Window Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );

    if (nMsgLen == 0)
    {
        return "Unidentified Error Code";
    }

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::WindowException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::WindowException::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}

//WINDOW CLASS 
Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClsName, GetInstance());
}


const char* Window::WindowClass::GetName() noexcept
{
    return wndClsName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndCls.hInst;
}

//Window 
Window::Window(int width, int height, const char* name)
{
    RECT wrect;
    wrect.left = 100;
    wrect.right = width + wrect.left;
    wrect.top = 100;
    wrect.bottom = height + wrect.top;
    if (FAILED(AdjustWindowRect(&wrect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE)))
    {
        throw WND_LAST_EXCEPT();
    }

    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, wrect.right - wrect.left, wrect.bottom - wrect.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );

    if (hWnd == nullptr)
    {
        throw WND_LAST_EXCEPT();
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    pRenderer = std::make_unique<Renderer>(hWnd);
    while (::ShowCursor(FALSE) >= 0);

    mouse.LockCursor(hWnd, true);

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = hWnd;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        throw WND_LAST_EXCEPT();
    }
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

std::optional<int> Window::ManageMsgs()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

Renderer& Window::Render()
{
    return *pRenderer;
}

bool Window::hasFocus()
{
    return focussed;
}

//Setup pointer to window instance
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const wndPtr = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndPtr));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return wndPtr->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const wndPtr = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return wndPtr->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    
    case WM_SETFOCUS:
        focussed = true;
        mouse.LockCursor(hWnd, true);
        mouse.Reset();
        break;
    //INPUTS
    case WM_KILLFOCUS:
        focussed = false;
        key.ClearState();
        break;
    
    case WM_KEYDOWN:
        // syskey commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        if (!(lParam & 0x40000000) || key.AutorepeatIsEnabled()) // filter autorepeat
        {
            key.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        key.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        key.OnChar(static_cast<unsigned char>(wParam));
        break;
        
    case WM_MOUSEMOVE:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMouseMove(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp(pt.x, pt.y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown(pt.x, pt.y);
        }
        break;
    }
    };

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
