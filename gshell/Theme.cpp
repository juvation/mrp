#include <Theme.h>

// XXX the theme stuff from common.h needs to be moved to
// this class
#include <common.h>


#include <CoreMIDI/MIDIServices.h>


#if 0
char *port_colors[] =
{
    "#FF7536",  //  dark orange
    "#17E0D8",  //  light turquoise
    "#F4F42F",  //  yellow
    "#A0EA4D",  //  lightest green
    "#8D8C9C"   //  medium purple tan
    "#D8569A",  //  rose
    "#11C23B",  //  green
    "#D374EA",  //  lavendar
    "#AEA5A0",  //  light purple tan
    "#64C4E2",  //  medium turquoise
    "#FFA832",  //  pale orange
    "#72D230",  //  light green
    "#9CAB82",  //  green tan
    "#27A6CF",  //  dark turquoise
    "#C9BE84",  //  yellow tan
    "#ED3149",  //  red
};
#endif


#if 0

#define THEME_COLOR_NOTE_BG      "#3875D7"   //  blue
#define THEME_COLOR_PORT_BG      "#27A6CF"   //  turquoise
#define THEME_COLOR_CHANNEL_BG   "#F4F42F"   //  yellow
#define THEME_COLOR_VELOCITY_BG  "#C9BE84"   //  tan
#define THEME_COLOR_GATE_BG      "#777777"   //  gray
#define THEME_COLOR_MUTE_BG      "#FF7536"   //  orange

#else

#define THEME_COLOR_NOTE_BG      "#999999"   //  gray
#define THEME_COLOR_PORT_BG      "#999999"   //  gray
#define THEME_COLOR_CHANNEL_BG   "#999999"   //  gray
#define THEME_COLOR_VELOCITY_BG  "#999999"   //  gray
#define THEME_COLOR_GATE_BG      "#999999"   //  gray
#define THEME_COLOR_MUTE_BG      "#999999"   //  gray


#endif


#define THEME_NOTE_SAME_BG_COLOR    "#bbbbbb"  //  gray
//#define THEME_NOTE_LOWER_BG_COLOR   "#31A1CF"  //  turquoise/blue
//#define THEME_NOTE_LOWER_BG_COLOR   "#11C23B"  //  green
#define THEME_NOTE_LOWER_BG_COLOR   "#FFFFFF"  //  white
//#define THEME_NOTE_HIGHER_BG_COLOR  "#EB0014"  //  red
//#define THEME_NOTE_HIGHER_BG_COLOR   "#11C23B"  //  green
#define THEME_NOTE_HIGHER_BG_COLOR   "#31A1CF"  //  turquoise/blue


#define THEME_NOTE_GRADIENT_SIZE    13


#define THEME_GRID_SPACING 1


#define LOW_GRADE    50
#define HIGH_GRADE  150




