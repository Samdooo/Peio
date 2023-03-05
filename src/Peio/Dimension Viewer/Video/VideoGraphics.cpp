#include "..\App.h"
#include "..\..\Media\Remux.h"

namespace Video {

	void VideoGraphics::Init()
	{
		graphics.Init(App::app.scene.info.screenSize, 1);
		
		if (path.points.size() > 1) {
			encoder.Init(AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, App::app.scene.info.screenSize, framerate, bitrate);
			ofile = std::ofstream(outPath + ".h264", std::ios::binary);
		}
		else {
			encoder.Init(AV_CODEC_ID_PNG, AV_PIX_FMT_RGBA, App::app.scene.info.screenSize, framerate, bitrate);
			ofile = std::ofstream(outPath, std::ios::binary);
		}
	}
	
	bool VideoGraphics::Update()
	{
		if (!path.Update())
			return false;
		App::app.scene.Update(graphics.GetCommandList());
		App::app.rayRenderer.Update(graphics.GetCommandList());
		App::app.denoiser.Update(graphics.GetCommandList());
		return true;
	}

	void VideoGraphics::Render()
	{
		graphics.Clear({});
		App::app.rayRenderer.Render(graphics.GetCommandList());
		App::app.denoiser.Render(graphics.GetCommandList());
		graphics.Render();
		
		Med::Frame frame;
		frame.Init(graphics.GetBuffer(), App::app.scene.info.screenSize, AV_PIX_FMT_RGBA);
		encoder.EncodeFrame(&frame, [this](const byte* data, int length) {
			this->ofile.write((const char*)data, length);
		});
	}

	void VideoGraphics::Cleanup()
	{
		encoder.Flush([this](const byte* data, int length) {
			this->ofile.write((const char*)data, length);
		});
		ofile.close();
		if (path.points.size() > 1) {
			Med::Remux(outPath + ".h264", outPath);
		}
	}

}