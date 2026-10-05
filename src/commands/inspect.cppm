export module glue.commands.inspect;

import std;
import glue.cli;
import glue.elf;

export namespace glue::commands {

auto inspect(std::span<const std::byte> data, cli::Target target) {
  switch (target) {
    case cli::Target::ELF: {
      auto elf_header = elf::parse_header(data);
      if (!elf_header) {
        std::println("couldn't parse elf header");
        return;
      }

      auto offset = elf_header->sh_off;
      for (std::size_t i{}; i < elf_header->sheader_entries; ++i) {
        if (i + 1 == elf_header->snstidx) continue;
        auto section_header = elf::parse_section_header(data, offset);
        if (!section_header) {
          std::println("couldn't parse section header");
          return;
        }

        std::println("{}", elf::parse_section_name(data, *elf_header,
                                                   section_header->name));
        std::println("Section data:");
        std::println("{}", elf::parse_section_data(data, *section_header));
      }
      break;
    }
    case cli::Target::NONE: {
      break;
    }
  }
}

}  // namespace glue::commands
