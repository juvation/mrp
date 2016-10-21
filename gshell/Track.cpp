#include <Track.h>

#include <main.h>
#include <Theme.h>

#include <qpainter.h>

#include <assert.h>



Track::Track(QWidget 	       *parent, 
	     SequencerWindow   *seq_win, 
	     int 	       track_num, 
	     MRPTrack 	       &mrp_track, 
	     MRPSequencer      &mrp_seq) 

    : QHBox(parent),
      track_num(track_num),
      mrp_track(mrp_track),
      mrp_seq(mrp_seq)
{
    //
    //  create Track header
    //
    header = new TrackHeader(this, mrp_seq, track_num);


    //
    //  create Steps
    //
    //  for (int i = 0; i < mrp_track.GetEventCount(); i++) 
    for (int i = 0; i < mrp_seq.GetStepCount(); i++) 
    {
        Step *step = new Step(this, 
                              seq_win,
                              mrp_seq, 
                              track_num, 
                              i);
        step->show();
        steps.push_back(step);
    }

    //
    //  this allows the background color to show through,
    //  creating the vertical gridlines
    //
    setEraseColor(QColor(GRID_COLOR));

    setSpacing(g_theme->get_grid_spacing());
}



Track::~Track()
{
    delete header;

    step_vector_t::iterator i = steps.begin();

    while (i != steps.end())
    {
        Step *step = *i;

        delete step;

        ++i;
    }
}



Step *Track::get_step(int step_num)
{
    return steps[step_num];
}

