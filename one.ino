#include <WiFi.h>
#include <PubSubClient.h> //include library

//=================================================================================================
WiFiClient   espClient;                     //คำสั่งจาก library
PubSubClient client(espClient);             //สร้างออปเจ็ค สำหรับเชื่อมต่อ mqtt
//=================================================================================================
const char* ssid = "wifi name";                   //wifi name
const char* password = "wifi password";           //wifi password
//=================================================================================================
const char* mqtt_broker = "IP mqtt server";       //IP mqtt server
const char* mqtt_username = "mqtt username";      //mqtt username
const char* mqtt_password = "mqtt password";      //mqtt password
const int   mqtt_port = port mqtt server;         //port mqtt server
//=================================================================================================

void setup_wifi() {   //ฟังก์ชั่นเชื่อมต่อwifi
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);                      //เลือกโหมดรับ wifi
  WiFi.begin(ssid, password);               //เชื่อมต่อ wifi
  while (WiFi.status() != WL_CONNECTED)     //รอจนกว่าจะเชื่อมต่อwifiสำเร็จ
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) {  //ฟังก์ชั่นsubscribe
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++)          //รับค่าโดยการปริ้นอักษรที่ละตัวออกมา เป็น char
    Serial.print((char) payload[i]);
  Serial.println();
  Serial.println("-----------------------");
}

void reconnect() {  //ฟังก์ชั่นเชื่อมต่อmqtt
  client.setServer(mqtt_broker, mqtt_port);   //เชื่อมต่อmqtt
  client.setCallback(callback);               //เลือกฟังก์ชั่นsubscribe
  while (!client.connected())                 //รอจนกว่าจะเชื่อมต่อmqttสำเร็จ
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
      Serial.println("Public emqx mqtt broker connected");
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi(); //เชื่อมต่อwifi
  reconnect();  //เชื่อมต่อmqtt
  client.subscribe("topic");  //กำหนด topic ที่จะ subscribe
  client.publish("topic", "xxxxxxx"); //กำหนด topic ที่จะ publish และ valu
}

void loop()
{
  client.loop();//วนลูปรอsubscribe valu จาก mqtt
}
