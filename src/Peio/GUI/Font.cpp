#define PEIO_GUI_EXPORTING
#include "Font.h"

namespace Peio::GUI {

	void Font::Init(Gfx::Graphics* graphics, std::string path, uint height, uint width)
	{
		int ret;

		this->graphics = graphics;
		this->height = height;
		this->width = width;
		Release();

		ret = FT_Init_FreeType(&library);
		if (ret) {
			throw PEIO_GUI_FONT_EXCEPTION("Failed to initialize FreeType.", ret);
		}

		ret = FT_New_Face(library, path.c_str(), 0, &face);
		if (ret) {
			throw PEIO_GUI_FONT_EXCEPTION("Failed to load font.", ret);
		}

		ret = FT_Set_Pixel_Sizes(face, width, height);
		if (ret) {
			throw PEIO_GUI_FONT_EXCEPTION("Failed to set font size.", ret);
		}
	}

	void Font::LoadLetters()
	{
		int ret;

		for (uint i = 0; i < 256; i++) {
			uint ind = FT_Get_Char_Index(face, i);

			ret = FT_Load_Glyph(face, ind, FT_LOAD_DEFAULT);
			if (ret) {
				throw PEIO_GUI_FONT_EXCEPTION("Failed to load glyph.", ret);
			}

			ret = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (ret) {
				throw PEIO_GUI_FONT_EXCEPTION("Failed to render glyph.", ret);
			}

			letters[i].Init({ (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows }, AV_PIX_FMT_GRAY8, true);
			letters[i].character = (char)i;
			letters[i].metrics = face->glyph->metrics;
			letters[i].offset = { face->glyph->metrics.horiBearingX / 64L, face->glyph->metrics.horiBearingY / 64L };

			memcpy(letters[i].GetData(), face->glyph->bitmap.buffer, letters[i].GetLength());
		}
	}

	void Font::LoadTextures()
	{
		for (uint i = 0; i < 256; i++) {
			if (letters[i].GetSize().x() && letters[i].GetSize().y() && letters[i].GetData()) {
				letters[i].texture.Init(graphics, letters[i], DXGI_FORMAT_R8_UNORM);
				letters[i].texture.Upload();
			}
		}
	}

	uint Font::GetHeight() const noexcept
	{
		return height;
	}

	uint Font::GetWidth() const noexcept
	{
		return width;
	}

	void Font::Release()
	{
		if (face) {
			FT_Done_Face(face);
			face = nullptr;
		}
		if (library) {
			FT_Done_FreeType(library);
			library = nullptr;
		}
	}

	Font::~Font()
	{
		Release();
	}

}