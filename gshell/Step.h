#ifndef STEP_H
#define STEP_H


#include <qlabel.h>
#include <qvbox.h>

#include <MRPEvent.h>
#include <MRPEventStatus.h>
#include <MRPSequencer.h>

#include <common.h>
#include <ChannelLabel.h>
#include <GateLabel.h>
#include <MuteLabel.h>
#include <NoteLabel.h>
#include <StepLabel.h>
#include <VelocityLabel.h>


class SequencerWindow;




class Step : public QVBox
{

public: 

    Step(QWidget         *parent, 
	 SequencerWindow *seq_win,
	 MRPSequencer    &mrp_seq,
	 int             track_num, 
	 int             step_num);

    ~Step();

    static int get_width();

    void update_from_event();

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

  

private:

    MRPSequencer   &mrp_seq;

    int            track_num, step_num;

    QVBox          sub_vbox;

    NoteLabel      note_label;

    ChannelLabel   channel_label;

    VelocityLabel  velocity_label;

    GateLabel      gate_label;

    MuteLabel      mute_label;
};




#endif




