#ifndef TRACK_HEADER_H
#define TRACK_HEADER_H


#include <qlabel.h>
#include <qhbox.h>
#include <qvbox.h>

#include <MRPSequencer.h>


#include <common.h>



class TrackHeader : public QHBox
{

public: 

    TrackHeader(QWidget *parent, MRPSequencer &mrp_seq, int track_num);

    ~TrackHeader();

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);


    static int get_width();



private:

    MRPSequencer &mrp_seq;

    int          track_num;

    QVBox        *vbox;

    QLabel       *track_label, *note_label, 
                 *channel_label, *velocity_label, *gate_label,
                 *mute_label;

    bool         select_toggle;
};




#endif




