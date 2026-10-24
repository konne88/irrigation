#include "control.h"

#include <time.h>

#include <array>
#include <cstdio>
#include <string>

#include "hardware.h"
#include "src/lua/lua.hpp"

namespace irrigation {
namespace {

struct Context {
  const std::string& program;
  const tm& time;
  long moisture;
  long flow;
  std::array<bool, kZones> active_zones = {};
};

void SetInteger(lua_State* lua, const char* name, long value) {
  lua_pushinteger(lua, value);
  lua_setglobal(lua, name);
}

int RunControl(lua_State* lua) {
  auto& context = *static_cast<Context*>(lua_touserdata(lua, 1));
  const tm& time = context.time;
  SetInteger(lua, "year", time.tm_year + 1900);
  SetInteger(lua, "month", time.tm_mon + 1);
  SetInteger(lua, "day", time.tm_mday);
  SetInteger(lua, "hour", time.tm_hour);
  SetInteger(lua, "minute", time.tm_min);
  SetInteger(lua, "second", time.tm_sec);
  SetInteger(lua, "weekday", (time.tm_wday + 6) % 7);  // Monday = 0.
  SetInteger(lua, "yearday", time.tm_yday + 1);
  SetInteger(lua, "isdst", time.tm_isdst);
  SetInteger(lua, "moisture", context.moisture);
  SetInteger(lua, "flow", context.flow);

  lua_createtable(lua, kZones, 0);
  for (int i = 1; i <= kZones; ++i) {
    lua_pushboolean(lua, false);
    lua_rawseti(lua, -2, i);
  }
  lua_setglobal(lua, "active_zones");

  if (luaL_loadbufferx(lua, context.program.data(), context.program.size(),
                       "control", "t") != LUA_OK) {
    return lua_error(lua);
  }
  lua_call(lua, 0, 0);

  lua_getglobal(lua, "active_zones");
  luaL_checktype(lua, -1, LUA_TTABLE);
  if (lua_rawlen(lua, -1) != kZones) {
    return luaL_error(lua, "active_zones must have eight entries");
  }
  for (int i = 0; i < kZones; ++i) {
    lua_rawgeti(lua, -1, i + 1);
    luaL_checktype(lua, -1, LUA_TBOOLEAN);
    context.active_zones[i] = lua_toboolean(lua, -1);
    lua_pop(lua, 1);
  }
  return 0;
}

}  // namespace

std::array<bool, kZones> Control(const std::string& program, const tm& time,
                                 long moisture, long flow) {
  Context context{program, time, moisture, flow};
  lua_State* lua = luaL_newstate();
  if (lua == nullptr) {
    std::fprintf(stderr, "Could not initialize Lua\n");
    return {};
  }
  lua_sethook(lua, [](lua_State* lua, lua_Debug*) {
    luaL_error(lua, "control program exceeded execution limit");
  }, LUA_MASKCOUNT, 10000);

  // Protect initialization, execution, and output validation with one handler.
  lua_pushcfunction(lua, RunControl);
  lua_pushlightuserdata(lua, &context);
  if (lua_pcall(lua, 1, 0, 0) != LUA_OK) {
    const char* error = lua_tostring(lua, -1);
    std::fprintf(stderr, "Control program: %s\n", error ? error : "Lua error");
    context.active_zones = {};
  }
  lua_close(lua);
  return context.active_zones;
}

}  // namespace irrigation
