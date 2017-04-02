#include <LiquidCrystal.h>
#include <BigCrystal.h>
#include <LcdProgressBar.h>
#include <CmdMessenger.h>
const int BAUD_RATE = 9600;
CmdMessenger c = CmdMessenger(Serial,',',';','/');

const int VERSION = 1;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

BigCrystal screen(&lcd);

byte percentage_1[8] = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
byte percentage_2[8] = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
byte percentage_3[8] = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
byte percentage_4[8] = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
byte percentage_5[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };



void lcd_percentage(int percentage, int cursor_x, int cursor_x_end, int cursor_y){
  
  int calc = (percentage*cursor_x_end*5/100)-(percentage*cursor_x*5/100);
  while(calc >= 5){
    screen.setCursor(cursor_x,cursor_y);
    screen.write((byte)6);
    calc-=5;
    cursor_x++;
  }
  while(calc >= 4 && calc < 5){
    screen.setCursor(cursor_x,cursor_y);
    screen.write((byte)6);
    calc-=4;

  }
  while(calc >= 3 && calc < 4){
    screen.setCursor(cursor_x,cursor_y);
    screen.write((byte)6);
    calc-=3;
  }
  while(calc >= 2 && calc < 3){
    screen.setCursor(cursor_x,cursor_y);
    screen.write((byte)6);
    calc-=2;
  }
  while(calc >= 1 && calc < 2){
    screen.setCursor(cursor_x,cursor_y);
    screen.write((byte)6);
    calc-=1;
  }
  
}

const int GEAR_X = 0;
const int GEAR_Y = 0;

const int ERS_X = 4;
const int ERS_Y = 0;
const int ERS_LENGTH = 12;

const int RPM_X = 4;
const int RPM_Y = 1;

enum {
  handshake,
  change_gear,
  change_rpm,
  change_ers,
};

// CALLBACKS
void onHandhsake() {
  c.sendCmd(handshake,"RaceDuino ver: " + String(VERSION));
}

void onChangeRPM()
{
  int rpm = c.readBinArg<int>();
  draw_rpm(rpm);
}

void onChangeERS() {
  float ers = c.readBinArg<float>();
  int ers_percent = (int)(ers*100);
  draw_kers(ers_percent);
}

void onChangeGear() {
  int gear = c.readBinArg<int>();
  char gear_s[2];
  if (gear == 0) {
    gear_s[0] = 'R';
  } else if (gear == 1) {
    gear_s[0] = 'N';
  } else {

    int a=1;
    char b[2];
    String str;
    str=String(a);
    str.toCharArray(b,2); 
    gear_s[0] = b[0];
  }
  draw_gear(gear_s);
}

void attach_callbacks() {
  c.attach(handshake, onHandhsake);
  c.attach(change_gear, onChangeGear);
  c.attach(change_rpm, onChangeRPM);
  c.attach(change_ers, onChangeERS);
}



void setup() {
  
  lcd.createChar(9, percentage_1);
  //lcd.createChar(10, percentage_2);
  //lcd.createChar(11, percentage_3);
  //lcd.createChar(12, percentage_4);
  //lcd.createChar(13, percentage_5);
  
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(BAUD_RATE);
  attach_callbacks();    
}

void print(auto data, int X, int Y)
{
  screen.setCursor(X,Y);
  screen.print(data);
}

void loop() {
  c.feedinSerialData();
  draw_labels();
  //print("arriba", 4,0);
  delay(200);  
}  

void draw_labels()
{
  // KERS Label
  print("ERS:",ERS_X,ERS_Y);
  print("RPM:", RPM_X, RPM_Y);
}

void draw_gear(char gear[])
{
  print("    ", GEAR_X, GEAR_Y);
  print("    ", GEAR_X, 1);

  screen.printBig(gear,GEAR_X,GEAR_Y);
}

void draw_kers(int kers)
{
  print("        ", ERS_X+4, ERS_Y);
  lcd_percentage(kers, ERS_X+4, ERS_LENGTH+4, ERS_Y);
}

void draw_rpm(int rpm)
{
  print("        ", RPM_X+4, RPM_Y);
  print(rpm, RPM_X+4, RPM_Y+4);
}

