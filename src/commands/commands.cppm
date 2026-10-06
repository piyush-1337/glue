export module glue.commands;

import std;
import glue.cli;
import glue.commands.inspect;

export namespace glue::commands {

auto execute(const std::span<const std::byte> data, const cli::CliConfig& config) -> int {

  switch (config.cmd) {
    case cli::Command::INSPECT: {
      return inspect(data, config.target, config.section_name);
    }
    case cli::Command::NONE: {
      return 1;
    }
  }

}

}
