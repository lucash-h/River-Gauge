/*
found at https://www.arduino.cc/en/Tutorial/MKRGSMExamplesSendSMS on nov 21 2023
*/

/*

 SMS sender

 This sketch, for the MKR GSM 1400 board,sends an SMS message

 you enter in the serial monitor. Connect your Arduino with the

 GSM shield and SIM card, open the serial monitor, and wait for

 the "READY" message to appear in the monitor. Next, type a

 message to send and press "return". Make sure the serial

 monitor is set to send a newline when you press return.

 Circuit:

 * MKR GSM 1400 board

 * Antenna

 * SIM card that can send SMS

 created 25 Feb 2012

 by Tom Igoe

*/

// Include the GSM library
#include <MKRGSM.h>
#include "arduino_secrets.h"
#include <string.h>
#include <iostream>  
using namespace std;  

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number

const char PINNUMBER[] = secret_pin_number;

const int trigPin = 13;
const int echoPin = 14;

// initialize the library instance

GSM gsmAccess;
GSM_SMS sms;

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // initialize serial communications and wait for port to open:

  Serial.begin(9600);

  while (!Serial) {
    delay(100);
    Serial.println("Waiting");
    ; // wait for serial port to connect. Needed for native USB port only

  }

  Serial.println("SMS Messages Sender");
  // connection state
  bool connected = false;

  while (!connected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      connected = true;
      Serial.println("MADEIT");

    } else {
      Serial.println("Not connected");
      delay(10);

    }
  }
  Serial.println("GSM initialized");
}

void loop() {

  Serial.print("Enter a mobile number: ");
  char remoteNum[20];
  readSerial(remoteNum);
  Serial.println(remoteNum);

  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);

  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  sms.beginSMS(remoteNum);

  if (strcmp(txtMsg, "GIVEMEVALUE") == 0) {
    int average_distance;
    // Ultrasonic sensor distance measurement loop
    Serial.println("Measuring distance...");

    average_distance = record_ultrasonic_values();

    Serial.print("avg value: ");
    Serial.println(average_distance);

    sms.println("Distance measurement complete:");
    sms.println(average_distance);

  } else {
    // Send the regular message
    sms.println(txtMsg);

    Serial.println("Nearly Done");

  }
//previous attempt stopped here, not sure why as it printed "Nearly done" into serial port
  sms.endSMS();

  Serial.println("\nCOMPLETE!\n");
}

/*
  Read input serial
 */

int readSerial(char result[]) {

  int i = 0;

  while (1) {

    while (Serial.available() > 0) {
      char inChar = Serial.read();

      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }

      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

int record_ultrasonic_values() {
  long duration;
  int distance;
  unsigned long long summed_distance = 0;
  int avg_distance;
  int count = 0;

  while (count < 50) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
      summed_distance += distance;

      Serial.print(count);
      Serial.print(" Distance: ");
      Serial.print(distance);
      Serial.print(" Summed distance: ");
      Serial.print(summed_distance);
      Serial.print("\n");

      count++;
    }

    avg_distance = summed_distance / count;
    return avg_distance;


}
