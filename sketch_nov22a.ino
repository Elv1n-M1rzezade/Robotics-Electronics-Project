#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESP32Servo.h>
Servo ESC;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define On 5
#define Off 6

#define Ena 13
#define In1 12
#define In2 14
#define In3 27
#define In4 26
#define Enb 25

const char* ssid = "Robo Gladiators";
const char* password = "Adnsu321";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* htmlHomePage PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>Robo Gladiators</title>
  <style>
    .bg {
      animation: slide 3s ease-in-out infinite alternate;
      background-image: linear-gradient(-60deg, #6c3 50%, #09f 50%);
      bottom: 0;
      left: -50%;
      opacity: .5;
      position: fixed;
      right: -50%;
      top: 0;
      z-index: -1;
    }

    .bg2 {
      animation-direction: alternate-reverse;
      animation-duration: 4s;
    }

    .bg3 {
      animation-duration: 5s;
    }

    .content {
      background-color: rgba(255, 255, 255, .8);
      border-radius: .25em;
      box-sizing: border-box;
      left: 50%;
      padding: 10vmin;
      position: fixed;
      text-align: center;
      top: 50%;
      transform: translate(-50%, -50%);
    }

    h1 {
      font-family: monospace;
    }

    @keyframes slide {
      0% {
        transform: translateX(-25%);
      }

      100% {
        transform: translateX(25%);
      }
    }

    .arrows {
      color: rgb(249, 249, 249);
      font-size: 80px;
    }

    td {
      background-color: #5df093;
      border-radius: 19%;
      box-shadow: 5px 5px grey;

    }

    td:active {
      transform: translate(5px, 5px);
      box-shadow: none;
    }

    .noselect {
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      -khtml-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
    }

    header h1 {
      font-size: 48px;
      font-weight: 700;
      background-image: linear-gradient(to left, #553c9a, #b393d3);
      color: transparent;
      background-clip: text;
      -webkit-background-clip: text;
      margin-top: -30px;
    }

    footer h1 {
      background-image: conic-gradient(#553c9a, #ee4b2b, #00c2cb);
      color: transparent;
      background-clip: text;
      -webkit-background-clip: text;
      margin-top: 10px;
      font-size: 20px
    }

    table {
      margin-top: -20px;
    }
    .button_test{
      color: white;
      font-family: monospace;
      font-size: 35px;
      height: 100px;
    }
  </style>
</head>

<body class="noselect" align="center">
  <div class="bg bg2"></div>
  <div class="bg bg3"></div>
  <div class="content">
    <header class="header">
      <h1>Robo Gladiators</h1>
    </header>

    <main>
      <table id="mainTable" style="width:400px;table-layout:fixed;margin-top: -40px;" CELLSPACING=10>
        <tr>
          <td style="background-color:white;box-shadow:none ; visibility: hidden;"></td>
          <td ontouchstart='onTouchStartAndEnd("1")' ontouchend='onTouchStartAndEnd("0")'><span
              class="arrows">&#8679;</span></td>
          <td style="background-color:white;box-shadow:none; visibility: hidden;"></td>
        </tr>

        <tr>
          <td ontouchstart='onTouchStartAndEnd("3")' ontouchend='onTouchStartAndEnd("0")'><span
              class="arrows">&#8678;</span></td>
          <td></td>
          <td ontouchstart='onTouchStartAndEnd("4")' ontouchend='onTouchStartAndEnd("0")'><span
              class="arrows">&#8680;</span></td>
        </tr>

        <tr>
          <td style="background-color:white;box-shadow:none; visibility: hidden;"></td>
          <td ontouchstart='onTouchStartAndEnd("2")' ontouchend='onTouchStartAndEnd("0")'><span
              class="arrows">&#8681;</span></td>
          <td style="background-color:white;box-shadow:none; visibility: hidden;"></td>
        </tr>
        <tr>
          <td ontouchstart='onTouchStartAndEnd("5")' class="button_test">On</td>
          <td style="background-color:white;box-shadow:none; visibility: hidden;"></td>
          <td ontouchstart='onTouchStartAndEnd("6")' ontouchend='onTouchStartAndEnd("0")' class="button_test">
            Off</td>
        </tr>

      </table>

    </main>

    <footer class="foot">
      <h1>Made by Robo Gladiators ✭ ZU.04-22  </h1>
    </footer>
  </div>
  <script>
    var webSocketUrl = "ws:\/\/" + window.location.hostname + "/ws";
    var websocket;

    function initWebSocket() {
      websocket = new WebSocket(webSocketUrl);
      websocket.onopen = function (event) { };
      websocket.onclose = function (event) { setTimeout(initWebSocket, 2000); };
      websocket.onmessage = function (event) { };
    }

    function onTouchStartAndEnd(value) {
      websocket.send(value);
    }

    window.onload = initWebSocket;
    document.getElementById("mainTable").addEventListener("touchend", function (event) {
      event.preventDefault()
    });


  </script>

</body>

</html>
)HTMLHOMEPAGE";

void Forward()
{
  analogWrite(Ena,255);
  analogWrite(Enb,255);
  digitalWrite(In1,HIGH);
  digitalWrite(In2,LOW);
  digitalWrite(In3,HIGH);
  digitalWrite(In4,LOW);

}

void Backward()
{
  analogWrite(Ena,255);
  analogWrite(Enb,255);
  digitalWrite(In2,HIGH);
  digitalWrite(In1,LOW);
  digitalWrite(In4,HIGH);
  digitalWrite(In3,LOW);
}

void Left()
{
  analogWrite(Ena,255);
  analogWrite(Enb,255);
  digitalWrite(In1,HIGH);
  digitalWrite(In2,LOW);
  digitalWrite(In4,HIGH);
  digitalWrite(In3,LOW);
}

void Right()
{
  analogWrite(Ena,255);
  analogWrite(Enb,255);
  digitalWrite(In2,HIGH);
  digitalWrite(In1,LOW);
  digitalWrite(In3,HIGH);
  digitalWrite(In4,LOW);
}
void Stop()
{
  analogWrite(Ena,0);
  analogWrite(Enb,0);
  digitalWrite(In2,LOW);
  digitalWrite(In1,LOW);
  digitalWrite(In3,LOW);
  digitalWrite(In4,LOW);
}
void Weapon_On()
{
  for(int i=0;i<50;i++){
    ESC.write(i);
    delay(40);
  }
  ESC.write(50);
}

void Weapon_Off()
{
  ESC.write(0);
}

void processCarMovement(String inputValue)
{
  Serial.printf("Got value as %s %d\n", inputValue.c_str(), inputValue.toInt());  
  switch(inputValue.toInt())
  {

    case UP:
    Forward();
      break;
  
    case DOWN:
    Backward();
      break;
  
    case LEFT:
    Left();
      break;
  
    case RIGHT:
    Right();
      break;
  
    case On:
    Weapon_On();
      break;
  
    case Off:
    Weapon_Off();
      break;
  
    default:
    Stop();
      break;
      
  }
}

void handleRoot(AsyncWebServerRequest *request) 
{
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "File Not Found");
}


void onWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      processCarMovement("0");
      break;

    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        processCarMovement(myData.c_str());       
      }
      break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;

    default:
      break;  

  }
}



void setup() 
{
  Serial.begin(115200);

  pinMode(Ena,OUTPUT);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT);
  pinMode(Enb,OUTPUT);
  pinMode(33,OUTPUT);

  ESC.attach(33,1000,2000);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  ws.cleanupClients(); 
}