#include <Peio/Media/Encoder.h>
#include <fstream>
#include <Peio/Media/Remux.h>

int main() {

	Peio::Med::Encoder encoder;
	encoder.Init(AVCodecID::AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, { 1920, 1080 }, 60, 1'000'000);

	Peio::Med::Frame frame;
	frame.Init({ 1920, 1080 }, AV_PIX_FMT_RGBA);

	std::ofstream ofile("out.h264", std::ios::binary);

	std::function<void(const Peio::byte* data, int length)> onPacket = [&ofile](const Peio::byte* data, int length) {
		ofile.write((char*)data, length);
	};

	for (int i = 0; i < 60 * 5; i++) {
		Peio::Byte3 col = { (Peio::byte)std::min(20 + i * 2, 255), (Peio::byte)std::min(50 + i, 255), 100};

		for (int y = 0; y < 1080; y++) {
			for (int x = 0; x < 1920; x++) {
				frame.GetData()[(y * 1920 + x) * 4 + 0] = col[0];
				frame.GetData()[(y * 1920 + x) * 4 + 1] = col[1] + x / 100;
				frame.GetData()[(y * 1920 + x) * 4 + 2] = col[2] + x / 100 + y / 50;
				frame.GetData()[(y * 1920 + x) * 4 + 3] = 255;
			}
		}

		encoder.EncodeFrame(&frame, onPacket);
	}
	encoder.Flush(onPacket);
	ofile.close();

	Peio::Med::Remux("out.h264", "out.mp4");

	return 0;
}