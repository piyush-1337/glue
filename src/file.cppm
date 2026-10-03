module;
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

export module glue.file;

import std;

export namespace glue::file {

class File {
 public:
  explicit File(std::byte* ptr, std::size_t size) : m_ptr{ptr}, m_size{size} {};

  // no copy
  File(File& other) = delete;
  File& operator=(File& other) = delete;

  // move constructor
  File(File&& other) noexcept : m_ptr{other.m_ptr}, m_size{other.m_size} {
    other.m_ptr = nullptr;
    other.m_size = 0;
  }
  
  // move assignment
  File& operator=(File&& other) noexcept {
    if (this != &other) {
      if (m_ptr != nullptr) {
        munmap(m_ptr, m_size);
      }

      m_ptr = other.m_ptr;
      m_size = other.m_size;

      other.m_ptr = nullptr;
      other.m_size = 0;
    }

    return *this;
  }

  auto data() -> std::span<const std::byte> { return {m_ptr, m_size}; }

  ~File() {
    if (m_ptr != nullptr) {
      ::munmap(m_ptr, m_size);
    }
  }

 private:
  std::byte* m_ptr;
  std::size_t m_size;
};

auto open(const std::filesystem::path& path)
    -> std::expected<File, std::string> {
  // no path.string().c_str() reduce heap allocation but wont work on widnows
  // can do condition compilation instead, using #ifdef and wopen for windows
  auto fd = ::open(path.c_str(), O_RDONLY);
  if (!fd) return std::unexpected("failed to open file");

  struct stat sb;
  if (::fstat(fd, &sb) == -1) {
    ::close(fd);
    return std::unexpected("Failed to read file stats");
  }

  auto size = sb.st_size;
  if (size == 0) return std::unexpected("empty file");

  auto ptr = reinterpret_cast<std::byte*>(
      ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));

  if (ptr == MAP_FAILED) {
    ::close(fd);
    return std::unexpected("Failed to map file into memory");
  }

  ::close(fd);

  return File(ptr, size);
}

}  // namespace glue::file
