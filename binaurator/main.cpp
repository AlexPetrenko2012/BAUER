//****************************************
//
// author: Petrenko Alexandr   17.12.2023
//
//*****************************************


#include <iostream>
#include "../src/bs2b.h"


using namespace std;




struct S_FRAME_16S  //TODO: Добавить 24 и 32  .
{
    int16_t sample1;
    int16_t sample2;
};


void process_frame_16s(t_bs2bdp &bs2bdp)

{
    S_FRAME_16S sf16;

    while (1)
    {
        cin >> sf16.sample1;
        if (cin.eof())break;
        cin >> sf16.sample2;
        if (cin.eof())break;

        //cout << "BINAURATOR 16S OUT:" << sf16.sample1 << "  " << sf16.sample2 << endl;

        bs2b_cross_feed_s16le(bs2bdp,(int16_t*)&sf16,1);

        cout << sf16.sample1 << " " << sf16.sample2 << endl;
      }
}




int main() // TODO Добавить параметры ком строки.
{   
    //cout << "MAIN_BINAURATOR" << endl;

    t_bs2bdp bs2bdp;

    uint32_t srate = BS2B_DEFAULT_SRATE;
    uint32_t level = BS2B_DEFAULT_CLEVEL;

    if(NULL == (bs2bdp = bs2b_open()))
        return -1 ;

    bs2b_set_srate( bs2bdp, srate );
    bs2b_set_level( bs2bdp, level );

    int16_t bit_rate = 16;
    bool is_stereo = true;

    switch (bit_rate)
    {
    case 16:
        if(is_stereo)
            process_frame_16s(bs2bdp);
        break;
    }

    bs2b_close(bs2bdp);

    return 0;
}
