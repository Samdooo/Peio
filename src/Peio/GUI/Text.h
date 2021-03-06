#pragma once

#include "Font.h"
#include "Rectangle.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Text : public virtual Rectangle {

		void Init(Gfx::Graphics* graphics, Offset<float, 2> position, Float2 size);

		void SetFont(Font* font);
		void SetString(std::string str);
		void SetSpaceWidth(float spaceWidth);
		void SetLineOffset(float lineOffset);

		void Draw() override;

		_NODISCARD std::string GetString() const noexcept;
		_NODISCARD float GetWidth() const noexcept;

	protected:

		using Rectangle::Init;
		using Rectangle::SetAlphaTexture;

		Font* font = nullptr;
		std::string str = {};
		std::vector<D3D12_VIEWPORT> viewPorts = {};
		float spaceWidth = 0.0f;
		float lineOffset = 0.0f;

	};

	//struct PEIO_GUI_EXPORT Text : public Rectangle {
	//
	//	struct PEIO_GUI_EXPORT Letter {
	//		const Font::Letter* letter = nullptr;
	//		RectangleOffset* offset = nullptr;
	//	};
	//
	//	void Init(Gfx::Graphics* graphics, Uint2 position, Uint2 size);
	//	void SetFont(Font* font);
	//	void SetSpaceWidth(uint spaceWidth);
	//
	//	void AddLetter(char character);
	//	void SetText(std::string text);
	//	void PopText(size_t amount);
	//	void ClearText();
	//
	//	void Upload() override;
	//	void Draw() override;
	//
	//	_NODISCARD const std::vector<Letter>& GetLetters() const noexcept;
	//
	//protected:
	//
	//	using Rectangle::Init;
	//	using Rectangle::SetAlphaTexture;
	//
	//	Font* font = nullptr;
	//	uint width = 0;
	//	uint spaceWidth = 0;
	//
	//	std::vector<Letter> letters = {};
	//
	//};

}