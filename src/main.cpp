import std;
import glue;

auto main(int argc, char* argv[]) -> int {
  auto config = glue::cli::parse(argc, argv);
  if (!config) {
    return config.error();
  }

  std::println("cmd: {}, target: {}, file: {}", config->cmd, config->target,
               config->path.string());
}
