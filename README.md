# Chocolate Smart Home Microcontrollers

Microcontroller plugins for the [Chocolate Smart Home](https://github.com/ZFudge/chocolate-smart-home) project, built for ESP32 microcontrollers using the Arduino IDE.

## Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software)

## Required Libraries

Clone the following repositories into your Arduino libraries directory:
- [Chocolate Smart Home ESP Connect](https://github.com/ZFudge/Chocolate_Smart_Home_ESP_Connect)
- [PubSubClient](https://github.com/ZFudge/pubsubclient)
- [EpoxyDuino](https://github.com/bxparks/EpoxyDuino)
- [AUnit](https://github.com/bxparks/AUnit)

## Testing

Run tests using:
```
make test | grep fail
```


## Plugins

Plugins are located in the project root directory. Each plugin has its own README.md with detailed information about:
- Setup instructions
- Additional dependencies
- Usage guidelines
- Configuration options

Please consult individual plugin documentation before use. Use at your own risk.
