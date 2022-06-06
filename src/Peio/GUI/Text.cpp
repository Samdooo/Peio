#define PEIO_GUI_EXPORTING
#include "Text.h"

namespace Peio::GUI {

	void Text::Init(Gfx::Graphics* graphics, Uint2 position, Uint2 size)
	{
		Rectangle::Init(graphics, position, size);
		rect[0].useAlpha = true;
		rect[0].useOffset = true;

		//viewPort = { 0.0f, 0.0f, (float)size.x(), (float)size.y(), 0.0f, 1.0f};
		//scissorRect = { 0, 0, (LONG)size.x(), (LONG)size.y() };
	}

	void Text::SetFont(Font* font)
	{
		this->font = font;
	}

	void Text::SetSpaceWidth(uint spaceWidth)
	{
		this->spaceWidth = spaceWidth;
	}

	void Text::AddLetter(char character)
	{
		if (character == ' ') {
			width += spaceWidth;
			letters.push_back(Letter());
			return;
		}

		Font::Letter& letter = font->letters[(uchar)character];
		if (!letter.GetLength())
			return;

		letters.push_back(Letter());
		letters.back().letter = &letter;

		letters.back().offset = new RectangleOffset;
		letters.back().offset->Init(graphics, { width, font->GetHeight() - letter.offset.y() }, (Uint2)letter.GetSize());

		width += letter.GetSize().x() + letter.offset.x();
	}

	void Text::SetText(std::string text)
	{
		ClearText();
		for (size_t i = 0; i < text.size(); i++)
			AddLetter(text[i]);
	}

	void Text::PopText(size_t amount)
	{
		for (size_t i = 0; i < amount; i++) {
			if (!letters.back().letter) {
				letters.pop_back();
				width -= spaceWidth;
				continue;
			}
			width -= letters.back().letter->GetSize().x() + letters.back().letter->offset.x();
			delete letters.back().offset;
			letters.pop_back();
		}
	}

	void Text::ClearText()
	{
		for (size_t i = 0; i < letters.size(); i++) {
			if (letters[i].letter)
				delete letters[i].offset;
		}
		letters.clear();
		width = 0;
	}

	void Text::Upload()
	{
		Rectangle::Upload();
		for (size_t i = 0; i < letters.size(); i++) {
			if (letters[i].letter)
				letters[i].offset->Upload();
		}
	}

	void Text::Draw()
	{
		scissorRect = { (LONG)rect[0].position.x(), (LONG)rect[0].position.y(), (LONG)rect[0].position.x() + (LONG)rect[0].size.x(), (LONG)rect[0].position.y() + (LONG)rect[0].size.y() };
		for (size_t i = 0; i < letters.size(); i++) {
			if (!letters[i].letter)
				continue;
			alphaTexture = &letters[i].letter->texture;
			letters[i].offset->Draw();
			Rectangle::Draw();
		}
	}

	const std::vector<Text::Letter>& Text::GetLetters() const noexcept
	{
		return letters;
	}

}