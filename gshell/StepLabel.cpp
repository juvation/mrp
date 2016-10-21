#include <StepLabel.h>

#include <Step.h>


StepLabel::StepLabel(QWidget  *parent, 
		     Step     *step)
    : QLabel(parent),
      step(step)
{

    setMargin(1);

    QFont font(DEFAULT_FONT);
    setFont(font);
    setAlignment(Qt::AlignCenter);

    setPaletteBackgroundColor(QColor(STEP_BG_COLOR));
    setPaletteForegroundColor(QColor(STEP_FG_COLOR));
}



void StepLabel::set_text(const char *text)
{
    setText(text);
}




Step *StepLabel::get_step()
{
    return step;
}



