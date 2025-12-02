#ifndef ADC_H_
#define ADC_H_

struct Measurement {
    int32_t x;
    int32_t y;
    int32_t z;
    int direction;  // <-- uusi kenttä
};

int initializeADC(void);
struct Measurement readADCValue(void);

#endif
