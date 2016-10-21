#ifndef THEME_H
#define THEME_H


#include <vector>
using std::vector;


#include <qcolor.h>




typedef vector<QColor *> color_vec_t;



class Theme
{

public:

    Theme();

    ~Theme();


    enum part_t
    {
        kChannelBackgroundColor,
        kVelocityBackgroundColor,
        kGateBackgroundColor,
        //    kMuteBackgroundColor
    };


    QColor get_color(part_t part);

    QColor get_note_background_color(int note, int previous_note);

    QColor get_graded_color(part_t part, int code);

    int get_grid_spacing();

  
private:

    int          num_midi_destinations;

    color_vec_t  note_gradient;

    float        note_gradient_m, note_gradient_b;
  
    int          note_gradient_middle;
};



#endif
