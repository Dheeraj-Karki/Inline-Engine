#pragma once


#include "Control.hpp"
#include "Layout.hpp"
#include "StandardControl.hpp"


namespace inl::gui {


class Frame : public StandardControl {
public:
	Frame();

	void SetSize(Vec2 size) override;
	Vec2 GetSize() const override;
	Vec2 GetMinimumSize() const override;
	Vec2 GetPreferredSize() const override;

	void SetPosition(Vec2 position) override;
	Vec2 GetPosition() const override;

	void Update(float elapsed = 0.0f) override;

	std::vector<const Control*> GetChildren() const override;

	// Frame specific properties.
	void SetLayout(Layout& layout) { SetLayout(MakeBlankShared(layout)); }
	void SetLayout(std::shared_ptr<Layout> layout);
	std::shared_ptr<Layout> GetLayout() const;

	void OnAttach(Control* parent) override;
	void OnDetach() override;

	float SetDepth(float depth) override;
	float GetDepth() const override;

protected:
	std::vector<std::reference_wrapper<std::unique_ptr<gxeng::ITextEntity>>> GetTextEntities() override;
	std::vector<std::reference_wrapper<std::unique_ptr<gxeng::IOverlayEntity>>> GetOverlayEntities() override;

private:
	std::shared_ptr<Layout> m_layout;
	std::unique_ptr<gxeng::IOverlayEntity> m_background;
};


} // namespace inl::gui