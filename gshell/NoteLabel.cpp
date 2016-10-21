#include <NoteLabel.h>

#include <main.h>
#include <Theme.h>

#include <MRPEvent.h>


NoteLabel::NoteLabel(QWidget  *parent, 
		     Step     *step)
  : StepLabel(parent, step)
{
}



void NoteLabel::set_note(int note, int previous_note)
{

    QColor color = g_theme->get_note_background_color(note,
						      previous_note);

    setPaletteBackgroundColor(color);

    char buf[16];
    sprintf(buf, "%s", MRPEvent::FormatNoteNumber(note).c_str ());
    setText(buf);
}







