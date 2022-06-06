#pragma once

#include "Texture.h"

#define PEIO_GUI_FONT_EXCEPTION(x, y) Peio::GUI::FontException(x, y, __FILE__, __LINE__)

namespace Peio::GUI {

	struct FontException {

		FontException(const char* msg, int ret, const char* file, int line) : msg(msg), ret(ret), file(file), line(line) {}

		const char* msg;
		const char* file;
		const int line;
		const int ret;

	};

	struct PEIO_GUI_EXPORT Font {

		struct PEIO_GUI_EXPORT Letter : public Med::Frame {
			char character = 0;
			Int2 offset = {};
			FT_Glyph_Metrics metrics = {};
			Texture texture = {};
		};
		Letter letters[256] = {};

		void Init(Gfx::Graphics* graphics, std::string path, uint height, uint width = 0);

		void LoadLetters();
		void LoadTextures();

		_NODISCARD uint GetHeight() const noexcept;
		_NODISCARD uint GetWidth() const noexcept;

		void Release();
		~Font();

	protected:

		Gfx::Graphics* graphics = nullptr;

		FT_Library library = nullptr;
		FT_Face face = nullptr;

		uint height = 0, width = 0;

	};

}