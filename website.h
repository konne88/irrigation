#ifndef IRRIGATION_WEBSITE_H_
#define IRRIGATION_WEBSITE_H_

#include <Arduino.h>

namespace irrigation {

inline const char kWebsite[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#205640">
<title>Weitz Gardens · Irrigation</title>
<style>
:root{font-family:Inter,ui-sans-serif,system-ui,-apple-system,sans-serif;color:#243d33;background:#f5f5ef;font-synthesis:none;--green:#205640;--muted:#758278;--line:#e1e6dc}
*{box-sizing:border-box}body{margin:0}button,select{font:inherit}button{cursor:pointer}button:disabled{cursor:wait;opacity:.45}button:focus-visible,select:focus-visible,textarea:focus-visible{outline:3px solid #c7984b;outline-offset:3px}
header{background:#fffefa;border-bottom:1px solid var(--line)}.bar{max-width:1140px;margin:auto;padding:22px 36px;display:flex;align-items:center;justify-content:space-between;gap:16px}.brand{font-size:25px;font-weight:750;letter-spacing:-1px;display:flex;align-items:center;gap:12px}.brand small{letter-spacing:1.8px;text-transform:uppercase;font-size:9px;font-weight:650;color:var(--muted);display:block;margin-top:3px}.mark{display:grid;place-items:center;border-radius:14px 14px 14px 3px;background:var(--green);color:#e5f1c5;width:43px;height:45px}.mark svg{width:31px;height:31px}.pill{display:inline-flex;align-items:center;gap:8px;font-size:12px;border:1px solid var(--line);padding:8px 12px;border-radius:30px;background:#fff}.dot{width:7px;height:7px;border-radius:50%;background:#91a093}.dot.live{background:#3b8b55;box-shadow:0 0 0 3px #e6efdf}
main{max-width:1140px;margin:auto;padding:42px 36px 24px}.eyebrow{text-transform:uppercase;letter-spacing:2px;color:var(--muted);font-size:10px;font-weight:750}.intro{display:flex;justify-content:space-between;align-items:end;gap:20px;margin-bottom:32px}h1{font-size:38px;line-height:1.15;letter-spacing:-1.7px;margin:10px 0 12px;font-weight:600}p{color:var(--muted);font-size:13px;line-height:1.7;margin:0}.date{text-align:right;font-size:12px;color:var(--muted);line-height:1.8}.section-head{display:flex;justify-content:space-between;align-items:center;gap:15px;margin-bottom:16px}h2{font-size:19px;letter-spacing:-.5px;margin:0 0 4px;font-weight:650}.button{border:1px solid var(--line);border-radius:9px;padding:11px 16px;background:#fffefa;color:#334b3c;font-size:12px;font-weight:600}.button.primary{background:var(--green);color:white;border-color:var(--green)}
fieldset{border:0;padding:0;margin:0;min-width:0}legend{padding:0}#zones{display:grid;grid-template-columns:repeat(4,1fr);gap:14px}.zone{display:block;border:1px solid var(--line);background:#fffefa;border-radius:14px;padding:19px}.zone-top{display:flex;justify-content:space-between;align-items:center;gap:8px}.zone-num{font-size:10px;color:var(--muted);font-weight:650;letter-spacing:1.5px}.badge{font-size:10px;color:#809083;background:#f0f1eb;padding:4px 7px;border-radius:5px}.zone-title{display:block;font-size:16px;font-weight:600;margin:19px 0 4px}.zone-description{display:block;color:var(--muted);font-size:11px;min-height:22px}.controls{display:flex;background:#eeefe8;border-radius:7px;padding:3px;margin-top:16px;gap:2px}.controls button{border:0;background:none;border-radius:5px;padding:10px 0;min-height:38px;flex:1;color:#728072;font-size:12px;font-weight:650}.controls button[aria-pressed="true"]{background:#fff;color:var(--green);box-shadow:0 1px 4px #243d3312}.controls button.on[aria-pressed="true"]{background:var(--green);color:white}.zone[data-mode="on"]{border-color:#a5bf9a;background:#f0f5e9}.duration{display:flex;align-items:center;gap:10px;color:var(--muted);font-size:12px;margin-bottom:18px}select{background:#fffefa;border:1px solid var(--line);padding:8px 12px;border-radius:7px;font-size:12px;color:#334b3c}.footnote{font-size:11px;margin-top:12px}#override-status{color:var(--green)}
.program{margin-top:32px;border:1px solid var(--line);border-radius:16px;padding:22px;background:#fffefa}.program textarea{display:block;width:100%;min-height:260px;resize:vertical;tab-size:2;font:13px/1.7 ui-monospace,SFMono-Regular,Consolas,monospace;background:#19362b;color:#e0ecd6;border:1px solid #2b5140;border-radius:10px;padding:18px;white-space:pre;overflow:auto;margin-top:12px}.program details{margin-top:14px;color:var(--muted);font-size:12px;line-height:1.8}.program summary{cursor:pointer}.program details p{margin-top:10px}.actions{display:flex;gap:10px;align-items:center;flex-wrap:wrap;margin-top:20px}#status{font-size:12px;color:var(--green)}.footer{border-top:1px solid var(--line);display:flex;justify-content:space-between;gap:12px;margin-top:30px;padding-top:18px;color:var(--muted);font-size:10px;line-height:1.8}.sr-only{position:absolute;width:1px;height:1px;padding:0;margin:-1px;overflow:hidden;clip:rect(0,0,0,0);white-space:nowrap;border:0}
@media(max-width:900px){#zones{grid-template-columns:repeat(2,1fr)}.bar,main{padding-left:22px;padding-right:22px}}@media(max-width:600px){.intro{display:block}.date{text-align:left;margin-top:15px}h1{font-size:32px}.zone{padding:14px}.footer{display:block}.bar{padding:16px 20px}main{padding-top:26px}.pill{font-size:10px;padding:7px 9px}.brand{font-size:22px}.section-head{align-items:start}.program{padding:16px}.duration{flex-wrap:wrap}}
</style>
</head>
<body>
<header><div class="bar">
  <div class="brand"><span class="mark" aria-hidden="true"><svg viewBox="0 0 36 36" fill="none"><path d="M5 13l5 16 8-12 7 12 6-19" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/><path d="M18 13C17 5 23 3 29 4c0 7-5 11-11 9Z" fill="currentColor"/><path d="m18 15 7-7" stroke="#205640" stroke-width="1.4" stroke-linecap="round"/></svg></span><div>Weitz Gardens<small>Family roots. Thoughtful care.</small></div></div>
  <span class="pill"><span id="connection-dot" class="dot"></span><span id="connection">Connecting</span></span>
</div></header>
<main>
<section class="intro"><div><div class="eyebrow">A little care. Greener gardens.</div><h1>A good day to grow.</h1><p>A little more care for every corner of the Weitz gardens.</p></div><div class="date" id="today"></div></section>
<form id="settings">
  <div class="section-head"><div><h2>Watering zones</h2><p>Follow your watering program, or take the reins for a while.</p></div><span class="pill">8 zones</span></div>
  <div class="duration"><label for="override-duration">Manual override duration</label><select id="override-duration"><option value="5">5 minutes</option><option value="15" selected>15 minutes</option><option value="30">30 minutes</option><option value="60">1 hour</option></select></div>
  <fieldset id="zones"><legend class="sr-only">Zone overrides</legend></fieldset>
  <p class="footnote">Changes apply immediately. Each click starts the selected duration for all current overrides; Auto returns that zone to your watering program.</p>
  <p id="override-status" class="footnote" role="status"></p>
  <section class="program" aria-labelledby="program-title">
    <div class="section-head"><div><h2 id="program-title">Your watering program</h2><p>A little logic to keep your gardens growing.</p></div><span class="pill">Lua</span></div>
    <label class="eyebrow" for="program">Automatic watering</label>
    <textarea id="program" rows="10" spellcheck="false" autocapitalize="off" autocorrect="off" aria-describedby="program-help"></textarea>
    <details id="program-help"><summary>Writing your watering program</summary><p>Set <code>active_zones[1]</code> through <code>active_zones[8]</code> to <code>true</code> or <code>false</code>. All zones start off on each run.</p><p>Inputs: <code>moisture</code> (ohms), <code>flow</code> (pulse count), <code>year</code>, <code>month</code>, <code>day</code>, <code>hour</code>, <code>minute</code>, <code>second</code>, <code>weekday</code> (Monday = 0), <code>yearday</code> (January 1 = 1), and <code>isdst</code>. Script errors turn all zones off before overrides are applied.</p></details>
    <div class="actions"><button class="button primary" id="save" disabled>Save program</button></div>
  </section>
  <p id="status" class="footnote" role="status"></p>
</form>
<noscript><p>Enable JavaScript to control your irrigation system.</p></noscript>
<footer class="footer"><span>Weitz Gardens · Rooted in family.</span><span>Settings reset when the controller restarts.</span></footer>
</main>
<script>
  document.getElementById('today').textContent = new Date().toLocaleDateString(undefined, {weekday:'long', month:'long', day:'numeric'});
  function connection(connected) {
    document.getElementById('connection').textContent = connected ? 'Controller connected' : 'Connection interrupted';
    document.getElementById('connection-dot').classList.toggle('live', connected);
  }
  const zones = document.getElementById('zones');
  const program = document.getElementById('program');
  const overrideDuration = document.getElementById('override-duration');
  const overrideStatus = document.getElementById('override-status');
  const status = document.getElementById('status');
  const save = document.getElementById('save');
  const settingsForm = document.getElementById('settings');
  let busy = false;
  let loaded = false;
  const inputs = Array.from({length: 8}, (_, i) => {
    const label = document.createElement('div');
    label.className = 'zone';
    label.innerHTML = `<span class="zone-top"><span class="zone-num">ZONE ${String(i + 1).padStart(2, '0')}</span><span class="badge">Override</span></span><span class="zone-title">Zone ${i + 1}</span><span class="zone-description">Following your program</span>`;
    const input = document.createElement('input');
    input.type = 'hidden';
    input.value = 'auto';
    input.disabled = true;
    const controls = document.createElement('div');
    controls.className = 'controls';
    controls.setAttribute('role', 'group');
    controls.setAttribute('aria-label', `Zone ${i + 1} override`);
    for (const [value, text] of [['auto', 'Auto'], ['on', 'On'], ['off', 'Off']]) {
      const button = document.createElement('button');
      button.type = 'button';
      button.className = value;
      button.textContent = text;
      button.dataset.mode = value;
      button.disabled = true;
      button.setAttribute('aria-pressed', String(value === 'auto'));
      button.onclick = () => applyZone(i, value);
      controls.append(button);
    }
    label.append(input, controls);
    zones.append(label);
    return input;
  });
  function updateZone(input, pending) {
    const card = input.parentElement;
    card.dataset.mode = input.value;
    card.querySelectorAll('button').forEach(button => {
      button.disabled = false;
      button.setAttribute('aria-pressed', String(button.dataset.mode === input.value));
    });
    card.querySelector('.zone-description').textContent = input.value === 'auto'
      ? 'Following your program'
      : `Manual override · ${input.value}${pending ? ' · applying' : ''}`;
  }
  async function request(path = '/settings', options) {
    const response = await fetch(path, {cache: 'no-store', ...options});
    const settings = await response.json();
    if (!response.ok) throw new Error(settings.error || 'Request failed');
    return settings;
  }
  function display(settings, updateProgram = true) {
    const active = settings.zone_overrides_end > Date.now() / 1000 &&
      settings.zone_overrides.some(value => value !== null);
    inputs.forEach((input, i) => {
      input.value = !active || settings.zone_overrides[i] === null ? 'auto' : settings.zone_overrides[i] ? 'on' : 'off';
      input.disabled = false;
      updateZone(input, false);
    });
    if (updateProgram) program.value = settings.control_program;
    program.disabled = false;
    connection(true);
    overrideStatus.textContent = active
      ? `Current overrides end at ${new Date(settings.zone_overrides_end * 1000).toLocaleTimeString()}.`
      : 'No active overrides.';
  }
  function setBusy(value) {
    busy = value;
    save.disabled = value || !loaded;
    program.disabled = value || !loaded;
    zones.querySelectorAll('button').forEach(button => { button.disabled = value || !loaded; });
  }
  async function load() {
    if (busy) return;
    setBusy(true);
    try {
      display(await request());
      loaded = true;
      status.textContent = 'Settings loaded.';
    } catch (error) {
      loaded = false;
      status.textContent = error.message;
      connection(false);
    } finally { setBusy(false); }
  }
  async function writeSettings(path, update) {
    return request(path, {
      method: 'PUT',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify(update)
    });
  }
  async function applyZone(index, mode) {
    if (busy || !loaded) return;
    setBusy(true);
    status.textContent = `Applying Zone ${index + 1}...`;
    try {
      display(await writeSettings('/settings/zone-override', {
        zone: index + 1,
        override: mode === 'auto' ? null : mode === 'on',
        zone_overrides_end: Math.floor(Date.now() / 1000) + Number(overrideDuration.value) * 60
      }), false);
      status.textContent = `Zone ${index + 1}: ${mode === 'auto' ? 'Auto' : mode === 'on' ? 'On' : 'Off'} applied.`;
    } catch (error) {
      loaded = false;
      connection(false);
      status.textContent = `Change not confirmed: ${error.message}. Refresh the page to check.`;
    } finally { setBusy(false); }
  }
  settingsForm.onsubmit = async event => {
    event.preventDefault();
    if (busy || !loaded) return;
    setBusy(true);
    try {
      display(await writeSettings('/settings/program', {
        control_program: program.value
      }));
      status.textContent = 'Program saved.';
    } catch (error) {
      loaded = false;
      connection(false);
      status.textContent = `Save not confirmed: ${error.message}. Refresh the page to check.`;
    } finally { setBusy(false); }
  };
  program.disabled = true;
  program.oninput = () => { status.textContent = 'Unsaved changes.'; };
  load();
</script>
</body>
</html>
)HTML";

}  // namespace irrigation

#endif  // IRRIGATION_WEBSITE_H_
