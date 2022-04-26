#include "huffman.hpp"

void display_map(const std::unordered_map<uint8_t, uint32_t> &m) {
	for (auto it : m) {
		std::cout << it.first << " : " << it.second << '\n';
	}
}

void display_queue(Huffman_tree &queue) {
	while (!queue.empty()) {
		std::cout << (int)queue.top()->val 
				  << " : " 
				  << queue.top()->count
				  << std::endl;
		queue.pop();
	}
}

void generate_map_from_file(
		std::unordered_map<uint8_t, uint32_t> &map,
		std::ifstream &input_file) {
	std::pair<map_it_t, bool> it;
	char current_char;
	while (!input_file.eof()) {
		input_file.get(current_char);
		it = map.emplace(unsigned(current_char), 0);
		++it.first->second;
	}
}
int main(int argc, char *argv[]) {
	//opening the input file
	std::ifstream input_file;
	input_file.open(argv[1], std::ifstream::in);
	if (!input_file.good()) {
		std::cout << "Error while opening the file.\n";
		return -1;
	}

	//generating the occurence map from the file
	std::unordered_map<uint8_t, uint32_t> occurence_map;
	generate_map_from_file(occurence_map, input_file);

	//creating the priority_queue
	Huffman_tree queue;
	for (auto el : occurence_map) {
		Node_ptr new_node = std::make_unique<struct Node>(el.first, el.second);
		queue.push(std::move(new_node));	
	}
	display_queue(queue);
	return 0;
}
