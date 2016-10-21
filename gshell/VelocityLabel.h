#ifndef VELOCITY_LABEL_H
#define VELOCITY_LABEL_H


#include <common.h>

#include <StepLabel.h>



class VelocityLabel : public StepLabel
{

public:

    VelocityLabel(QWidget  *parent,        
                  Step     *step);

    void set_velocity(int velocity);


private:

};



#endif

