import { DGGAL } from './dggal.js';

let dggal;
const initPromise = DGGAL.init();

document.addEventListener('DOMContentLoaded', () => {
  // Utility to log messages to the consoleLog textarea
  function addToLog(message) {
    console.log(message);
    const logArea = document.getElementById('consoleLog');
    if (logArea) {
      logArea.value += message + '\n';
      logArea.scrollTop = logArea.scrollHeight;
    }
  }

  // Clear the log output
  function clearLog() {
    const logArea = document.getElementById('consoleLog');
    if (logArea) {
      logArea.value = '';
    }
  }

  // Expose clearLog globally for the Clear button
  window.clearLog = clearLog;

  // Main form handler
  window.processForm = async function () {
    try {
      // Ensure WASM module is initialized
      dggal = await initPromise;

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

      // Create DGGRS instance and process zone
      const dggrs = dggal.createDGGRS(dggrsName);
      const zone = dggrs.getZoneFromTextID(zoneID);

      if(zone == DGGAL.nullZone)
         addToLog('Invalid zone identifier');
      else
      {
         const centroid = dggrs.getZoneWGS84Centroid(zone);

         addToLog(`Centroid: { latitude: ${centroid.lat * 180 / Math.PI}, longitude: ${centroid.lon * 180 / Math.PI} }`);

         // Get refined vertices and print them
         const vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0);

         if (!Array.isArray(vertices)) {
           addToLog('Error: getZoneRefinedWGS84Vertices did not return an array.');
         } else {
           addToLog(`Refined vertices (count: ${vertices.length}):`);
           vertices.forEach((v, i) => {
             const latDeg = v.lat * 180 / Math.PI;
             const lonDeg = v.lon * 180 / Math.PI;
             addToLog(`  ${i}: { latitude: ${latDeg}, longitude: ${lonDeg} }`);
           });
         }
      }

      // Clean up
      dggrs.delete();
    } catch (err) {
      console.error(err);
      addToLog(`Error: ${err.message}`);
    }
  };

  // Optional: clear log on page load
  clearLog();
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
