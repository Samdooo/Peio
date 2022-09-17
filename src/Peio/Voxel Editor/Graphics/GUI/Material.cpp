#include "..\..\App.h"

namespace Graphics::GUI {

	void Material::Init()
	{
		reflectionText.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 4.0f) }, { 1000.0f, 1000.0f });
		reflectionText.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		reflectionText.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		reflectionText.Upload();
		reflectionText.SetSpaceWidth(5.0f);
		reflectionText.SetString("Material reflection: ");

		emissionText.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 5.0f) }, { 1000.0f, 1000.0f });
		emissionText.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		emissionText.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		emissionText.Upload();
		emissionText.SetSpaceWidth(5.0f);
		emissionText.SetString("Material emission: ");

		for (UINT i = 0; i < 3; i++) {
			reflection[i].fVal = &App::app.input.build.place.material.reflection[i];
			reflection[i].text.position.x() = reflectionText.position.x() + reflectionText.GetWidth() + (float)(30 * i);
			reflection[i].text.position.y() = reflectionText.position.y();
			reflection[i].Init();
			emission[i].fVal = &App::app.input.build.place.material.emission[i];
			emission[i].text.position.x() = emissionText.position.x() + emissionText.GetWidth() + (float)(30 * i);
			emission[i].text.position.y() = emissionText.position.y();
			emission[i].maxVal = INT_MAX;
			emission[i].Init();
		}
	}

	void Material::Render()
	{
		reflectionText.Draw();
		emissionText.Draw();
		for (UINT i = 0; i < 3; i++) {
			reflection[i].Render();
			emission[i].Render();
		}
	}

	void Material::UpdateMaterial()
	{
		for (UINT i = 0; i < 3; i++) {
			reflection[i].UpdateMaterial();
			emission[i].UpdateMaterial();
		}
	}

	void Material::Value::Init()
	{
		text.Init(&App::app.graphics.graphics, text.position, { 1000.0f, 1000.0f });
		text.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		text.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		text.Upload();
		text.SetSpaceWidth(5.0f);

		Peio::GUI::Button::Init(&App::app.graphics.graphics, { text.position.x() - 2.0f, text.position.y() }, { 0.0f, 20.0f });
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();

		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());

		UpdateMaterial();
	}

	void Material::Value::Update()
	{
		*fVal = (float)val / 255.0f;
		text.SetString(std::to_string(val));
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void Material::Value::UpdateMaterial()
	{
		val = (int)(*fVal * 255.0f);
		Update();
	}

	void Material::Value::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void Material::Value::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void Material::Value::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void Material::Value::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		int amount = 1 + (int)(1.0 / clock.Restart().Seconds() * 0.2);
		if (event->delta > 0) {
			val += std::min(amount, maxVal - val);
			Update();
		}
		else {
			val -= std::min(val, amount);
		}
		Update();
	}

}