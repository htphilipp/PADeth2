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

enum class BankMap { RegMap, RevMap };


class PADdataVac
{
public:
    PADdataVac();
    ~PADdataVac();
    Tins::SnifferConfiguration sniffconfig;
    Tins::Sniffer *sniffy;
    Tins::PDU *packetData;
    Tins::PDU::serialization_type inputbuff;

    int frameCount;
    int bmap[76];
    uint32_t digtemp;

    int pixbytes = (64+12)*4;

    void setBankMap(BankMap);
    void getBank(int loc_in_packet ,int analogoffset_in_bank, uint16_t*, uint16_t*);
    void getBank(int loc_in_packet,int analogoffset_in_bank, int  digitaloffset_in_bank, uint16_t*, uint16_t*);
    void getFrame(int analogoffset_in_bank, uint16_t*, uint16_t*);
    void getFrame(int analogoffset_in_bank, int digitaloffset_in_bank, uint16_t*, uint16_t*);
    void getFrame(uint16_t*, uint16_t*);
    void nextFrame();

};





} // end PADspace
#endif // PADDATAVAC_H
