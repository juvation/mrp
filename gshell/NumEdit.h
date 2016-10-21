#ifndef NUM_EDIT_H
#define NUM_EDIT_H


#include <common.h>

#include <qlineedit.h>



class NumEdit : public QLineEdit
{
    Q_OBJECT

public:


    NumEdit(QWidget *parent, const char *initial_value, const char format);

    QSize sizeHint() const;

    void keyPressEvent(QKeyEvent *e);

    void set_value(double value);

    double get_value();

    void write_value(char *buf, int buf_len);


signals:

    void value_changed(double value);

private:
    double        value;
    const char    format;
};



#endif

