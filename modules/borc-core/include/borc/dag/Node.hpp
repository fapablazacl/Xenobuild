
#ifndef __BORC_DAG_NODE_HPP__
#define __BORC_DAG_NODE_HPP__

#include <string>
#include <vector>

namespace borc {
	class Node {
	public:
		explicit Node(const std::string &filePath);
		~Node();

		void addDependency(Node *dependency);

		void removeDependency(Node *child);
		
		std::vector<Node*> getDependencies() const;

		std::string getFilePath() const;
	private:
		std::string filePath;
		std::vector<Node*> dependencies;
	};
}

#endif
