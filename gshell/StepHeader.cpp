#include <StepHeader.h>

#include <Step.h>



StepHeader::StepHeader(QWidget *parent, MRPSequencer &mrp_seq, int step_num) 
    : QLabel(parent),
      mrp_seq(mrp_seq),
      step_num(step_num),
      select_toggle(false),
      is_current_step(false),
      is_loop_step(false),
      is_hovered(false)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));

    setPaletteForegroundColor(QColor(HEADER_FG_COLOR));

    setFont(QFont(HEADER_FONT));

    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setMargin(4);

    //
    //  step header number -1 is the corner header - no label
    //

    if (step_num >= 0)
    {
        char num_s[16];
        sprintf(num_s, "%d", step_num + 1);
        setText(num_s);
    }

    setFixedWidth(Step::get_width());
}




StepHeader::~StepHeader()
{
}




void StepHeader::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    if (select_toggle)
    {
        select_toggle = false;
        mrp_seq.UnSelectStep(step_num + 1);
    }
    else
    {
        select_toggle = true;
        mrp_seq.SelectStep(step_num + 1);
    }
}




void StepHeader::set_is_current_step(bool is_current_step)
{
    this->is_current_step = is_current_step;

    reset_color();
}




void StepHeader::set_is_loop_step(bool is_loop_step)
{
    this->is_loop_step = is_loop_step;

    reset_color();
}




void StepHeader::reset_color()
{
    QColor color;


    if (is_current_step)
    {
        color = QColor(CURRENT_STEP_BG_COLOR);
    }
    else
    {
        if (is_loop_step)
        {
            color = QColor(LOOP_STEP_BG_COLOR);
        }
        else
        {
            color = QColor(HEADER_BG_COLOR);
        }
    }

    if (is_hovered)
    {
        color = color.light(125);
    }

    setPaletteBackgroundColor(color);
}




void StepHeader::enterEvent(QEvent *e)
{
    is_hovered = true;

    reset_color();
}




void StepHeader::leaveEvent(QEvent *e)
{
    is_hovered = false;

    reset_color();
}




