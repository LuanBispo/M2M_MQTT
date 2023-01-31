#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Seus dados do Wifi.
const char* ssid = "nome da sua rede";
const char* password = "senha do seu wifi";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];


char Botao;


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Conectando à rede ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Aguardando conexão MQTT...");
    // Create a random client ID
    String clientId = "ENGEASIER_MQTT";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("seutopico");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println("tentando novamente em 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  Botao = D0;

  pinMode(Botao, INPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {

  int btn = digitalRead(Botao);

  if (btn == 1) {
    Serial.println("Botão pressionado");
    client.publish("seutopico", "S");
    delay(200);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
