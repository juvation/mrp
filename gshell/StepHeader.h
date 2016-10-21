#ifndef STEP_HEADER_H
#define STEP_HEADER_H


#include <qlabel.h>

#include <MRPSequencer.h>

#include <common.h>

class StepHeader : public QLabel
{

public: 

    StepHeader(QWidget *parent, MRPSequencer &mrp_seq, int step_num);

    ~StepHeader();

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

    void set_is_current_step(bool is_current_step);

    void set_is_loop_step(bool is_loop_step);

private:

    void reset_color();

    MRPSequencer &mrp_seq;

    int          step_num;
  
    bool         select_toggle;

    bool         is_current_step;

    bool         is_loop_step;

    bool         is_hovered;
};




#endif




