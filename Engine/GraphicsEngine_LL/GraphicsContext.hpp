#pragma once

#include "MemoryObject.hpp"
#include "ResourceView.hpp"
#include <cstdint>


namespace inl {
namespace gxeng {

class MemoryManager;
class PersistentResViewHeap;
class RTVHeap;
class DSVHeap;

class GraphicsContext {
public:
	GraphicsContext(MemoryManager* memoryManager = nullptr,
					PersistentResViewHeap* srvHeap = nullptr,
					RTVHeap* rtvHeap = nullptr,
					DSVHeap* dsvHeap = nullptr,
					int processorCount = 0,
					int deviceCount = 0);
	GraphicsContext(const GraphicsContext& rhs) = default;
	GraphicsContext(GraphicsContext&& rhs) = default;
	GraphicsContext& operator=(const GraphicsContext& rhs) = default;
	GraphicsContext& operator=(GraphicsContext&& rhs) = default;

	// Parallelism
	int GetProcessorCoreCount() const;
	int GetGraphicsDeviceCount() const;

	// Create pipeline textures
	Texture2D CreateTexture2D(uint64_t width, uint32_t height, gxapi::eFormat format, uint16_t arraySize = 1) const;
	Texture2D CreateRenderTarget2D(uint64_t width, uint32_t height, gxapi::eFormat format, bool shaderResource, uint16_t arraySize = 1) const;
	Texture2D CreateDepthStencil2D(uint64_t width, uint32_t height, gxapi::eFormat format, bool shaderResource, uint16_t arraySize = 1) const;
	Texture2DSRV CreateSrv(Texture2D& texture, gxapi::eFormat format, gxapi::SrvTexture2DArray desc = {}) const;
	RenderTargetView CreateRtv(Texture2D& renderTarget, gxapi::RtvTexture2DArray desc) const;
	DepthStencilView CreateDsv(Texture2D& depthStencilView, gxapi::DsvTexture2DArray desc) const;

private:
	MemoryManager* m_memoryManager;
	PersistentResViewHeap* m_srvHeap;
	RTVHeap* m_rtvHeap;
	DSVHeap* m_dsvHeap;
	int m_processorCount;
	int m_deviceCount;
};



} // namespace gxeng
} // namespace inl
