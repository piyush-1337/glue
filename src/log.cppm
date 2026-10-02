export module glue.log;

import std;

export namespace glue {

auto log() -> void {
  std::println("[glue]: modules are working");
}

}  // namespace glue
