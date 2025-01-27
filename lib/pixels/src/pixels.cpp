#include <pixels.h>


// This block will select if the ESP32 or ESP8266 is selected. It is in the cpp file to stay private,
// if I were coding this library at work, I would do this, but it could easily be placed in the header
// file if you want to interact with it in main.cpp
#ifdef ESP32
    // This section selects whether we want to initialize the Neopixel lib in RGB or RGBW. Since I made
    /// a class, I can make it so you can hot select RGB or RGBW. I just don't see a use for that. ( I 
    // looked into putting it into class, we can if we use a different lib. This one is pretty inflexable
    // after compile time. But idc rn)
    #ifdef RGBW
        NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1800KbpsMethod> strip(PIXELCOUNT, 19);
    #else
        NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXELCOUNT, PIXELPIN);
    #endif
#elif ESP8266
    #ifndef RGBW
        NeoPixelBus<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod> strip(PIXELCOUNT, 3);
    #else
        NeoPixelBus<NeoGrbwFeature, NeoEsp8266Dma800KbpsMethod> strip(PIXELCOUNT, 3);
    #endif
#else
    // Place for someone who wants to support another platform to get started easily
    NeoPixelBus<, > strip(PIXELCOUNT, 3);
    #error This was written for ESPs, if you would like to use something else, define it above
#endif

PIXELS::PIXELS(){} // I'll do something with this, I swear.

void PIXELS::init(){
    // Starts the LEDs and blanks them out
    strip.Begin();
    strip.Show();
}

void PIXELS::show(unsigned location, uint8_t R, uint8_t G, uint8_t B, uint8_t W){
    #ifdef RGBW
    strip.setPixelColor(location, RgbwColor(R,G,B,W));
    #else
    strip.SetPixelColor(location, RgbColor(R,G,B));
    #endif
    strip.Show();
}

void PIXELS::show(pixel *pixels, unsigned cnt){
    for(unsigned i = 0; i<cnt; i++){
        #ifdef RGBW
        strip.setPixelColor(i, RgbwColor(pixels[i].R,pixels[i].G,pixels[i].B,pixels[i].W));
        #else
        strip.SetPixelColor(i, RgbColor(pixels[i].R,pixels[i].G,pixels[i].B));
        #endif
    }
    strip.Show();
}

pixel *PIXELS::marshal(uint8_t *pyld, unsigned len, unsigned *pixCnt){
    if(pyld[0]!=0x50){
        // Set pixCnt to zero as we have not decoded any pixels and return NULL
        *pixCnt = 0;
        return NULL;
    }
    if (pyld[1]!=syncWord||syncWord==0x0){
        *pixCnt = 0;
        return NULL;
    }
    // Decode number of pixels, we don't have to send the entire strip if we don't want to
    uint16_t cnt = pyld[2] || pyld[3]<<8;
    
    if(cnt>PIXELCOUNT){
        // We got more pixels than the strip allows
        *pixCnt = 0;
        return NULL;
    }
    pixel *result = new pixel[cnt];
    // TODO Add logic to return if len is impossibly large or small
    for(uint16_t i = 0; i<cnt; i++){
        #ifdef RGBW
        result[i].R = pyld[4+(i*4)];
        result[i].G = pyld[4+(i*4)+1];
        result[i].B = pyld[4+(i*4)+2];
        result[i].W = pyld[4+(i*4)+3]
        #else
        result[i].R = pyld[4+(i*3)];
        result[i].G = pyld[4+(i*3)+1];
        result[i].B = pyld[4+(i*3)+2];
        #endif
    }


    // TODO Add CRC check before setting pixCnt
    *pixCnt = cnt;

    return result; // REMOVE WHEN FUNCTION FINISHED
}