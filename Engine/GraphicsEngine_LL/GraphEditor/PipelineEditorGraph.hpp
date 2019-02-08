#include "../GraphicsNodeFactory.hpp"

#include <BaseLibrary/GraphEditor/IEditorGraph.hpp>
#include <BaseLibrary/Graph_All.hpp>

#include "PipelineEditorNode.hpp"


namespace inl::gxeng {


class PipelineEditorGraph : public IEditorGraph {
public:
	PipelineEditorGraph(const GraphicsNodeFactory& factory);

	std::vector<std::string> GetNodeList() const override;

	IGraphEditorNode* AddNode(std::string name) override;
	void RemoveNode(IGraphEditorNode* node) override;

	inl::Link Link(IGraphEditorNode* sourceNode, int sourcePort, IGraphEditorNode* targetNode, int targetPort) override;
	void Unlink(IGraphEditorNode* targetNode, int targetPort) override;

	std::vector<IGraphEditorNode*> GetNodes() const override;
	std::vector<inl::Link> GetLinks() const override;

	void Validate() override;
	std::string SerializeJSON() override;
	void LoadJSON(const std::string& description) override;

	const std::string& GetContentType() const override;
	void Clear() override;

private:
	const GraphicsNodeFactory& m_factory;
	std::vector<std::unique_ptr<PipelineEditorNode>> m_nodes;
};



} // namespace inl::gxeng