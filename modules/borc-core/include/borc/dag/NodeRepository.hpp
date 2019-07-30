
#ifndef __BORC_DAG_NODEREPOSITORY_HPP__
#define __BORC_DAG_NODEREPOSITORY_HPP__

#include <string>
#include <map>
#include <memory>

namespace borc {
	class Node;
	class NodeRepository {
	public:
		NodeRepository();
		~NodeRepository();

		Node* getNode(const std::string &filePath) const;

	private:
		mutable std::map<std::string, std::unique_ptr<Node>> nodeMap;
	};
}

#endif
