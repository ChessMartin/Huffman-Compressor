#pragma once

#include <DataStructure.hpp>

#include <unordered_map>
#include <array>
#include <string>

namespace huffman {
using CharMap    = std::array<uint64_t, 256>;
using Dictionary = std::unordered_map<uint8_t, CodeBuffer>;

class Compressor {
  public:
    Compressor() : _occurence_map({0}) {}

    void run(std::string const& infile_path,
             std::string const& outfile_path = "default.huf");

  private:
    CharMap       _occurence_map;
    PriorityQueue _queue;
    NodePtr       _tree;
    Dictionary    _dictionary;

  void generate_dictionary(NodePtr const& node_ptr,
                           batch_t const  batch,
                           size_t  const  depth,
                           Side    const  side);
  };
}//namespace

