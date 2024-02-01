#include <Wire.h>
#include <Servo.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

/* LCD init
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
hd44780_I2Cexp lcd;
*/

String message = "";
String commandString = "";
bool stringComplete = false;

// Laser
const int laserPin = 13;

// Servo
Servo x_servo;
Servo y_servo;
int pos = 0;

// default positions of servo and settings
float min_x = 5;
float max_x = 50;
float min_y = 5;
float max_y = 35;
int min_freeze = 200;
int max_freeze = 3000;
int minimal_movement = 5;

// starting point and auto positions
int random_delay;
float x_position = min_x + (max_x - min_x)/2;
float y_position = min_y + (max_y - min_y)/2; 
float x_old_position = x_position;
float y_old_position = y_position;
float x_new_position;
float y_new_position;
float x_speed;
float y_speed;
int movement_time;

int rotspeedx;
int rotspeedy;

// automated servo
bool automated = false;



void setup() {
  int status;
  Serial.begin(9600);
  //status = lcd.begin(LCD_COLS, LCD_ROWS);
  //if(status){
    //hd44780::fatalError(status);
  //}
  pinMode(laserPin, OUTPUT);
  x_servo.attach(6);
  y_servo.attach(9);


  //lcd.print("Ready to connect");
}

void loop() {
    //LCD();
    servoLogic();
 
}

void servoLogic(){
  serialEvent();
  if(stringComplete){
    stringComplete = false;
  if(commandString.equals("1")){
    automated = true;
    commandString = "";
    while(automated){
      serialEvent();
      if(commandString.equals("Z")){
        commandString = "";
        digitalWrite(laserPin, HIGH);
      } else if (commandString.equals("S")){
        commandString = "";
        digitalWrite(laserPin, LOW);
      }
      if(commandString.equals("2")){
        commandString="";
        automated = false;
      } 
      movement_time = random(10,40);
      random_delay = random(min_freeze, max_freeze);
      x_new_position = random(min_x+minimal_movement, max_x-minimal_movement);
      y_new_position = random(min_y+minimal_movement, max_y-minimal_movement);
    
      if( (y_new_position > y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
        y_new_position = y_new_position + minimal_movement;
      }  else if ( (y_new_position < y_old_position) && (abs(y_new_position - y_old_position) < 5 )) {
        y_new_position = y_new_position - minimal_movement;
      }
    
      if( (x_new_position > x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
        x_new_position = x_new_position + minimal_movement;
      }  else if ( (x_new_position < x_old_position) && (abs(x_new_position - x_old_position) < 5 )) {
        x_new_position = x_new_position - minimal_movement;
      }
    
      x_speed = 5*(x_new_position - x_old_position)/movement_time;
      y_speed = 5*(y_new_position - y_old_position)/movement_time;  
      for (pos = 0; pos < movement_time; pos += 1) { 
          x_position = x_position + x_speed;
          y_position = y_position + y_speed;
          x_servo.write(x_position);  
          y_servo.write(y_position);                    
        delay(10); 
      }
      x_old_position = x_new_position;
      y_old_position = y_new_position;
      delay(random_delay);
  }
  } else if (commandString.equals("2")) {
    commandString="";
  } else if (commandString.equals("U")){
    commandString="";
    movement_time = random(3,6);
    for(pos=0; pos < movement_time; pos+=1){
      y_position = y_position-5;
      y_servo.write(y_position);
    }
    delay(5);
  } else if (commandString.equals("D")){
    commandString="";
    movement_time = random(3,6);
    for(pos=0; pos < movement_time; pos+=1){
      y_position = y_position+5;
      y_servo.write(y_position);
    }
    delay(5);
  } else if (commandString.equals("R")){
    commandString="";
    movement_time = random(4,10);
    for(pos=0; pos < movement_time; pos+=1){
      x_position = x_position-5;
      x_servo.write(x_position);
    }
    delay(5);
  } else if (commandString.equals("L")){
    commandString="";
    movement_time = random(4,10);
    for(pos=0; pos < movement_time; pos+=1){
      x_position = x_position+5;
      x_servo.write(x_position);
    }
    delay(5);
  } else if (commandString.equals("Z")){
    commandString = "";
    digitalWrite(laserPin, HIGH);
    delay(5);
  } else if (commandString.equals("S")){
    commandString="";
    digitalWrite(laserPin, LOW);
    delay(5);
  }
  }


}
void serialEvent(){
  if(Serial.available()){
    char c = (char)Serial.read();
    commandString+=c;
    stringComplete = true;
    Serial.println(commandString);
    /*if(c = "\n"){
      stringComplete = true;
    } else {
      message+=c;

    }
    */
  }
  
}


// BREAKING BETWEEN SETUP AND LOOP

/* SerialEvent up to LCD are for LCD DISPLAY CODE */
/*


String getTextToPrint(){
  String value = message.substring(5,message.length()-2);
  return value;
}

void printText(String text){
  lcd.clear();
  lcd.setCursor(0, 1);
  if(text.length()<16){
    lcd.print(text);
  } else {
    lcd.print(text.substring(0,16));
    lcd.setCursor(1, 1);
  }
}

void LCD(){
  serialEvent();
  if(stringComplete){
    stringComplete = false;
    if(commandString.equals("STAR")){
      lcd.clear();
      //lcd.print("Connected");
      lcd.print(message);
      message="";
      commandString = "";
    }
      else if(commandString.equals("STOP")){
      lcd.clear();
      lcd.print("Ready to connect");
      message="";
      commandString="";
    } 
    else if (commandString.equals("TEXT")){
      String text = getTextToPrint();
      printText(text);
      message="";
      commandString="";
    } else {
      lcd.clear();
      lcd.print("else");
      message="";
      commandString="";
    }
  }
}
*/



