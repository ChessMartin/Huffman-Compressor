#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#define HEAP_CAPACITY 512
#define EMPTY_BUFFER 0
#define NULL_CHAR 0
#define BYTE_SIZE 8

namespace huffman {
using Node    = struct Node;
using NodePtr = std::unique_ptr<Node>;

enum clase Side {
  kLeft  = 0,
  kRight = 1
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
  static size_t get_buffer_index(size_t&& bit_index);

  CodeBuffer(size_t&& code_len)
      : code_size(code_len)
      , batch(get_buffer_index(code_len), EMPTY_BUFFER)
  {}

  void push_bit(size_t&& bit_index);

  size_t code_size;
  std::vector<uint8_t> batch;
};

inline size_t&& CodeBuffer::get_buffer_index(size_t&& bit_index) {
  return std::move( bit_index % (BYTE_SIZE - 1) == 0
                  ? bit_index /  BYTE_SIZE
                  : bit_index /  BYTE_SIZE + 1);
}

void CodeBuffer::push_bit(size_t&& bit_index) {
  batch.at(get_buffer_index(bit_index)) |= 1 << bit_index;
}
}//namespace

