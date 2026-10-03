export module glue.cli;

import std;
import cli11;

export namespace glue::cli {

enum class Command {
  INSPECT,
  NONE,
};

enum class Target {
  ELF,
  NONE,
};

struct CliConfig {
  Command cmd = Command::NONE;
  Target target = Target::NONE;
  std::filesystem::path path = "";
};

auto parse(int argc, char* argv[]) -> std::expected<CliConfig, int> {
  CliConfig config;

  auto app = CLI::App{"", "glue"};
  app.require_subcommand(1);

  auto inspect = app.add_subcommand("inspect", "inspect a target file");
  inspect->require_subcommand(1);
  inspect->callback([&]() { config.cmd = Command::INSPECT; });

  auto elf = inspect->add_subcommand("elf", "inspect elf file");
  elf->callback([&]() { config.target = Target::ELF; });

  elf->add_option("file", config.path, "target elf file")
      ->required()
      ->check(CLI::ExistingFile);

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& e) {
    return std::unexpected(app.exit(e));
  }

  return config;
}

}  // namespace glue::cli

namespace std {

using namespace glue::cli;

template <>
struct std::formatter<Command> : std::formatter<std::string_view> {
  auto format(Command cmd, std::format_context& ctx) const {
    std::string_view name = "UNKNOWN";
    switch (cmd) {
      case Command::INSPECT:
        name = "INSPECT";
        break;
      case Command::NONE:
        name = "NONE";
        break;
    }
    return std::formatter<std::string_view>::format(name, ctx);
  }
};

template <>
struct std::formatter<Target> : std::formatter<std::string_view> {
  auto format(Target target, std::format_context& ctx) const {
    std::string_view name = "UNKNOWN";
    switch (target) {
      case Target::ELF:
        name = "ELF";
        break;
      case Target::NONE:
        name = "NONE";
        break;
    }
    return std::formatter<std::string_view>::format(name, ctx);
  }
};

}  // namespace std
