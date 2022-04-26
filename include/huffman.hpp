#include <iostream>
#include <queue>
#include <memory>
#include <fstream>
#include <unordered_map>

typedef std::unordered_map<uint8_t, uint32_t>::iterator map_it_t;
typedef std::unique_ptr<struct Node> Node_ptr;
typedef std::priority_queue<Node_ptr, std::vector<Node_ptr>, struct Node_cmp>
		Huffman_tree;
struct Node {
	uint8_t val;
	uint32_t count;
	Node_ptr left;
	Node_ptr right;

	//constructor for the occurence map
	Node(const uint8_t &_val, const uint32_t &_count)
			: val(_val), count(_count)
	{}

	//constructor of artificial Nodes
	Node(Node *const first, Node *const second)
			: val('*'), count(first->count + second->count)
	{}

};

//creating a custom comparator for 
struct Node_cmp {
	bool operator()(const Node_ptr &left,
					const Node_ptr &right) {
		if (left->count < right->count)
			return false;
		return true;
	}
};

void generate_map_from_file(
		std::unordered_map<uint8_t, uint32_t> &map,
		std::ifstream &input_file);

