#pragma once

#ifdef PEIO_MED_EXPORTING
#define PEIO_MED_EXPORT __declspec(dllexport)
#else
#define PEIO_MED_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"

extern "C" {
#include "FFmpeg/libavcodec/avcodec.h"
#include "FFmpeg/libavutil/timestamp.h"
#include "FFmpeg/libavformat/avformat.h"
#include "FFmpeg/libswscale/swscale.h"
#include "FFmpeg/libavutil/opt.h"
#include "FFmpeg/libavutil/pixdesc.h"
}

#pragma comment(lib, "FFmpeg/avcodec.lib")
#pragma comment(lib, "FFmpeg/avdevice.lib")
#pragma comment(lib, "FFmpeg/avfilter.lib")
#pragma comment(lib, "FFmpeg/avformat.lib")
#pragma comment(lib, "FFmpeg/avutil.lib")
#pragma comment(lib, "FFmpeg/postproc.lib")
#pragma comment(lib, "FFmpeg/swresample.lib")
#pragma comment(lib, "FFmpeg/swscale.lib")