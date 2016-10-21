#ifndef STEP_HEADER_ROW_H
#define STEP_HEADER_ROW_H


#include <qhbox.h>

#include <MRPSequencer.h>

#include <common.h>
#include <StepHeader.h>
#include <CornerHeader.h>
#include <Step.h>




class StepHeaderRow : public QHBox
{

public: 

    StepHeaderRow(QWidget *parent, MRPSequencer &mrp_seq, int num_steps);

    ~StepHeaderRow();

    void set_current_step(int step);

    void set_loop_step(int step);

private:
    MRPSequencer  &mrp_seq;

    int           num_steps;

    int           current_step;

    int           loop_step;

    CornerHeader *corner_header;

    StepHeader   **step_headers;
};




#endif




