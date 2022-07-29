#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <bitset>

#define HEAP_CAPACITY 512
#define BUFFER_SIZE 1024
#define EMPTY_BUFFER 0
#define NULL_CHAR 0
#define BYTE_SIZE 8

namespace huffman {
using Node    = struct Node;
using NodePtr = std::unique_ptr<Node>;
using batch_t = std::vector<uint8_t>;

enum class Side {
  kLeft,
  kRight
};

inline size_t get_byte_index(size_t bit_index) {
  return bit_index / BYTE_SIZE;
}

inline size_t get_bit_index(size_t bit_index) {
  return bit_index % BYTE_SIZE;
}

struct Node {
  Node(uint8_t c_, uint64_t count_)
       : c(c_)
       , count(count_)
       , left(nullptr)
       , right(nullptr)
  {}

  Node(NodePtr left_, NodePtr right_)
       : c(NULL_CHAR)
       , count(left_->count + right_->count)
       , left(std::move(left_))
       , right(std::move(right_))
  {}

  uint8_t c;
  uint64_t count;
  NodePtr left;
  NodePtr right;
};

struct NodeComp {
  bool operator()(std::unique_ptr<Node> const& left,
                  std::unique_ptr<Node> const& right) {
    return left->count > right->count;
  }
};

struct PriorityQueue {
  PriorityQueue() {
    _heap.reserve(HEAP_CAPACITY);
  }

  size_t size() const;
  void push(NodePtr node_ptr);
  NodePtr pop();

  private:
    std::vector<NodePtr> _heap;
    NodeComp _cmp;
};

size_t PriorityQueue::size() const {
  return _heap.size();
}

void PriorityQueue::push(NodePtr node_ptr) {
  _heap.push_back(std::move(node_ptr));
  std::push_heap(_heap.begin(), _heap.end(), _cmp);
}

NodePtr PriorityQueue::pop() {
  std::pop_heap(_heap.begin(), _heap.end(), _cmp);
  NodePtr top = std::move(_heap.back());
  _heap.pop_back();
  return top;
}

struct CodeBuffer {
  CodeBuffer(size_t  const  depth,
             batch_t const& batch_)
      : size(depth + 1)
      , batch(batch_)
  {}

  static void insert_bit_in_code(batch_t& batch, size_t bit_index);

  size_t  size;
  batch_t batch;
};

void CodeBuffer::insert_bit_in_code(batch_t& batch, size_t bit_index) {
  batch.at(get_byte_index(bit_index))
      |= (1 << get_bit_index(bit_index));
}


struct BufferWrapper {
  char   container[BUFFER_SIZE] = {EMPTY_BUFFER};
  size_t bit_size   = 0;
  size_t bit_margin = 0;
};

void display(uint8_t c, CodeBuffer code) {
  std::cout << (char)signed(c) << " :: ";
  for (auto it = code.batch.rbegin(); it != code.batch.rend(); ++it) {
    std::bitset<8> b_set {*it};
    std::cout << b_set << " ";
  }
  std::cout << " :: " << signed(code.size);
  std::cout << std::endl;
}
}//namespace

