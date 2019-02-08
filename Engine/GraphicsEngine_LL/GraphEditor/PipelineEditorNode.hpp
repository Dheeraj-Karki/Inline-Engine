#include <BaseLibrary/GraphEditor/IGraphEditorNode.hpp>
#include <BaseLibrary/Graph_All.hpp>
#include <BaseLibrary/GraphEditor/GraphParser.hpp>


#undef GetClassName // retarded Windows


namespace inl::gxeng {


class PipelineEditorNode : public IGraphEditorNode {
public:
	PipelineEditorNode() = default;
	PipelineEditorNode(std::unique_ptr<NodeBase> realNode);
	
	std::string GetName() const override;
	void SetName(std::string name) override;
	std::string GetClassName() const override;

	int GetNumInputs() const override;
	int GetNumOutputs() const override;

	bool HasVariableInputs() const override;
	bool HasVariableOutputs() const override;
	void SetNumInputs() override;
	void SetNumOutputs() override;

	std::string GetInputName(int idx) const override;
	std::string GetOutputName(int idx) const override;

	bool HasPortTypes() const override;
	std::type_index GetInputType(int idx) const override;
	std::type_index GetOutputType(int idx) const override;

	std::string GetInputTypeName(int idx) const override;
	std::string GetOutputTypeName(int idx) const override;

	NodeBase* GetRealNode() const;

	void SetMetaData(NodeMetaDescription data) override;
	NodeMetaDescription GetMetaData() const override;
private:
	std::unique_ptr<NodeBase> m_realNode;
	NodeMetaDescription m_metaData;
};



} // namespace inl