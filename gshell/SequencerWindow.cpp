#include <SequencerWindow.h>

#include <main.h>
#include <StepLabel.h>
#include <Step.h>
#include <Theme.h>

#include <MIDICLDestination.h>
#include <MIDICLOutputPort.h>
#include <MRPSelectionAction.h>


#include <qapplication.h>
#include <qfiledialog.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


#include <assert.h>
#include <unistd.h>



SequencerWindow::SequencerWindow(QWidget* parent, const char* name, WFlags f) 
    : QMainWindow(parent, name, f),
      cur_filename(NULL)
{
    //
    //  build menus
    //
    QMenuBar* menubar = menuBar();

    //
    //  build File menu
    //
    QPopupMenu* file_menu = new QPopupMenu(menubar);
    file_menu->insertItem("&New Window", this, SLOT(new_window()),           CTRL+Key_N);

    file_menu->insertItem("&Open...",    this, SLOT(open_file()), CTRL+Key_O);
    file_menu->insertItem("&Read...",    this, SLOT(read_file()),            CTRL+Key_R);
    file_menu->insertSeparator();
    file_menu->insertItem("Close",       this, SLOT(close()),                CTRL+Key_W);
    file_menu->insertItem("&Save",       this, SLOT(save_file()),            CTRL+Key_S);
    file_menu->insertItem("Save As...",  this, SLOT(save_file_as()),         SHIFT+CTRL+Key_S);
    file_menu->insertSeparator();
    file_menu->insertItem("&Quit",       this, SLOT(quit()), CTRL+Key_Q);

    menubar->insertItem("&File", file_menu);

    //
    //  help menu - just contains about item which gets moved
    //  to the app menu in osx.
    //
    QPopupMenu* help_menu = new QPopupMenu(menubar);
    help_menu->insertItem("&About", this, SLOT(show_about()), Key_F1);
    menubar->insertItem("&Help", help_menu);


    //
    //  outer hbox contains the outer vbox and the button bar
    //
    outer_hbox = new QHBox(this);

    outer_hbox->setMargin(0);

    outer_hbox->setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    setCentralWidget(outer_hbox);


    //
    //  outer vbox contains the upper hbox and the grid vbox
    //
    outer_vbox = new QVBox(outer_hbox);

    outer_vbox->setMargin(0);

    outer_vbox->setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    outer_vbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);


  
    //
    //  control bar
    //
    control_bar = new ControlBar(outer_vbox, this);


    //
    //  grid vbox contains tracks
    //
    grid_vbox = new QVBox(outer_vbox);

    grid_vbox->setEraseColor(QColor(GRID_COLOR));

    grid_vbox->setSpacing(g_theme->get_grid_spacing());

    grid_vbox->setFocusPolicy(QWidget::StrongFocus);


#if 0
    //
    //  port display
    //
    port_display = new PortDisplay(outer_vbox);
#endif


    //
    //  the shell widget
    //
    shell_text_edit = new ShellTextEdit(outer_vbox);

    shell_text_edit->setFocus();


    //
    //  button bar - have to delay creation of this until
    //  after shell_text_edit is created.
    //
    button_bar = new ButtonBar(outer_hbox, this);




    pthread_mutex_init(&step_header_row_lock, NULL);

    step_header_row = new StepHeaderRow(grid_vbox, mrp_seq, 16);

    //
    //  setup sequencer and shell
    //
    mrp_seq.AddListener(this);

    mrp_seq.SetScale(1, 16);

    mrp_seq.SetBPM(120);


    //
    //  open midi ports
    //
    midicl_client = new MIDICLClient(CFSTR("MRPSequencer"));

    int num_destinations = MIDIGetNumberOfDestinations();

    if (num_destinations == 0)
    {

        QMessageBox mb(NULL,
                       "No MIDI destinations found. MRP Sequencer will make\n"
                       "a dummy one so you can play with the program.",
                       QMessageBox::NoIcon,
                       QMessageBox::Yes,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton,
                       this);

        mb.setButtonText(QMessageBox::Yes, "Okay");

        mb.exec();

        dummy_destination = midicl_client->MakeDestination (CFSTR ("MRPSequencer dummy destination"));

        num_destinations = MIDIGetNumberOfDestinations();
    }
    else
    {
        dummy_destination = NULL;
    }

    for (int i = 0; i < num_destinations; i++)
    {
        MIDICLOutputPort *outputPort = midicl_client->MakeOutputPort(CFSTR("output"));

        outputPort->SetDestination(MIDIGetDestination(i));

        mrp_seq.AddOutputPort(outputPort);
    }

    shell = new MRPShell(&mrp_seq, shell_text_edit);

    shell->SetConfirmSaves(false);

    shell_text_edit->set_shell(shell);


    //
    //  if we have any midi ports, create one track to start
    //  with
    //
    if (num_destinations > 0)
    {
        MRPTrack track;
        mrp_seq.AddTrack(track);
    }

    modified = false;

    setCaption("MRP Sequencer");

    show();

    setMinimumWidth(width());
    setMaximumWidth(width());
    setMinimumHeight(height());
}




