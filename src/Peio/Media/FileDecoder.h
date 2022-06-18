#pragma once

#include "Scaler.h"
#include <string>

namespace Peio::Med {

	struct PEIO_MED_EXPORT FileDecoder {

		void Init(std::string filePath);
		bool DecodeFrame(Frame* dstFrame, bool createFrame = false, int algorithm = SWS_BICUBIC);
		void Release();

		~FileDecoder();

	protected:

		std::string filePath = "";
		const AVCodec* codec = nullptr;
		AVCodecContext* codecContext = nullptr;
		AVFormatContext* formatContext = nullptr;
		uint streamIndex = -1;

		AVFrame* avFrame = nullptr;
		AVPacket* avPacket = nullptr;

		Scaler scaler = {};

	};

}