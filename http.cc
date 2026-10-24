#include "http.h"

#include <time.h>

#include <cstdint>
#include <limits>
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

void SendActiveZones() {
  JsonDocument document;
  JsonArray zones = document["active_zones"].to<JsonArray>();
  for (bool active : GetActiveZones()) zones.add(active);
  String body;
  serializeJson(document, body);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body);
}

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

bool ReadObject(JsonDocument& document) {
  if (deserializeJson(document, server.arg("plain")) ||
      !document.is<JsonObject>()) {
    server.send(400, "application/json",
                "{\"error\":\"Expected a JSON object\"}");
    return false;
  }
  return true;
}

void OverrideZone(Settings& settings) {
  JsonDocument document;
  if (!ReadObject(document)) return;
  JsonObject object = document.as<JsonObject>();
  if (object.size() != 3 || !object["zone"].is<int>() ||
      object["zone"].as<int>() < 1 || object["zone"].as<int>() > kZones ||
      !object.containsKey("override") ||
      (!object["override"].isNull() && !object["override"].is<bool>())) {
    server.send(400, "application/json",
                "{\"error\":\"Expected zone (1-8), override (boolean or null), "
                "and zone_overrides_end\"}");
    return;
  }
  JsonVariant end = object["zone_overrides_end"];
  const time_t now = time(nullptr);
  if (!end.is<std::int64_t>() || end.as<std::int64_t>() <= now ||
      end.as<std::int64_t>() <= 0 ||
      end.as<std::int64_t>() > std::numeric_limits<time_t>::max()) {
    server.send(400, "application/json",
                "{\"error\":\"zone_overrides_end must be a future Unix "
                "timestamp within the controller's time range\"}");
    return;
  }
  // Expired overrides must not become active again when the timeout changes.
  if (settings.zone_overrides_end <= now) settings.zone_overrides = {};
  const int zone = object["zone"].as<int>() - 1;
  if (object["override"].isNull()) {
    settings.zone_overrides[zone].reset();
  } else {
    settings.zone_overrides[zone] = object["override"].as<bool>();
  }
  settings.zone_overrides_end = static_cast<time_t>(end.as<std::int64_t>());
  SendSettings(settings);
}

void SetProgram(Settings& settings) {
  JsonDocument document;
  if (!ReadObject(document)) return;
  JsonObject object = document.as<JsonObject>();
  if (object.size() != 1 || !object["control_program"].is<String>()) {
    server.send(400, "application/json",
                "{\"error\":\"Expected control_program string\"}");
    return;
  }
  settings.control_program = object["control_program"].as<std::string>();
  SendSettings(settings);
}

}  // namespace

void SetupHttp(Settings& settings) {
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html; charset=utf-8", kWebsite);
  });
  server.on("/settings", HTTP_GET, [&settings]() { SendSettings(settings); });
  server.on("/active-zones", HTTP_GET, SendActiveZones);
  server.on("/settings/zone-override", HTTP_PUT,
            [&settings]() { OverrideZone(settings); });
  server.on("/settings/program", HTTP_PUT,
            [&settings]() { SetProgram(settings); });
  server.onNotFound([]() {
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });
  server.begin();
}

void HandleHttp() { server.handleClient(); }

}  // namespace irrigation
