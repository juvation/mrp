#include <ButtonBar.h>


#include <LabelButton.h>
#include <NumEdit.h>
#include <SequencerWindow.h>




ButtonBar::ButtonBar(QWidget *parent, SequencerWindow *seq_win)
    : QFrame(parent),
      seq_win(seq_win)
{

    ShellTextEdit *shell_widget = seq_win->get_shell_widget();

    setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    layout = new QVBoxLayout(this);

    layout->setSpacing(1);

    play_button = new LabelButton(this, "play", true);
    layout->addWidget(play_button, 1);
    connect((QObject *)play_button, SIGNAL(clicked()), 
	    (QObject *)seq_win, SLOT(toggle_playing()));



    const char *set_note_params[] = {"c3"};
    set_note_button = new CommandButton(this, "set note", "setnote", "n", set_note_params);
    layout->addWidget(set_note_button, 1);
    connect(set_note_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *set_port_params[] = {"0"};
    set_port_button = new CommandButton(this, "set port", "setport", "i", set_port_params);
    layout->addWidget(set_port_button, 1);
    connect(set_port_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *set_channel_params[] = {"1"};
    set_channel_button = new CommandButton(this, "set channel", "setchannel", "i", set_channel_params);
    layout->addWidget(set_channel_button, 1);
    connect(set_channel_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *set_velocity_params[] = {"64"};
    set_velocity_button = new CommandButton(this, "set velocity", "setvelocity", "i", set_velocity_params);
    layout->addWidget(set_velocity_button, 1);
    connect(set_velocity_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *set_gate_params[] = {"70"};
    set_gate_button = new CommandButton(this, "set gate", "setgate", "i", set_gate_params);
    layout->addWidget(set_gate_button, 1);
    connect(set_gate_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *set_loop_params[] = {"17"};
    set_loop_button = new CommandButton(this, "set loop", "setloop", "i", set_loop_params);
    layout->addWidget(set_loop_button, 1);
    connect(set_loop_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    mute_button = new CommandButton(this, "mute", "mute", "", NULL);
    layout->addWidget(mute_button, 1);
    connect(mute_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    unmute_button = new CommandButton(this, "unmute", "unmute", "", NULL);
    layout->addWidget(unmute_button, 1);
    connect(unmute_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    invert_mute_button = new CommandButton(this, "invert mute", "invertmute", "", NULL);
    layout->addWidget(invert_mute_button, 1);
    connect(invert_mute_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    hsolo_button = new CommandButton(this, "hsolo", "hsolo", "", NULL);
    layout->addWidget(hsolo_button, 1);
    connect(hsolo_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    hunsolo_button = new CommandButton(this, "hunsolo", "hunsolo", "", NULL);
    layout->addWidget(hunsolo_button, 1);
    connect(hunsolo_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    vsolo_button = new CommandButton(this, "vsolo", "vsolo", "", NULL);
    layout->addWidget(vsolo_button, 1);
    connect(vsolo_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    vunsolo_button = new CommandButton(this, "vunsolo", "vunsolo", "", NULL);
    layout->addWidget(vunsolo_button, 1);
    connect(vunsolo_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    invert_selection_button = new CommandButton(this, "invert selection", "invertselection", "", NULL);
    layout->addWidget(invert_selection_button, 1);
    connect(invert_selection_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *fit_to_key_params[] = {"C"};
    fit_to_key_button = new CommandButton(this, "fit to key", "fitkey", "n", fit_to_key_params);
    layout->addWidget(fit_to_key_button, 1);
    connect(fit_to_key_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *rotate_params[] = {"1"};
    rotate_button = new CommandButton(this, "rotate", "rotate", "i", rotate_params);
    layout->addWidget(rotate_button, 1);
    connect(rotate_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    vary_button = new CommandButton(this, "vary", "vary", "", NULL);
    layout->addWidget(vary_button, 1);
    connect(vary_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *randomise_note_params[] = {"c3", "79"};
    randomise_note_button = new CommandButton(this, "randomise note", "randomnote", "ni", randomise_note_params);
    layout->addWidget(randomise_note_button, 1);
    connect(randomise_note_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *randomise_port_params[] = {"0", "10"};
    randomise_port_button = new CommandButton(this, "randomise port", "randomport", "ii", randomise_port_params);
    layout->addWidget(randomise_port_button, 1);
    connect(randomise_port_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *randomise_channel_params[] = {"1", "15"};
    randomise_channel_button = new CommandButton(this, "randomise channel", "randomchannel", "ii", randomise_channel_params);
    layout->addWidget(randomise_channel_button, 1);
    connect(randomise_channel_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *randomise_velocity_params[] = {"0", "127"};
    randomise_velocity_button = new CommandButton(this, "randomise velocity", "randomvelocity", "ii", randomise_velocity_params);
    layout->addWidget(randomise_velocity_button, 1);
    connect(randomise_velocity_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *randomise_gate_params[] = {"50", "40"};
    randomise_gate_button = new CommandButton(this, "randomise gate", "randomgate", "ii", randomise_gate_params);
    layout->addWidget(randomise_gate_button, 1);
    connect(randomise_gate_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    randomise_mute_button = new CommandButton(this, "randomise mute", "randommute", "", NULL);
    layout->addWidget(randomise_mute_button, 1);
    connect(randomise_mute_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *new_track_params[] = {"1"};
    new_track_button = new CommandButton(this, "new track", "newtrack", "i", new_track_params);
    layout->addWidget(new_track_button, 1);
    connect(new_track_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *copy_track_params[] = {"1", "2"};
    copy_track_button = new CommandButton(this, "copy track", "copytrack", "ii", copy_track_params);
    layout->addWidget(copy_track_button, 1);
    connect(copy_track_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));


    const char *delete_track_params[] = {"1"};
    delete_track_button = new CommandButton(this, "delete track", "deletetrack", "i", delete_track_params);
    layout->addWidget(delete_track_button, 1);
    connect(delete_track_button, SIGNAL(clicked(char *)), 
            (QObject *)shell_widget, SLOT(do_command(char *)));



    layout->addStretch(30);
}




ButtonBar::~ButtonBar()
{
    delete new_track_button;
    delete copy_track_button;
    delete delete_track_button;
    delete set_note_button;
    delete set_port_button;
    delete set_channel_button;
    delete set_velocity_button;
    delete set_gate_button;
    delete mute_button;
    delete unmute_button;
    delete invert_mute_button;
    delete hsolo_button;
    delete hunsolo_button;
    delete vsolo_button;
    delete vunsolo_button;
    delete invert_selection_button;
    delete fit_to_key_button;
    delete rotate_button;
    delete vary_button;
    delete randomise_note_button;
    delete randomise_port_button;
    delete randomise_channel_button;
    delete randomise_velocity_button;
    delete randomise_gate_button;
    delete randomise_mute_button;
    delete set_loop_button;

    delete layout;
}




