#ifndef CHANNEL_LABEL_H
#define CHANNEL_LABEL_H


#include <common.h>

#include <StepLabel.h>

class MRPSequencer;


class ChannelLabel : public StepLabel
{

public:

    ChannelLabel(QWidget       *parent,        
                 Step          *step,
                 MRPSequencer  &mrp_seq);

    void set_channel(int port, int channel);


private:

    MRPSequencer &mrp_seq;

};



#endif

