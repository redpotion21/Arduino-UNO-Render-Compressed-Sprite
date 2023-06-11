/*
  Based on: Simple "Hello World" for ILI9341 LCD
  https://wokwi.com/arduino/projects/308024602434470466

  sprite from: https://opengameart.org/content/tiny-platform-quest-sprites
  sprite license: CC-BY-SA 3.0

*/

///////////////////////////////// DO NOT TOUCH/////////////////
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
/////////////////////////////////////////////////////////////////////

void DecodeImg(byte* arr, byte arr_size, byte x_pos, byte y_pos, Adafruit_ILI9341 tft);
byte ReadBits(byte x, byte low_range, byte high_range);
int pow2(int i);

uint16_t palette[7] = {0x0000, 0x4000, 0x630C, 0xC618, 0xFF14, 0x4498, 0xFFFF};
//{Black, Red, Dark_gray, Light_gray, Pale, Blue, White}

byte sprite_size = 62+ 16;
byte sprite[] = {0x00, //L1
0x00, //L2
0x17, 0x04,
0x37, 0x01, 0x12, 0x01,
0x17, 0x04,
0x34, 0x04, 0x22, 0x04,
0x34, 0x01, 0x28, 0x01,
0x34, 0x01, 0x28, 0x01, //L8
0x74, 0x01, 0x21, 0x42, 0x22, 0x42, 0x21, 0x01,
0x74, 0x01, 0x21, 0x41, 0x51, 0x22, 0x51, 0x05, //L10
0x94, 0x01, 0x21, 0x41, 0x51, 0x22, 0x51, 0x01, 0x33, 0x01,
0x53, 0x03, 0x45, 0x01, 0x33, 0x01,
0x73, 0x01, 0x31, 0x01, 0x25, 0x01, 0x33, 0x01,
0x53, 0x03, 0x25, 0x01, 0x33, 0x01,
0x54, 0x01, 0x22, 0x06, 0x31, 0x02,
0x34, 0x04, 0x62, 0x05};

int deb_lines = 0;
int deb_y = 0;

void setup() {
  tft.begin();
  tft.fillScreen(0xFFFF);
  //tft.drawLine(0,0,100,100,0xFFFF);  
  DecodeImg(sprite, sprite_size, 50, 50, tft);

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println(deb_lines);
   tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println(deb_y);

  /*

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println("Hello, TFT!");

  tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println("I can has colors?");

  */
  // Meme reference: https://english.stackexchange.com/questions/20356/origin-of-i-can-haz
}

void loop() { }

void DecodeImg(byte arr[], byte arr_size, byte x_pos, byte y_pos, Adafruit_ILI9341 tft){
	byte cnt = 0;
	byte num_lines;
	byte start_pos;
	byte trans_x = x_pos - 7;
	byte trans_y = y_pos + 7;
	byte tmp_x = 0;
	byte tmp_y = 0;

  //tft.drawFastHLine(trans_x , trans_y , 1, 0xC000); //debug

  byte i = 0;
	while(cnt != arr_size){
		num_lines = ReadBits(*(arr + cnt), 4,7);
		start_pos = ReadBits(*(arr + cnt), 0,3);
		cnt+=1;
		for(int i = 0; i < num_lines; i ++){
		  tft.drawFastHLine(start_pos + trans_x + tmp_x, trans_y + tmp_y, ReadBits(*(arr + cnt), 0,3), *(palette + ReadBits(*(arr + cnt), 4,7)) );
      tmp_x += ReadBits(*(arr + cnt), 0,3);
      cnt += 1;

      //deb_lines+=1; //debug
    }
    tmp_x = 0;
    tmp_y +=1;

    //deb_y+=1;//debug

  }
}

byte ReadBits(byte x, byte low_range, byte high_range){
	byte result;
	for (int i = 0; i <= high_range - low_range; i++){
	result += bitRead(x, low_range + i) * pow2(i); //bitRead is Arduino only
}
return result;
}

int pow2(int i){
  int result = 1;

  for (int j = 0; j<i; j++) result*=2;

  return result;
}