#pragma once

#include "Exception.h"
#include "..\Array.h"
#include "Input.h"

#include <atlbase.h>
#include <atlconv.h>
#include <string>

#undef CreateWindow
#undef RegisterClass

namespace Peio::Win {

	struct PEIO_WIN_EXPORT Window {

		void CreateClass(std::string name, UINT styles, WNDPROC proc = Input::WindowProc);
		void RegisterClass();

		void CreateWindow(std::string title, DWORD styles, DWORD exStyles
			, Int2 position, Int2 size, HWND parent = nullptr, HMENU menu = nullptr, LPVOID lpParam = nullptr);

		void SetAbsoluteStyles(LONG styles);
		void SetAbsoluteExStyles(LONG styles);

		void HandleMessages();

		void Show();
		void ShowMinimized();
		void ShowMaximized();
		void Hide();
		void Minimize();
		void Restore();
		void Close();

		_NODISCARD RECT GetRect();
		void Remap(Int2 position, Int2 size);
		void SetOpacity(BYTE opacity); /* Only works if the window is layered */

		_NODISCARD HWND GetHWND() const noexcept;
		_NODISCARD WNDCLASSEX GetWindowClass() const noexcept;

		~Window();

		ProcedureSet<WinEvent*> listeners = {};

	protected:

		HWND hwnd = nullptr;
		std::wstring wName = {};
		WNDCLASSEX wndClass = {};
		Procedure<WinEvent*> listener = {};

	};

}