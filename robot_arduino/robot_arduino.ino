#define p_ind    13
#define p_m1_d    4
#define p_m1_s    5
#define p_m2_s    6
#define p_m2_d    7
#define srx      11
#define stx      12
#define t_speed   1
#define MID     128

#include <SoftwareSerial.h>
SoftwareSerial Serial1(srx, stx);

unsigned long t_prev = 0;

void setup(){
	pinMode(p_ind,  OUTPUT);
	pinMode(p_m1_d, OUTPUT);
	pinMode(p_m1_s, OUTPUT);
	pinMode(p_m2_s, OUTPUT);
	pinMode(p_m2_d, OUTPUT);
	Serial.begin(115200);
	Serial.println("\nrobot has been started");
	digitalWrite(p_ind, HIGH);
	Serial1.begin(115200);
	digitalWrite(p_ind, LOW);
}

void q()
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, MID);
	digitalWrite(p_m2_s, HIGH);
}

void w(){
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void e()
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, MID);
}

void a()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void s()
{
	digitalWrite(p_m1_s, LOW);
	digitalWrite(p_m2_s, LOW);
}

void d()
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void z()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, MID);
	digitalWrite(p_m2_s, HIGH);
}

void x()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void c()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, MID);
}

void loop(){
	if(Serial1.available()){
		digitalWrite(p_ind, HIGH);
		char ch = Serial1.read();
		switch(ch){
			case 'q': q(); break;
			case 'w': w(); break;
			case 'e': e(); break;
			case 'a': a(); break;
			case 's': s(); break;
			case 'd': d(); break;
			case 'z': z(); break;
			case 'x': x(); break;
			case 'c': c(); break;
		}
		Serial.print("recieved: ");
		Serial.println(ch);
		t_prev = millis();
		digitalWrite(p_ind, LOW);
	}
	if(t_prev-millis()>5000){
		s();
	}
}

