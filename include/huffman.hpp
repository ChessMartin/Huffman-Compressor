#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <iostream>
#include <queue>
#include <memory>
#include <fstream>
#include <array>
#include <unordered_map>
#include <string>
#include <bitset>

#define HEADER_SIZE 1
#define CODE_BUFFER_SIZE 8

#define to_digit(c) (c-'0')

typedef std::array<uint32_t, 256> HashMap;

class Huffman {
public:
  Huffman(const std::string &in_path = "default.txt",
          const std::string &out_path = "default.huf");

  ~Huffman();

  void compress();

  void decompress();

private:
  std::fstream input_file;
  std::fstream output_file;

  HashMap occurence_map;

  std::unordered_map<uint8_t, std::string> _dictionary;

  uint8_t nb_node;
  struct Node;

  struct Node_cmp {
    bool operator()(const std::shared_ptr<Node> &left,
                    const std::shared_ptr<Node> &right);
  };

  std::priority_queue<
          std::shared_ptr<Node>,
          std::vector<std::shared_ptr<Node>>,
          Node_cmp
      > _queue;

//encrypting
  void generate_map();

  void generate_queue();

  void generate_tree();

  void generate_dictionary(const Node *node_ptr,
                           const std::string code);

  void publish(const std::bitset<64> &buffer);

  void write_file();

//decrypting
  void recreate_tree();

  void add_branch_rec(const std::shared_ptr<Node> &current_node,
                      const uint8_t &c,
                      const std::string &code,
                      uint32_t depth);

  std::string get_formated_path(const std::string &buffer);
};

struct Huffman::Node {
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

//creating a custom comparator for priority queue
bool Huffman::Node_cmp::operator()(
    const std::shared_ptr<Node> &left,
    const std::shared_ptr<Node> &right) {
  if (left->count < right->count)
    return false;
  return true;
}

#endif

