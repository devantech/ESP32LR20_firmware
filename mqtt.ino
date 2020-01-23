
#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.


WiFiClient espClient;
PubSubClient MQTTclient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int MQTTconnectionTimer = 1;
int MQTTPortInUse = 0;

void setupMQTT() {
  if(MqttPort==0) return;                     // do not try to connect if port is zero
  MQTTclient.setServer(MqttServer, MqttPort);
  MQTTclient.setCallback(callback);
  MQTTconnectionTimer = millis()-5000;
  MQTTPortInUse = MqttPort;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic, R1Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly1, HIGH);    // Switch on the Relay if an 1 was received as first character
    else digitalWrite(Rly1, LOW); 
  }
  if(strcmp(topic, R2Topic)==0) {
    if ((char)payload[0] == '1') digitalWrite(Rly2, HIGH);
    else digitalWrite(Rly2, LOW); 
  }
}

void reconnect() {
    if(WiFi.status() != WL_CONNECTED) return; //WIFI disconnected so don't attempt reconnect
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (MQTTclient.connect(MqttID)) {
      Serial.println("connected");
      
// subscribe
      MQTTclient.subscribe(R1Topic);
      MQTTclient.loop();
      MQTTclient.subscribe(R2Topic);
      MQTTclient.loop();
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 10 seconds");
    }
}



void modeMQTT() {
char Inp;
char Msg[] = "0";
static int x=0;

if(MQTTPortInUse != MqttPort){
  setupMQTT();
  return;
}

if (MQTTclient.connected()) {
    MQTTclient.loop();
  }
  else {
    if(MqttPort==0) return;                     // do not try to connect if port is zero  
    if(millis()-MQTTconnectionTimer<10000) return;
    reconnect();
    MQTTconnectionTimer = millis();
  }
}
