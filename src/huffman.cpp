#include "huffman.hpp"

Huffman::Huffman() {
	occurence_map = {0};
}

void Huffman::generate_map(std::ifstream &input_file) {
	char current_char;
	while (!input_file.eof()) {
		input_file.get(current_char);
		++occurence_map.at(unsigned(current_char));
	}
}

void Huffman::generate_queue() {
	for (size_t i = 0; i < occurence_map.size(); i++) {
		if (occurence_map.at(i) > 0) {
			q.push(std::make_shared<Node>(i, occurence_map.at(i)));
		}
	}
}

void Huffman::generate_tree() {
	while (q.size() > 1) {
		std::shared_ptr<Node> left = q.top();
		q.pop();
		std::shared_ptr<Node> right = q.top();
		q.pop();
		std::shared_ptr<Node> new_node = std::make_shared<Node>(std::move(left),
																std::move(right));
		q.emplace(std::move(new_node));
	}
}

void Huffman::generate_dictionary(
		const Node *node_ptr,
		const std::string path) {
	std::cout << node_ptr->c << " : " << path << std::endl;
	if (node_ptr->c != 0)
		dic.emplace(node_ptr->c, path);
	if (node_ptr->left != NULL)
		generate_dictionary(node_ptr->left.get(), path + "0");
	if (node_ptr->right != NULL)
		generate_dictionary(node_ptr->right.get(), path + "1");
}

void Huffman::compress(std::ifstream &input_file) {
	generate_map(input_file);
	generate_queue();
	generate_tree();
	generate_dictionary(q.top().get(), "1");
}

int main(int argc, char *argv[]) {
	//opening the input file
	std::ifstream input_file;
	input_file.open(argv[1], std::ifstream::in);

	//check if the file was open successfuly
	if (!input_file.good()) {
		std::cout << "Error while opening the file.\n";
		return EXIT_FAILURE;
	}
	Huffman huff;
	huff.compress(input_file);
	return EXIT_SUCCESS;
}

