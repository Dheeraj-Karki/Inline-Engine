#pragma once
#include "GraphicsEngine_LL/GraphicsEngine.hpp"

#include "Layer.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Collapsable.hpp"
#include "List.hpp"
#include "Slider.hpp"
#include "Splitter.hpp"
#include "Image.hpp"
#include "Menu.hpp"
#include "Scrollable.hpp"
#include "CheckBox.hpp"

#include <BaseLibrary/Platform/System.hpp>
#include <vector>
#include <functional>

namespace inl::gui {

class GuiEngine
{
	friend class Gui;
public:
	GuiEngine(gxeng::GraphicsEngine* graphicsEngine, Window* targetWindow);
	~GuiEngine();

	Layer* CreateLayer();
	Layer* AddLayer();

	void Update(float deltaTime);
	void Render();

	void SetCursorVisual(eCursorVisual cursorVisual);

	void FreezeHover() { bHoverFreezed = true; }
	void DefreezeHover() { bHoverFreezed = false; }

	void SetResolution(Vec2u& size);

	void TraverseGuiControls(const std::function<void(Gui&)>& fn);

	inline Vec2 GetCursorPos() { return targetWindow->GetClientCursorPos(); }
	inline float GetCursorPosX() { return GetCursorPos().x; }
	inline float GetCursorPosY() { return GetCursorPos().y; }

	inline Window* GetTargetWindow() { return targetWindow; }
	Gdiplus::Graphics* GetGdiGraphics() { return gdiGraphics; }
	Layer* GetPostProcessLayer() { return postProcessLayer; }

	bool IsHoverFreezed() { return bHoverFreezed; }
	bool IsUsingCustomCursor() { return cursorVisual != eCursorVisual::ARROW; }
	Gui* GetHoveredGui() { return hoveredGui; }

	std::vector<Layer*> GetLayers();

protected:
	void Register(Gui* g) { guis.push_back(g); }

public:
	Event<CursorEvent&> OnCursorClick;
	Event<CursorEvent&> OnCursorPress;
	Event<CursorEvent&> OnCursorRelease;
	Event<CursorEvent&> OnCursorMove;

protected:
	gxeng::GraphicsEngine* graphicsEngine;
	Window* targetWindow;

	std::vector<Layer*> layers; // "layers" rendered first
	Layer* postProcessLayer; // postProcessLayer renders above "layers"
	std::vector<Gui*> guis;

	Gui* hoveredGui;
	Gui* activeContextMenu;

	bool bHoverFreezed;
	bool bOperSysDragging;
	DragDropEvent lastDropEvent;

	eCursorVisual cursorVisual;

	// TODO TEMPORARY GDI+, RATHER MAKE A RENDER INTERFACE AND IMPLEMENT DX12 ETC ABOVE IT
	Gdiplus::Graphics* gdiGraphics;
	HDC hdc;
	HDC memHDC;
	HBITMAP memBitmap;

	// Global variables for lambdas
	Gui* hoveredGuiOnPress = nullptr;
	Vec2 mousePosWhenPress = Vec2(-1, -1);
};

} //namespace inl::gui
