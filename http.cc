#include "http.h"

#include <time.h>

#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>

#include <Arduino.h>
#include <ArduinoJson.h>  // ArduinoJson 7
#include <WebServer.h>

#include "hardware.h"
#include "settings.h"
#include "website.h"

namespace irrigation {
namespace {

WebServer server(80);

void SendSettings(const Settings& settings) {
  JsonDocument document;
  JsonArray zones = document["zone_overrides"].to<JsonArray>();
  for (const auto& mode : settings.zone_overrides) {
    if (mode.has_value()) {
      zones.add(*mode);
    } else {
      zones.add(nullptr);
    }
  }
  document["control_program"] = settings.control_program.c_str();
  document["zone_overrides_end"] = settings.zone_overrides_end;
  String body;
  serializeJson(document, body);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body);
}

void SetSettings(Settings& settings) {
  JsonDocument document;
  if (deserializeJson(document, server.arg("plain")) ||
      !document.is<JsonObject>()) {
    server.send(400, "application/json",
                "{\"error\":\"Expected a JSON object\"}");
    return;
  }
  JsonObject object = document.as<JsonObject>();
  if (object.size() != 3 || !object["control_program"].is<String>() ||
      !object.containsKey("zone_overrides_end") ||
      !object.containsKey("zone_overrides")) {
    server.send(400, "application/json",
                "{\"error\":\"Expected zone_overrides, zone_overrides_end, "
                "and control_program string\"}");
    return;
  }
  JsonVariant end = object["zone_overrides_end"];
  if (!end.is<std::int64_t>() || end.as<std::int64_t>() <= 0 ||
      end.as<std::int64_t>() > std::numeric_limits<time_t>::max()) {
    server.send(400, "application/json",
                "{\"error\":\"zone_overrides_end must be a positive Unix "
                "timestamp in seconds within the controller's time range\"}");
    return;
  }
  JsonVariant zones = object["zone_overrides"];
  std::array<std::optional<bool>, kZones> zone_overrides = {};
  if (!zones.is<JsonArray>() || zones.size() != kZones) {
    server.send(400, "application/json",
                "{\"error\":\"zone_overrides must be an array of eight "
                "booleans or nulls\"}");
    return;
  }
  for (int i = 0; i < kZones; ++i) {
    if (!zones[i].isNull() && !zones[i].is<bool>()) {
      server.send(400, "application/json",
                  "{\"error\":\"Each zone must be a boolean or null\"}");
      return;
    }
    if (!zones[i].isNull()) {
      zone_overrides[i] = zones[i].as<bool>();
    }
  }
  // Commit only after the entire request has been validated.
  settings.control_program = object["control_program"].as<std::string>();
  settings.zone_overrides = zone_overrides;
  settings.zone_overrides_end = static_cast<time_t>(end.as<std::int64_t>());
  SendSettings(settings);
}

}  // namespace

void SetupHttp(Settings& settings) {
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html; charset=utf-8", kWebsite);
  });
  server.on("/settings", HTTP_GET, [&settings]() { SendSettings(settings); });
  server.on("/settings", HTTP_PUT, [&settings]() { SetSettings(settings); });
  server.onNotFound([]() {
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });
  server.begin();
}

void HandleHttp() { server.handleClient(); }

}  // namespace irrigation
