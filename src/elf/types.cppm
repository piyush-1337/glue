export module glue.elf:types;

import std;

export namespace glue::elf {

constexpr std::string_view ELF_MAGIC = "\x7f" "ELF";

using ElfAddr = std::uint64_t;
using ElfOff = std::uint64_t;
using ElfHalf = std::uint16_t;
using ElfWord = std::uint32_t;
using ElfSword = std::int32_t;
using ElfXword = std::uint64_t;
using ElfSxword = std::int64_t;
using ElfChar = std::uint8_t;

// can directly memcpy, no need to pack, everything is already aligned
struct ElfHeader {
  /// elf identification
  ElfChar ident[16];

  // object file type
  ElfHalf type;

  /// machine type
  ElfHalf machine;

  /// object file version
  ElfWord version;

  /// entry point address
  ElfAddr entry;

  /// program header offset
  ElfOff ph_off;

  /// section header offset
  ElfOff sh_off;

  /// processor specific flags
  ElfWord flags;

  /// elf header size
  ElfHalf header_size;

  /// size of program header entry
  ElfHalf pheader_size;

  /// number of program header entries
  ElfHalf pheader_entries;

  /// size of section header entry
  ElfHalf sh_size;

  /// number of section header entries
  ElfHalf sheader_entries;

  /// section name string table index
  ElfHalf snstidx;
};

}