SequencerWindow::~SequencerWindow()
{
    mrp_seq.Stop();

    pthread_mutex_destroy(&step_header_row_lock);

    delete midicl_client;

		if (dummy_destination != NULL)
		{
			delete dummy_destination;
		}

    delete button_bar;

    delete shell_text_edit;

    delete shell;

    delete grid_vbox;

    delete control_bar;

    delete outer_vbox;

    delete outer_hbox;
}



ShellTextEdit *SequencerWindow::get_shell_widget()
{
    return shell_text_edit;
}




//
//
//  XXX notes for new keyboard input paradigm TBD:
//
//   type one key to start input, resets if no other key
//   pressed within key_input_timeout.  e.g.
//
//   n c4    setnote c4
//   c bas   setchannel bas
//   v 50    setvelocity 50
//   g 90    setgate 90
//
//   n ,     decrease note
//   n .     increase note
//   [ same for c, v, g ]
//
//   r n     randomnote
//   r c     randomchannel
//   r v     randomvelocity
//   r g     randomgate
//
//   f c     fitkey c
//
//   ideally have some mode indicator, e.g. pressing n
//   highlights the selected notes, until the input is done,
//   either by setting the note or timeout.
//

void SequencerWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {

    case Qt::Key_Q:
        increment_note(-1);
        break;

    case Qt::Key_W:
        increment_note(1);
        break;

    case Qt::Key_A:
        increment_port(-1);
        break;

    case Qt::Key_S:
        increment_port(1);
        break;

    case Qt::Key_Z:
        increment_channel(-1);
        break;

    case Qt::Key_X:
        increment_channel(1);
        break;

    case Qt::Key_E:
        increment_velocity(-1);
        break;

    case Qt::Key_R:
        increment_velocity(1);
        break;

    case Qt::Key_D:
        increment_gate(-1);
        break;

    case Qt::Key_F:
        increment_gate(1);
        break;

    case Qt::Key_Space:
        toggle_playing();
        break;


    default:
        QMainWindow::keyPressEvent(e);
    }
}




void SequencerWindow::increment_note(int delta)
{
    MRPSelectionIncrementNote action(delta);

    mrp_seq.RunSelection(&action);
}




void SequencerWindow::increment_port(int delta)
{
    MRPSelectionIncrementPort action(delta);

    mrp_seq.RunSelection(&action);
}




void SequencerWindow::increment_channel(int delta)
{
    MRPSelectionIncrementChannel action(delta);

    mrp_seq.RunSelection(&action);
}




void SequencerWindow::increment_velocity(int delta)
{
    MRPSelectionIncrementVelocity action(delta);

    mrp_seq.RunSelection(&action);
}




void SequencerWindow::increment_gate(int delta)
{
    MRPSelectionIncrementGateTime action(delta);

    mrp_seq.RunSelection(&action);
}




void SequencerWindow::toggle_playing()
{
    if (mrp_seq.IsPlaying())
    {
        mrp_seq.Stop();
    }
    else
    {
        mrp_seq.Play();
    }
}



void SequencerWindow::set_bpm(double bpm)
{
    mrp_seq.SetBPM(bpm);
}




