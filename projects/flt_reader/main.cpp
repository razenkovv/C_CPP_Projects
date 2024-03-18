#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>


namespace FLT {

/*
PUSHLV - Push Level
POPLV  - Pop Level
GROUP  - Group
OBJECT - Object
FACE   - Face
LONGID - Long ID
CLRPLT - Color Palette
MATPLT - Material Palette
LGTPLT - Light Source Palette
EPTPLT - Eyepoint and Trackplane Palette
VRXPLT - Vertex Palette
VRXCRN - Vertex with Color and Normal
VRXLST - Vertex List
*/

const std::unordered_map<std::string, uint16_t> opcodes{{"PUSHLV", 0xA},  {"POPLV", 0xB},   {"GROUP", 0x2},   {"FACE", 0x5},   {"CLRPLT", 0x20}, {"MATPLT", 0x71}, {"LGTPLT", 0x66},
                                                        {"EPTPLT", 0x53}, {"VRXPLT", 0x43}, {"VRXCRN", 0x45}, {"OBJECT", 0x4}, {"LONGID", 0x21}, {"VRXLST", 0x48}};

enum class node_t { DB, GROUP, OBJECT, FACE };

const uint32_t id_offset{4};
const uint32_t record_length_offset{2};

const uint32_t color_offset{20};
const uint32_t first_color_palette_offset{132};
const uint32_t color_size{4};

const uint32_t material_offset{30};

const uint32_t id_length{8};

class Reader {
 private:
  std::vector<uint8_t> buffer_;
  uintmax_t file_size_;

 public:
  Reader(const std::filesystem::path& file_path) : file_size_{std::filesystem::file_size(file_path)}, clrplt_offset_{}, matplt_offsets_{}, clrplt_size_{}, matplt_sizes_{} {
    std::ifstream file(file_path, std::ios_base::in | std::ios_base::binary);
    file.unsetf(std::ios::skipws);
    buffer_.reserve(file_size_);
    buffer_.insert(buffer_.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());
    file.close();
  }

  uintmax_t clrplt_offset_;
  std::vector<uintmax_t> matplt_offsets_;
  uintmax_t clrplt_size_;
  std::vector<uintmax_t> matplt_sizes_;

  const std::vector<uint8_t>& buffer() const& { return buffer_; }
  const uintmax_t& file_size() const& { return file_size_; }

  uint8_t get_1b(uintmax_t i) const { return buffer_[i]; }
  uint16_t get_2b(uintmax_t i) const { return static_cast<uint16_t>(buffer_[i + 1]) | (static_cast<uint16_t>(buffer_[i]) << 8); }
  uint32_t get_4b(uintmax_t i) const {
    return static_cast<uint32_t>(buffer_[i + 3]) | (static_cast<uint32_t>(buffer_[i + 2]) << 8) | (static_cast<uint32_t>(buffer_[i + 1]) << 16) |
           (static_cast<uint32_t>(buffer_[i]) << 24);
  }

  void print_2ub(const std::string& opcode, uintmax_t i) {
    uint64_t x = std::bitset<8>(get_1b(i)).to_ulong();
    uint64_t y = std::bitset<8>(get_1b(i + 1)).to_ulong();
    std::cout << std::setw(6) << opcode << " : " << std::setw(6) << std::dec << i << "  :  " << std::setw(6) << std::uppercase << std::hex << i << "  :  " << std::uppercase
              << std::hex << x << y << std::endl;
  }

  std::string get_id(uintmax_t i) {
    std::string id{};
    while (get_1b(i) != '\0') {
      id.push_back(get_1b(i));
      ++i;
    }
    return id;
  }
};

class Node {
 public:
  std::string id_;
  uintmax_t offset_;
  node_t type_;

  Node(const std::string& id, uintmax_t offset, const node_t& type) : id_(id), offset_(offset), type_(type) {}
};

}  // namespace FLT

int main() {
  FLT::Reader reader{"Model_3_ver164.flt"};
  std::vector<FLT::Node> nodes;
  uintmax_t current_offset{};
  uint16_t opcode{};

  nodes.emplace_back(reader.get_id(current_offset + FLT::id_offset), current_offset, FLT::node_t::DB);
  current_offset += reader.get_2b(FLT::record_length_offset);

  // skip everything before first PUSH
  while (true) {
    opcode = reader.get_2b(current_offset);
    if (opcode == FLT::opcodes.at("CLRPLT")) {
      reader.clrplt_offset_ = current_offset;
      reader.clrplt_size_ = reader.get_2b(current_offset + FLT::record_length_offset);
      current_offset += reader.clrplt_size_;
      continue;
    }
    if (opcode == FLT::opcodes.at("MATPLT")) {
      reader.matplt_offsets_.push_back(current_offset);
      auto size = reader.get_2b(current_offset + FLT::record_length_offset);
      reader.matplt_sizes_.push_back(size);
      current_offset += size;
      continue;
    }
    if (opcode == FLT::opcodes.at("LGTPLT")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("EPTPLT")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("VRXPLT")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("VRXCRN")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    break;
  }

  while (true) {
    opcode = reader.get_2b(current_offset);
    if (opcode == FLT::opcodes.at("PUSHLV")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("POPLV")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("GROUP")) {
      nodes.emplace_back(reader.get_id(current_offset + FLT::id_offset), current_offset, FLT::node_t::GROUP);
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("OBJECT")) {
      nodes.emplace_back(reader.get_id(current_offset + FLT::id_offset), current_offset, FLT::node_t::OBJECT);
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("FACE")) {
      nodes.emplace_back(reader.get_id(current_offset + FLT::id_offset), current_offset, FLT::node_t::FACE);
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("VRXLST")) {
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    if (opcode == FLT::opcodes.at("LONGID")) {
      nodes[nodes.size() - 1].id_ = reader.get_id(current_offset + FLT::id_offset);
      current_offset += reader.get_2b(current_offset + FLT::record_length_offset);
      continue;
    }
    break;
  }

  std::cout << "        ID : "
            << "offset" << std::endl;
  std::cout << "-------------------------" << std::endl;
  for (auto& node : nodes)
    std::cout << std::setw(10) << node.id_ << " : " << std::hex << node.offset_ << std::setw(5) << std::dec << " : " << std::setw(5) << node.offset_ << std::endl;

  std::cout << std::endl << std::endl;

  for (auto& node : nodes) {
    if (node.type_ == FLT::node_t::FACE) {
      uint16_t color_name_index = reader.get_2b(node.offset_ + FLT::color_offset);
      int16_t material_index = reader.get_2b(node.offset_ + FLT::material_offset);
      std::cout << node.id_ << std::endl
                << "  Color name index: " << color_name_index << "; Color: " << std::uppercase << std::hex
                << reader.get_4b(reader.clrplt_offset_ + FLT::first_color_palette_offset + FLT::color_size * color_name_index) << std::dec << "; Material index: " << material_index
                << "." << std::endl;
    }
  }
}
