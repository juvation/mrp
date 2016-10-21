#ifndef GATE_LABEL_H
#define GATE_LABEL_H


#include <common.h>

#include <StepLabel.h>



class GateLabel : public StepLabel
{

public:

    GateLabel(QWidget  *parent,        
              Step     *step);

    void set_gate(int gate);


private:

};



#endif

