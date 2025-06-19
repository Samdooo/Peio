export module Peio.Windows.Input;

import <Windows.h>;
export import Peio.Essentials.Exception;
import <unordered_set>;
import <functional>;

export namespace Peio {

	namespace Windows {

		template <typename... Args>
		using Proc = std::function<void(Args...)>;

		struct WindowsEvent {
			MSG msg = {};
		};

		struct MessageEvent : public WindowsEvent {
			bool returnDefaultProc = true;
			LRESULT returnValue = 0;
		};

		using MessageHandler = Proc<MessageEvent&>;

		void AddMessageHandler(MessageHandler* msgHandler);
		void RemoveMessageHandler(MessageHandler* msgHandler);

		LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

	}
	using namespace Windows;

}

namespace Peio::Windows {

	extern std::unordered_set<MessageHandler*> msgHandlers;

}