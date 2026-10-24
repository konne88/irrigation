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
  <label for="override-end">Override end time (your local time)</label>
  <input id="override-end" type="datetime-local" step="1" required>
  <p>All overrides end together. Zones then return to the control program.</p>
  <label for="program">Control program (stored only; not executed)</label>
  <textarea id="program" rows="10" spellcheck="false"></textarea>
  <button id="save" disabled>Save state</button>
  <button id="reload" type="button">Reload</button>
</form>
<p id="status" role="status"></p>
<p>Settings reset when the controller restarts.</p>
<script>
  const zones = document.getElementById('zones');
  const program = document.getElementById('program');
  const overrideEnd = document.getElementById('override-end');
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
    inputs.forEach((input, i) => {
      input.value = state.zone_overrides[i] === null ? 'auto' : state.zone_overrides[i] ? 'on' : 'off';
    });
    program.value = state.control_program;
    if (state.zone_overrides_end > 0) {
      const date = new Date(state.zone_overrides_end * 1000);
      const pad = value => String(value).padStart(2, '0');
      overrideEnd.value = `${date.getFullYear()}-${pad(date.getMonth() + 1)}-${pad(date.getDate())}T${pad(date.getHours())}:${pad(date.getMinutes())}:${pad(date.getSeconds())}`;
    } else {
      overrideEnd.value = '';
    }
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
    const end = new Date(overrideEnd.value).getTime() / 1000;
    if (!Number.isSafeInteger(end) || end <= Date.now() / 1000) {
      status.textContent = 'Choose an override end time in the future.';
      return;
    }
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
