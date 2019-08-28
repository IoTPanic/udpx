/**
 * s - Little Stream - A simple data transport protocol
 * 
 * Please read the README for more information
 */
#ifndef s_h
#define s_h

#include <Arduino.h>

#define VERSION 0

#define PKT_ACK 0x0
#define PKT_STREAM 0x1

class s
{
    public:
    typedef struct{
        uint8_t version = VERSION;
        uint8_t nodeID = 0;
        uint8_t sessionID = 0x0;
    }session;

    // Constructor
    // Takes in a node id and a sync word.
    s(uint8_t nodeId, uint8_t sync);
};
#endif