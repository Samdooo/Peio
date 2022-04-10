#pragma once

#include "EventSocket.h"
#include "Hint.h"

namespace Peio::Net {

	template <typename T_sock>
	struct BindSocket : public T_sock {

		void Bind(const Hint& local) {
			if (bind(Socket<>::sock, local.GetSockaddr(), sizeof(local)) != 0) {
				throw PEIO_NET_EXCEPTION("Failed to bind socket.");
			}
			this->local = local;
		}

		_NODISCARD const Hint& GetLocal() const noexcept {
			return local;
		}

	protected:

		Hint local = {};

	};

}