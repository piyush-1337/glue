import std;
import glue;

auto main(int argc, char* argv[]) -> int {
  auto config = glue::cli::parse(argc, argv);
  if (!config) {
    return config.error();
  }

  auto file = glue::file::open(config->path);
  if (!file) {
    std::println("{}", file.error());
    return 1;
  }

  return glue::commands::execute(file->data(), *config);
}
