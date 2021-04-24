#pragma once
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError())