#include "huffman.hpp"

Huffman::Huffman(const std::string &in_path,
								 const std::string &out_path) {
	occurence_map = {0};
	nb_node = 0;

	input_file.open(in_path, std::ios::in | std::ios::binary);
	if (!input_file.good()) {
		std::cout << "Error while opening the input file.\n";
		return;
	}

	output_file.open(out_path, std::ios::out | std::ios::binary);
	if (!output_file.good()) {
		std::cout << "Error while opening the output file.\n";
		return;
	}
}

Huffman::~Huffman() {
	if (input_file.is_open())
		input_file.close();
	if (output_file.is_open())
		output_file.close();
}

void Huffman::generate_map() {
	char current_char;
	while (!input_file.eof()) {
		input_file.get(current_char);
    ++occurence_map.at(unsigned(current_char));
	}
}

void Huffman::generate_queue() {
	for (size_t i = 0; i < occurence_map.size(); i++) {
		if (occurence_map.at(i) > 0) {
			_queue.push(std::make_shared<Node>(i, occurence_map.at(i)));
		}
	}
}

void Huffman::generate_tree() {
	while (_queue.size() > 1) {
		std::shared_ptr<Node> left = _queue.top();
		_queue.pop();
		std::shared_ptr<Node> right = _queue.top();
		_queue.pop();
		std::shared_ptr<Node> new_node = std::make_shared<Node>(std::move(left),
																														std::move(right));
		_queue.emplace(std::move(new_node));
	}
}

void Huffman::generate_dictionary(const Node *node_ptr,
		                              const std::string code) {
	if (node_ptr->c != 0) {
		_dictionary.emplace(node_ptr->c, code);
		++nb_node;
	}
	if (node_ptr->left != NULL)
		generate_dictionary(node_ptr->left.get(), code + "0");
	if (node_ptr->right != NULL)
		generate_dictionary(node_ptr->right.get(), code + "1");
}

//void Huffman::generate_encoded_file(std::ofstream)

void Huffman::compress() {
	generate_map();
	generate_queue();
	generate_tree();
	generate_dictionary(_queue.top().get(), "");
	for (auto d : _dictionary) {
		std::cout << d.first << " : " << d.second << std::endl;
	}
	write_file();
	std::cout << "this is nb of nodes " << signed(nb_node) << ' ' << sizeof(nb_node)<< std::endl;
}

void Huffman::publish(const std::bitset<64> &buffer) {
	uint32_t n = buffer.to_ulong();
	std::cout << std::bitset<16>(n) << ' ';
	const char *test = reinterpret_cast<const char *>(&n);
	unsigned long l = 0;
	for (unsigned long i = 0; i < 8; ++i) {
		l = l | ((unsigned long)test[i] << (i * (unsigned long)8));
		std::cout << std::bitset<8>((unsigned long)test[i]) << ' ';
	}
	std::cout << std::bitset<16>(l) << std::endl;
	output_file.write(reinterpret_cast<char *const>(&n), 8);
}


void Huffman::write_file() {
	//writing the header of the compressed file
	output_file.put(nb_node);
	//writting the dictionary in the file
	auto it = _dictionary.begin();
	while (it != _dictionary.end()) {
		output_file.put(it->first);
		std::bitset<64> code(it->second);
		publish(code);
		++it;
	}

	input_file.clear();
	input_file.seekg(0, std::ios::beg);

	size_t buffer_size = 64;
	std::bitset<64> buffer;
	size_t cursor = 0;
	char current_char;
	std::string huff_code;
	while (!input_file.eof()) {
		input_file.get(current_char);
		huff_code = _dictionary.at(unsigned(current_char));
		for (char c : huff_code) {
			if (cursor == buffer_size) {
				//publish(buffer);
				buffer.reset();
				cursor = 0;
			}
			buffer[cursor] = to_digit(c);
			++cursor;
		}
	}
	//publish(buffer);
}

void Huffman::decompress() {
	recreate_tree();
}

void Huffman::recreate_tree() {
	char byte_buffer;
	uint32_t code_buffer;
	input_file.get(byte_buffer);

}

int main(int argc, char *argv[]) {
	Huffman huff("./data/sample.txt");
	huff.compress();
	return EXIT_SUCCESS;
}

