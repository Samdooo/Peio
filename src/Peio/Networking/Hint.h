#pragma once

#include "Exception.h"

#include <string>

namespace Peio::Net {

	struct PEIO_NET_EXPORT Hint {

		void Init(ushort port, const char* ip = nullptr);
		void Init(ushort port, const uchar* ip);

		_NODISCARD std::string GetIp() const;
		_NODISCARD ushort GetPort() const noexcept;

		_NODISCARD sockaddr* GetSockaddr() noexcept;
		_NODISCARD const sockaddr* GetSockaddr() const noexcept;

		sockaddr_in hint = {};

	};

}