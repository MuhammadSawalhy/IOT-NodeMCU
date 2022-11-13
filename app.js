const PORT = process.env.PORT || 3000;
const express = require("express");
const app = new express();
const expressWs = require("express-ws")(app);

app.use(express.static("public"));

let bulbState = { level: 50 };

app.ws("/", (ws, req, res) => {
  console.log("New websocket connection!");
  ws.send(JSON.stringify(bulbState));
  ws.on("message", (data) => {
    bulbState = JSON.parse(data);
    expressWs.getWss().clients.forEach((client) => {
      client.send(data);
    });
  });
});

app.listen(PORT, () => {
  console.log("Server is running now on: http://localhost:" + PORT);
});
