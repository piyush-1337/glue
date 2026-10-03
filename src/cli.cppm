export module glue.cli;

import std;
import cli11;

enum class Subcommand {
  INSPECT,
  NONE,
};

enum class Target {
  ELF,
  NONE,
};

struct CliConfig {
  Subcommand cmd = Subcommand::NONE;
  Target target = Target::NONE;
  std::filesystem::path path = "";
};

export namespace glue::cli {

auto parse(int argc, char* argv[]) -> std::expected<CliConfig, int> {
  CliConfig config;

  auto app = CLI::App{"", "glue"};
  app.require_subcommand(1);

  auto inspect = app.add_subcommand("inspect", "inspect a target file");
  inspect->require_subcommand(1);
  inspect->callback([&]() { config.cmd = Subcommand::INSPECT; });

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
struct std::formatter<Subcommand> : std::formatter<std::string_view> {
  auto format(Subcommand cmd, std::format_context& ctx) const {
    std::string_view name = "UNKNOWN";
    switch (cmd) {
      case Subcommand::INSPECT:
        name = "INSPECT";
        break;
      case Subcommand::NONE:
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
