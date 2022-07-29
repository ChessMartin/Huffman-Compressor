#include <Compressor.hpp>

#include <iostream>
#include <fstream>
#include <cstring>

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
  generate_dictionary(_tree->left, {EMPTY_BUFFER}, 0, Side::kLeft);
  generate_dictionary(_tree->right, {EMPTY_BUFFER}, 0, Side::kRight);

  //Encoding Part
  infile.open(infile_path, std::ios::binary);
  if (!infile.good()) {
    std::cout << "Error while opening the input file.\n";
    return;
  }

  std::ofstream outfile(outfile_path, std::ios::binary);
  if (!outfile.good()) {
    std::cout << "Error while opening the output file.\n";
    return;
  }

  for (auto el : _dictionary) {
    display(el.first, el.second);
  }

  BufferWrapper buffer;
  while (!infile.eof()) {
    infile.get(current_char);
    CodeBuffer code = _dictionary.at(current_char);
    std::memcpy(buffer.container + get_byte_index(buffer.bit_size),
                code.batch.data(),
                code.batch.size());
    buffer.bit_index += code.size;
    std::cout << std::endl;
    display(current_char, code);
  }
  infile.close();
  outfile.close();


  std::cout << std::endl;
}

void Compressor::generate_dictionary(NodePtr const& node_ptr,
                                     batch_t        batch,
                                     size_t  const  depth,
                                     Side    const  side) {
  if (node_ptr == nullptr)
    return;

  if (get_byte_index(depth + 1) >= batch.size())
    batch.emplace_back(EMPTY_BUFFER);

  if (side == Side::kRight)
    CodeBuffer::insert_bit_in_code(batch, depth);

  if (node_ptr->c != NULL_CHAR) {
    _dictionary.emplace(node_ptr->c, CodeBuffer(depth + 1, batch));
  }

  generate_dictionary(node_ptr->left, batch,
                      depth + 1, Side::kLeft);
  generate_dictionary(node_ptr->right, batch,
                      depth + 1, Side::kRight);
}
}//namespace

int main(int argc, char *argv[]) {
  huffman::Compressor compressor;
  compressor.run("./data/sample.txt", "./output/default.huf");
  return EXIT_SUCCESS;
}

