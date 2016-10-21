#ifndef MUTE_LABEL_H
#define MUTE_LABEL_H


#include <common.h>

#include <StepLabel.h>



class MuteLabel : public StepLabel
{

public:

    MuteLabel(QWidget  *parent,        
              Step     *step);

    void set_mute(bool muted, bool hsoloed, bool vsoloed);


private:

};



#endif

