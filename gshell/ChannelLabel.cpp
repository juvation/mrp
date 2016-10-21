#include <ChannelLabel.h>

#include <main.h>

#include <MRPSequencer.h>
#include <Theme.h>



ChannelLabel::ChannelLabel(QWidget       *parent, 
			   Step          *step,
                           MRPSequencer  &mrp_seq)
    : StepLabel(parent, step),
      mrp_seq(mrp_seq)
{
}



void ChannelLabel::set_channel(int port, int channel)
{
    //
    //  set graded background color
    //
    QColor color = g_theme->get_graded_color(Theme::kChannelBackgroundColor, channel);

    setPaletteBackgroundColor(color);

    string name = mrp_seq.GetChannelName(port, channel);

    if (name.length() > 0)
    {
        setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setText(name);
    }
    else
    {
        setAlignment(Qt::AlignCenter);

        char buf[16];
        sprintf(buf, "%x%2d", port, channel);
        setText(buf);
    }
}







