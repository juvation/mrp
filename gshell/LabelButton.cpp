#include <LabelButton.h>




LabelButton::LabelButton(QWidget *parent, const char *text, bool is_toggle)
  : QHBox(parent),
    is_toggle(is_toggle),
    toggle(false)
{

    setLineWidth(0);

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setPaletteForegroundColor(QColor(DEFAULT_BG_COLOR));

    setSpacing(1);


    label = new QLabel(this);

    label->setPaletteBackgroundColor(QColor(BUTTON_BG_COLOR));

    label->setPaletteForegroundColor(QColor(BUTTON_FG_COLOR));

    label->setMargin(3);


    QFont font(BUTTON_FONT);

    label->setFont(font);

    label->setAlignment(Qt::AlignLeft);

    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    label->setText(text);


    setFixedHeight(20);
}




LabelButton::~LabelButton()
{
    delete label;
}




void LabelButton::mousePressEvent(QMouseEvent *e)
{

    if (is_toggle)
    {
	if (toggle)
	{
	    set_toggle(false);
	}
	else
	{
	    set_toggle(true);
	}
    }

    emit clicked();
}




void LabelButton::enterEvent(QEvent *e)
{
    label->setPaletteBackgroundColor(QColor(BUTTON_BG_COLOR).light(125));
}




void LabelButton::leaveEvent(QEvent *e)
{
    if (!(is_toggle && toggle))
    {
        label->setPaletteBackgroundColor(QColor(BUTTON_BG_COLOR));
    }
}




void LabelButton::set_toggle(bool on)
{
  if (on)
  {
    toggle = true;

    //    label->setPaletteBackgroundColor(QColor(BUTTON_ACTIVE_BG_COLOR));
    label->setPaletteBackgroundColor(QColor(BUTTON_BG_COLOR).light(125));

    label->setPaletteForegroundColor(QColor(BUTTON_ACTIVE_FG_COLOR));
  }
  else
  {
    toggle = false;

    label->setPaletteBackgroundColor(QColor(BUTTON_BG_COLOR));

    label->setPaletteForegroundColor(QColor(BUTTON_FG_COLOR));
  }
}


