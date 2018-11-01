#include<ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);
String header;
String outputState = "OFF";
const int led = D5;


void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Started Server");
}

void loop() {
  WiFiClient client = server.available();
  if(client)
  {
    Serial.println("New Client");
    String currentLine = "";
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        header+=c;
        if(c=='\n'){
          if(currentLine.length()==0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection:close");
            client.println("");
            client.println("<html>");
            if(header.indexOf("GET /on")>=0){
              digitalWrite(led,HIGH);
              outputState="ON";
            }
            else if(header.indexOf("GET /off")>=0){
              digitalWrite(led,LOW);
              outputState="OFF";
            }
            client.println("<body>");
            client.print("<h1>LED is currently ");
            client.print(outputState);
            client.println("</h1>");
            String button = "";
            if(outputState=="OFF"){
              button = "<p><a href=\"/on\"><button>Turn LED ON</button></a></p>";
            }else{
              button = "<p><a href=\"/off\"><button>Turn LED OFF</button></a></p>";
            }
            client.println(button);
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if(c!='\r') {
          currentLine+=c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client Disconnected\n");
  }
}
