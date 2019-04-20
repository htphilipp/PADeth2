//This software is used to collect data from small scale pixel array detector prototypes.
//It requires the Qt (not really but it's there), libtins, and opencv libraries.
//Version 0.01 Super Beta-, Hugh Philipp 2019_03_06, Cornell University

//If bugs are found, please inform me so I can fix the code.
//If code is copied or modified and used elsewhere, please note its origin.
//updating identity


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
    frameCount = 0;
    setBankMap(PADspace::BankMap::RevMap);
    setBankMapA(PADspace::BankMap::RevMapfours);
}

PADspace::PADdataVac::PADdataVac(PADspace::BankMap mapOB)
{
    sniffconfig.set_promisc_mode(true);
    sniffconfig.set_direction(PCAP_D_IN);
    sniffconfig.set_immediate_mode(true);
    sniffconfig.set_snap_len(1257);

    sniffconfig.set_buffer_size(long(200000000));
    inputbuff.clear();
    inputbuff.reserve(1257);
    sniffy = new Tins::Sniffer("eth0",sniffconfig);
    frameCount = 0;
    setBankMap(mapOB);
}


PADspace::PADdataVac::~PADdataVac()
{
    delete sniffy;
    delete packetData;

}


void PADspace::PADdataVac::getBank(int loc, int aoffsetin, uint16_t *abank, uint16_t *dbank)
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
        *(abank+bmapA[i]) = 0;
        //*(dbank+bmap[i]) = 0;
        digtemp = 0;

        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4])&(0x3F))<<10);
        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4+1]))<<2);
        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4+2])&(0xC0))>>6);
        digtemp |=((uint16_t(inputbuff[k*4])&(0x3F))<<10);
        digtemp |=((uint16_t(inputbuff[k*4+1]))<<2);
        digtemp |=((uint16_t(inputbuff[k*4+2])&(0xC0))>>6);

        *(abank+bmapA[i])|=((uint16_t(inputbuff[j*4+2])&(0x3F))<<8);
        *(abank+bmapA[i])|=((uint16_t(inputbuff[j*4+3])&0xFF));

        //*(dbank+bmap[i]) = float(digtemp);
        *(dbank+bmap[i]) = (digtemp);
    }

    // std::cout<<std::endl;
}

void PADspace::PADdataVac::getBank(int loc, int aoffsetin, int doffsetin, uint16_t *abank, uint16_t *dbank)
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

        *(abank+bmapA[i]) = 0;
        //*(dbank+bmap[i]) = 0;
        digtemp = 0;


        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4])&(0x3F))<<10);
        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4+1]))<<2);
        //*(dbank+bmap[i])|=((uint32_t(inputbuff[k*4+2])&(0xC0))>>6);
        digtemp |=((uint16_t(inputbuff[k*4])&(0x3F))<<10);
        digtemp |=((uint16_t(inputbuff[k*4+1]))<<2);
        digtemp |=((uint16_t(inputbuff[k*4+2])&(0xC0))>>6);

        *(abank+bmapA[i])|=((uint16_t(inputbuff[j*4+2])&(0x3F))<<8);
        *(abank+bmapA[i])|=((uint16_t(inputbuff[j*4+3])&0xFF));
        //*(dbank+bmap[i]) = float(digtemp);
        *(dbank+bmap[i]) = (digtemp);

    }
   // std::cout<<std::endl;
}

void PADspace::PADdataVac::getBankGain(int loc, int doffsetin, uint8_t *gbank)
{
    int k;

    int doffset;
    doffset = doffsetin;

    if((doffset>75)||(doffset<0))
    {
        doffset = 0;
    }


    for(auto i = 0; i< 76; i++)
    {
        k= loc + (i - doffset + 76)%76;

        *(gbank+bmap[i]) = 0;

        *(gbank+bmap[i]) |=((uint8_t(inputbuff[k*4])&(0x80))>>7);

    }
    //std::cout<<std::endl;
}



void PADspace::PADdataVac::nextFrame()
{
     packetData =   sniffy->next_packet();
     inputbuff  =   packetData->serialize();
}

//void PADspace::PADdataVac::getFrame(uint16_t* aframe, uint16_t* dframe)
//{

//    nextFrame();
// //    getBank(BANK4_OFFSET,9,0,(aframe),(dframe));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
// //    getBank(BANK2_OFFSET,9,0,(aframe+76),(dframe+76));
// //    getBank(BANK3_OFFSET,9,0,(aframe+152),(dframe+152));
// //    getBank(BANK1_OFFSET,9,0,(aframe+228),(dframe+228));
//    getBank(BANK4_OFFSET,9,0,(aframe),(dframe+228));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
//    getBank(BANK2_OFFSET,9,0,(aframe+76),(dframe+76));
//    getBank(BANK3_OFFSET,9,0,(aframe+152),(dframe+152));
//    getBank(BANK1_OFFSET,9,0,(aframe+228),(dframe));

//    frameCount++;
//    //std::cout<<" Frame number: "<<frameCount<<std::endl;

//}

