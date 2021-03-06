//This software is used to collect data from small scale pixel array detector prototypes.
//It requires the Qt (not really but it's there), libtins, and opencv libraries.
//Version 0.01 Super Beta-, Hugh Philipp 2019_03_06, Cornell University

//If bugs are found, please inform me so I can fix the code.
//If code is copied or modified and used elsewhere, please note its origin.

// updating identity

#ifndef PADDATAVAC_H
#define PADDATAVAC_H

#include <tins/tins.h>
#include <tins/hw_address.h>
#include <vector>
#include <bitset>
#include <iostream>


namespace PADspace
{

const int BANK1_OFFSET = 4;  //define things in terms of pixels
const int BANK2_OFFSET = 81;  // these are the offsets in the received packets that denote the beginning of data for the banks.
const int BANK3_OFFSET = 158; // (235+76)*4 = 1244, reserved packet size: 1257 - check this out
const int BANK4_OFFSET = 235;

enum class BankMap { RegMap, RevMap, RevMapfours, RevMapones, RevMaptwos,RevMapthrees };


class PADdataVac
{
public:
    PADdataVac();
    PADdataVac(BankMap);
    ~PADdataVac();
    Tins::SnifferConfiguration sniffconfig;
    Tins::Sniffer *sniffy;
    Tins::PDU *packetData;
    Tins::PDU::serialization_type inputbuff;

    int frameCount;
    int bmap[76]; //bank map for digital
    int bmapA[76]; //bank map for analog
    uint32_t digtemp;

    int pixbytes = (64+12)*4;

    void setBankMap(BankMap); //set digital Bank Map
    void setBankMapA(BankMap); //set analog bank Map
    void getBank(int loc_in_packet ,int analogoffset_in_bank, uint16_t*, uint16_t*);
    void getBank(int loc_in_packet,int analogoffset_in_bank, int  digitaloffset_in_bank, uint16_t*, uint16_t*);
    void getBankGain(int loc_in_packet, int  digitaloffset_in_bank, uint8_t*);

   // void getFrame(int analogoffset_in_bank, uint16_t*, uint16_t*);
   // void getFrame(int analogoffset_in_bank, int digitaloffset_in_bank, uint16_t*, uint16_t*);
   // void getFrame(uint16_t*, uint16_t*);
    void getFrame(uint16_t*, uint16_t*,uint8_t*,int = 9, int = 12);
    void nextFrame();

};





} // end PADspace
#endif // PADDATAVAC_H
