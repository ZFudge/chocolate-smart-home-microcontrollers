#ifndef RGBS_H
#define RGBS_H

const byte NUM_COLORS = 9;
const byte MAX_PALETTES = 2;

// Default "Outrun" palette
byte rgbs[MAX_PALETTES][NUM_COLORS][3] = {
  {
    { 0, 255, 255 },    // cyan
    { 3, 148, 252 },    // turquoise
    { 3, 65, 252 },     // blue 1
    { 0, 0, 255 },      // blue 2
    { 85, 0, 255 },     // purple 1
    { 119, 3, 252 },    // purple 2
    { 186, 3, 252 },    // pink 1
    { 222, 25, 255 },   // pink 2
    { 252, 3, 165 },    // ruby
  },
};

#endif
