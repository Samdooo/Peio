#define PEIO_MED_EXPORTING
#include "FileDecoder.h"

namespace Peio::Med {

	void FileDecoder::Init(std::string filePath)
	{
		int ret;

		this->filePath = filePath;

		formatContext = avformat_alloc_context();
		if (!formatContext) {
			throw PEIO_EXCEPTION("Failed to allocate format context.");
		}

		ret = avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to open file.", ret);
		}

		streamIndex = -1;
		for (uint i = 0; i < formatContext->nb_streams; i++) {
			if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				streamIndex = i;
				break;
			}
		}
		if (streamIndex == -1) {
			throw PEIO_EXCEPTION("No video stream found.");
		}

		codec = avcodec_find_decoder(formatContext->streams[streamIndex]->codecpar->codec_id);
		if (!codec) {
			throw PEIO_EXCEPTION("Failed to create decoder.");
		}

		codecContext = avcodec_alloc_context3(codec);
		if (!codecContext) {
			throw PEIO_EXCEPTION("Failed to allocate codec context.");
		}
		ret = avcodec_parameters_to_context(codecContext, formatContext->streams[streamIndex]->codecpar);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to create codec context.", ret);
		}

		ret = avcodec_open2(codecContext, codec, nullptr);
		if (ret < 0) {
			throw PEIO_MED_EXCEPTION("Failed to open codec context.", ret);
		}

		avFrame = av_frame_alloc();
		if (!avFrame) {
			throw PEIO_EXCEPTION("Failed to allocate AVFrame.");
		}
		avPacket = av_packet_alloc();
		if (!avPacket) {
			throw PEIO_EXCEPTION("Failed to allocate AVPacket.");
		}
	}

	bool FileDecoder::DecodeFrame(Frame* dstFrame, bool createFrame, int algorithm)
	{
		int ret;

		while (true) {
			while (true) {
				ret = av_read_frame(formatContext, avPacket);
				if (ret < 0) {
					if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
						return false;
					}
					else {
						throw PEIO_MED_EXCEPTION("Failed to read frame.", ret);
					}
				}
				if (avPacket->stream_index == streamIndex)
					break;
				av_packet_unref(avPacket);
			}

			ret = avcodec_send_packet(codecContext, avPacket);
			if (ret < 0) {
				throw PEIO_MED_EXCEPTION("Failed to send packet to codec.", ret);
			}
			av_packet_unref(avPacket);

			ret = avcodec_receive_frame(codecContext, avFrame);
			if (ret < 0) {
				if (ret == AVERROR(EAGAIN)) {
					continue;
				}
				else if (ret == AVERROR_EOF) {
					return false;
				}
				else {
					throw PEIO_MED_EXCEPTION("Failed to receive frame from codec.", ret);
				}
			}
			break;
		}

		if (createFrame) {
			dstFrame->Init({ dstFrame->GetSize().x() ? dstFrame->GetSize().x() : avFrame->width, 
							 dstFrame->GetSize().y() ? dstFrame->GetSize().y() : avFrame->height }, 
							 dstFrame->GetFormat() != AV_PIX_FMT_NONE ? dstFrame->GetFormat() : (AVPixelFormat)avFrame->format);
		}

		dstFrame->filePath = filePath;

		scaler.Init({ avFrame->width, avFrame->height }, dstFrame->GetSize(), (AVPixelFormat)avFrame->format, dstFrame->GetFormat(), algorithm);
		scaler.Scale(avFrame, dstFrame);

		return true;
	}

	void FileDecoder::Release()
	{
		if (codecContext) {
			avcodec_free_context(&codecContext);
			codecContext = nullptr;
		}
		if (formatContext) {
			avformat_close_input(&formatContext);
			avformat_free_context(formatContext);
			formatContext = nullptr;
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

	FileDecoder::~FileDecoder()
	{
		Release();
	}

}