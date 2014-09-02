#ifndef __allduino_h
#define __allduino_h

// General include for all files.
// has some basic macros and includes proper arduino libraries
//

#ifdef SPARK
 #include <application.h>
#elif (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

#ifdef SPARK
 #define ANALOG_MAX 4095
 #define LED_PIN 7
 #define NAN -1
#else
 #define ANALOG_MAX 1023
 #define LED_PIN 13
#endif

#define TIME_SINCE(time) ((uint16_t)(millis() - (time)))

#define RUN_ONLY_EVERY(time) \
    static uint16_t start = millis() - ((time) + 10); \
    if((uint16_t)(millis() - start) < (time)) \
        return; \
    start = millis();


#define HEARTBEAT() \
    static uint8_t  _HB_led = 2; \
    static uint16_t _HB_start = millis() - ((1000) + 10); \
    if((uint16_t)(millis() - _HB_start) > (1000)) { \
        if(_HB_led == 2){_HB_led=1;pinMode(LED_PIN, OUTPUT);} \
        _HB_start = millis(); \
        digitalWrite(LED_PIN, _HB_led); \
        _HB_led = 1 - _HB_led; \
    }

extern char* itoa(int a, char* buffer, unsigned char radix);
uint16_t bstrncpy(char **destination, const char *source, uint16_t len);
char *d2strf (double val, signed char width, unsigned char prec, char *sout);

#endif
