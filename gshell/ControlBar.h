#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H


#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>



#include <NumEdit.h>
#include <LabelButton.h>


class SequencerWindow;





class ControlBar : public QFrame
{

public:

    ControlBar(QWidget *parent, SequencerWindow *seq_window);

    ~ControlBar();

    void set_bars(int bars);

    void set_steps(int steps);

    void set_bpm(double bpm);


private:
    SequencerWindow *seq_window;

    QHBoxLayout     *layout;

    QLabel          *bars_label, *steps_label, *bpm_label;

    NumEdit         *bars_edit, *steps_edit, *bpm_edit;
};








#endif
