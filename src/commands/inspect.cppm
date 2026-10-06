export module glue.commands.inspect;

import std;
import glue.cli;
import glue.elf;

export namespace glue::commands {

auto inspect(std::span<const std::byte> data, cli::Target target,
             std::string_view section_name) -> int {
  switch (target) {
    case cli::Target::ELF: {
      if (section_name.empty()) return 0;
      auto elf = elf::create(data);
      if (!elf) {
        std::println("{}", elf.error());
        return 1;
      };

      auto section = elf->section(section_name);
      if (!section) {
        std::println("couldn't find section: {}", section_name);
        return 1;
      }

      auto data = section->data();
      std::println("{}", data);
    }
    case cli::Target::NONE: {
      break;
    }
  }

  return 0;
}

}  // namespace glue::commands
