all:
	gcc oled.cpp ssd1306.cpp -o oled -std=c++11

clean:
	rm -f oled

