#pragma once

#include <GraphicsEngine/Scene/ICamera2D.hpp>

#include <InlineMath.hpp>
#include <string>

namespace inl::gxeng {


/// <summary>
/// An orthographic camera for top-viewing 2D scenes.
/// </summary>
class Camera2D : public ICamera2D {
public:
	Camera2D();
	virtual ~Camera2D() {} // This is virtual solely for the ObservableCamera2D helper in GraphicsEngine.

	/// <summary> You can identify cameras by their name in the pipeline graph. </summary>
	void SetName(std::string name) override;

	/// <summary> You can identify cameras by their name in the pipeline graph. Set it first to your taste. </summary>
	const std::string& GetName() const override;


	/// <summary> This is going to be shown on the center of the rendered image. </summary>
	void SetPosition(Vec2 position) override;

	/// <summary> Rotate the camera (not the scene) anti-clockwise (as in math). </summary>
	/// <param name="rotation"> In radians, CCW. </param>
	void SetRotation(float rotation) override;

	/// <summary> If true, Y axis is flipped so that coordinates increase downwards. </summary>
	void SetVerticalFlip(bool enable) override;


	/// <summary> Get which world-space point is the center of the rendered image. </summary>
	Vec2 GetPosition() const override;

	/// <summary> Get how many radians the camera is rotated CCW. </summary>
	float GetRotation() const override;

	/// <summary> The size of the rectangle the camera's field of view covers. </summary>
	void SetExtent(Vec2 extent) override;

	/// <summary> The size of the rectangle the camera's field of view covers. </summary>
	/// <remarks> The rectangle's center point corresponds to <see cref="SetPosition/>. </remarks>
	Vec2 GetExtent() const override;

	/// <summary> False if Y coordinates increase upwards. </summary>
	bool GetVerticalFlip() const override;

	/// <summary> The matrix transforms world space to camera space, with Y being screen-up post-transform. </summary>
	Mat33 GetViewMatrix() const override;

	/// <summary> The matrix takes the rectangle defined by <see cref="SetExtent"/> to the unit rectangle [-1,1]x[-1,1]. </summary>
	Mat33 GetProjectionMatrix() const override;

private:
	std::string m_name;
	Vec2 m_position;
	float m_rotation;
	Vec2 m_extent;
	bool m_verticalFlip = false;
};


} // namespace inl::gxeng
