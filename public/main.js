const WEBSOCKET_URL = "ws://" + window.location.host;
const webSocket = new WebSocket(WEBSOCKET_URL);
const rangeValue = document.querySelector(".range-value");
const range = document.querySelector(".range");

let state = {
  level: 0,
};

range.addEventListener("input", () => {
  rangeValue.innerText = range.value + "%";
});

range.addEventListener("change", () => {
  webSocket.send(
    JSON.stringify({ ...state, level: parseInt(range.value) })
  );
});

webSocket.addEventListener("message", (e) => {
  state = JSON.parse(e.data);
  range.value = state.level;
  rangeValue.innerText = state.level + "%";
});
