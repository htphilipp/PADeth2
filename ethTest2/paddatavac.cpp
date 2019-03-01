#include "paddatavac.h"

PADspace::PADdataVac::PADdataVac()
{
    sniffconfig.set_promisc_mode(true);
    sniffconfig.set_direction(PCAP_D_IN);
    sniffconfig.set_immediate_mode(true);
    sniffconfig.set_snap_len(1257);

    sniffconfig.set_buffer_size(long(200000000));
    inputbuff.clear();
    inputbuff.reserve(1257);
    sniffy = new Tins::Sniffer("eth0",sniffconfig);

}

void PADspace::PADdataVac::getBank(int loc, int aoffsetin, uint16_t *abank, uint32_t *dbank)
{
    int j;
    int k;
    int aoffset;
    aoffset = aoffsetin;

    if((aoffset>75)||(aoffset<0))
    {
        aoffset = 0;
    }

    for(auto i = 0; i< 76; i++)
    {
        j= loc+(i - aoffset + 76)%76;
        k= loc+i;
        *(abank+i) = 0;
        *(dbank+i) = 0;

        *(dbank+i)|=((uint32_t(inputbuff[k*4])&(0x3F))<<10);
        *(dbank+i)|=((uint32_t(inputbuff[k*4+1]))<<2);
        *(dbank+i)|=((uint32_t(inputbuff[k*4+2])&(0xC0))>>6);
        *(abank+i)|=((uint32_t(inputbuff[j*4+2])&(0x3F))<<8);
        *(abank+i)|=((uint32_t(inputbuff[j*4+3])&0xFF));

       // std::cout<<"j"<< j*4+3 << ", k: "<< k*4+2 << "i: "<< i <<std::endl;

    }
}

void PADspace::PADdataVac::getBank(int loc, int aoffsetin, int doffsetin, uint16_t *abank, uint32_t *dbank)
{
    int j;
    int k;

    int aoffset,doffset;
    doffset = doffsetin;
    aoffset = aoffsetin;

    if((aoffset>75)||(aoffset<0))
    {
        aoffset = 0;
    }
    if((doffset>75)||(doffset<0))
    {
        doffset = 0;
    }


    for(auto i = 0; i< 76; i++)
    {
        j= loc +(i - aoffset + 76)%76;
        k= loc + (i - doffset + 76)%76;

        *(abank+i) = 0;
        *(dbank+i) = 0;

        *(dbank+i)|=((uint32_t(inputbuff[k*4])&(0x3F))<<10);
        *(dbank+i)|=((uint32_t(inputbuff[k*4+1]))<<2);
        *(dbank+i)|=((uint32_t(inputbuff[k*4+2])&(0xC0))>>6);
        *(abank+i)|=((uint32_t(inputbuff[j*4+2])&(0x3F))<<8);
        *(abank+i)|=((uint32_t(inputbuff[j*4+3])&0xFF));
       //std::cout<<"j"<< j*4+3 << ", k: "<< k*4+2 << "i: "<< i <<std::endl;

    }

}



void PADspace::PADdataVac::nextFrame()
{
     packetData =   sniffy->next_packet();
     inputbuff  =   packetData->serialize();
}

void PADspace::PADdataVac::getFrame(uint16_t* aframe, uint32_t* dframe)
{

    nextFrame();
    std::cout<<"bank 1"<<std::endl;
    getBank(BANK4_OFFSET,9,0,(aframe),(dframe));
    std::cout<<"bank 2"<<std::endl;
    getBank(BANK2_OFFSET,9,0,(aframe+76),(dframe+76));
    std::cout<<"bank 3"<<std::endl;
    getBank(BANK3_OFFSET,9,0,(aframe+152),(dframe+152));
    std::cout<<"bank 4"<<std::endl;
    getBank(BANK1_OFFSET,9,0,(aframe+228),(dframe+228));

}

void PADspace::PADdataVac::getFrame(int aoffset, uint16_t* aframe, uint32_t* dframe)
{
    nextFrame();
    getBank(BANK4_OFFSET,aoffset,0,(aframe),(dframe));
    getBank(BANK2_OFFSET,aoffset,0,(aframe+76),(dframe+76));
    getBank(BANK3_OFFSET,aoffset,0,(aframe+152),(dframe+152));
    getBank(BANK1_OFFSET,aoffset,0,(aframe+228),(dframe+228));

}

void PADspace::PADdataVac::getFrame(int aoffset, int doffset, uint16_t* aframe, uint32_t* dframe)
{
    nextFrame();
    getBank(BANK4_OFFSET,aoffset,doffset,(aframe),(dframe));
    getBank(BANK2_OFFSET,aoffset,doffset,(aframe+76),(dframe+76));
    getBank(BANK3_OFFSET,aoffset,doffset,(aframe+152),(dframe+152));
    getBank(BANK1_OFFSET,aoffset,doffset,(aframe+228),(dframe+228));

}
