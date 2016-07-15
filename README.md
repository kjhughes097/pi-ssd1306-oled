# Raspberry Pi - SSD1306 128x64 OLED Display - C Code

## Background
This project was an exercise in reinventing the wheel for the purpose of learning...
There are already a number of libraries that allow you to write to these little 128x64 OLED displays, but that would be too easy (and wouldn't teach us anything), so I decided to write my own.

## Features
The current version allows you to instantiate a 'SSD1306' object and use that to send text to the display.Here's a few of the other features:

 * LINE WRAP - If enabled (off by default) it allows lines of text that are longer than the display can show on one line to wrap onto the next line.
 * SCROLL - If enabled (on by default) then if a new line of text is to be displayed and the screen is 'full' at the moment then the existing text will scroll up (top line disappears) and the new line will be displayed/appended to the bottom.
 * CLEAR - Blanks the display, removing all text
 * DISPLAY - Currently it supports all letters (A-Z), lowercase characters are converted to uppercase. It also supports numbers and the folloing punctuation: !"#$%&'()*+,-./:;<=>?@[\]^_` (and <SPACE>). Any other characters sent to it will be displayed as a small solid block.
 
 

 


