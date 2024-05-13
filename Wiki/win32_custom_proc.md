# Function for custom win32 proc and custom NC-area
Here the code example for creating custom window without Non-Client area.
That function can be found in `app.h` header file.
```
#ifdef _WIN32
LRESULT CALLBACK app::internal_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_NCCALCSIZE:
		{
			// Remove the window's standard sizing border
			if (wParam == TRUE && lParam != NULL)
			{
				NCCALCSIZE_PARAMS* pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				pParams->rgrc[0].top += 1;
				pParams->rgrc[0].right -= 2;
				pParams->rgrc[0].bottom -= 2;
				pParams->rgrc[0].left += 2;
			}
			return 0;
		}
		case WM_NCACTIVATE:
		{
			RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW);
			return 0;
		}
		case WM_NCPAINT:
		{
			return 0;
		}
		case WM_NCHITTEST:
		{
			// Expand the hit test area for resizing
			const int borderWidth = 12; // Adjust this value to control the hit test area size

			POINTS mousePos = MAKEPOINTS(lParam);
			POINT clientMousePos = { mousePos.x, mousePos.y };
			ScreenToClient(hWnd, &clientMousePos);

			RECT windowRect;
			GetClientRect(hWnd, &windowRect);

			if (clientMousePos.y >= windowRect.bottom - borderWidth)
			{
				if (clientMousePos.x <= borderWidth)
					return HTBOTTOMLEFT;
				else if (clientMousePos.x >= windowRect.right - borderWidth)
					return HTBOTTOMRIGHT;
				else
					return HTBOTTOM;
			}
			else if (clientMousePos.y <= borderWidth)
			{
				if (clientMousePos.x <= borderWidth)
					return HTTOPLEFT;
				else if (clientMousePos.x >= windowRect.right - borderWidth)
					return HTTOPRIGHT;
				else
					return HTTOP;
			}
			else if (clientMousePos.x <= borderWidth)
				return HTLEFT;
			else if (clientMousePos.x >= windowRect.right - borderWidth)
				return HTRIGHT;

			break;
		}
	}

	auto inst = app::get_instance();
	return CallWindowProc(inst.m_original_proc, hWnd, uMsg, wParam, lParam);
}
#endif
```
Also you need change internal init function (that also stored in `app.h`)
```
void app::internal_init() {
#ifdef _WIN32
	HWND hWnd = glfwGetWin32Window(m_window);

	LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
	style |= WS_THICKFRAME;
	style &= ~WS_CAPTION;

	SetWindowLongPtr(hWnd, GWL_STYLE, style);
	m_original_proc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	(WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app::internal_proc));

	RECT window_rect;
	GetWindowRect(hWnd, &window_rect);
	int width = window_rect.right - window_rect.left;
	int height = window_rect.bottom - window_rect.top;
	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE);

	// Use Mica
	int use_mica = 1;
	DwmSetWindowAttribute(
		hWnd, DWMWA_MICA_EFFECT, 
		&use_mica, sizeof(int)
	);

	// Remove rounded corners
	int attribute = DWMWCP_DONOTROUND;
	DwmSetWindowAttribute(
		hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, 
		&attribute, sizeof(int)
	);
#endif
}
```