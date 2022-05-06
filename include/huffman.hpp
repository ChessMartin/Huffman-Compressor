#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <iostream>
#include <queue>
#include <memory>
#include <fstream>
#include <array>
#include <unordered_map>
#include <string>

class Huffman {
public:
	Huffman();
	
	void compress(std::ifstream &input_file);

private:
	typedef std::array<uint32_t, 256> HashMap;
	HashMap occurence_map;

	typedef std::unordered_map<uint8_t, std::string> Dictionary;
	Dictionary dic;

	struct Node {
		Node(uint8_t c_, uint32_t count_) : c(c_), count(count_) {}
		
		Node(std::shared_ptr<Node> left_, std::shared_ptr<Node> right_) :
			 	c(0),
				count(left_->count + right_->count),
			 	left(left_),
				right(right_) {}

		uint8_t c;
		uint32_t count;
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
	};
	
	struct Node_cmp {
		bool operator()(const std::shared_ptr<Node> &left,
						const std::shared_ptr<Node> &right);
	};

	typedef std::priority_queue<std::shared_ptr<Node>,
						std::vector<std::shared_ptr<Node>>,
						Node_cmp> Queue;
	Queue q;

	void generate_map(std::ifstream &input_file);

	void generate_queue();

	void generate_tree();

	void generate_dictionary(
			const Node *node_ptr,
			const std::string path);
};


//creating a custom comparator for priority queue
bool Huffman::Node_cmp::operator()(
		const std::shared_ptr<Node> &left,
		const std::shared_ptr<Node> &right) {
	if (left->count < right->count)
		return false;
	return true;
}

#endif

