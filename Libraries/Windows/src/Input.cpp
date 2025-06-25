module Peio.Windows.Input;

void Peio::Windows::AddMessageHandler(MessageHandler* msgHandler)
{
	msgHandlers.insert(msgHandler);
}

void Peio::Windows::RemoveMessageHandler(MessageHandler* msgHandler)
{
	msgHandlers.erase(msgHandler);
}

LRESULT Peio::Windows::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MessageEvent msgEvent{};
	msgEvent.msg = { hwnd, msg, wParam, lParam };
	msgEvent.returnDefaultProc = true;
	msgEvent.returnValue = 0;

	for (auto msgHandler : msgHandlers) {
		(*msgHandler)(msgEvent);
	}
	
	if (msgEvent.returnDefaultProc)
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	else
		return msgEvent.returnValue;
}