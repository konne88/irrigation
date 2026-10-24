#ifndef IRRIGATION_WEBSITE_H_
#define IRRIGATION_WEBSITE_H_

#include <Arduino.h>

namespace irrigation {

inline const char kWebsite[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Garden irrigation</title>
<style>
  body { font: 16px system-ui; max-width: 38rem; margin: 3rem auto; padding: 0 1rem; }
  fieldset { margin: 1rem 0; }
  #zones label { display: inline-block; margin: .5rem; }
  textarea { box-sizing: border-box; width: 100%; margin: .5rem 0 1rem; }
  button { padding: .5rem 1rem; }
</style>
<h1>Garden irrigation</h1>
<form id="settings">
  <fieldset id="zones"><legend>Zone overrides</legend></fieldset>
  <label for="override-duration">Override duration</label>
  <select id="override-duration">
    <option value="5">5 minutes</option>
    <option value="15" selected>15 minutes</option>
    <option value="30">30 minutes</option>
    <option value="60">1 hour</option>
  </select>
  <p id="override-status" role="status"></p>
  <p>Choose a duration, set zones to On or Off, and save. All overrides end together, then zones return to the control program.</p>
  <label for="program">Control program (Lua)</label>
  <textarea id="program" rows="10" spellcheck="false"></textarea>
  <p>Set <code>active_zones[1]</code> through <code>active_zones[8]</code> to
    <code>true</code> or <code>false</code>. All zones start off on each run.
    Inputs: <code>moisture</code> (ohms), <code>flow</code> (pulse count),
    <code>year</code>, <code>month</code>, <code>day</code>, <code>hour</code>,
    <code>minute</code>, <code>second</code>, <code>weekday</code> (Monday = 0),
    <code>yearday</code> (January 1 = 1), and <code>isdst</code>.
    Script errors turn all zones off before overrides are applied.</p>
  <button id="save" disabled>Save state</button>
  <button id="reload" type="button">Reload</button>
</form>
<p id="status" role="status"></p>
<p>Settings reset when the controller restarts.</p>
<script>
  const zones = document.getElementById('zones');
  const program = document.getElementById('program');
  const overrideDuration = document.getElementById('override-duration');
  const overrideStatus = document.getElementById('override-status');
  const status = document.getElementById('status');
  const save = document.getElementById('save');
  const reload = document.getElementById('reload');
  const settings = document.getElementById('settings');
  const inputs = Array.from({length: 8}, (_, i) => {
    const label = document.createElement('label');
    const input = document.createElement('select');
    for (const [value, text] of [['auto', 'Auto'], ['on', 'On'], ['off', 'Off']]) {
      input.add(new Option(text, value));
    }
    label.append(`Zone ${i + 1} `, input);
    zones.append(label);
    return input;
  });
  async function request(options) {
    const response = await fetch('/state', options);
    const state = await response.json();
    if (!response.ok) throw new Error(state.error || 'Request failed');
    return state;
  }
  function display(state) {
    const active = state.zone_overrides_end > Date.now() / 1000 &&
      state.zone_overrides.some(value => value !== null);
    inputs.forEach((input, i) => {
      input.value = !active || state.zone_overrides[i] === null ? 'auto' : state.zone_overrides[i] ? 'on' : 'off';
    });
    program.value = state.control_program;
    overrideStatus.textContent = active
      ? `Current overrides end at ${new Date(state.zone_overrides_end * 1000).toLocaleTimeString()}.`
      : 'No active overrides.';
  }
  async function load() {
    save.disabled = true;
    try {
      display(await request());
      status.textContent = 'State loaded.';
      save.disabled = false;
    } catch (error) { status.textContent = error.message; }
  }
  reload.onclick = load;
  settings.onsubmit = async event => {
    event.preventDefault();
    const end = Math.floor(Date.now() / 1000) + Number(overrideDuration.value) * 60;
    save.disabled = reload.disabled = true;
    try {
      display(await request({
        method: 'PUT',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({
          zone_overrides: inputs.map(input => input.value === 'auto' ? null : input.value === 'on'),
          zone_overrides_end: end,
          control_program: program.value
        })
      }));
      status.textContent = 'State saved.';
    } catch (error) { status.textContent = error.message; }
    finally { save.disabled = reload.disabled = false; }
  };
  load();
</script>
</html>
)HTML";

}  // namespace irrigation

#endif  // IRRIGATION_WEBSITE_H_
