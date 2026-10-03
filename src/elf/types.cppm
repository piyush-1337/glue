export module glue.elf:types;

import std;

export namespace glue::elf {

constexpr std::string_view ELF_MAGIC =
    "\x7f"
    "ELF";

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

  /// section header table index of the section containing the section name
  /// string table. If there is no section name string table, this field has the
  /// value SHN_UNDEF.
  ElfHalf snstidx;
};

struct SectionHeader {
  /// contains the offset, in bytes, to the section name, relative to the start
  /// of the section name string table.
  ElfWord name;

  /// section type
  ElfWord type;

  /// section attributes
  ElfXword flags;

  /// contains the virtual address of the beginning of the section in memory. If
  /// the section is not allocated to the memory image of the program, this
  /// field should be zero.
  ElfAddr addr;

  /// contains the offset, in bytes, of the beginning of the section contents in
  /// the file.
  ElfOff offset;

  /// size of this section payload
  ElfXword size;

  /// contains the section index of an associated section. This field is used
  /// for several purposes, depending on the type of section
  ElfWord link;

  /// contains extra information about the section. This field is used for
  /// several purposes, depending on the type of section
  ElfWord info;

  /// contains the required alignment of the section. This field must be a power
  /// of two.
  ElfXword addr_align;

  /// contains the size, in bytes, of each entry, for sections that contain
  /// fixed-size entries. Otherwise, this field contains zero.
  ElfXword entsize;
};

enum class SectionType : ElfWord {
  /// Marks an unused section header
  NULL = 0,

  /// Contains information defined by the program
  PROGBITS = 1,

  /// Contains a linker symbol table
  SYMTAB = 2,

  /// Contains a string table
  STRTAB = 3,

  /// Contains “Rela” type relocation entries
  RELA = 4,

  /// Contains a symbol hash table
  HASH = 5,

  /// Contains dynamic linking tables
  DYNAMIC = 6,

  /// Contains note information
  NOTE = 7,

  /// Contains uninitialized space; does not occupy any space in the file
  NOBITS = 8,

  /// Contains “Rel” type relocation entries
  REL = 9,

  /// Reserved
  SHLIB = 10,

  /// Contains a dynamic loader symbol table
  DYNSYM = 11,

  /// Array of constructors
  INIT_ARRAY = 14,
  /// Array of destructors
  FINI_ARRAY = 15,
  /// Array of pre-constructors
  PREINIT_ARRAY = 16,
  /// Section group
  GROUP = 17,
  /// Extended section indices
  SYMTAB_SHNDX = 18,
  /// RELR relative relocations
  RELR = 19,
  /// Number of defined types.
  NUM = 20,
  /// Start OS-specific.
  LOOS = 0x60000000,
  /// Object attributes.
  GNU_ATTRIBUTES = 0x6ffffff5,
  /// GNU-style hash table.
  GNU_HASH = 0x6ffffff6,
  /// Prelink library list
  GNU_LIBLIST = 0x6ffffff7,
  /// Checksum for DSO content.
  CHECKSUM = 0x6ffffff8,

  /// Sun-specific low bound.
  LOSUNW = 0x6ffffffa,
  SUNW_move = 0x6ffffffa,
  SUNW_COMDAT = 0x6ffffffb,
  SUNW_syminfo = 0x6ffffffc,

  /// Version definition section.
  GNU_verdef = 0x6ffffffd,
  /// Version needs section.
  GNU_verneed = 0x6ffffffe,
  /// Version symbol table.
  GNU_versym = 0x6fffffff,
  /// Sun-specific high bound.
  HISUNW = 0x6fffffff,
  /// End OS-specific type
  HIOS = 0x6fffffff,
  /// Start of processor-specific
  LOPROC = 0x70000000,
  /// End of processor-specific
  HIPROC = 0x7fffffff,
  /// Start of application-specific
  LOUSER = 0x80000000,
  /// End of application-specific
  HIUSER = 0x8fffffff,

};

}  // namespace glue::elf
