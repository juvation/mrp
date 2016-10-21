#include <TrackHeader.h>

#include <Step.h>


TrackHeader::TrackHeader(QWidget *parent, MRPSequencer &mrp_seq, int track_num) 
    : QHBox(parent),
      mrp_seq(mrp_seq),
      track_num(track_num),
      select_toggle(false)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));

    setPaletteForegroundColor(QColor(HEADER_FG_COLOR));


    track_label = new QLabel(this);
    track_label->setFont(QFont(HEADER_FONT));
    track_label->setAlignment(Qt::AlignCenter);

    char num_s[16];
    sprintf(num_s, "%d", track_num + 1);
    track_label->setText(num_s);


    vbox = new QVBox(this);

    note_label = new QLabel(vbox);
    note_label->setFont(QFont(HEADER_FONT));
    note_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    note_label->setText("n");
  
    channel_label = new QLabel(vbox);
    channel_label->setFont(QFont(HEADER_FONT));
    channel_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    channel_label->setText("c");
  
    velocity_label = new QLabel(vbox);
    velocity_label->setFont(QFont(HEADER_FONT));
    velocity_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    velocity_label->setText("v");
  
    gate_label = new QLabel(vbox);
    gate_label->setFont(QFont(HEADER_FONT));
    gate_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    gate_label->setText("g");
  
    mute_label = new QLabel(vbox);
    mute_label->setFont(QFont(HEADER_FONT));
    mute_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    mute_label->setText("s");
  


    //  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedWidth(get_width());
}




TrackHeader::~TrackHeader()
{
    delete mute_label;

    delete gate_label;

    delete velocity_label;

    delete channel_label;

    delete note_label;

    delete vbox;

    delete track_label;
}




void TrackHeader::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    if (select_toggle)
    {
        select_toggle = false;
        mrp_seq.UnSelectTrack(track_num + 1);
    }
    else
    {
        select_toggle = true;
        mrp_seq.SelectTrack(track_num + 1);
    }
}




void TrackHeader::enterEvent(QEvent *e)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR).light(125));
}




void TrackHeader::leaveEvent(QEvent *e)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));
}





int TrackHeader::get_width()
{
    QFontMetrics fm(QFont(DEFAULT_FONT));

    //  XXX

    return fm.width("MMM");
}

