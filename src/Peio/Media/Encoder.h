#pragma once

#include "Scaler.h"
#include <string>
#include <vector>
#include <functional>

namespace Peio::Med {

	struct PEIO_MED_EXPORT Encoder {

		void Init(AVCodecID codecID, AVPixelFormat dstFormat, Int2 dstSize, int framerate, int bitrate, int gopSize = 10, int maxBframes = 2, std::vector<Array<std::string, 2>> opts = {});
		
		void EncodeFrame(const Frame* frame, std::function<void(const byte*, int)> onPacket);
		void Flush(std::function<void(const byte*, int)> onPacket);

		void Release();
		~Encoder();

	protected:

		void Encode(const AVFrame* frame, std::function<void(const byte*, int)> onPacket);

		const AVCodec* codec = nullptr;
		AVCodecContext* codecContext = nullptr;

		AVPacket* avPacket = nullptr;
		AVFrame* avFrame = nullptr;
		int frameIndex;

		Scaler scaler = {};

	};

}