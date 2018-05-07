#include <string.h>

String data;
bool toReadOrNotToRead = false;

int magicNum = 0;
int feedTime[10];

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {

    if (toReadOrNotToRead == false) {

        while (Serial.available()) {
             data = Serial.readString();
        }
    
        //if (data == "9 40 10 40 11 40") digitalWrite(13, HIGH);

        // parse data
        for(int i = 0; i < 10; i++) {
            int index = data.indexOf(" ");
            feedTime[i] = atoi(data.substring(0, index).c_str());
            data = data.substring(index + 1);
        }

        // count how mush times cat eats
        for(int i = 0; i < 9; i += 2) {
            if (feedTime[i] != 0 && feedTime[i + 1] != 0)
              magicNum ++;
        }

        // if(magicNum == 3) digitalWrite(13, HIGH);
    }
  
}