void SequencerWindow::set_bars(double bars)
{
    mrp_seq.SetScale((int)bars, mrp_seq.GetStepCount());
}




void SequencerWindow::set_steps(double steps)
{
    mrp_seq.SetScale(mrp_seq.GetBarCount(), (int)steps);
}




void SequencerWindow::new_window()
{
    SequencerWindow *seq_window = new SequencerWindow(0, 0, WDestructiveClose);
    qApp->setMainWidget(seq_window);
    seq_window->show();
    qApp->setMainWidget(0);
}




void SequencerWindow::open_file()
{
    QString fn = QFileDialog::getOpenFileName(
        NULL,
        NULL,
        this
        );

    if (fn.isNull()) 
    {
        return;
    }


    //
    //  if this window is bound to a filename or modified,
    //  open in a new window, otherwise in this one.
    //
    if (modified || cur_filename)
    {
        SequencerWindow *seq_window = new SequencerWindow(0, 0, WDestructiveClose);
        qApp->setMainWidget(seq_window);
        seq_window->show();
        qApp->setMainWidget(0);
        seq_window->load_file(fn.ascii(), true);
    }
    else
    {
        load_file(fn.ascii(), true);
        modified = false;
    }
}




void SequencerWindow::read_file()
{
    QString fn = QFileDialog::getOpenFileName(
        NULL,
        NULL,
        this
        );

    if (!fn.isNull()) 
    {
				snprintf(buf, sizeof(buf), "read '%s'", fn.ascii ());

				shell_text_edit->do_command(buf);
    }
}




void SequencerWindow::load_file(const char *filename, bool make_cur_file)
{
    snprintf(buf, sizeof(buf), "load '%s'", filename);

    shell_text_edit->do_command(buf);

    if (make_cur_file)
    {
        if (cur_filename)
        {
            free(cur_filename);
        }

        cur_filename = strdup(filename);

        setCaption(cur_filename);
    }
}




void SequencerWindow::save_file()
{
    if (cur_filename) 
    {
        save_file(cur_filename);
    } 
    else 
    {
        save_file_as();
    }
}




void SequencerWindow::save_file(char *filename)
{
    snprintf(buf, sizeof(buf), "save '%s'", filename);

    shell_text_edit->do_command(buf);

    set_modified(false);
}




void SequencerWindow::save_file_as()
{
    QString fn = QFileDialog::getSaveFileName(
        NULL,
        NULL,
        this
        );

    if (!fn.isNull()) 
    {
        //
        //  prompt to overwrite if exists
        //
        if (access (fn.ascii(), R_OK) == 0)
        {
            snprintf(buf, sizeof(buf),
                     "The file %s already exists.  Do you want to replace it?",
                     fn.ascii());


            QMessageBox mb(NULL,
                           buf,
                           QMessageBox::NoIcon,
                           QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape,
                           QMessageBox::Yes,
                           QMessageBox::NoButton,
                           this);

            mb.setButtonText(QMessageBox::Yes, "Replace");


            if (mb.exec() == QMessageBox::Cancel)
            {
                return;
            }
        }

      

        if (cur_filename)
        {
            free(cur_filename);
        }
        cur_filename = strdup(fn.ascii());

        save_file((char *)fn.ascii());

        setCaption(cur_filename);
    }
}






void SequencerWindow::show_about()
{
    static QMessageBox* about = new QMessageBox(NULL, 
                                                "<center><b>MRP Sequencer <i>2501</i></b>"
                                                "<p>Jason Proctor</p>"
                                                "<p>Adam Marks</p></center>",
                                                QMessageBox::NoIcon, 
                                                QMessageBox::NoButton,
                                                QMessageBox::NoButton, 
                                                QMessageBox::NoButton, 
                                                this, 
                                                0, 
                                                FALSE);
    about->show();
}




void SequencerWindow::set_modified(bool modified)
{
    this->modified = modified;

    if (cur_filename)
    {
        if (modified)
        {
            snprintf(buf, sizeof(buf), "%s*", cur_filename);
            setCaption(buf);
        }
        else
        {
            setCaption(cur_filename);
        }
    }
}




