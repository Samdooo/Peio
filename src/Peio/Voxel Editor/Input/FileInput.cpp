#include "..\App.h"

#include <iostream>

namespace Input {

	void FileInput::Init()
	{
		Peio::Win::Input::AddListener(this);
		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());
	}

	bool FileInput::Handle(Peio::Win::WinMessageEvent* event)
	{
		if (event->msg.message == WM_DROPFILES) {
			HDROP hDrop = (HDROP)event->msg.wParam;
			char fileName[512];
			DragQueryFileA(hDrop, 0, fileName, 512);
			DragFinish(hDrop);

			std::string pathStr(fileName);
			std::ifstream ifile(pathStr, std::ios::binary);
			App::app.scene.world.LoadFromFile(ifile);
			ifile.close();

			std::cout << "Loaded world from \"" << pathStr << "\"" << std::endl;
		}
		return false;
	}

	bool FileInput::Handle(Peio::Win::RawKeyUpEvent* event)
	{
		if (event->foreground && event->key == VK_RETURN) {
			SaveWorld();
		}
		return false;
	}

	void FileInput::SaveWorld()
	{
		time_t t = time(0);
		tm curTime = {};
		localtime_s(&curTime, &t);

		std::string path = "world ";
		path += std::to_string(curTime.tm_year + 1900) + "-";
		path += std::to_string(curTime.tm_mon + 1) + "-";
		path += std::to_string(curTime.tm_mday) + " ";
		path += std::to_string(curTime.tm_hour) + ";";
		path += std::to_string(curTime.tm_min) + ";";
		path += std::to_string(curTime.tm_sec) + ".vxw";

		path.resize(2048, 0);

		if (!std::filesystem::is_directory("Worlds")) {
			std::filesystem::create_directory("Worlds");
		}
		std::string initPath = std::filesystem::absolute(std::filesystem::path("Worlds")).string();

		OPENFILENAMEA file = {};
		file.lStructSize = sizeof(OPENFILENAME);
		file.lpstrFile = &path[0];
		file.nMaxFile = path.size();
		file.lpstrInitialDir = initPath.c_str();
		file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

		std::filesystem::path curPath(std::filesystem::current_path());
		BOOL ret = GetSaveFileNameA(&file);
		std::filesystem::current_path(curPath);
		if (ret == FALSE)
			return;

		std::ofstream ofile(path, std::ios::binary);
		App::app.scene.world.SaveToFile(ofile);
		ofile.close();

		std::cout << "Saved world to \"" << path << "\"" << std::endl;
	}

}