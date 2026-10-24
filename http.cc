#include "http.h"

#include <ArduinoJson.h>  // ArduinoJson 7
#include <WebServer.h>

#include "website.h"

namespace {
WebServer server(80);
http_state state;

void send_state() {
  JsonDocument document;
  JsonArray zones = document["active_zones"].to<JsonArray>();
  for (const auto& mode : state.active_zones) {
    if (mode.has_value()) zones.add(*mode);
    else zones.add(nullptr);
  }
  document["control_program"] = state.control_program;
  String body;
  serializeJson(document, body);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body);
}

void set_state() {
  JsonDocument document;
  if (deserializeJson(document, server.arg("plain")) ||
      !document.is<JsonObject>()) {
    server.send(400, "application/json", "{\"error\":\"Expected a JSON object\"}");
    return;
  }
  JsonObject object = document.as<JsonObject>();
  if (object.size() != 2 || !object["control_program"].is<String>() ||
      !object.containsKey("active_zones")) {
    server.send(400, "application/json",
                "{\"error\":\"Expected active_zones and control_program string\"}");
    return;
  }
  JsonVariant zones = object["active_zones"];
  std::array<std::optional<bool>, ZONES> active_zones = {};
  if (!zones.is<JsonArray>() || zones.size() != ZONES) {
    server.send(400, "application/json",
                "{\"error\":\"active_zones must be an array of eight booleans or nulls\"}");
    return;
  }
  for (int i = 0; i < ZONES; ++i) {
    if (!zones[i].isNull() && !zones[i].is<bool>()) {
      server.send(400, "application/json",
                  "{\"error\":\"Each zone must be a boolean or null\"}");
      return;
    }
    if (!zones[i].isNull()) {
      active_zones[i] = zones[i].as<bool>();
    }
  }
  // Commit only after the entire request has been validated.
  state.control_program = object["control_program"].as<String>();
  state.active_zones = active_zones;
  send_state();
}
}  // namespace

void setup_http() {
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html; charset=utf-8", WEBSITE);
  });
  server.on("/state", HTTP_GET, send_state);
  server.on("/state", HTTP_PUT, set_state);
  server.onNotFound([]() {
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });
  server.begin();
}

void handle_http() { server.handleClient(); }

const http_state& get_http_state() { return state; }
