#ifndef NOTE_LABEL_H
#define NOTE_LABEL_H


#include <common.h>

#include <StepLabel.h>



class NoteLabel : public StepLabel
{

public:

    NoteLabel(QWidget  *parent,        
              Step     *step);

    void set_note(int note, int previous_note);


private:

};



#endif

