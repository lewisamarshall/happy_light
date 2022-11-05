#include <WiFiNINA.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <InterpolationLib.h>


//please enter your sensitive data in the Secret tab
char ssid[] = "ATTPQRIMAJ";                // your network SSID (name)
char pass[] = "T849rna%d#66";                // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;             // the Wi-Fi radio's status
int ledState = LOW;                       //ledState used to set the LED
unsigned long previousMillisInfo = 0;     //will store last time Wi-Fi information was updated
unsigned long previousMillisLED = 0;      // will store the last time LED was updated
const int intervalInfo = 5000;            // interval at which to update the board information

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3600*7);


// LED Port Setup
const int LED_W = 2;
const int LED_R = 5; 
const int LED_G = 6;
const int LED_B = 3;

const int loop_time=20;

// Time Calculations
long TimeOfDay = 0; 
const long LengthOfDay =  86400L;


// Interpolation Colors and Times
const int numValues = 4;
double xValues[4] = {   65*360,  7*3600,  75*360, 8*3600};
// test values
// double xValues[4] = {   188*360,  189*360,  189*360, 19*3600};
double rValues[4] = {0, 255, 255, 0};
double gValues[4] = {0, 0, 255, 0};
double bValues[4] = {0, 0, 255, 0};
double wValues[4] = {0, 255, 255, 0};

void setup() {
  //Initialize serial
  Serial.begin(9600);

  //You can wait for Serial to begin, but this will hang when disconnected
  // while (!Serial);

  // set the LED as output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // attempt to connect to Wi-Fi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  Serial.println("---------------------------------------");  
  
  timeClient.begin();

  // set the LED high once wifi begins
  digitalWrite(LED_BUILTIN, HIGH);
}


void loop() {
  timeClient.update();
  TimeOfDay = timeClient.getEpochTime() % LengthOfDay;
  Serial.println(timeClient.getFormattedTime());
  Serial.println(TimeOfDay);

  if (TimeOfDay<xValues[0]){
    update_LED(0, 0, 0, 0);
  } else if (TimeOfDay>xValues[3]){
    update_LED(0, 0, 0, 0);
  } else {
    update_color(TimeOfDay);
  }

  delay(1000);
  // unsigned long currentMillisInfo = millis();

}

void update_LED(int R, int G, int B, int W){
  Serial.print(R);
  Serial.print(G);
  Serial.print(B);
  Serial.println(W);
  analogWrite(LED_R, R);
  analogWrite(LED_G, G);
  analogWrite(LED_B, B);
  analogWrite(LED_W, W);
}

void update_color(long TOD){
  int new_R = Interpolation::Linear(xValues, rValues, numValues, TOD, false);
  int new_G = Interpolation::Linear(xValues, gValues, numValues, TOD, false);
  int new_B = Interpolation::Linear(xValues, bValues, numValues, TOD, false);
  int new_W = Interpolation::Linear(xValues, wValues, numValues, TOD, false);
  update_LED(new_R, new_G, new_B, new_W);
}