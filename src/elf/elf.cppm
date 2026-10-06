export module glue.elf;

import std;
import :types;

export namespace glue::elf {

class Section {
 public:
  explicit Section(const SectionHeader& section_header,
                   const std::span<const std::byte> data)
      : m_section_header{section_header}, m_data{data} {};
  auto data() -> std::string_view;
  auto name() -> std::string_view;

 private:
  const SectionHeader m_section_header;
  const std::span<const std::byte> m_data;
};

class Elf {
 public:
  Elf() = delete;

  Elf(Elf&) = delete;
  Elf& operator=(Elf&) = delete;

  Elf(Elf&&) = default;
  explicit Elf(const ElfHeader elf_header,
               const std::span<const std::byte> data)
      : m_elf_header{elf_header}, m_data{data} {};

  auto section(std::string_view section) -> std::optional<Section>;

 private:
  const ElfHeader m_elf_header;
  const std::span<const std::byte> m_data;
};

auto create(const std::span<const std::byte> data)
    -> std::expected<Elf, std::string>;

}  // namespace glue::elf
