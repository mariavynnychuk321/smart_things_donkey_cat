#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

// pins
volatile short button = 2;
short servo = 11;
// state
volatile bool isEmpty = false;

int magicNum = 3;     				// [how much times a day cat eats] - 1
short i = 0;        				// how much times worked
int feedTime[4][2];  				// array with feed time
  
void getTiming() {
	
}

// react on button presure changes
void isr() {
  	
  	if (!isEmpty) { 				// when button is PRESSED (food is there)
  		unsigned short zeros = 1; 	// need to react on LOW
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

  	else if (isEmpty) {				// when button UNPRESSED (there's no food)
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

// controling servo
int prewAngle = 0;
void moveServo(int degree){
  
	int start = millis();
  
  	while(millis() - start < map(abs(degree - prewAngle), 0, 180, 10, 500)) {
    	int impulseTime = map(degree, -90, 90, 0, 2000) + 500;
    	digitalWrite(servo, HIGH);
    	delayMicroseconds(impulseTime);
    	digitalWrite(servo, LOW);
    	delay(20);
  	}
  
	prewAngle = degree;
  
}

void giveFood() {
  Serial.println("inside giveFood()");
	
	if (isEmpty) {

		// need delay on some time to not throw food straight on cat 
		// + 10 mins i think

		// open container
		moveServo(120);
		
		// when enough food close
		if (!isEmpty) {
			moveServo(0);
		}

	}

}

void setup() {

	pinMode(button, INPUT);
	pinMode(servo, OUTPUT);
  	attachInterrupt(0, isr, LOW);

  	Serial.begin(9600);
}

void loop () {
	
/*
  	Serial.print(feedTime[0][0]);
  	Serial.print(":");
  	Serial.print(feedTime[0][1]);
  	Serial.println(); 
*/

//  TEST
//  feedTime[0][0] = 11;
//  feedTime[0][1] = 21;

	tmElements_t currentTime;
  
	// read time
	if (RTC.read(currentTime)) {

		if (currentTime.Hour == feedTime[i][0] && currentTime.Minute == feedTime[i][1]) {
			
			giveFood();
			i++;
      
      		if (i == magicNum) {
        		i = 0;
      		}
		  	
		  	Serial.println(i);
      
		}

   		else {
	    	Serial.print("wrong time, i = ");
        	Serial.println(i);
	    	Serial.print(currentTime.Hour);
	    	Serial.print(":");
	    	Serial.print(currentTime.Minute);
  		}

	}
	
	else {
		
		if (RTC.chipPresent()) {
	    	Serial.println("The DS1307 is stopped.  Please run the SetTime");
	    	Serial.println("example to initialize the time and begin running.");
	    	Serial.println();
		} 

		else {
	      	Serial.println("DS1307 read error!  Please check the circuitry.");
	      	Serial.println();
	    }
    	
    	delay(9000);
  	}

//   delay(1000);
}
