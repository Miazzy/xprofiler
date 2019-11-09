#include "../library/json.hpp"

#include "../logger.h"
#include "../platform/platform.h"
#include "./send.h"

namespace xprofiler {
using nlohmann::json;
using std::exception;
using std::string;

#define HANDLE_CMD(cmd_str, handle)                                            \
  if (strcmp(cmd.c_str(), #cmd_str) == 0)                                      \
    handle(parsed);

static void SendVersion(json command) {
  char version[32];
  snprintf(version, sizeof(version), "%s", XPROFILER_VERSION);
  SendMessageToAgent(version);
}

void ParseCmd(char *command) {
  Debug("parser", "received command: %s", command);
  json parsed;
  try {
    parsed = json::parse(command);
  } catch (exception &e) {
    Error("parser", "parse command error: %s", e.what());
    return;
  }

  string cmd = parsed["cmd"];
  HANDLE_CMD(check_version, SendVersion);
}
} // namespace xprofiler