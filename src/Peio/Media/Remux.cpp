#define PEIO_MED_EXPORTING
#include "Remux.h"

namespace Peio::Med {

	void Remux(std::string inPath, std::string outPath)
	{
        const AVOutputFormat* ofmt = nullptr;
        AVFormatContext* ifmt_ctx = nullptr, * ofmt_ctx = nullptr;

        AVPacket pkt2;

        int ret;

        int stream_index = 0;
        int* stream_mapping = nullptr;
        int stream_mapping_size = 0;

        ret = avformat_open_input(&ifmt_ctx, inPath.c_str(), nullptr, nullptr);
        if (ret < 0) {
            throw PEIO_MED_EXCEPTION("Failed to open input file.", ret);
        }

        ret = avformat_find_stream_info(ifmt_ctx, nullptr);
        if (ret < 0) {
            throw PEIO_MED_EXCEPTION("Failed to retrieve input stream information.", ret);
        }

        av_dump_format(ifmt_ctx, 0, inPath.c_str(), 0);

        avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, outPath.c_str());
        if (!ofmt_ctx) {
            throw PEIO_EXCEPTION("Failed to create output context.");
        }

        stream_mapping_size = ifmt_ctx->nb_streams;

        stream_mapping = new int[stream_mapping_size];

        ofmt = ofmt_ctx->oformat;

        for (uint i = 0; i < ifmt_ctx->nb_streams; i++) {
            AVStream* out_stream;
            AVStream* in_stream = ifmt_ctx->streams[i];
            AVCodecParameters* in_codecpar = in_stream->codecpar;

            if (in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO && in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO && in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                stream_mapping[i] = -1;
                continue;
            }

            stream_mapping[i] = stream_index++;

            out_stream = avformat_new_stream(ofmt_ctx, nullptr);
            if (!out_stream) {
                throw PEIO_EXCEPTION("Failed to allocate memory to output stream.");
            }

            ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
            if (ret < 0) {
                throw PEIO_MED_EXCEPTION("Failed to copy codec parameters.", ret);
            }

            out_stream->codecpar->codec_tag = 0;
        }
        av_dump_format(ofmt_ctx, 0, outPath.c_str(), 1);

        if (!(ofmt->flags & AVFMT_NOFILE)) {
            ret = avio_open(&ofmt_ctx->pb, outPath.c_str(), AVIO_FLAG_WRITE);
            if (ret < 0) {
                throw PEIO_MED_EXCEPTION("Failed to open output file.", ret);
            }
        }

        ret = avformat_write_header(ofmt_ctx, NULL);
        if (ret < 0) {
            throw PEIO_MED_EXCEPTION("Failed to write header.", ret);
        }

        int64_t ii = 0;
        while (true) {
            AVStream* in_stream, * out_stream;
            ret = av_read_frame(ifmt_ctx, &pkt2);
            if (ret < 0) {
                break;
            }

            in_stream = ifmt_ctx->streams[pkt2.stream_index];
            if (pkt2.stream_index >= stream_mapping_size || stream_mapping[pkt2.stream_index] < 0) {
                av_packet_unref(&pkt2);
                continue;
            }

            pkt2.stream_index = stream_mapping[pkt2.stream_index];

            out_stream = ofmt_ctx->streams[pkt2.stream_index];

            pkt2.duration = av_rescale_q(pkt2.duration, in_stream->time_base, out_stream->time_base);
            pkt2.pts = ii;
            pkt2.dts = ii;
            ii += pkt2.duration;
            pkt2.pos = -1;

            ret = av_interleaved_write_frame(ofmt_ctx, &pkt2);
            if (ret < 0) {
                throw PEIO_MED_EXCEPTION("Failed mux packet.", ret);
            }
            av_packet_unref(&pkt2);
        }

        av_write_trailer(ofmt_ctx);

        avformat_close_input(&ifmt_ctx);

        if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE)) {
            avio_closep(&ofmt_ctx->pb);
        }
        avformat_free_context(ofmt_ctx);
	}

}