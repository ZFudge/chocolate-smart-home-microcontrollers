Install Adafruit NeoPixel library

https://github.com/adafruit/Adafruit_NeoPixel

Install ESP32 core version =< 2.0.17
Higher versions have a refactored implementation of RMT, which is used by esps in the Adafruit NeoPixel library. This refactor causes crash loops when calling strip.show if using higher numbers of neo pixels.

https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-boards-manager
