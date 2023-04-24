#include "activity.hpp"
#include "djson/json.hpp"
#include "ipc.hpp"

#include <chrono>
#include <processthreadsapi.h>
#include <random>
#include <sstream>
#include <tuple>

using namespace RPC;

std::string nonce() {
  static auto generator = std::mt19937{std::random_device{}()};
  static auto distribution = std::uniform_int_distribution<>{0, 15};

  static constexpr auto map = std::string_view{"0123456789abcdef"};

  auto tunnel = std::stringstream{};
  for (int i = 0; i < 32; i++) {
    if (i == 8 || i == 12 || i == 16 || i == 20)
      tunnel << '-';

    if (i == 12)
      tunnel << '4';
    else
      tunnel << map[distribution(generator)];
  }

  return tunnel.str();
}

Activity &Activity::setTimestamps(const detail::timestamps_t &timestamps) {

  if (timestamps.start || timestamps.end) {
    auto &ref = activity_json["timestamps"];

    if (timestamps.start)
      ref["start"] = *timestamps.start;

    if (timestamps.end)
      ref["end"] = *timestamps.end;
  }
  return *this;
}

Activity &Activity::setDetails(std::string_view details) {
  activity_json["details"] = details;
  return *this;
}

Activity &Activity::setState(std::string_view state) {
  activity_json["state"] = state;
  return *this;
}

Activity &Activity::setButton(uint8_t index, const detail::button_t &button) {

  if (index > 1)
    throw std::runtime_error("Button index must be either 0 or 1");

  auto &ref = activity_json["buttons"][index];
  ref["label"] = button.label;
  ref["url"] = button.url;

  return *this;
}

uint64_t Activity::time() noexcept {
  return std::chrono::duration_cast<std::chrono::seconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

Activity Activity::FromFile(const std::filesystem::path &path) {
  auto result = Activity{};
  result.activity_json = dj::Json::from_file(path.string().c_str());

  return result;
}

Message Activity::to_message() const {

  static const auto PID = GetCurrentProcessId();
  auto message_json = dj::Json{};
  {
    message_json["cmd"] = "SET_ACTIVITY";
    message_json["nonce"] = nonce();

    message_json["args"]["pid"] = GetCurrentProcessId();
    message_json["args"]["activity"] = activity_json;
  }

  return Message::Create(message_json.serialized(0));
}
