#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Servo.h>
#include <string.h>

// pins
volatile short button = 2;
short servo = 11;
Servo myServo;

volatile bool isEmpty = true;

String data = "-9";					// data incoming from user
short magicNum;						// [how much times a day cat eats]
short i;							// how much times worked
int feedTime[10];					// array with feed time  

void isr1() {
  	
	if (isEmpty == false) {					// when button is PRESSED (food is there)
		unsigned short zeros = 1;	// need to react on LOW
		unsigned short ones = 0;

		for (unsigned short i = 0; i < 32; i++) {
			if (digitalRead(button) == LOW) {
		    	zeros++; 
			}
			else {
				ones++;
			}  
		}

		if (zeros > ones) {
			isEmpty = true; 		// means button UNPRESSED -> food is over  
		}
	}
  
}

void isr2() {

	if (isEmpty = true) {					// when button UNPRESSED (there's no food)
		unsigned short zeros = 0; 
		unsigned short ones = 1; 	// need to react on HIGH

		for (unsigned short i = 0; i < 32; i++) {
			if (digitalRead(button) == LOW) {
				zeros++; 
			}
			else {
				ones++;
			}  
		}

		if (ones > zeros) {
			isEmpty = false; 		// means button PRESSED again -> food is there
		}
	}

}

void giveFood() {

	// green when gives food
    digitalWrite(10, HIGH); // red
    digitalWrite(12, LOW);  // green
    digitalWrite(13, HIGH); // blue

	if (isEmpty) {

		// need delay on some time to not throw food straight on cat
    
		while (isEmpty) {
			myServo.write(99);
		}
   
    	if (!isEmpty) myServo.write(0); // close
	
	}

	// check iterations
	short check = i;
	i += 2;

	if (i > check) {
	// if iterated - blink red
		digitalWrite(10, LOW); // red
        digitalWrite(12, HIGH);  // green
        digitalWrite(13, HIGH); // blue
        delay(300);
	}

	if (i == magicNum - 1) {
		i = 0;
	}
	
}

void parseData() {

	for (int i = 0; i < 10; i++) {
		int index = data.indexOf(" ");
		feedTime[i] = atoi(data.substring(0, index).c_str());
		data = data.substring(index + 1);
	}

	// count how mush times cat eats
	for (int i = 0; i < 9; i += 2) {
		if (feedTime[i] != 0 && feedTime[i + 1] != 0)
			magicNum += 2;
	}

}

void setup() {

  	pinMode(10, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    
	pinMode(button, INPUT);
	pinMode(servo, OUTPUT);

	attachInterrupt(0, isr1, LOW);
	attachInterrupt(0, isr2, HIGH);
	myServo.attach(servo);

	Serial.begin(9600);
	
	myServo.write(0);
	i = 0;
	magicNum = 0;

}

void loop () {

	while (Serial.available()) {
	    if (data == "-9") {
		    data = Serial.readString();
			parseData();
	    }
    }

  	// PURPLE LIGHT IN WAITING MODE
	digitalWrite(10, LOW); // red
	digitalWrite(12, HIGH);  // green
	digitalWrite(13, LOW); // blue
  	
  	// tracking data existing (red)
  	if (feedTime[0] != 9) {
    	digitalWrite(10, LOW); // red
    	digitalWrite(12, HIGH);  // green
    	digitalWrite(13, HIGH); // blue
  	}

    tmElements_t currentTime;
 
//	TEST
//	feedTime[0] = 15;
//	feedTime[1] = 26;  
  
	// read time
	if (RTC.read(currentTime)) {
    
    	// time stores like [hour] [minute] [hour] [minute] ......
		if (currentTime.Hour == feedTime[i] && currentTime.Minute == feedTime[i + 1]) {
			
			/* works actualy, so problem is not here
			digitalWrite(10, LOW); // red
        	digitalWrite(12, HIGH);  // green
        	digitalWrite(13, HIGH); // blue
        	delay(300);
         	*/

			giveFood();

		}
   
	}
	
	else {
		
		if (RTC.chipPresent()) {
			Serial.println("The DS1307 is stopped.  Please run the SetTime");
		} 

		else {
			Serial.println("DS1307 read error!  Please check the circuitry.");
	    }

		delay(5000);
	}

//   delay(1000);
}
