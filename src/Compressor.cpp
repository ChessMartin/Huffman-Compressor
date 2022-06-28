#include <Compressor.hpp>

#include <iostream>
#include <fstream>

namespace huffman {
void Compressor::run(std::string const& infile_path,
                     std::string const& outfile_path) {
  std::ifstream infile(infile_path, std::ios::binary);
  if (!infile.good()) {
    std::cout << "Error while opening the input file.\n";
    return;
  }

  //generate occurence map from file
  char current_char;
  while (!infile.eof()) {
    infile.get(current_char);
    ++_occurence_map.at(static_cast<uint8_t>(current_char));
  }
  infile.close();

  //generate priority queue from occurence map
  for (size_t key = 0; key < _occurence_map.size(); key++) {
    uint8_t value = _occurence_map.at(key);
    if (value > 0) {
      _queue.push(std::move(std::make_unique<Node>(key, value)));
    }
  }

  //generate huffman tree from priority queue
  while (_queue.size() > 1) {
    NodePtr left  = _queue.pop();
    NodePtr right = _queue.pop();
    auto internal_node = std::make_unique<Node>(std::move(left),
                                                std::move(right));
    _queue.push(std::move(internal_node));
  }
  _tree = _queue.pop();

  //generate dictionary recursively throughout the tree
  generate_dictionary(_tree, TREE_TOP_INDEX);
}

void Compressor::generate_dictionary(NodePtr const&  node_ptr,
                                     size_t  const&& depth,
                                     Side    const&& side) {
  if (node_ptr == nullptr)
    return;

  if (node_ptr->c != NULL_CHAR && depth > TREE_TOP_INDEX) {
    auto code_buffer = CodeBuffer(node_ptr->depth);
    for (size_t i = 0; i < code_buffer.bit_size; i++) {
      code_buffer.push(CodeBuffer::get_buffer_index(index));
    }
  }

  generate_dictionary(node_ptr->left, depth + 1);
  generate_dictionary(node_ptr->right, depth + 1);
}
}//namespace

int main(int argc, char *argv[]) {
  huffman::Compressor compressor;
  compressor.run("./data/sample.txt", "./output/default.huf");
  return EXIT_SUCCESS;
}

