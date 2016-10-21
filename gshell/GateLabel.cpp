#include <GateLabel.h>

#include <main.h>

#include <Theme.h>



GateLabel::GateLabel(QWidget  *parent, 
		     Step     *step)
  : StepLabel(parent, step)
{
}



void GateLabel::set_gate(int gate)
{
    //
    //  set graded background color
    //
    QColor color = g_theme->get_graded_color(Theme::kGateBackgroundColor, gate);

    setPaletteBackgroundColor(color);

    char buf[16];
    sprintf(buf, "%d", gate);
    setText(buf);
}







