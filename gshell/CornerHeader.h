#ifndef CORNER_HEADER_H
#define CORNER_HEADER_H


#include <qlabel.h>

#include <MRPSequencer.h>

#include <common.h>

class CornerHeader : public QLabel
{

public: 

    CornerHeader(QWidget *parent, MRPSequencer &mrp_seq);

    ~CornerHeader();

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

private:

    MRPSequencer &mrp_seq;

    bool         select_toggle;
};




#endif




