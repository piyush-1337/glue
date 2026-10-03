export module glue.commands;

import std;
import glue.cli;
import glue.commands.inspect;

export namespace glue::commands {

auto execute(const std::span<const std::byte> data, const cli::CliConfig& config) -> void {

  switch (config.cmd) {
    case cli::Command::INSPECT: {
      inspect(data, config.target);
      return;
    }
    case cli::Command::NONE: {
      return;
    }
  }

}

}
