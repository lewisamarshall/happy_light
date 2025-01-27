#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <InterpolationLib.h>
// #include <TimeLib.h>
#include <NTPClient.h>
// #include <ezTime.h>



//please enter your sensitive data in the Secret tab
char ssid[] = "Eshu";                // your network SSID (name)
char pass[] = "---";                // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;             // the Wi-Fi radio's status
int ledState = LOW;                       //ledState used to set the LED
unsigned long previousMillisInfo = 0;     //will store last time Wi-Fi information was updated
unsigned long previousMillisLED = 0;      // will store the last time LED was updated
const int intervalInfo = 5000;            // interval at which to update the board information

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3600*7);


// LED Port Setup
const int LED_R = 2; 
const int LED_G = 3;
const int LED_B = 5;
const int LED_W = 6;


// Time Calculations
long TimeOfDay = 0; 
const long LengthOfDay =  86400L;


// Interpolation Colors and Times
const int numValues = 4;
double xValues[4] = {   65*360,  7*3600,  75*360, 8*3600};
// test values
// double xValues[4] = {   229*360, 23*3600, 235*360, 24*3600};
double rValues[4] = {0, 255, 255, 0};
double gValues[4] = {0, 0, 255, 0};
double bValues[4] = {0, 0, 255, 0};
double wValues[4] = {0, 255, 255, 0};

void setup() {
  //Initialize serial
  // Serial.begin(9600);

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
    // Serial.print("Attempting to connect to network: ");
    // Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  // Serial.println("You're connected to the network");
  // Serial.println("---------------------------------------");  
  
  timeClient.begin();
  // waitForSync();
  // Timezone myTZ;
  // myTZ.setLocation(F("US/Pacific"));



  // time_t getNtpTime() { return (timeClient.getEpochTime()); } ;

  // setSyncProvider(getNtpTime);

  // set the LED high once wifi begins
  digitalWrite(LED_BUILTIN, HIGH);
}


void loop() {
  timeClient.update();
  // setTime(timeClient.getEpochTime());
  TimeOfDay = timeClient.getEpochTime() % LengthOfDay;
  // Serial.println(timeClient.getFormattedTime());
  // Serial.println(TimeOfDay);
  // Serial.println(myTZ.dateTime());
  // Serial.println(year());
  // Serial.println(month());
  // Serial.println(day());
  // Serial.println(hour());
  // Serial.println(minute());
  // Serial.println(second());

  if (TimeOfDay<xValues[0]){
    update_LED(0, 0, 0, 0);
  } else if (TimeOfDay>xValues[3]){
    update_LED(0, 0, 0, 0);
  } else {
    update_color(TimeOfDay);
  }

  if (status == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    }
  delay(1000);
  // unsigned long currentMillisInfo = millis();

}

void update_LED(int R, int G, int B, int W){
  // Serial.print(R);
  // Serial.print(G);
  // Serial.print(B);
  // Serial.println(W);
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



// // Output 38kHz PWM on digital pin D7 on Nano33
// void setup()
// {
//   // Feed GCLK0 at 48MHz to TCC0 and TCC1
//   GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK0 as a clock source
//                       GCLK_CLKCTRL_GEN_GCLK0 |     // Select GCLK0 at 48MHz
//                       GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK0 to TCC0 and TCC1
//   while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

//   // Enable the port multiplexer for pins D7
//   PORT->Group[g_APinDescription[7].ulPort].PINCFG[g_APinDescription[7].ulPin].bit.PMUXEN = 1;

//   // D7 is on EVEN port pin PA06 and TCC1/WO[0] channel 0 is on peripheral E
//   PORT->Group[g_APinDescription[7].ulPort].PMUX[g_APinDescription[7].ulPin >> 1].reg = /*PORT_PMUX_PMUXO_E |*/ PORT_PMUX_PMUXE_E;
  
//   // Normal (single slope) PWM operation: timer countinuouslys count up to PER register value and then is reset to 0
//   TCC1->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;         // Setup single slope PWM on TCC1
//   while (TCC1->SYNCBUSY.bit.WAVE);                 // Wait for synchronization
  
//   TCC1->PER.reg = 1262;                            // Set the frequency of the PWM on TCC1 to 38kHz: 48MHz / (1262 + 1) = 38kHz
//   while (TCC1->SYNCBUSY.bit.PER);                  // Wait for synchronization
  
//   TCC1->CC[0].reg = 630;                          // TCC1 CC0 - 50% duty cycle on D7
//   while (TCC1->SYNCBUSY.bit.CC0);                 // Wait for synchronization
  
//   TCC1->CTRLA.bit.ENABLE = 1;                     // Enable the TCC1 counter
//   while (TCC1->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
// }

// void loop() 
// { 
//   // Using buffered counter compare registers (CCBx)
//   TCC1->CCB[0].reg = 315;                         // TCC1 CCB1 - 25% duty cycle on D7
//   while (TCC1->SYNCBUSY.bit.CCB0);                // Wait for synchronization
//   delay(1000);                                    // Wait for 1 second
//   TCC1->CCB[0].reg = 945;                         // TCC1 CCB1 - 75% duty cycle on D7
//   while (TCC1->SYNCBUSY.bit.CCB0);                // Wait for synchronization
//   delay(1000);                                    // Wait for 1 second
// }
