#include <Peio/Media/Encoder.h>
#include <fstream>
#include <Peio/Media/Remux.h>
#include <Peio/Graphics/MediaGraphics.h>

int main() {

	Peio::Gfx::Init();

	Peio::Med::Encoder encoder;
	encoder.Init(AVCodecID::AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, { 1920, 1080 }, 60, 1'000'000);

	std::ofstream ofile("out.h264", std::ios::binary);

	std::function<void(const Peio::byte* data, int length)> onPacket = [&ofile](const Peio::byte* data, int length) {
		ofile.write((char*)data, length);
	};

	Peio::Gfx::MediaGraphics graphics;
	graphics.Init({ 1920, 1080 }, 3);
	
	Peio::Med::Frame frame;

	float col = 1.0f;

	for (int i = 0; i < 60 * 5; i++) {
		graphics.Clear({ (1.0f - col), 0.0f, col, 1.0f });
		graphics.Render();

		col *= 0.995f;

		frame.Init(graphics.GetBuffer(), { 1920, 1080 }, AV_PIX_FMT_RGB0);

		encoder.EncodeFrame(&frame, onPacket);
	}
	encoder.Flush(onPacket);
	ofile.close();

	Peio::Med::Remux("out.h264", "out.mp4");

	return 0;
}