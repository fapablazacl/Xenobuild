
#ifndef __BORC_DAG_BUILDGRAPHGENERATOR_HPP__
#define __BORC_DAG_BUILDGRAPHGENERATOR_HPP__

namespace borc {
	class Toolchain;
	class Artifact;
	class Source;
	class Node;
	class NodeRepository;
	class BuildGraphGenerator {
	public:
		explicit BuildGraphGenerator(NodeRepository *nodeRepository, const Toolchain *toolchain);

		Node* generateGraph(const Artifact *artifact) const;

	private:
		Node* generateGraph(const Source *source) const;

	private:
		NodeRepository *nodeRepository = nullptr;
		const Toolchain *toolchain = nullptr;
	};
}

#endif
