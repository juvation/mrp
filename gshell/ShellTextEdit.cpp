#include <ShellTextEdit.h>

#include <main.h>


#define SHELL_PROMPT_FMT "%d mrp> "


ShellTextEdit::ShellTextEdit(QWidget* parent)
    : QTextEdit(parent),
      shell(shell),
      cmd_num(1),
      selected_cmd_num(1)
{
    g_mac_style->setFocusRectPolicy(this, QMacStyle::FocusDisabled);

    setTextFormat(Qt::PlainText);

    resize(350, 900);

    QFont font(SHELL_FONT);
    font.setStyleStrategy(QFont::NoAntialias);

    setFont(font);

    setMinimumHeight(150);

    add_prompt();

    connect(this, SIGNAL(clicked(int, int)),
            (QObject *)this, SLOT(move_cursor_to_end()));
}




ShellTextEdit::~ShellTextEdit()
{
}




void ShellTextEdit::set_shell(MRPShell *shell)
{
    this->shell = shell;
}




void ShellTextEdit::add_prompt()
{
    setColor(Qt::red);

    sprintf(shell_prompt, SHELL_PROMPT_FMT, cmd_num);
    insert(shell_prompt);

    scrollToBottom();
}




void ShellTextEdit::keyPressEvent(QKeyEvent *e)
{
    int cur_ind;
    int cur_par;

    switch (e->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        QTextEdit::keyPressEvent(e);
        parse_command();
        break;

    case Qt::Key_Up:
        select_previous_command();
        break;

    case Qt::Key_Down:
        select_next_command();
        break;

    case Qt::Key_Tab:
        parentWidget()->setFocus();
        break;

    case Qt::Key_Backspace:
        //
        //  do not erase the current prompt
        //
        getCursorPosition(&cur_par, &cur_ind);

        if (!(cur_par == paragraphs() - 1 && cur_ind == (int)strlen(shell_prompt)))
        {
            QTextEdit::keyPressEvent(e);
        }
        break;

    default:
        QTextEdit::keyPressEvent(e);
    }
}





void ShellTextEdit::focusInEvent(QFocusEvent *e)
{
    move_cursor_to_end();
}




void ShellTextEdit::move_cursor_to_end()
{
    moveCursor(QTextEdit::MoveEnd, false);

    scrollToBottom();
}




void ShellTextEdit::parse_command()
{
    setColor(Qt::black);

    //
    //  get last line, remove prompt and newline, and execute
    //
    QString line = text(paragraphs() - 2);

    int n = line.find(shell_prompt);

    if (n == 0)
    {
        line.remove(0, strlen(shell_prompt));
    }

    line = line.stripWhiteSpace();

    if (line.length())
    {
        //    fprintf(stderr, "executing: %s\n", line.ascii());

        //
        //  XXX special - intercept 'help' command and display
        //  the output in a separate window
        //

        //
        //  reset output buf, execute command, and flush output buf
        //
        output_buf.clear();

        shell->Execute(line);

        cmd_num++;

        selected_cmd_num = cmd_num;

        insert(output_buf);
    }

    add_prompt();
}




void ShellTextEdit::do_command(char *cmd)
{
    //
    //  move cursor to end, insert cmd and execute it
    //
    setColor(Qt::red);

    moveCursor(QTextEdit::MoveEnd, false);

    insert(cmd);

    insert("\n");

    setColor(Qt::black);

    output_buf.clear();

    shell->Execute(cmd);

    cmd_num++;

    selected_cmd_num = cmd_num;

    insert(output_buf);

    scrollToBottom();

    add_prompt();
}




void ShellTextEdit::select_previous_command()
{
    if (selected_cmd_num > 1)
    {
        selected_cmd_num--;

        string hist_entry = shell->GetHistoryEntryByNumber(selected_cmd_num);

        replace_cur_command(hist_entry);
    }
}



void ShellTextEdit::select_next_command()
{
    if (selected_cmd_num < cmd_num)
    {
        selected_cmd_num++;

        string hist_entry = shell->GetHistoryEntryByNumber(selected_cmd_num);

        replace_cur_command(hist_entry);
    }
}




void ShellTextEdit::replace_cur_command(string cmd)
{
    //
    //  replace current cmd, if any, with this cmd
    //
    removeParagraph(paragraphs() - 1);

    insert("\n");

    add_prompt();

    insert(cmd);
}




//
//  XXX ------------------------------------------------------------------
//  not really using these the way they were intended;
//  would be sufficient to collapse this interface down to
//  Shell::Execute(char *cmd, char *output_buf, int output_buf_len)
//

int ShellTextEdit::ReadLine(char *inBuffer, int inBufferLength)
{
    return 0;
}




int ShellTextEdit::Write(const char *inBuffer)
{
    output_buf.append(inBuffer);
    return 0;
}




int ShellTextEdit::WriteFormattedD(const char *inFormat, int inDecimal)
{
    sprintf(buf, inFormat, inDecimal);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedDD(const char *inFormat, int inDecimal1, int inDecimal2)
{
    sprintf(buf, inFormat, inDecimal1, inDecimal2);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedLD(const char *inFormat, long inDecimal)
{
    sprintf(buf, inFormat, inDecimal);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedLF(const char *inFormat, double inFloat)
{
    sprintf(buf, inFormat, inFloat);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedLU(const char *inFormat, unsigned long inDecimal)
{
    sprintf(buf, inFormat, inDecimal);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedLUD(const char *inFormat, unsigned long inDecimal1, int inDecimal2)
{
    sprintf(buf, inFormat, inDecimal1, inDecimal2);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedS(const char *inFormat, const char *inString)
{
    sprintf(buf, inFormat, inString);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedSS(const char *inFormat, const char *inString1, const char *inString2)
{
    sprintf(buf, inFormat, inString1, inString2);
    output_buf.append(buf);
    return 0;
}




int ShellTextEdit::WriteFormattedSD(const char *inFormat, const char *inString, int inDecimal)
{
    sprintf(buf, inFormat, inString, inDecimal);
    output_buf.append(buf);
    return 0;
}



