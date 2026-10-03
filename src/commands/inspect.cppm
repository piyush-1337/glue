export module glue.commands.inspect;

import std;
import glue.cli;
import glue.elf;

export namespace glue::commands {

auto inspect(std::span<const std::byte> data, cli::Target target) {
  switch (target) {
    case cli::Target::ELF: {
      auto header = elf::parse_header(data);
      if (!header) {
        std::println("couldn't parse header");
        return;
      }
      std::println("version: {}", header->version);
      break;
    }
    case cli::Target::NONE: {
      break;
    }
  }
}

}  // namespace glue::commands
