#include <pthread.h>

#include <qframe.h>
#include <qhbox.h>
#include <qmainwindow.h>
#include <qvbox.h>


#include <vector>
using std::vector;


#include <ButtonBar.h>
#include <common.h>
#include <ControlBar.h>
#include <ShellTextEdit.h>
#include <StepHeaderRow.h>
#include <Track.h>

#include <MIDICLClient.h>
#include <MRPSequencer.h>
#include <MRPSequencerListener.h>
#include <MRPShell.h>


typedef vector<Track *> track_vector_t;




class SequencerWindow 
  : public QMainWindow,
    public MRPSequencerListener
{
    Q_OBJECT

public:

    SequencerWindow(QWidget* parent=0, const char* name=0, WFlags f=0);

    ~SequencerWindow();

    void keyPressEvent(QKeyEvent *e);

    void increment_note(int delta);

    void increment_port(int delta);

    void increment_channel(int delta);

    void increment_velocity(int delta);

    void increment_gate(int delta);

    ShellTextEdit *get_shell_widget();

    void save_file(char *filename);

    void set_modified(bool modified);

    void closeEvent(QCloseEvent *e);


public slots:
  
    void new_window();

    void open_file();

    void read_file();

    void load_file(const char *filename, bool make_cur_file=false);

    void save_file();

    void save_file_as();

    void show_about();

    // XXX replace calls to these with execute_command()
    void toggle_playing();

    void set_bpm(double bpm);

    void set_bars(double bars);

    void set_steps(double bars);

    void quit();

public:
    //
    //  MRPSequencerListener methods
    //
    void BPMChanged(MRPSequencer *seq);

    void ClockReset(MRPSequencer *seq);

    void ConfigChanged(MRPSequencer *seq);

    void EventChanged(MRPSequencer *seq, int inTrack, int inStep);

    void LoopChanged(MRPSequencer *seq);

    void Pause(MRPSequencer *seq);

    void PortListChanged(MRPSequencer *seq);

    void ScaleChanged(MRPSequencer *seq);

    void Start(MRPSequencer *seq);

    void StepTick(MRPSequencer *seq);

    void Stop(MRPSequencer *seq);

    void TimerTick(MRPSequencer *seq);

    void TrackChanged(MRPSequencer *seq, int inTrack);

    void TrackAdded(MRPSequencer *seq, int inTrack);

    void TrackDeleted(MRPSequencer *seq, int inTrack);

private:

    MIDICLClient     *midicl_client;

		MIDICLDestination	*dummy_destination;

    MRPSequencer     mrp_seq;

    MRPShell         *shell;

    QHBox            *outer_hbox;

    QVBox            *outer_vbox, *grid_vbox;

    ButtonBar        *button_bar;

    ControlBar       *control_bar;

    StepHeaderRow    *step_header_row;

    track_vector_t   tracks;

    ShellTextEdit    *shell_text_edit;

    pthread_mutex_t  step_header_row_lock;

    char             buf[1024];

    char             *cur_filename;

    bool             modified;
};
