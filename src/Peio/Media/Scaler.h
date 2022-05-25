#pragma once

#include "Frame.h"
#include "..\Exception.h"
#include "Exception.h"

namespace Peio::Med {

	struct PEIO_MED_EXPORT Scaler {

		void Init(Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm = SWS_BICUBIC);
		void Scale(const Frame* src, Frame* dst) const;
		void Scale(const AVFrame* src, Frame* dst) const;
		void Release();

		_NODISCARD Int2 GetSrcSize() const;
		_NODISCARD Int2 GetDstSize() const;

		_NODISCARD AVPixelFormat GetSrcFormat() const;
		_NODISCARD AVPixelFormat GetDstFormat() const;

		_NODISCARD int GetAlgorithm() const;

		~Scaler();

	protected:

		SwsContext* swsContext = nullptr;
		
		Int2 srcSize = {}, dstSize = {};
		AVPixelFormat srcFormat = AV_PIX_FMT_NONE, dstFormat = AV_PIX_FMT_NONE;
		int algorithm = SWS_BICUBIC;

		int srcLinesize[8] = {};
		int dstLinesize[8] = {};

	};

}