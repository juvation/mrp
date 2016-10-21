#ifndef SHELL_TEXT_EDIT_H
#define SHELL_TEXT_EDIT_H


#include <string>
using std::string;


#include <common.h>
#include <MRPShellInterface.h>
#include <MRPShell.h>

#include <qtextedit.h>


class ShellTextEdit : public QTextEdit, public MRPShellInterface
{
    Q_OBJECT

public:

    ShellTextEdit(QWidget* parent);

    ~ShellTextEdit();

    void set_shell(MRPShell *shell);

    void add_prompt();

    void parse_command();

    void select_previous_command();

    void select_next_command();

    void replace_cur_command(string cmd);

    void keyPressEvent(QKeyEvent *e);

    void focusInEvent(QFocusEvent *e);

    //
    //  MRPShellInterface methods
    //
    int ReadLine(char *inBuffer, int inBufferLength);

    int Write(const char *inBuffer);

    int WriteFormattedD(const char *inFormat, int inDecimal);

    int WriteFormattedDD(const char *inFormat, int inDecimal1, int inDecimal2);

    int WriteFormattedLD(const char *inFormat, long inDecimal);

    int WriteFormattedLF(const char *inFormat, double inFloat);

    int WriteFormattedLU(const char *inFormat, unsigned long inDecimal);

    int WriteFormattedLUD(const char *inFormat, unsigned long inDecimal1, int inDecimal2);

    int WriteFormattedS(const char *inFormat, const char *inString);

    int WriteFormattedSS(const char *inFormat, const char *inString1, const char *inString2);

    int WriteFormattedSD(const char *inFormat, const char *inString, int inDecimal);



public slots:

    void do_command(char *cmd);

    void move_cursor_to_end();



private:

    MRPShell *shell;

    char buf[1024];

    char shell_prompt[32];

    string output_buf;

    int cmd_num;

    int selected_cmd_num;
};



#endif

