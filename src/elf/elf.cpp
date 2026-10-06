module glue.elf;
import std;

namespace glue::elf {

auto parse_elf_header(const std::span<const std::byte> data)
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
                        const ElfHeader& elf_header, ElfWord name)
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

auto create(const std::span<const std::byte> data)
    -> std::expected<Elf, std::string> {
  auto elf_header = parse_elf_header(data);
  if (!elf_header) return std::unexpected("couldn't parse elf header");
  auto elf = Elf(*elf_header, data);
  return elf;
}

auto Elf::section(std::string_view section) -> std::optional<Section> {
  auto offset = m_elf_header.sh_off;

  for (auto i{0}; i < m_elf_header.sheader_entries; ++i) {
    auto section_header = parse_section_header(m_data, offset);
    auto section_name =
        parse_section_name(m_data, m_elf_header, section_header->name);
    if (section_name == section) {
      return Section(*section_header, m_data);
    }
  }
  return std::nullopt;
}

auto Section::data() -> std::string_view {
  auto offset = m_section_header.offset;
  return {reinterpret_cast<const char*>(m_data.data() + offset),
          m_section_header.size};
}

}  // namespace glue::elf
