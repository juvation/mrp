#include <MuteLabel.h>

#include <main.h>

#include <Theme.h>



MuteLabel::MuteLabel(QWidget  *parent, 
		     Step     *step)
  : StepLabel(parent, step)
{
    QFont font(MUTE_FONT);
    setFont(font);
}



void MuteLabel::set_mute(bool muted, bool hsoloed, bool vsoloed)
{
    //
    //  set graded background color
    //
    //  QColor color = g_theme->get_graded_color(Theme::kMuteBackgroundColor, port_num);

    //  setPaletteBackgroundColor(color);

    char buf[16];
    sprintf(buf, "%s%s%s", muted ? "M" : "", hsoloed ? "H" : "", vsoloed ? "V" : "");
    setText(buf);
}







