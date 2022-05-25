#define PEIO_MED_EXPORTING
#include "Scaler.h"

namespace Peio::Med {

    void Scaler::Init(Int2 srcSize, Int2 dstSize, AVPixelFormat srcFormat, AVPixelFormat dstFormat, int algorithm)
    {
        if (swsContext && 
            this->srcSize == srcSize && this->dstSize == dstSize && 
            this->srcFormat == srcFormat && this->dstFormat == dstFormat &&
            this->algorithm == algorithm)
            return;
        this->srcSize = srcSize;
        this->dstSize = dstSize;
        this->srcFormat = srcFormat;
        this->dstFormat = dstFormat;
        this->algorithm = algorithm;

        Release();

        swsContext = sws_getContext(srcSize.x(), srcSize.y(), srcFormat, dstSize.x(), dstSize.y(), dstFormat, algorithm, nullptr, nullptr, nullptr);
        if (!swsContext) {
            throw PEIO_EXCEPTION("Failed to get scaler context.");
        }

        srcLinesize[0] = srcSize.x() * av_get_bits_per_pixel(av_pix_fmt_desc_get(srcFormat)) / 8;
        dstLinesize[0] = dstSize.x() * av_get_bits_per_pixel(av_pix_fmt_desc_get(dstFormat)) / 8;
    }

    void Scaler::Scale(const Frame* src, Frame* dst) const
    {
        int ret = sws_scale(swsContext, &src->GetData(), srcLinesize, 0, srcSize.y(), &dst->GetData(), dstLinesize);
        if (ret < 0) {
            throw PEIO_MED_EXCEPTION("Failed to scale data.", ret);
        }
    }

    void Scaler::Scale(const AVFrame* src, Frame* dst) const
    {
        int ret = sws_scale(swsContext, src->data, src->linesize, 0, srcSize.y(), &dst->GetData(), dstLinesize);
        if (ret < 0) {
            throw PEIO_MED_EXCEPTION("Failed to scale data.", ret);
        }
    }

    void Scaler::Release()
    {
        if (swsContext) {
            sws_freeContext(swsContext);
            swsContext = nullptr;
        }
    }

    Int2 Scaler::GetSrcSize() const
    {
        return srcSize;
    }

    Int2 Scaler::GetDstSize() const
    {
        return dstSize;
    }

    AVPixelFormat Scaler::GetSrcFormat() const
    {
        return srcFormat;
    }

    AVPixelFormat Scaler::GetDstFormat() const
    {
        return dstFormat;
    }

    int Scaler::GetAlgorithm() const
    {
        return algorithm;
    }

    Scaler::~Scaler()
    {
        Release();
    }

}