#include <CornerHeader.h>


#include <TrackHeader.h>



CornerHeader::CornerHeader(QWidget *parent, MRPSequencer &mrp_seq) 
  : QLabel(parent),
    mrp_seq(mrp_seq),
    select_toggle(false)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));

    setPaletteForegroundColor(QColor(HEADER_FG_COLOR));

    setFixedWidth(TrackHeader::get_width());
}




CornerHeader::~CornerHeader()
{
}




void CornerHeader::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    if (select_toggle)
    {
        select_toggle = false;

        for (int i = 0; i < mrp_seq.GetTrackCount(); i++)
        {
            mrp_seq.UnSelectTrack(i + 1);
        }

    }
    else
    {
        select_toggle = true;

        for (int i = 0; i < mrp_seq.GetTrackCount(); i++)
        {
            mrp_seq.SelectTrack(i + 1);
        }
    }
}




void CornerHeader::enterEvent(QEvent *e)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR).light(125));
}




void CornerHeader::leaveEvent(QEvent *e)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));
}




