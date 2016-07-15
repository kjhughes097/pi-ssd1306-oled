#include "ssd1306.h"



int main( int argc, char * argv [] ) {

	SSD1306 myDisplay;
	myDisplay.initDisplay();

    for (int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-i")==0) {
			myDisplay.initDisplay();
		}
		if(strcmp(argv[i], "-c")==0) {
			myDisplay.clearDisplay();
		}
		if(strcmp(argv[i], "-ww")==0) {
			myDisplay.setWordWrap(TRUE);
		}
		if(strcmp(argv[i], "-dw")==0) {
			myDisplay.setDisplayMode(SSD1306::Mode::WRAP);
		}
		if(strcmp(argv[i], "-t")==0) {
			if((i + 1) < argc)
			{
				myDisplay.textDisplay(argv[i+1]);
				i++;
			}
		}
    } 

/*
	SSD1306 myDisplay;
	myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);
	myDisplay.setWordWrap(TRUE);
	myDisplay.textDisplay("THIS IS THE FIRST LINE.");
	myDisplay.textDisplay("LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("PUNCTUATION !\"#$%&'()*+,-./:;<=>?@[\\]^_`");
	myDisplay.textDisplay("NUMBERS 0123456789");
	myDisplay.setWordWrap(FALSE);
	myDisplay.textDisplay("LONG NO WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("THIS IS THE LAST LINE.");
	myDisplay.textDisplay("DISPLAY WRAP.");

	//myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);
	for(int i=0; i<20; i++)
	{
		char buffer [10];
		sprintf (buffer, "LINE %c", i + 64);
		myDisplay.textDisplay(buffer);
	}
	*/
}