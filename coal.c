#include <SoftwareSerial.h> #include <LiquidCrystal.h> #include <DHT.h>
#define DHT_PIN A0 #define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
const int bz = A3; const int gas = A1;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); int x=0;
#define DEBUG 1
//SoftwareSerial esp8266Module(8, 9);	// RX, TX String network = "project";
String password = "project1234";
#define IP "184.106.153.149"	// IP address of thingspeak.com String GET = "GET /update?key=QR6DP8J7G1J8T3ZS";
int cl =0;


void setup() { pinMode(bz,OUTPUT); pinMode(gas, INPUT); Serial.begin(115200);
lcd.begin(16, 2); // Set up the LCD's number of columns and rows lcd.setCursor(0,0);
lcd.print("WELCOME TO");
 
lcd.setCursor(0,1); lcd.print("IOT BASED COAL"); delay(2000);
lcd.clear(); lcd.setCursor(0,0); lcd.print("MINE SAFETY "); lcd.setCursor(0,1);
lcd.print("MONITORING ALERT");
digitalWrite(bz,LOW); delay(2000); lcd.clear(); lcd.setCursor(0,0); lcd.print("T:	H: "); lcd.setCursor(0,1); lcd.print("GAS:"); dht.begin();
//esp8266Module.begin(115200);
// setupEsp8266();


}


void loop() {
x = digitalRead(gas);
float temperature = dht.readTemperature(); float humidity = dht.readHumidity(); lcd.setCursor(0, 0);
lcd.print("T:"); lcd.print(temperature); lcd.print(" C ");
 
lcd.print("HM:"); lcd.print(humidity); lcd.print("% "); if(temperature >35)
{
digitalWrite(bz, HIGH); delay(500);
}
else
{
digitalWrite(bz, LOW);
}
if(x == LOW)
{
lcd.setCursor(0,1); lcd.print("GAS:YES "); digitalWrite(bz, HIGH); delay(500);
}
else
{
lcd.setCursor(0,1); lcd.print("GAS:NO "); digitalWrite(bz, LOW);
}
delay(1000);
//cl =cl + 1; if(cl >= 60)
{
 
lcd.setCursor(14,1); lcd.print("UP");
updateTemp(String(temperature),String(humidity), String(x)); cl = 0;
lcd.setCursor(14,1); lcd.print(" ");
}
}
void setupEsp8266()
{
Serial.flush(); Serial.println(F("AT+RST")); delay(7000);
if (Serial.find("OK"))
{
if(DEBUG){
// Serial.println("Found OK");
// Serial.println("Changing espmode");
}
Serial.flush(); changingMode(); delay(5000); Serial.flush(); connectToWiFi();
}
else
{
if(DEBUG){
// Serial.println("OK not found");
 
}
}
}
bool changingMode()
{
Serial.println(F("AT+CWMODE=1")); if (Serial.find("OK"))
{
if(DEBUG){
// Serial.println("Mode changed");
}
return true;
}
else if(Serial.find("NO CHANGE")){ if(DEBUG){
// Serial.println("Already in mode 1");
}
return true;
}
else
{
if(DEBUG){
// Serial.println("Error while changing mode");
}
return false;
}
}
bool connectToWiFi()
{
 
if(DEBUG){
//Serial.println("inside connectToWiFi");
}
String cmd = F("AT+CWJAP=\""); cmd += network;
cmd += F("\",\""); cmd += password; cmd += F("\""); Serial.println(cmd); delay(15000);

if (Serial.find("OK"))
{
if(DEBUG){
// Serial.println("Connected to Access Point");
}
return true;
}
else
{
if(DEBUG){
// Serial.println("Could not connect to Access Point");
}
return false;
}
}
void updateTemp(String Field1,String Field2,String Field3)
{
String cmd = "AT+CIPSTART=\"TCP\",\"";
 
cmd += IP;
cmd += "\",80"; Serial.println(cmd); delay(3000); if(Serial.find("Error")){ if(DEBUG){
// Serial.println("ERROR while SENDING");
}
return;
}
cmd = GET + "&field1=" + Field1 + "&field2=" + Field2 + "&field3=" + Field3 + "\r\n"; Serial.print("AT+CIPSEND=");
Serial.println(cmd.length()); delay(13000); if(Serial.find(">"))
{
Serial.print(cmd); if(DEBUG){
// Serial.println("Data sent");
}
}else
{
Serial.println("AT+CIPCLOSE"); if(DEBUG){
//Serial.println("Connection closed");
}

