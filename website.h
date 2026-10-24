#pragma once

#include <Arduino.h>

const char WEBSITE[] PROGMEM = R"HTML(
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
      input.value = state.active_zones[i] === null ? 'auto' : state.active_zones[i] ? 'on' : 'off';
    });
    program.value = state.control_program;
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
    save.disabled = reload.disabled = true;
    try {
      display(await request({
        method: 'PUT',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({
          active_zones: inputs.map(input => input.value === 'auto' ? null : input.value === 'on'),
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
