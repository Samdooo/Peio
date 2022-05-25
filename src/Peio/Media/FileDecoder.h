#pragma once

#include "Scaler.h"
#include <string>

namespace Peio::Med {

	struct PEIO_MED_EXPORT FileDecoder {

		void Init(std::string path);
		bool DecodeFrame(Frame* dstFrame, bool createFrame = false, int algorithm = SWS_BICUBIC);
		void Release();

		~FileDecoder();

	protected:

		const AVCodec* codec = nullptr;
		AVCodecContext* codecContext = nullptr;
		AVFormatContext* formatContext = nullptr;
		uint streamIndex = -1;

		AVFrame* avFrame = nullptr;
		AVPacket* avPacket = nullptr;

		Scaler scaler = {};

	};

}