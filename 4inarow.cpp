/*
4 in a row game

6 row seven column grid base
See adafruit-gfx-graphics-library.pdf on eClass (associated with Lecture 11),
for more details on these and other functions in the graphics library.

By default, the display has a width of 240 pixels and a height of 320 pixels.

However: the orientation we will use for the assignment is horizontal
so one should think of the width as 320 and the height as 240. This is why
we do tft.setRotation(3);
*/

#include <Arduino.h>

// The Adafruit_ILI9341 class from here inherits from the Adafruit_GFX class,
// which implements the graphics library functions.
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// TFT display and SD card will share the hardware SPI interface.
// For the Adafruit shield, these are the default.
// The display uses hardware SPI, plus #9 & #10
// Mega2560 Wiring: connect TFT_CLK to 52, TFT_MISO to 50, and TFT_MOSI to 51
#define TFT_DC 9
#define TFT_CS 10
#define SD_CD 6


// height is 240, width is 320 when oriented horizontally
#define TFT_HEIGHT 240
#define TFT_WIDTH 320

// Use hardware SPI (on Mega2560, #52, #51, and #50) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void setup() {
	init();
	Serial.begin(9600);
	// Initialize the breakout board.
	// Required for both the display and the SD card.
	tft.begin();
	// because we hold it horizontally
	tft.setRotation(3);
}
int columns = 0, rows = 5;
int fourinarow[6][7];		// fourinarow[row][col]

void drawGrid(){
	//horizontal lines
	uint16_t customColour = tft.color565(255, 0, 0);

	for(int i = 0;i < 7;i++){
		for(int c = 0;c < 4;c++){
			tft.drawLine((45*i)+c, 0, (45*i)+c, TFT_HEIGHT, customColour);
		}
	}
	//vertical lines
	for(int i = 0;i < 6;i++){
		for(int c = 0;c < 4;c++){
			tft.drawLine(0, (40*i)+c, TFT_WIDTH,(40*i)+c , customColour);
		}
	}
	//border
	for(int i = 0;i < 4;i++){
		tft.drawRect(0, 0,TFT_WIDTH-i,TFT_HEIGHT-i,customColour);
	}
}

void checkFill(){
	//checks the closest piece thats filled, then makes sure to fill the
	//one above it
	Serial.println("CHECKING BUDDY");
	for(int i = 0;i < 5;i++){
		if(fourinarow[columns][i] == 1){
			 Serial.print("row: ");Serial.println(i);
			 Serial.print("col: ");Serial.println(columns);
			rows = i;
			break;
		}
		else rows = 5;
	}
}

void drawPiece(){
	Serial.println("in here");
	for(int col = 0;col < 7;col++){
		for(int row = 5; row > 0;row--){
			// Serial.print("row: ");Serial.println(row);
			// Serial.print("col: ");Serial.println(col);
			// Serial.print("value: ");Serial.println(fourinarow[col][row]);

			if(fourinarow[col][row] == 1){
				tft.drawCircle(col*50+22-col*3,TFT_HEIGHT-(5-row)*39-21,15,WHITE);
				tft.fillCircle(col*50+22-col*3,TFT_HEIGHT-(5-row)*39-21,15,WHITE);
			}
		}
	}
	// Serial.println("GOT OUT");
}
// bool columnchange = false
void processMovement(){
	enum condition {player,CPU};
	condition state = player;

	char readKey;
	if(state == player){
		if(Serial.available() > 0){
			readKey = Serial.read();
			Serial.println(readKey);
		}
		//left and right movements
		if(readKey == 'a' && columns > 0){
			columns--;
			checkFill();
		}
		else if(readKey == 's' && columns < 6){
			columns++;
			checkFill();
		}
		//if space is pressed
		else if(readKey == 0x20){
			Serial.println("in here (space pressed)");
			//whatever column should remain the same, row should be filled with 1 (for reference)
			fourinarow[columns][rows] = 1;
			//draw circle at position [columns][rows]
			Serial.print("Col is: ");Serial.print(columns);Serial.print(" Row is:");Serial.println(rows);
			Serial.println(readKey,HEX);


			//row-- (therefore next time we'll draw at [columns][rows-1])
			if(rows >= 0){
				rows--;
				drawPiece();
			}
		}
	}
}
int main() {
	setup();
	Serial.println("here");
	// fill the screen with black
	tft.fillScreen(ILI9341_BLACK);


	//draw background grid
	drawGrid();

	Serial.println("getting here");
	//prefill array with 0
	for(int i = 0;i < 6;i++){
		for(int c = 0; c < 7;c++){
			fourinarow[i][c] = 0;
		}
	}
	Serial.println("got out");
	while(true){
		processMovement();
	}

	Serial.end();

	return 0;
}
