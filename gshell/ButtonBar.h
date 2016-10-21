#ifndef BUTTON_BAR_H
#define BUTTON_BAR_H

#include <list>
using std::list;

#include <qframe.h>
#include <qlayout.h>

#include <CommandButton.h>
#include <LabelButton.h>

class NumEdit;
class SequencerWindow;


class ButtonBar : public QFrame
{

public:  

    ButtonBar(QWidget *parent, SequencerWindow *seq_win);

    ~ButtonBar();


private:

    QVBoxLayout      *layout;

    SequencerWindow  *seq_win;

    LabelButton      *play_button;

    CommandButton    *new_track_button,
                     *copy_track_button,
                     *delete_track_button,
                     *set_note_button,
                     *set_port_button,
                     *set_channel_button,
                     *set_velocity_button,
                     *set_gate_button,
                     *mute_button,
		     *unmute_button,
		     *invert_mute_button,
		     *hsolo_button,
		     *hunsolo_button,
		     *vsolo_button,
                     *vunsolo_button,
		     *invert_selection_button,
		     *fit_to_key_button,
		     *rotate_button,
		     *vary_button,
		     *randomise_note_button,
		     *randomise_port_button,
		     *randomise_channel_button,
		     *randomise_velocity_button,
		     *randomise_gate_button,
                     *randomise_mute_button,
                     *set_loop_button;
};





#endif
