// Params for width and height
const uint8_t kMatrixWidth = 6;
const uint8_t kMatrixHeight = 6;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];
#define LAST_VISIBLE_LED 35
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
    30,  29,  18,  17,   6,   5,
    31,  28,  19,  16,   7,   4,
    32,  27,  20,  15,   8,   3,
    33,  26,  21,  14,   9,   2,
    34,  25,  22,  13,  10,   1,
    35,  24,  23,  12,  11,   0
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}
