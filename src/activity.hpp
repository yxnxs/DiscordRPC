#pragma once

#include "ipc.hpp"
#include <djson/json.hpp>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>

namespace RPC {

namespace detail {

using snowflake = uint64_t;

// secrets technicall works

struct button_t {
  std::string label{};
  std::string url{};
};

struct timestamps_t {
  std::optional<uint64_t> start{};
  std::optional<uint64_t> end{};
};

struct assets_t {
  std::optional<std::string> large_image{};
  std::optional<std::string> large_text{};
  std::optional<std::string> small_image{};
  std::optional<std::string> small_text{};
};

class HandshakeException : public std::runtime_error {
public:
  HandshakeException(const std::string &message)
      : std::runtime_error(message) {}
};

}; // namespace detail

struct Activity {

  Activity &setUrl(std::string_view url);
  Activity &setTimestamps(const detail::timestamps_t &timestamps);
  Activity &setDetails(std::string_view details);
  Activity &setState(std::string_view state);
  Activity &setButton(uint8_t index, const detail::button_t &button);

  static uint64_t time() noexcept;
  static Activity FromFile(const std::filesystem::path &path);

  Message to_message() const;

private:
  dj::Json activity_json{};
};
} // namespace RPC