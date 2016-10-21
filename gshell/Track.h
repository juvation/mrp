#ifndef TRACK_H
#define TRACK_H


#include <qhbox.h>

#include <vector>
using std::vector;

#include <MRPSequencer.h>
#include <MRPTrack.h>

#include <TrackHeader.h>
#include <Step.h>

#include <common.h>


typedef vector<Step *> step_vector_t;

class SequencerWindow;


class Track : public QHBox
{

public: 

    Track(QWidget 	   *parent, 
          SequencerWindow  *seq_win, 
          int 		   track_num, 
          MRPTrack 	   &mrp_track, 
          MRPSequencer 	   &mrp_seq);

    ~Track();

    Step *get_step(int step_num);


private:

    int track_num;

    MRPTrack &mrp_track;

    MRPSequencer &mrp_seq;

    TrackHeader *header;

    step_vector_t steps;
};




#endif




