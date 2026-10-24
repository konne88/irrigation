#include "http.h"

#include <array>
#include <optional>
#include <string>

#include <Arduino.h>
#include <ArduinoJson.h>  // ArduinoJson 7
#include <WebServer.h>

#include "hardware.h"
#include "state.h"
#include "website.h"

namespace irrigation {
namespace {

WebServer server(80);

void SendState(const State& state) {
  JsonDocument document;
  JsonArray zones = document["zone_overrides"].to<JsonArray>();
  for (const auto& mode : state.zone_overrides) {
    if (mode.has_value()) {
      zones.add(*mode);
    } else {
      zones.add(nullptr);
    }
  }
  document["control_program"] = state.control_program.c_str();
  String body;
  serializeJson(document, body);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body);
}

void SetState(State& state) {
  JsonDocument document;
  if (deserializeJson(document, server.arg("plain")) ||
      !document.is<JsonObject>()) {
    server.send(400, "application/json",
                "{\"error\":\"Expected a JSON object\"}");
    return;
  }
  JsonObject object = document.as<JsonObject>();
  if (object.size() != 2 || !object["control_program"].is<String>() ||
      !object.containsKey("zone_overrides")) {
    server.send(400, "application/json",
                "{\"error\":\"Expected zone_overrides and control_program "
                "string\"}");
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
  state.control_program = object["control_program"].as<std::string>();
  state.zone_overrides = zone_overrides;
  SendState(state);
}

}  // namespace

void SetupHttp(State& state) {
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html; charset=utf-8", kWebsite);
  });
  server.on("/state", HTTP_GET, [&state]() { SendState(state); });
  server.on("/state", HTTP_PUT, [&state]() { SetState(state); });
  server.onNotFound([]() {
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });
  server.begin();
}

void HandleHttp() { server.handleClient(); }

}  // namespace irrigation
