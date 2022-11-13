const WEBSOCKET_URL = "ws://" + window.location.host;
const webSocket = new WebSocket(WEBSOCKET_URL);

const slider = document.getElementById("slider");
const display = document.getElementById("display");

let bulbState = {
  level: 0,
};

slider.addEventListener("change", (e) => {
  webSocket.send(JSON.stringify({ ...bulbState, level: parseInt(e.target.value) }));
});

webSocket.addEventListener("message", (e) => {
  bulbState = JSON.parse(e.data);
  slider.value = bulbState.level;
  display.innerText = bulbState.level;
});

// init
bulbState.level = parseInt(slider.value);
display.innerText = bulbState.level;