void SequencerWindow::closeEvent(QCloseEvent *e)
{
    if (modified)
    {
        snprintf(buf, sizeof(buf),
                 "Do you want to save changes to this document before closing?");

        QMessageBox mb(NULL,
                       buf,
                       QMessageBox::NoIcon,
                       QMessageBox::No,
                       QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape,
                       QMessageBox::Yes,
                       this);

        mb.setButtonText(QMessageBox::No, "Don't Save");

        mb.setButtonText(QMessageBox::Yes, "Save");

        switch(mb.exec())
        {
        case QMessageBox::No:

            QMainWindow::closeEvent(e);

            return;

        case QMessageBox::Cancel:

            //  cancel the close
            e->ignore();

            return;

        case QMessageBox::Yes:

            save_file();

            //  cancel the close if the save did not work
            if (modified)
            {
                e->ignore();
                return;
            }

            QMainWindow::closeEvent(e);

            return;
        }
    }
    else
    {
        QMainWindow::closeEvent(e);
    }
}



void SequencerWindow::quit()
{
    qApp->closeAllWindows();
}




//
//  --------------------------------------------------------
//
//  MRPSequencerListener methods
//
//

void SequencerWindow::BPMChanged(MRPSequencer *seq)
{
    control_bar->set_bpm(seq->GetBPM());
    set_modified(true);
}




void SequencerWindow::ClockReset(MRPSequencer *seq)
{
  // XXX?
}



void SequencerWindow::ConfigChanged(MRPSequencer *seq)
{
    //
    //  update all steps in all tracks - this catches
    //  e.g. channel name changes
    //
    int step_count = seq->GetStepCount();

    track_vector_t::iterator i = tracks.begin();

    while (i != tracks.end())
    {
        Track *track = *i;

        for (int j = 0; j < step_count; j++)
        {
            Step *step = track->get_step(j);

            step->update_from_event();
        }

        ++i;
    }
}



void SequencerWindow::EventChanged(MRPSequencer *seq, int track_num1, int step_num1)
{
    Track *track = tracks[track_num1 - 1];
    Step  *step  = track->get_step(step_num1 - 1);

    step->update_from_event();

    //
    //  also update the next step as well - since note colors
    //  depend on the previous note
    //
    if (step_num1 < seq->GetStepCount())
    {
        step = track->get_step(step_num1);

        step->update_from_event();
    }

    set_modified(true);
}



void SequencerWindow::LoopChanged(MRPSequencer *seq)
{

    pthread_mutex_lock(&step_header_row_lock);

    if (step_header_row)
    {
        step_header_row->set_loop_step(seq->GetLoopStep() - 1);
    }

    pthread_mutex_unlock(&step_header_row_lock);

    set_modified(true);
}




void SequencerWindow::Pause(MRPSequencer *seq)
{
}




void SequencerWindow::PortListChanged(MRPSequencer *seq)
{
  // XXX update port display ?
}




void SequencerWindow::ScaleChanged(MRPSequencer *seq)
{
    //
    //  delete current tracks
    //
    track_vector_t::iterator i = tracks.begin();

    while (i != tracks.end())
    {
        Track *track = *i;

        delete track;

        ++i;
    }

    tracks.clear();


    //
    //  recreate header
    //
    int step_count = seq->GetStepCount();

    pthread_mutex_lock(&step_header_row_lock);

    delete step_header_row;

    step_header_row = new StepHeaderRow(grid_vbox, mrp_seq, step_count);

    step_header_row->show();

    pthread_mutex_unlock(&step_header_row_lock);


    //
    //  create new tracks
    //
    int num_tracks = mrp_seq.GetTrackCount();

    for (int i = 0; i < num_tracks; i++) 
    {
        Track *track = new Track(grid_vbox, 
                                 this,
                                 i, 
                                 seq->GetTrack(i + 1), 
                                 mrp_seq);

        track->show();

        tracks.push_back(track);
    }


    //
    //  resize
    //
    adjustSize();


    control_bar->set_bars(mrp_seq.GetBarCount());

    control_bar->set_steps(step_count);

    set_modified(true);
}



void SequencerWindow::Start(MRPSequencer *seq)
{
}




