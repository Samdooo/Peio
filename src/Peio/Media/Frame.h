#pragma once

#include "MediaHeader.h"
#include "..\Array.h"

namespace Peio::Med {

	struct PEIO_MED_EXPORT Frame {

		void Init(Int2 size, AVPixelFormat format, bool createData = true);
		void Init(byte* data, Int2 size, AVPixelFormat format);

		void Release();

		_NODISCARD byte* const& GetData() const;
		_NODISCARD Int2 GetSize() const;
		_NODISCARD AVPixelFormat GetFormat() const;
		_NODISCARD int GetBytesPP() const;
		_NODISCARD size_t GetLength() const;

		_NODISCARD byte* const GetPixel(int row, int col) const;

	protected:

		byte* data = nullptr;
		Int2 size = {};
		AVPixelFormat format = AV_PIX_FMT_NONE;
		int bytes_pp = 0;

	};

}