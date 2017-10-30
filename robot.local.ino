/*******************************************************
 * robot                                               *
 * wifi controlled wheel robot                         *
 * esp8266 wifi with webserver, mdns, websocket server *
 * © Roganov Sergey, 2017                              *
 *******************************************************/
#define LED       13
#define LED_RED   15
#define LED_GREEN 13
#define LED_BLUE  12
#define M1_D       2
#define M1_S       0
#define M2_S       4
#define M2_D       5
#define MID      255
#define FULL    1023
#define DNS_PORT  53

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <Hash.h>
#include <FS.h>

DNSServer dnsServer;
ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);
bool webSocketConnected = false;

void setup() {
	pinMode(LED, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	
	pinMode(M1_D, OUTPUT);
	pinMode(M1_S, OUTPUT);
	pinMode(M2_S, OUTPUT);
	pinMode(M2_D, OUTPUT);
	
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	
	digitalWrite(M1_D, LOW);
	digitalWrite(M1_S, LOW);
	digitalWrite(M2_S, LOW);
	digitalWrite(M2_D, LOW);
	
	Serial.begin(115200);
	Serial.println("\nrobot is starting...configuring...");
	
	Serial.print("...access point: ");
	
	WiFi.mode(WIFI_AP);
	WiFi.softAP("robot.local");
	WiFi.hostname("robot.local");
	Serial.print("ssid is ");
	Serial.print("robot.local");
	IPAddress ip = WiFi.softAPIP();
	Serial.print(", ip is ");
	Serial.print(ip);
	Serial.println(", done");
	
	Serial.print("...file system: ");
	if(SPIFFS.begin()){
		Serial.println("done");
		listRoot();
		Serial.println("");
	}else{
		Serial.println("failure");
	}
	
	Serial.print("...web socket: ");
	webSocket.begin();
	webSocket.onEvent(webSocketEvent);
	Serial.println("done");
	
	Serial.print("...mdns: ");
	if(MDNS.begin("robot", ip)){
		MDNS.addService("http", "tcp", 80);
		Serial.println("done");
	}else{
		Serial.println("failure");
	}
	
	dnsServer.setTTL(300);
	dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
	Serial.print("...dns: ");
	if(dnsServer.start(DNS_PORT, "robot.local", ip)){
		Serial.println("done");
	}else{
		Serial.println("failure");
	}
	
	
	Serial.print("...web server: ");
	webServer.onNotFound([](){
		if(!handleFileRead(webServer.uri()))
			webServer.sendHeader("Location", String("/"), true);
			webServer.send(302, "text/plain", "");
			//webServer.send(404, "text/plain", "404: Not Found");
	});
	webServer.begin();
	Serial.println("done");
	
	Serial.print("awaiting connection...");
}

void loop() {
	if(WiFi.softAPgetStationNum()<=0){
		digitalWrite(LED_RED, HIGH);
		digitalWrite(LED_GREEN, LOW);
		digitalWrite(LED_BLUE, LOW);
	}else{
		if(webSocketConnected){
			digitalWrite(LED_RED, LOW);
			digitalWrite(LED_GREEN, HIGH);
			digitalWrite(LED_BLUE, LOW);
		}else{
			digitalWrite(LED_RED, HIGH);
			digitalWrite(LED_GREEN, HIGH);
			digitalWrite(LED_BLUE, LOW);
		}
	}
	webServer.handleClient();
	MDNS.update();
	dnsServer.processNextRequest();
	webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
	switch(type){
		case WStype_DISCONNECTED:
		{
			Serial.printf("[%u] Disconnected!\n", num);
			webSocketConnected = false;
		}
			break;
		case WStype_CONNECTED:
		{
			IPAddress ip = webSocket.remoteIP(num);
			Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
			webSocket.sendTXT(num, "Connected");
		}
			break;
		case WStype_TEXT:
		{
			Serial.printf("[%u] get Text: %s\n", num, payload);
			if(payload[0] == '#') {
				uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
				analogWrite(LED_RED,    ((rgb >> 16) & 0xFF));
				analogWrite(LED_GREEN,  ((rgb >> 8) & 0xFF));
				analogWrite(LED_BLUE,   ((rgb >> 0) & 0xFF));
			}
			
			if(webSocketConnected){
				Serial.printf("len: %d - %s\n", length, payload);
				if(length==1){
					char ch = payload[0];
					switch(ch){
						case 'q': {s(); q();} break;
						case 'w': {s(); w();} break;
						case 'e': {s(); e();} break;
						case 'a': {s(); a();} break;
						case 's': {s(); s();} break;
						case 'd': {s(); d();} break;
						case 'z': {s(); z();} break;
						case 'x': {s(); x();} break;
						case 'c': {s(); c();} break;
					}
				}
			}
			
			if(payload[0]=='R'&&payload[1]=='e'&&payload[2]=='a'&&payload[3]=='d'&&payload[4]=='y'){
				webSocket.sendTXT(num, "Go!");
				webSocketConnected = true;
				
			}
		}
			break;
		case WStype_BIN:
		{
			Serial.printf("[%u] get binary length: %u\n", num, length);
			hexdump(payload, length);
		}
			break;
	}
}

bool handleFileRead(String path){
	Serial.println("handleFileRead: " + path);
	if(path.endsWith("/")){path += "index.html";}
	String contentType = getContentType(path);
	if(SPIFFS.exists(path)){
		File file = SPIFFS.open(path, "r");
		size_t sent = webServer.streamFile(file, contentType);
		file.close();
		return true;
	}
	Serial.println("\tFile Not Found");
	return false;
}

String getContentType(String filename){
	if(filename.endsWith(".htm")) return "text/html";
	else if(filename.endsWith(".html")) return "text/html";
	else if(filename.endsWith(".css")) return "text/css";
	else if(filename.endsWith(".js")) return "application/javascript";
	else if(filename.endsWith(".png")) return "image/png";
	else if(filename.endsWith(".gif")) return "image/gif";
	else if(filename.endsWith(".jpg")) return "image/jpeg";
	else if(filename.endsWith(".ico")) return "image/x-icon";
	else if(filename.endsWith(".xml")) return "text/xml";
	else if(filename.endsWith(".pdf")) return "application/x-pdf";
	else if(filename.endsWith(".zip")) return "application/x-zip";
	else if(filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

void listRoot(){
	Serial.println("fs root:");
	Dir dir = SPIFFS.openDir("/");
	while(dir.next()){
		Serial.print("\t");
		Serial.println(dir.fileName());
	}
}

void q()
{
	digitalWrite(M1_D, LOW);
	digitalWrite(M2_D, LOW);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, MID);
	Serial.println("q");
}

void w(){
	digitalWrite(M1_D, LOW);
	digitalWrite(M2_D, LOW);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, FULL);
	Serial.println("w");
}

void e()
{
	digitalWrite(M1_D, LOW);
	digitalWrite(M2_D, LOW);
	analogWrite(M1_S, MID);
	analogWrite(M2_S, FULL);
	Serial.println("e");
}

void a()
{
	digitalWrite(M1_D, LOW);
	digitalWrite(M2_D, HIGH);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, FULL);
	Serial.println("a");
}

void s()
{
	analogWrite(M1_S, 0);
	analogWrite(M2_S, 0);
	digitalWrite(M1_S, LOW);
	digitalWrite(M2_S, LOW);
	Serial.println("s");
}

void d()
{
	digitalWrite(M1_D, HIGH);
	digitalWrite(M2_D, LOW);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, FULL);
	Serial.println("d");
}

void z()
{
	digitalWrite(M1_D, HIGH);
	digitalWrite(M2_D, HIGH);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, MID);
	Serial.println("z");
}

void x()
{
	digitalWrite(M1_D, HIGH);
	digitalWrite(M2_D, HIGH);
	analogWrite(M1_S, FULL);
	analogWrite(M2_S, FULL);
	Serial.println("x");
}

void c()
{
	digitalWrite(M1_D, HIGH);
	digitalWrite(M2_D, HIGH);
	analogWrite(M1_S, MID);
	analogWrite(M2_S, FULL);
	Serial.println("c");
}
