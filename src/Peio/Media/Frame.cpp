#define PEIO_MED_EXPORTING
#include "Frame.h"

namespace Peio::Med {

	void Frame::Init(Int2 size, AVPixelFormat format, bool createData)
	{
		this->size = size;
		this->format = format;
		this->bytes_pp = av_get_bits_per_pixel(av_pix_fmt_desc_get(format)) / 8;
		if (createData)
			this->data = new byte[size.x() * size.y() * this->bytes_pp];
	}

	void Frame::Init(byte* data, Int2 size, AVPixelFormat format)
	{
		this->data = data;
		this->size = size;
		this->format = format;
		this->bytes_pp = av_get_bits_per_pixel(av_pix_fmt_desc_get(format)) / 8;
	}

	void Frame::Release()
	{
		PEIO_SAFE_DELETE(data);
	}

	byte* const& Frame::GetData() const
	{
		return data;
	}

	Int2 Frame::GetSize() const
	{
		return size;
	}

	AVPixelFormat Frame::GetFormat() const
	{
		return format;
	}

	int Frame::GetBytesPP() const
	{
		return bytes_pp;
	}

	size_t Frame::GetLength() const
	{
		return (size_t)size.x() * (size_t)size.y() * (size_t)bytes_pp;
	}

	byte* const Frame::GetPixel(int row, int col) const
	{
		return &data[(row * size.x() + col) * bytes_pp];
	}

}