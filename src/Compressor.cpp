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
    uint64_t value = _occurence_map.at(key);
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
  CodeBuffer code_buffer{};
  generate_dictionary(_tree, code_buffer,
                      TREE_TOP_INDEX, Side::kLeft);
  for (auto &el : _dictionary) {
    el.second.display();
  }
}

void Compressor::generate_dictionary(NodePtr const& node_ptr,
                                     CodeBuffer     code_buffer,
                                     int     const  depth,
                                     Side    const  side) {
  if (node_ptr == nullptr)
    return;

  if (side == Side::kRight)
    code_buffer.insert_bit(unsigned(depth));

  if (node_ptr->c != NULL_CHAR) {
    _dictionary.emplace(node_ptr->c, code_buffer);
  }

  generate_dictionary(node_ptr->left, code_buffer,
                      depth + 1, Side::kLeft);
  generate_dictionary(node_ptr->right, code_buffer,
                      depth + 1, Side::kRight);
}
}//namespace

int main(int argc, char *argv[]) {
  huffman::Compressor compressor;
  compressor.run("./data/sample.txt", "./output/default.huf");
  return EXIT_SUCCESS;
}

