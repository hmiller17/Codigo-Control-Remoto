#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Datos del Access Point
const char* ssid = "ESP_CAR";
const char* password = "12345678";

// Crear servidor en el puerto 80
ESP8266WebServer server(80);

// Pines del L293D
const int ENA = D1;   // Enable motor A
const int IN1 = D2;
const int IN2 = D3;
const int ENB = D5;   // Enable motor B
const int IN3 = D6;
const int IN4 = D7;

void setup() {
  Serial.begin(115200);

  // Configura pines como salida
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Crear red WiFi como Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point creado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Rutas web
  server.on("/", handleRoot);
  server.on("/adelante", adelante);
  server.on("/atras", atras);
  server.on("/izquierda", izquierda);
  server.on("/derecha", derecha);
  server.on("/parar", parar);

  server.begin();
}

void loop() {
  server.handleClient();
}

// HTML de control responsivo
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="es">
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Control de Robot WiFi</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          text-align: center;
          background-color: #f4f4f4;
          padding: 20px;
        }
        h2 {
          color: #333;
        }
        #estado {
          font-size: 18px;
          margin-bottom: 20px;
          color: #444;
        }
        .button {
          display: inline-block;
          width: 100px;
          height: 100px;
          margin: 10px;
          font-size: 36px;
          background-color: #2196F3; /* Azul */
          color: white;
          border: none;
          border-radius: 12px;
          box-shadow: 2px 2px 5px rgba(0,0,0,0.3);
          transition: background-color 0.2s ease;
        }
        .button:active {
          background-color: #1976D2;
        }
        .stop {
          background-color: #f44336; /* Rojo */
        }
        .stop:active {
          background-color: #d32f2f;
        }
        .grid {
          display: grid;
          grid-template-columns: 1fr 1fr 1fr;
          grid-gap: 10px;
          max-width: 320px;
          margin: auto;
        }
        @media (max-width: 400px) {
          .button {
            width: 80px;
            height: 80px;
            font-size: 28px;
          }
        }
      </style>
    </head>
    <body>
      <h2>Control del Robot WiFi</h2>
      <p id="estado">Estado: Parado</p>
      <div class="grid">
        <div></div>
        <button class="button" onclick="enviar('/adelante', 'Adelante')">⬆</button>
        <div></div>

        <button class="button" onclick="enviar('/izquierda', 'Izquierda')">⬅</button>
        <button class="button stop" onclick="enviar('/parar', 'Parado')">⏹</button>
        <button class="button" onclick="enviar('/derecha', 'Derecha')">➡</button>

        <div></div>
        <button class="button" onclick="enviar('/atras', 'Atrás')">⬇</button>
        <div></div>
      </div>

      <script>
        function enviar(ruta, estado) {
          document.getElementById("estado").innerText = "Estado: " + estado;
          window.location.href = ruta;
        }
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Acciones de movimiento
void adelante() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 1023); analogWrite(ENB, 1023);
  server.sendHeader("Location", "/");
  server.send(303);
}

void atras() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 1023); analogWrite(ENB, 1023);
  server.sendHeader("Location", "/");
  server.send(303);
}

void izquierda() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 1023); analogWrite(ENB, 1023);
  server.sendHeader("Location", "/");
  server.send(303);
}

void derecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 1023); analogWrite(ENB, 1023);
  server.sendHeader("Location", "/");
  server.send(303);
}

void parar() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  server.sendHeader("Location", "/");
  server.send(303);
}
