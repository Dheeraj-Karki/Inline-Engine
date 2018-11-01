#include "Button.hpp"

#include "Layout.hpp"
#include "Placeholders/PlaceholderOverlayEntity.hpp"
#include "Placeholders/PlaceholderTextEntity.hpp"

#include <BaseLibrary/Platform/System.hpp>


namespace inl::gui {


Button::Button() {
	m_text.reset(new PlaceholderTextEntity());
	m_background.reset(new PlaceholderOverlayEntity());
	m_text->SetZDepth(0.1f);
	m_background->SetZDepth(0.0f);

	OnEnterArea += [] {
		System::SetCursorVisual(eCursorVisual::ARROW, nullptr);
	};
}

void Button::SetSize(Vec2u size) {
	m_background->SetScale(size);
	m_text->SetSize(size);
}

Vec2u Button::GetSize() const {
	return m_background->GetScale();
}

void Button::SetPosition(Vec2i position) {
	m_background->SetPosition(position);
	m_text->SetPosition(position);
}
Vec2i Button::GetPosition() const {
	return m_background->GetPosition();
}


void Button::Update(float elapsed) {
	ColorF foreground;
	switch (GetState()) {
		case eStandardControlState::DEFAULT: foreground = GetStyle().foreground; break;
		case eStandardControlState::MOUSEOVER: foreground = GetStyle().hover; break;
		case eStandardControlState::FOCUSED: foreground = GetStyle().focus; break;
		case eStandardControlState::PRESSED: foreground = GetStyle().pressed; break;
	}
	m_background->SetColor(foreground.v);
	m_text->SetColor(GetStyle().text.v);
}

void Button::SetText(std::u32string text) {
	m_text->SetText(std::move(text));
}
const std::u32string& Button::GetText() const {
	return m_text->GetText();
}

void Button::SetZOrder(int rank) {
	m_background->SetZDepth(rank);
	m_text->SetZDepth(rank + 0.1f);
}

std::vector<std::reference_wrapper<std::unique_ptr<gxeng::ITextEntity>>> Button::GetTextEntities() {
	return { m_text };
}

std::vector<std::reference_wrapper<std::unique_ptr<gxeng::IOverlayEntity>>> Button::GetOverlayEntities() {
	return { m_background };
}


} // namespace inl::gui