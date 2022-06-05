#define PEIO_MED_EXPORTING
#include "Images.h"

namespace Peio::Med {

	Frame Images::Load(std::string path, AVPixelFormat targetFormat, Int2 targetSize, int algorithm)
	{
		Frame frame;
		frame.Init(nullptr, targetSize, targetFormat);

		FileDecoder decoder;
		decoder.Init(path);
		if (!decoder.DecodeFrame(&frame, true, algorithm)) {
			throw PEIO_EXCEPTION("No image was found.");
		}
		decoder.Release();
		
		return frame;
	}

}