#include <Step.h>



Step::Step(QWidget          *parent, 
	   SequencerWindow  *seq_win,
	   MRPSequencer     &mrp_seq,
	   int              track_num,
	   int              step_num) 
    : QVBox(parent),
      mrp_seq(mrp_seq),
      track_num(track_num),
      step_num(step_num),
      sub_vbox(this),
      note_label(&sub_vbox,     this),
      channel_label(&sub_vbox,  this, mrp_seq),
      velocity_label(&sub_vbox, this),
      gate_label(&sub_vbox,     this),
      mute_label(&sub_vbox,     this)
{

    //
    //  we use the frame of this widget itself to show
    //  selection, by setting up the frame style and toggling
    //  the foreground color.
    //
    setMargin(0);
    setLineWidth(1);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setPaletteForegroundColor(QColor(STEP_BG_COLOR));

    QFont font(DEFAULT_FONT);
    setFont(font);


    update_from_event();


    setFixedWidth(get_width());

    sub_vbox.setSpacing(0);

    sub_vbox.setMargin(0);

    sub_vbox.setPaletteBackgroundColor(QColor(STEP_BG_COLOR));

    sub_vbox.setPaletteForegroundColor(QColor(STEP_FG_COLOR));
}




Step::~Step()
{
}



int Step::get_width()
{
    QFontMetrics fm(QFont(DEFAULT_FONT));

    //
    //  the width will be the widest string plus some padding
    //  XXX

    return fm.width("MMM");
}




void Step::update_from_event()
{
    MRPEvent &mrp_event = mrp_seq.GetEvent
			(track_num + 1, step_num + 1);
    MRPEventStatus &mrp_event_status = mrp_seq.GetEventStatus
			(track_num + 1, step_num + 1);

    int note = mrp_event.GetData1();

    int previous_note = -1;

    if (step_num > 0)
    {
	MRPEvent &previous_mrp_event = mrp_seq.GetTrack(track_num + 1).GetEvent(step_num);

	previous_note = previous_mrp_event.GetData1();
    }

    note_label.set_note(note, previous_note);

    channel_label.set_channel(mrp_event.GetPort(), mrp_event.GetChannel());

    velocity_label.set_velocity(mrp_event.GetData2());

    gate_label.set_gate(mrp_event.GetGateTime());

    mute_label.set_mute(mrp_event_status.IsMuted(), 
			mrp_event_status.IsHSoloed(), 
			mrp_event_status.IsVSoloed());

    if (mrp_event_status.IsSelected())
    {
	setPaletteForegroundColor(QColor(SELECTION_COLOR));
    }
    else
    {
	setPaletteForegroundColor(QColor(STEP_BG_COLOR));
    }
}




void Step::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    MRPEventStatus &mrp_event_status = mrp_seq.GetEventStatus
			(track_num + 1, step_num + 1);

    if (mrp_event_status.IsSelected())
    {
	mrp_seq.UnSelectEvent(track_num + 1, step_num + 1);
    }
    else
    {
	mrp_seq.SelectEvent(track_num + 1, step_num + 1);
    }
}




void Step::enterEvent(QEvent *e)
{
    setPaletteForegroundColor(QColor(STEP_HOVER_COLOR));
}




void Step::leaveEvent(QEvent *e)
{
    MRPEventStatus &mrp_event_status = mrp_seq.GetEventStatus
			(track_num + 1, step_num + 1);

    if (mrp_event_status.IsSelected())
    {
	setPaletteForegroundColor(QColor(SELECTION_COLOR));
    }
    else
    {
	setPaletteForegroundColor(QColor(STEP_BG_COLOR));
    }
}