void PADspace::PADdataVac::getFrame(uint16_t* aframe, uint16_t* dframe, uint8_t* gframe,int aoffset, int doffset)
{

    nextFrame();
//    getBank(BANK4_OFFSET,aoffset,doffset,(aframe),(dframe));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
//    getBank(BANK2_OFFSET,aoffset,doffset,(aframe+76),(dframe+76));
//    getBank(BANK3_OFFSET,aoffset,doffset,(aframe+152),(dframe+152));
//    getBank(BANK1_OFFSET,aoffset,doffset,(aframe+228),(dframe+228));
    getBank(BANK4_OFFSET,aoffset,doffset,(aframe),(dframe+228));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
    getBank(BANK2_OFFSET,aoffset,doffset,(aframe+76),(dframe+76));
    getBank(BANK3_OFFSET,aoffset,doffset,(aframe+152),(dframe+152));
    getBank(BANK1_OFFSET,aoffset,doffset,(aframe+228),(dframe));

    getBankGain(BANK4_OFFSET,doffset,(gframe+228));
    getBankGain(BANK2_OFFSET,doffset,(gframe+76));
    getBankGain(BANK3_OFFSET,doffset,(gframe+152));
    getBankGain(BANK1_OFFSET,doffset,(gframe));

    frameCount++;
    //std::cout<<" Frame number: "<<frameCount<<std::endl;

}

//void PADspace::PADdataVac::getFrame(int aoffset, uint16_t* aframe, uint16_t* dframe)
//{
//    nextFrame();
// //    getBank(BANK4_OFFSET,aoffset,0,(aframe),(dframe));
// //    getBank(BANK2_OFFSET,aoffset,0,(aframe+76),(dframe+76));
// //    getBank(BANK3_OFFSET,aoffset,0,(aframe+152),(dframe+152));
// //    getBank(BANK1_OFFSET,aoffset,0,(aframe+228),(dframe+228));
//    getBank(BANK4_OFFSET,9,0,(aframe),(dframe+228));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
//    getBank(BANK2_OFFSET,9,0,(aframe+76),(dframe+76));
//    getBank(BANK3_OFFSET,9,0,(aframe+152),(dframe+152));
//    getBank(BANK1_OFFSET,9,0,(aframe+228),(dframe));
//    frameCount++;
//    //std::cout<<" Frame number: "<<frameCount<<std::endl;

//}

//void PADspace::PADdataVac::getFrame(int aoffset, int doffset, uint16_t* aframe, uint16_t* dframe)
//{
//    nextFrame();
// //    getBank(BANK4_OFFSET,aoffset,doffset,(aframe),(dframe));
// //    getBank(BANK2_OFFSET,aoffset,doffset,(aframe+76),(dframe+76));
// //    getBank(BANK3_OFFSET,aoffset,doffset,(aframe+152),(dframe+152));
// //    getBank(BANK1_OFFSET,aoffset,doffset,(aframe+228),(dframe+228));
//    getBank(BANK4_OFFSET,aoffset,doffset,(aframe),(dframe+228));   // note changed default offset to 0 for analog. - see if readin works with old octave function.
//    getBank(BANK2_OFFSET,aoffset,doffset,(aframe+76),(dframe+76));
//    getBank(BANK3_OFFSET,aoffset,doffset,(aframe+152),(dframe+152));
//    getBank(BANK1_OFFSET,aoffset,doffset,(aframe+228),(dframe));
//    frameCount++;
//   // std::cout<<" Frame number: "<<frameCount<<std::endl;

//}

void PADspace::PADdataVac::setBankMap(BankMap bm)
{
    int i;

    if(bm == PADspace::BankMap::RegMap)
    {
        for(i = 0; i<76; i++)
        {
            bmap[i] = (i%4)*19+i/4;
        }
    }
    else if(bm == PADspace::BankMap::RevMap)
    {
        for(i = 0; i<76; i++)
        {
            bmap[i] = (3-(i%4))*19+i/4;
        }
    }

}

void PADspace::PADdataVac::setBankMapA(BankMap bm)
{
    int i;
    int dd;
    dd=0;

    if(bm == PADspace::BankMap::RegMap)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (i%4)*19+i/4;
        }
    }
    else if(bm == PADspace::BankMap::RevMap)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (3-(i%4))*19+i/4;
        }
    }
    else if(bm == PADspace::BankMap::RevMapfours)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (3-(i%4))*19+i/4;
        }
        for(i=0;i<76;i++)
        {
            if(!((i+4)%4))
            {
                bmapA[i]-=(2);

                if((bmapA[i]/19)!=(bmapA[i]+2)/19)
                {
                    bmapA[i]+=19;
                }
            }
            std::cout<<bmapA[i]<<",";
        }
        std::cout<<std::endl;
    }
    else if(bm == PADspace::BankMap::RevMapthrees)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (3-(i%4))*19+i/4;
        }
        for(i=0;i<76;i++)
        {
            if(!((i+3)%4))
            {
                bmapA[i]-=(2);
                if((bmapA[i]/19)!=(bmapA[i]+2)/19)
                {
                    bmapA[i]+=19;
                }

            }

        }

    }
    else if(bm == PADspace::BankMap::RevMaptwos)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (3-(i%4))*19+i/4;
        }
        for(i=0;i<76;i++)
        {
            if(!((i+2)%4))
            {
                bmapA[i]-=(2);
                if((bmapA[i]/19)!=(bmapA[i]+2)/19)
                {
                    bmapA[i]+=19;
                }
            }

        }

    }
    else if(bm == PADspace::BankMap::RevMapones)
    {
        for(i = 0; i<76; i++)
        {
            bmapA[i] = (3-(i%4))*19+i/4;
        }
        for(i=0;i<76;i++)
        {
            if(!((i+1)%4))
            {
                bmapA[i]-=(2);
                if((bmapA[i]/19)!=(bmapA[i]+2)/19)
                {
                    bmapA[i]+=19;
                }
            }

        }

    }
}


