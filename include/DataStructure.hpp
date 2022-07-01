#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <bitset>

#define HEAP_CAPACITY 512
#define EMPTY_BUFFER 0
#define NULL_CHAR 0
#define BYTE_SIZE 8

namespace huffman {
using Node    = struct Node;
using NodePtr = std::unique_ptr<Node>;

enum class Side {
  kLeft,
  kRight
};

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
  CodeBuffer()
      : code_size(0)
      , batch({EMPTY_BUFFER})
  {}

  CodeBuffer(CodeBuffer const& code_buffer)
      : code_size(code_buffer.code_size)
      , batch(code_buffer.batch)
  {}

  void insert_bit(size_t bit_index);
  void display();

  static size_t get_batch_index(size_t bit_index);
  static size_t get_byte_index(size_t bit_index);

  size_t code_size;
  std::vector<uint8_t> batch;
};

void CodeBuffer::insert_bit(size_t bit_index) {
  if (get_batch_index(bit_index) >= batch.size())
    batch.emplace_back(EMPTY_BUFFER);

  batch.at(get_batch_index(bit_index))
      |= (1 << get_byte_index(bit_index));
  
  ++code_size;
}

inline size_t CodeBuffer::get_batch_index(size_t bit_index) {
  return bit_index / BYTE_SIZE;
}

inline size_t CodeBuffer::get_byte_index(size_t bit_index) {
  return bit_index % BYTE_SIZE;
}

void CodeBuffer::display() {
  for (uint8_t b : batch) {
    std::bitset<8> b_set {b};
    std::cout << b_set << " ";
  }
  std::cout << std::endl;
}
}//namespace

