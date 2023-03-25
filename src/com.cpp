#include "com.h"
#include "defines.h"
#include <Arduino.h>

enum recv_states
{
    RECV_SB, //Start Byte
    RECV_PLS, //Payload Size
    RECV_PL, //Payload
    RECV_CS //Checksum
};

enum send_states
{
    SEND_IDLE,
    SEND_RDY
};

#pragma pack(1)
typedef struct{
    uint8_t start_byte;
    uint8_t command;
    uint8_t payload_size;
    uint8_t payload[256];
    uint16_t fletcher16;
} pack_t;
#pragma pack()

pack_t incoming_pack, rdy_in_pack;
uint8_t incoming_eb = 0; //Expected bytes

pack_t outgoing_pack;
enum send_states send_state = SEND_IDLE;

enum recv_states recv_state = RECV_SB;

uint16_t fletcher16(void* data, uint16_t len) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    uint8_t* p = (uint8_t*)data;
    while (len) {
        uint16_t tlen = len > 20 ? 20 : len;
        len -= tlen;
        do {
            sum2 += sum1 += *p++;
        } while (--tlen);
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }
    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;
}

void init_com() {
    Serial.begin(115200);
    //Serial1.begin(115200);
    memset(&incoming_pack, 0, sizeof(pack_t));
    incoming_pack.start_byte = 0xFE;
    memset(&outgoing_pack, 0, sizeof(pack_t));
    outgoing_pack.start_byte = 0xFE;
    
}

uint8_t send_pack(uint8_t cmd, void* data, uint8_t len) {
    outgoing_pack.command = cmd;
    outgoing_pack.payload_size = len;
    memcpy(&outgoing_pack.payload, data, len);
    outgoing_pack.fletcher16 = fletcher16(&outgoing_pack.payload, len);
    Serial.write((uint8_t*) &outgoing_pack, 3 + outgoing_pack.payload_size);
    Serial.write((uint8_t*) &outgoing_pack.fletcher16, 2);
}

void tick_com() {
    if(Serial.available()){
        uint8_t in_byte = (uint8_t) Serial.read();
        switch(recv_state){
            case RECV_SB:
                if(in_byte == 0xFE){
                    //Serial.println("0xFE recv");
                    recv_state = RECV_PLS;
                }
                break;
            case RECV_PLS:
                //Serial.print("PLS ");
                //Serial.println(in_byte);
                incoming_pack.payload_size = in_byte;
                incoming_eb = in_byte;
                recv_state = RECV_PL;
                break;
            case RECV_PL:
                //Serial.print("|");
                incoming_pack.payload[incoming_pack.payload_size - incoming_eb] = in_byte;
                if(incoming_eb - 1 > 0){
                    incoming_eb--;
                }else{
                    //Serial.println("");
                    incoming_eb = 2; //We expect two bytes for the checksum
                    recv_state = RECV_CS;
                }
                break;
            case RECV_CS:
                if(incoming_eb == 2){
                    incoming_pack.fletcher16 |= in_byte; //Set lower 8 bits
                    incoming_eb--;
                }else{
                    incoming_pack.fletcher16 |= in_byte << 8; //Set upper 8 bits

                    if(incoming_pack.fletcher16 == fletcher16(incoming_pack.payload, incoming_pack.payload_size)){ //Verified
                        //Serial.println("CS ok");
                        memcpy(&incoming_pack, &rdy_in_pack, sizeof(pack_t));
                        digitalWrite(LED3_PIN, !digitalRead(LED3_PIN));
                    }else{
                        //Serial.println("CS rejected");
                        //Reject
                    }

                    recv_state = RECV_SB;
                }
        }
    }
}