#ifndef LABEL_BUTTON_H
#define LABEL_BUTTON_H


#include <common.h>

#include <qhbox.h>
#include <qlabel.h>



class LabelButton : public QHBox
{
    Q_OBJECT

public:

    LabelButton(QWidget *parent, const char *text, bool is_toggle);

    ~LabelButton();

    void set_toggle(bool on);

protected:

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);


signals:

    void clicked();


private:

    QLabel *label;
    bool   is_toggle;
    bool   toggle;
};



#endif

