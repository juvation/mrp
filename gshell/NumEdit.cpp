#include <NumEdit.h>

#include <MRPEvent.h>

#include <main.h>



NumEdit::NumEdit(QWidget *parent, const char *initial_value, const char format)
  : QLineEdit(parent),
    format(format)
{
    setFrameStyle(QFrame::NoFrame);

    setText(initial_value);

    setPaletteBackgroundColor(QColor(EDIT_BG_COLOR));

    setLineWidth(0);

    QFont font(DEFAULT_FONT);

    setFont(font);

    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setPaletteForegroundColor(QColor(EDIT_FG_COLOR));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    g_mac_style->setFocusRectPolicy(this, QMacStyle::FocusDisabled);

}



QSize NumEdit::sizeHint() const
{
    return QSize(30, 20);
}



void NumEdit::keyPressEvent(QKeyEvent *e)
{

    switch (e->key())
    {
    case Qt::Key_Return:
        value = text().toDouble();
        emit value_changed(value);
        break;

    case Qt::Key_Up:
        set_value(value + 1.0);
        emit value_changed(value);
        break;

    case Qt::Key_Down:
        set_value(value - 1.0);
        emit value_changed(value);
        break;

    default:
        QLineEdit::keyPressEvent(e);
    }
}




void NumEdit::set_value(double value)
{
    this->value = value;

    char buf[16];

    switch (format)
    {
    // int
    case 'i':
        snprintf(buf, sizeof(buf), "%d", (int)value);
        break;

    // float
    case 'f':
        snprintf(buf, sizeof(buf), "%1.1lf", value);
        break;

    }

    setText(buf);
}



double NumEdit::get_value()
{
    return value;
}




void NumEdit::write_value(char *buf, int buf_len)
{

    switch (format)
    {
    // int
    case 'i':
        snprintf(buf, buf_len, "%d", text().toInt());
        break;

    // float
    case 'f':
        snprintf(buf, buf_len, "%1.1lf", text().toDouble());
        break;

    // note
    case 'n':
        snprintf(buf, buf_len, "%s", text().ascii());
        break;

    default:
        buf[0] = '\0';
    }
}
