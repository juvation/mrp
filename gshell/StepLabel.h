#ifndef STEP_LABEL_H
#define STEP_LABEL_H


#include <common.h>

#include <qlabel.h>


class Step;


class StepLabel : public QLabel
{

public:

    StepLabel(QWidget         *parent,        
              Step            *step);

    void set_text(const char *text);

    Step *get_step();

private:
    Step            *step;
};



#endif