Theme::Theme()
{
    num_midi_destinations = MIDIGetNumberOfDestinations();

    //
    //  populate note_gradient.  this is a vector of QColors
    //  that looks like this:
    //  
    //                              index    color
    //  ---------------------------------    --------------------------
    //                                  0    THEME_NOTE_LOWER_BG_COLOR
    //                                    
    //                                       [gradients]
    //
    //      THEME_NOTE_GRADIENT_SIZE / 2    THEME_NOTE_SAME_BG_COLOR
    //
    //                                       [gradients]
    //
    //       THEME_NOTE_GRADIENT_SIZE - 1   THEME_NOTE_HIGHER_BG_COLOR
    //
    //

    //
    //  gradient from THEME_NOTE_LOWER_BG_COLOR to
    //  THEME_NOTE_SAME_BG_COLOR
    //
    QColor *lower  = new QColor(THEME_NOTE_LOWER_BG_COLOR);

    QColor *same   = new QColor(THEME_NOTE_SAME_BG_COLOR);

    QColor *higher = new QColor(THEME_NOTE_HIGHER_BG_COLOR);


    int mR = same->red() - lower->red();
    int bR = lower->red();

    int mG = same->green() - lower->green();
    int bG = lower->green();

    int mB = same->blue() - lower->blue();
    int bB = lower->blue();

    note_gradient_middle = THEME_NOTE_GRADIENT_SIZE / 2;

    for (int i = 0; i <= note_gradient_middle; i++)
    {
	float x = ((float)i / note_gradient_middle);

	int r = (int)(mR * x + bR);
	int g = (int)(mG * x + bG);
	int b = (int)(mB * x + bB);

	QColor *c = new QColor(r, g, b);

	note_gradient.push_back(c);
	//fprintf(stderr, "%s\n", c->name().ascii());
    }



    //
    //  gradient from THEME_NOTE_SAME_BG_COLOR to
    //  THEME_NOTE_HIGHER_BG_COLOR
    //

    mR = higher->red() - same->red();
    bR = same->red();

    mG = higher->green() - same->green();
    bG = same->green();

    mB = higher->blue() - same->blue();
    bB = same->blue();

    for (int i = 1; i <= note_gradient_middle; i++)
    {
	float x = ((float)i) / note_gradient_middle;

	int r = (int)(mR * x + bR);
	int g = (int)(mG * x + bG);
	int b = (int)(mB * x + bB);

	QColor *c = new QColor(r, g, b);

	note_gradient.push_back(c);
	//fprintf(stderr, "%s\n", c->name().ascii());
    }


    //
    //  compute m and b for mapping;
    //
    //  [-128, 128] -> [0, gradient_size - 1]
    //  
    note_gradient_m = ((float)THEME_NOTE_GRADIENT_SIZE - 1.0) / 256.0;

    note_gradient_b = (THEME_NOTE_GRADIENT_SIZE - 1.0) / 2.0;


    delete lower;

    delete same;

    delete higher;

    //fprintf(stderr, "m: %1.1f  b: %1.1f\n", note_gradient_m, note_gradient_b);
}




Theme::~Theme()
{
    color_vec_t::iterator i = note_gradient.begin();

    while (i != note_gradient.end())
    {
        QColor *c = *i;

        delete c;

        ++i;
    }
}




QColor Theme::get_note_background_color(int note, int previous_note)
{
    //
    //  map interval to color in note gradient
    //
    int index;

    if (previous_note < 0)
    {
	index = note_gradient_middle;
    }
    else
    {
	int interval = note - previous_note;

	index = (int)(note_gradient_m * (float)interval + note_gradient_b);

	//
	//  special - force index away from center, to
	//  indicate that the note is different.
	//
	if (index == note_gradient_middle && note != previous_note)
	{

	    if (note < previous_note)
	    {
		index--;
	    }
	    else
	    {
		index++;
	    }
	}

	//fprintf(stderr, "interval: %d  index: %d\n", interval, index);
    }

    return *note_gradient[index];
}




QColor Theme::get_color(part_t part)
{
    // XXX
    char *c = "#ff0000";

    return QColor(c);
}




//  XXX channel, velocity, gate color coding is disabled for
//  now since i'm not happy with the results yet.

#if 0
static int map_grade(int n, int range_start, int range_stop)
{
    return (LOW_GRADE - HIGH_GRADE) * n / (range_start - range_stop) + LOW_GRADE + (HIGH_GRADE - LOW_GRADE) / (range_start - range_stop);
}
#endif




QColor Theme::get_graded_color(part_t part, int code)
{
    char *c   = STEP_BG_COLOR;

    int grade = 100;


#if 0


    switch(part)
    {
    case kChannelBackgroundColor:
        c = THEME_COLOR_CHANNEL_BG;
        grade = map_grade(code, 0, 15);
        break;

    case kVelocityBackgroundColor:
        c = THEME_COLOR_VELOCITY_BG;
        grade = map_grade(code, 0, 127);
        break;

    case kGateBackgroundColor:
        c = THEME_COLOR_GATE_BG;
        grade = map_grade(code, 1, 99);
        break;


    }
#endif

    return QColor(c).light(grade);
}






int Theme::get_grid_spacing()
{
    return THEME_GRID_SPACING;
}

