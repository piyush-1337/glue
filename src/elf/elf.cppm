export module glue.elf;

import std;
import :types;

export namespace glue::elf {

auto parse_header(const std::span<const std::byte> data)
    -> std::optional<ElfHeader> {
  if (data.size() < sizeof(ElfHeader)) return std::nullopt;

  ElfHeader header;
  std::memcpy(&header, data.data(), sizeof(ElfHeader));

  if (std::memcmp(ELF_MAGIC.data(), header.ident, 4) != 0) {
    return std::nullopt;
  }

  return header;
}

}  // namespace glue::elf
