#define PEIO_MED_EXPORTING
#include "Encoder.h"

namespace Peio::Med {

	void Encoder::Init(AVCodecID codecID, AVPixelFormat dstFormat, Int2 dstSize, int framerate, int bitrate, int gopSize, int maxBframes, std::vector<Array<std::string, 2>> opts)
	{
		int ret;

		avPacket = av_packet_alloc();
		if (!avPacket) {
			throw PEIO_EXCEPTION("Failed to allocate AVPacket.");
		}
		avFrame = av_frame_alloc();
		if (!avFrame) {
			throw PEIO_EXCEPTION("Failed to allocate AVFrame.");
		}
		avFrame->width = dstSize.x();
		avFrame->height = dstSize.y();
		avFrame->format = dstFormat;
		
		ret = av_frame_get_buffer(avFrame, 0);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to get frame buffer.", ret);
		}

		codec = avcodec_find_encoder(codecID);
		if (!codec) {
			throw PEIO_EXCEPTION("Failed to find codec.");
		}
		
		codecContext = avcodec_alloc_context3(codec);
		if (!codec) {
			throw PEIO_EXCEPTION("Failed to allocate codec context.");
		}

		codecContext->width = avFrame->width;
		codecContext->height = avFrame->height;
		codecContext->framerate = { framerate, 1 };
		codecContext->time_base = { 1, framerate };
		codecContext->bit_rate = bitrate;
		codecContext->pix_fmt = (AVPixelFormat)avFrame->format;
		codecContext->gop_size = gopSize;
		codecContext->max_b_frames = maxBframes;

		for (Array<std::string, 2> opt : opts) {
			av_opt_set(codecContext->priv_data, opt.x().c_str(), opt.y().c_str(), 0);
		}

		ret = avcodec_open2(codecContext, codec, nullptr);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to open codec.", ret);
		}

		frameIndex = 0;
	}

	void Encoder::Encode(const AVFrame* frame, std::function<void(const byte*, int)> onPacket)
	{
		int ret;

		ret = avcodec_send_frame(codecContext, frame);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to send frame to codec.", ret);
		}

		while (ret >= 0) {
			ret = avcodec_receive_packet(codecContext, avPacket);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				frameIndex++;
				break;
			}
			if (ret < 0) {
				throw PEIO_MED_EXCEPTION("Failed to receive packet from codec.", ret);
			}
			onPacket(avPacket->data, avPacket->size);
			av_packet_unref(avPacket);
		}
	}

	void Encoder::EncodeFrame(const Frame* frame, std::function<void(const byte*, int)> onPacket)
	{
		scaler.Init(frame->GetSize(), { avFrame->width, avFrame->height }, frame->GetFormat(), (AVPixelFormat)avFrame->format);
		scaler.Scale(frame, avFrame);
		
		avFrame->pts = frameIndex;
		Encode(avFrame, onPacket);
	}

	void Encoder::Flush(std::function<void(const byte*, int)> onPacket)
	{
		Encode(nullptr, onPacket);
	}

	void Encoder::Release()
	{
		if (codecContext) {
			avcodec_free_context(&codecContext);
			codecContext = nullptr;
		}
		if (avFrame) {
			av_frame_free(&avFrame);
			avFrame = nullptr;
		}
		if (avPacket) {
			av_packet_free(&avPacket);
			avPacket = nullptr;
		}
		scaler.Release();
	}

	Encoder::~Encoder()
	{
		Release();
	}

}