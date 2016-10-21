#include <VelocityLabel.h>

#include <main.h>

#include <Theme.h>



VelocityLabel::VelocityLabel(QWidget  *parent, 
			     Step     *step)
    : StepLabel(parent, step)
{
}



void VelocityLabel::set_velocity(int velocity)
{
    //
    //  set graded background color
    //
    QColor color = g_theme->get_graded_color(Theme::kVelocityBackgroundColor, velocity);

    setPaletteBackgroundColor(color);

    char buf[16];
    sprintf(buf, "%d", velocity);
    setText(buf);
}