void SequencerWindow::StepTick(MRPSequencer *seq)
{
    //
    //  this is called from a separate thread, and since the
    //  main gui thread may be in the middle of modifying the
    //  step_header_row (i.e. on newtrack), we need to get the
    //  lock on it if we are going to highlight.  but we don't
    //  want to block this thread since it is playing music;
    //  thus try the lock and only update the highlight if we
    //  get it.
    //
    if (pthread_mutex_trylock(&step_header_row_lock) < 0)
    {
        return;
    }

    if (step_header_row)
    {
        step_header_row->set_current_step(seq->GetCurrentStep());
    }

    pthread_mutex_unlock(&step_header_row_lock);
}




void SequencerWindow::Stop(MRPSequencer *seq)
{
}




void SequencerWindow::TimerTick(MRPSequencer *seq)
{
  // XXX ?
}


void SequencerWindow::TrackChanged(MRPSequencer *seq, int track_num1)
{
    int	trackNumber0 (track_num1 - 1);

		track_vector_t::iterator track = tracks.begin();

		// point the iterator at the right spot
		for (int i = 0; i < (int) trackNumber0; i++)
		{
				track++;
		}

    int step_count (seq->GetStepCount());

		Track	*theTrack (*track);

		// update the steps
		for (int j = 0; j < step_count; j++)
		{
				Step *step = theTrack->get_step(j);

				step->update_from_event();
		}

		// tell the world that stuff has changed
    set_modified(true);
}



void SequencerWindow::TrackAdded(MRPSequencer *seq, int track_num1)
{
    // XXX optimization: modify grid_vbox such that layout
    // widget is exposed - then can insert track widget
    // directly, without having to delete and recreate
    // tracks.

    if (track_num1 <= (int)tracks.size())
    {
        track_vector_t::iterator track = tracks.begin();

        //
        //  delete all the Track objects in the list past
        //  the insertion point
        //
        for (int i = 1; i <= (int)tracks.size(); ++i, ++track)
        {
            if (i >= track_num1)
            {
                Track *t = *track;

                delete t;
            }
        }

        //
        //  now erase the list entries past the insertion
        //  point
        //
        track = tracks.begin();

        for (int i = 1; i < track_num1; ++i, ++track);

        tracks.erase(track, tracks.end());


        //
        //  add the new track and the ones after it
        //
        for (int i = track_num1; i <= mrp_seq.GetTrackCount(); ++i)
        {
            Track *track = new Track(grid_vbox, 
                                     this,
                                     i - 1, 
                                     seq->GetTrack(1), 
                                     mrp_seq);

            track->show();

            tracks.push_back(track);
        }
		
    }
    else
    {
        // NOTE we're deliberately not handling the case
        // where (eg) someone adds track 10 to a sequence with only 2 tracks
        // because MRPSequencer will generate a TrackAdded() notification
        // for each new track in this case

        Track *track = new Track(grid_vbox, 
                                 this,
                                 track_num1 - 1, 
                                 seq->GetTrack(track_num1), 
                                 mrp_seq);

        track->show();

        tracks.push_back(track);
    }

    set_modified(true);
}




void SequencerWindow::TrackDeleted(MRPSequencer *seq, int track_num1)
{
    if (track_num1 <= (int)tracks.size())
    {
        track_vector_t::iterator track = tracks.begin();

        //
        //  delete all the Track objects in the list at
        //  and past the deletion point
        //
        for (int i = 1; i <= (int)tracks.size(); ++i, ++track)
        {
            if (i >= track_num1)
            {
                Track *t = *track;

                delete t;
            }
        }

        //
        //  now erase the list entries at and past the
        //  deletion point
        //
        track = tracks.begin();

        for (int i = 1; i < track_num1; ++i, ++track);

        tracks.erase(track, tracks.end());


        //
        //  re-add the tracks after the deletion point
        //
        for (int i = track_num1; i <= mrp_seq.GetTrackCount(); ++i)
        {
            Track *track = new Track(grid_vbox, 
                                     this,
                                     i - 1, 
                                     seq->GetTrack(1), 
                                     mrp_seq);

            track->show();

            tracks.push_back(track);
        }
		
    }
    else
    {
        // hmmm....
    }

    set_modified(true);
}

