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

auto parse_section_header(const std::span<const std::byte> data,
                          std::size_t& offset) -> std::optional<SectionHeader> {
  auto section_header = SectionHeader{};
  std::memcpy(&section_header, data.data() + offset, sizeof(SectionHeader));
  offset += sizeof(SectionHeader);

  return section_header;
}

auto parse_section_name(const std::span<const std::byte> data,
                        ElfHeader& elf_header, ElfWord name)
    -> std::string_view {
  // offset into the string table section header
  auto offset = std::size_t{elf_header.sh_off +
                            (elf_header.snstidx * sizeof(SectionHeader))};
  auto section_header = SectionHeader{};
  std::memcpy(&section_header, data.data() + offset, sizeof(SectionHeader));

  // absolute offset of the contents of this header in file
  offset = section_header.offset + name;

  return std::string_view{reinterpret_cast<const char*>(data.data() + offset)};
}

auto parse_section_data(const std::span<const std::byte> data,
                        const SectionHeader& section_header) -> std::string_view {
  auto offset = std::size_t{section_header.offset};
  return {reinterpret_cast<const char*>(data.data() + offset), section_header.size};
}

}  // namespace glue::elf
