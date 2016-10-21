#include <CommandButton.h>


CommandButton::CommandButton(QWidget    *parent, 
                             const char *label, 
                             const char *cmd,
                             const char *param_spec,
                             const char *values[])
    : QHBox(parent),
      button(this, label, false)
{

    this->cmd        = strdup(cmd);

    this->param_spec = strdup(param_spec);

    connect((QObject *)&button, SIGNAL(clicked()),
            (QObject *)this, SLOT(button_clicked()));


    //
    //  create edit widgets from param_spec
    //
    int n = strlen(param_spec);

    for (int i = 0; i < n; i++)
    {
        param_widgets.push_back(new NumEdit(this, values[i], param_spec[i]));
    }

}




CommandButton::~CommandButton()
{
    num_edit_list_t::iterator i = param_widgets.begin();

    while (i != param_widgets.end())
    {
        NumEdit *num_edit = *i;
        
        delete num_edit;

        ++i;
    }        

    free(param_spec);

    free(cmd);
}



void CommandButton::button_clicked()
{
    char cmd_buf[1024];

    strcpy(cmd_buf, cmd);

    int n = strlen(cmd);



    num_edit_list_t::iterator i = param_widgets.begin();

    while (i != param_widgets.end())
    {
        if (i == param_widgets.begin())
        {
            cmd_buf[n++] = ' ';
        }

        NumEdit *num_edit = *i;
        
        char buf[16];

        num_edit->write_value(buf, 16);

        strcpy(cmd_buf + n, buf);

        n += strlen(buf);

        cmd_buf[n++] = ' ';

        ++i;
    }

    cmd_buf[n] = '\0';

    emit clicked(cmd_buf);
}


