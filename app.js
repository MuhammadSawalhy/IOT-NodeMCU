const PORT = process.env.PORT || 3000;
const express = require("express");
const app = new express();
const expressWs = require("express-ws")(app);

app.use(express.static("public"));

let state = { level: 50 };

app.ws("/", (ws, req, res) => {
  console.log("New websocket connection!");
  ws.send(JSON.stringify(state));
  ws.on("message", (data) => {
    state = JSON.parse(data);
    console.log("state changed", state);
    expressWs.getWss().clients.forEach((client) => {
      client.send(data);
    });
  });
});

app.listen(PORT, () => {
  console.log("Server is running now on: http://localhost:" + PORT);
  const localNetworkIP = getLocalIP();
  if (localNetworkIP)
    console.log(`Server is running now on: http://:${localNetworkIP}:${PORT}`);
});

function getLocalIP() {
  const { networkInterfaces } = require('os');
  const net = networkInterfaces();
  for (let n of Object.values(net).flat()) {
    const {internal, family, address} = n;
    if (internal || family != "IPv4") continue;
    return address;
  }
}