#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

// pins
volatile short button = 2;
short servo = 11;
// state
volatile bool isEmpty = true;

int magicNum = 0;     				// [how much times a day cat eats]
short i = 0;        				// how much times worked
int feedTime[10];	  				// array with feed time  

// react on button presure changes
void isr1() {
  	
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
  
}

void isr2() {

	if (isEmpty) {				// when button UNPRESSED (there's no food)
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
		delay(1000);
    
		while (isEmpty) {
		  moveServo(120);
		}

    delay(1000);
    moveServo(0);

	}

}

void setup() {

	pinMode(button, INPUT);
	pinMode(servo, OUTPUT);
  	
  	attachInterrupt(0, isr1, LOW);
	attachInterrupt(0, isr2, HIGH);
  	
  	Serial.begin(9600);

}


void loop () {

  //delay(5000);
  
	tmElements_t currentTime;
 //  TEST
//feedTime[0] = 0;
//feedTime[1] = 52;  
  
	// read time
	if (RTC.read(currentTime)) {

		if (currentTime.Hour == feedTime[i] && currentTime.Minute == feedTime[i + 1]) {
			
			giveFood();
			i += 2;
      
      		if (i == magicNum - 1) {
        		i = 0;
      		}
		  	
		  	// Serial.println(i);
      
		}

   		else {
	    	Serial.print("wrong time, i = ");
        	Serial.println(i);
	    	Serial.print(currentTime.Hour);
	    	Serial.print(":");
	    	Serial.println(currentTime.Minute);
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
