#ifndef COMMAND_BUTTON_H
#define COMMAND_BUTTON_H

#include <list>
using std::list;

#include <qhbox.h>

#include <LabelButton.h>
#include <NumEdit.h>


typedef list<NumEdit *>    num_edit_list_t;



class CommandButton : public QHBox
{
    Q_OBJECT

public:

    CommandButton(QWidget      *parent, 
                  const char   *label, 
                  const char   *cmd,
                  const char   *param_spec,
                  const char   *values[]);

    ~CommandButton();


public slots:

    void button_clicked();



signals:

    void clicked(char *);



private:

    char             *cmd;

    char             *param_spec;

    LabelButton      button;

    num_edit_list_t  param_widgets;
};






#endif
