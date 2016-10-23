#include "Node_TescoRender.hpp"

#include "../MeshEntity.hpp"
#include "../Mesh.hpp"

#include "../../GraphicsApi_LL/IGxapiManager.hpp"

#include <array>
#include <iostream> // debug only


namespace inl {
namespace gxeng {
namespace nodes {


bool CheckMeshFormat(const Mesh& mesh) {
	for (size_t i = 0; i < mesh.GetNumStreams(); i++) {
		auto& elements = mesh.GetVertexBufferElements(i);
		if (elements.size() != 2) return false;
		if (elements[0].semantic != eVertexElementSemantic::POSITION) return false;
		if (elements[1].semantic != eVertexElementSemantic::COLOR) return false;
	}

	return true;
}


const std::string SHADER_SRC = R"(

/*
cbuffer ConstantBuffer : register(b0)
{
	float4x4 invTrModel;
	float4x4 MVP;
};
*/

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
	PSInput result;

	//result.position = mul(position, MVP);
	//float4 worldNormal = mul(normal, invTrModel);
	//result.color = max(0.05, dot(float4(1, 1, -1, 0), worldNormal));

	result.position = position;
	result.color = color;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
)";


TescoRender::TescoRender(gxapi::IGraphicsApi* graphicsApi, gxapi::IGxapiManager* gxapiManager) :
	m_binder(graphicsApi, {})
{
	//this->GetInput<0>().Set(RenderTargetView());
	this->GetInput<1>().Set(nullptr);


	BindParameterDesc bindParamDesc;
	bindParamDesc.parameter = BindParameter(eBindParameterType::CONSTANT, 0);
	bindParamDesc.constantSize = 2*(sizeof(float)*4*4);
	bindParamDesc.relativeAccessFrequency = 0;
	bindParamDesc.relativeChangeFrequency = 0;
	bindParamDesc.shaderVisibility = gxapi::eShaderVisiblity::VERTEX;

	m_binder = Binder{ graphicsApi, {bindParamDesc}};

#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	gxapi::eShaderCompileFlags compileFlags = gxapi::eShaderCompileFlags::DEBUG;
	compileFlags += gxapi::eShaderCompileFlags::NO_OPTIMIZATION;
#else
	gxapi::eShaderCompileFlags compileFlags = gxapi::eShaderCompileFlags::OPTIMIZATION_HIGH;
#endif

	gxapi::ShaderProgramBinary vertexShader = gxapiManager->CompileShader(SHADER_SRC, "VSMain", gxapi::eShaderType::VERTEX, compileFlags, {});
	gxapi::ShaderProgramBinary fragmentShader =  gxapiManager->CompileShader(SHADER_SRC, "PSMain", gxapi::eShaderType::PIXEL, compileFlags, {});

	std::vector<gxapi::InputElementDesc> inputElementDesc = {
		gxapi::InputElementDesc("POSITION", 0, gxapi::eFormat::R32G32B32_FLOAT, 0, 0),
		gxapi::InputElementDesc("COLOR", 0, gxapi::eFormat::R32G32B32_FLOAT, 0, 12)
	};

	gxapi::GraphicsPipelineStateDesc psoDesc;
	psoDesc.inputLayout.elements = inputElementDesc.data();
	psoDesc.inputLayout.numElements = (unsigned)inputElementDesc.size();
	psoDesc.rootSignature = m_binder.GetRootSignature();
	psoDesc.vs.shaderByteCode = vertexShader.data.data();
	psoDesc.vs.sizeOfByteCode = vertexShader.data.size();
	psoDesc.ps.shaderByteCode = fragmentShader.data.data();
	psoDesc.ps.sizeOfByteCode = fragmentShader.data.size();
	//psoDesc.rasterization = RasterizerState();
	//psoDesc.blending = BlendState();
	//psoDesc.depthStencilState = DepthStencilState();
	psoDesc.primitiveTopologyType = gxapi::ePrimitiveTopologyType::TRIANGLE;
	psoDesc.numRenderTargets = 1;
	psoDesc.renderTargetFormats[0] = gxapi::eFormat::R8G8B8A8_UNORM;

	m_PSO.reset(graphicsApi->CreateGraphicsPipelineState(psoDesc));
}


void TescoRender::RenderScene(RenderTargetView& rtv, const EntityCollection<MeshEntity>& entities, GraphicsCommandList& commandList) {
	// Set render target
	auto pRTV = &rtv;
	commandList.SetResourceState(rtv.GetResource(), 0, gxapi::eResourceState::RENDER_TARGET);
	commandList.SetRenderTargets(1, &pRTV, nullptr); // no depth yet

	gxapi::Rectangle rect{ 0, (int)rtv.GetResource()->GetHeight(), 0, (int)rtv.GetResource()->GetWidth() };
	gxapi::Viewport viewport;
	viewport.width = rect.right;
	viewport.height = rect.bottom;
	viewport.topLeftX = 0;
	viewport.topLeftY = 0;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	commandList.SetScissorRects(1, &rect);
	commandList.SetViewports(1, &viewport);

	// Iterate over all entities
	for (const MeshEntity* entity : entities) {
		//std::cout << "Rendering entity " << entity << std::endl;
		
		// Get entity parameters
		Mesh* mesh = entity->GetMesh();
		auto position = entity->GetPosition();

		// Draw mesh
		if (!CheckMeshFormat(*mesh)) {
			//std::cout << "Invalid mesh format." << std::endl;
			continue;
		}

		std::vector<const gxeng::VertexBuffer*> vertexBuffers;
		std::vector<unsigned> sizes;
		std::vector<unsigned> strides;

		for (int streamID = 0; streamID < mesh->GetNumStreams(); streamID++) {
			auto vb = mesh->GetVertexBuffer(streamID);
			auto ptr = vb.get();
			vertexBuffers.push_back(mesh->GetVertexBuffer(streamID).get());
			sizes.push_back((unsigned)vertexBuffers.back()->GetSize());
			strides.push_back((unsigned)mesh->GetVertexBufferStride(streamID));
		}

		assert(vertexBuffers.size() == sizes.size());
		assert(sizes.size() == strides.size());

		commandList.SetPipelineState(m_PSO.get());
		commandList.SetGraphicsBinder(&m_binder);
		commandList.SetPrimitiveTopology(gxapi::ePrimitiveTopology::TRIANGLELIST);
		commandList.SetVertexBuffers(0, (unsigned)vertexBuffers.size(), vertexBuffers.data(), sizes.data(), strides.data());
		commandList.SetIndexBuffer(mesh->GetIndexBuffer().get(), mesh->GetIndexBuffer32Bit());
		commandList.DrawIndexedInstanced((unsigned)mesh->GetIndexBuffer()->GetIndexCount());
	}
}



} // namespace nodes
} // namespace gxeng
} // namespace inl