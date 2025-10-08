// dggalExample2.js
import { DGGAL } from './dggal.js';

let dggal;
const initPromise = DGGAL.init();

// Create OpenLayers map using global ol (assumes ol.css and ol.js are included in HTML before this module)
document.addEventListener('DOMContentLoaded', () => {
  if (!window.ol) {
    const logArea = document.getElementById('consoleLog');
    const msg = 'OpenLayers not found. Include ol.js and ol.css in your HTML before this module.';
    console.error(msg);
    if (logArea) logArea.value += msg + '\n';
    return;
  }

  const ol = window.ol;
  const vectorSource = new ol.source.Vector();

  const vectorLayer = new ol.layer.Vector({
    source: vectorSource,
    style: feature => {
      const t = feature.get('type');
      if (t === 'polygon') {
        return new ol.style.Style({
          stroke: new ol.style.Stroke({ color: 'rgba(0,120,255,0.9)', width: 2 }),
          fill: new ol.style.Fill({ color: 'rgba(0,120,255,0.15)' })
        });
      } else if (t === 'centroid') {
        return new ol.style.Style({
          image: new ol.style.Circle({
            radius: 6,
            fill: new ol.style.Fill({ color: '#ff5722' }),
            stroke: new ol.style.Stroke({ color: '#fff', width: 2 })
          }),
          text: new ol.style.Text({ text: feature.get('label') || '', offsetY: -14, fill: new ol.style.Fill({ color: '#222' }) })
        });
      }
      return null;
    }
  });

  const map = new ol.Map({
    target: 'map',
    layers: [
      new ol.layer.Tile({ source: new ol.source.OSM() }),
      vectorLayer
    ],
    view: new ol.View({ center: ol.proj.fromLonLat([0, 0]), zoom: 2 })
  });

  window.__dggal_map = map;
  window.__dggal_vectorSource = vectorSource;
});

// Utility logging helpers
function addToLog(message) {
  console.log(message);
  const logArea = document.getElementById('consoleLog');
  if (logArea) {
    logArea.value += message + '\n';
    logArea.scrollTop = logArea.scrollHeight;
  }
}

function clearLog() {
  const logArea = document.getElementById('consoleLog');
  if (logArea) {
    logArea.value = '';
  }
}
window.clearLog = clearLog;

// Main form handler (kept intact, hardened slightly)
document.addEventListener('DOMContentLoaded', () => {
 (async () => {
  dggal = await initPromise;

  populateDGGRSDropdown('IVEA7H_Z7');

  window.processForm = async function () {
    try {
      // ensure map is initialized
      if (!window.__dggal_map) {
        addToLog('Map not initialized');
        return;
      }

      const nameInput = document.getElementById('name');
      const idInput = document.getElementById('id');

      if (!nameInput || !idInput) {
        addToLog('Error: Missing input elements.');
        return;
      }

      const dggrsName = nameInput.value.trim();
      const zoneID = idInput.value.trim();

      if (!dggrsName || !zoneID) {
        addToLog('Error: Both DGGRS Name and Zone ID are required.');
        return;
      }

      const dggrs = dggal.createDGGRS(dggrsName);

      const zone = dggrs.getZoneFromTextID(zoneID);

      let vertices, latDeg, lonDeg;

      if (zone === DGGAL.nullZone) {
        addToLog('Invalid zone identifier');
        dggrs.delete();
        return;
      } else {
        const centroid = dggrs.getZoneWGS84Centroid(zone);
        latDeg = centroid.lat * 180 / Math.PI;
        lonDeg = centroid.lon * 180 / Math.PI;
        addToLog(`Centroid: latitude: ${latDeg}, longitude: ${lonDeg}`);

        vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0);
      }

      // Clear previous features
      const vectorSrc = window.__dggal_vectorSource;
      if (!vectorSrc) {
        addToLog('Map not initialized yet');
        dggrs.delete();
        return;
      }
      vectorSrc.clear();

      // Convert vertices to [lon, lat] degrees and ensure ring is closed
      const coordsDeg = (Array.isArray(vertices) ? vertices : []).map(v => [v.lon * 180 / Math.PI, v.lat * 180 / Math.PI]);
      if (coordsDeg.length > 0) {
        const first = coordsDeg[0];
        const last = coordsDeg[coordsDeg.length - 1];
        if (first[0] !== last[0] || first[1] !== last[1]) coordsDeg.push([first[0], first[1]]);
      } else {
        addToLog('No vertices returned for zone');
        dggrs.delete();
        return;
      }

      const ol = window.ol;
      const polygonCoords = [coordsDeg.map(c => ol.proj.fromLonLat(c))];
      const polygonFeature = new ol.Feature({
        geometry: new ol.geom.Polygon(polygonCoords),
        type: 'polygon'
      });
      vectorSrc.addFeature(polygonFeature);

      const centroidPoint = ol.proj.fromLonLat([lonDeg, latDeg]);
      const centroidFeature = new ol.Feature({
        geometry: new ol.geom.Point(centroidPoint),
        type: 'centroid',
        label: `Centroid (${latDeg.toFixed(6)}, ${lonDeg.toFixed(6)})`
      });
      vectorSrc.addFeature(centroidFeature);

      window.__dggal_map.getView().fit(polygonFeature.getGeometry().getExtent(), { padding: [40, 40, 40, 40], maxZoom: 18, duration: 300 });

      dggrs.delete();
    } catch (err) {
      console.error(err);
      addToLog(`Error: ${err && err.message ? err.message : err}`);
    }
  };

  // Optional: clear log on page load
  clearLog();
 })();
});

window.addEventListener('pagehide', (event) => {
  if (!event.persisted && dggal) {
    dggal.terminate();
    dggal = null;
  }
});

document.getElementById('userForm').addEventListener('submit', function(event) {
  event.preventDefault();
  processForm();
});

function populateDGGRSDropdown(defaultValue = null) {
  const select = document.getElementById('name');
  select.innerHTML = '';

  const names = dggal.listDGGRS();

  for (const name of names) {
    const option = document.createElement('option');
    option.value = name;
    option.textContent = name;
    if (name === defaultValue) {
      option.selected = true;
    }
    select.appendChild(option);
  }
  if (!defaultValue) {
    const placeholder = document.createElement('option');
    placeholder.textContent = 'Select a DGGRS...';
    placeholder.disabled = true;
    placeholder.selected = true;
    select.appendChild(placeholder);
  }
}
