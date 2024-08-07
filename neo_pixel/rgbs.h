#ifndef RGBS_H
#define RGBS_H


const byte NUM_COLORS = 9;
const byte PIXELS_LENGTH = 27;


// Default "Outrun" palette
byte rgbs[NUM_COLORS][3] = {
    { 0, 255, 255 },    // cyan
    { 3, 148, 252 },    // turquoise
    { 3, 65, 252 },     // blue 1
    { 0, 0, 255 },      // blue 2
    { 85, 0, 255 },     // purple 1
    { 119, 3, 252 },    // purple 2
    { 186, 3, 252 },    // pink 1
    { 222, 25, 255 },   // pink 2
    { 252, 3, 165 },    // ruby
};

String getCsvRGBs() {
    String csvRGBs;
    for (byte i = 0; i < PIXELS_LENGTH; i++) {
        csvRGBs += rgbs[i / 3][i % 3];
        if (i < PIXELS_LENGTH - 1)
            csvRGBs += ",";
    }
    return csvRGBs;
}


#endif
