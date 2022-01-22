#define PEIO_NET_EXPORTING
#include "Hint.h"

namespace Peio::Net {

	void Hint::Init(ushort port, const char* ip)
	{
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		if (ip) {
			if (inet_pton(AF_INET, ip, &hint.sin_addr) != 1) {
				throw PEIO_NET_EXCEPTION("Invalid ip string.");
			}
		}
		else {
			hint.sin_addr.S_un.S_addr = INADDR_ANY;
		}
	}

	void Hint::Init(ushort port, const uchar* ip)
	{
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		memcpy(&hint.sin_addr, ip, 4);
	}

	std::string Hint::GetIp() const
	{
		char buf[16];
		const char* ret = inet_ntop(AF_INET, &hint.sin_addr, buf, 16);
		if (!ret) {
			throw PEIO_NET_EXCEPTION("Failed to get ip from hint.");
		}
		return std::string(buf);
	}

	ushort Hint::GetPort() const noexcept
	{
		return ntohs(hint.sin_port);
	}

	sockaddr* Hint::GetSockaddr() noexcept
	{
		return reinterpret_cast<sockaddr*>(&hint);
	}

	const sockaddr* Hint::GetSockaddr() const noexcept
	{
		return reinterpret_cast<const sockaddr*>(&hint);
	}

}