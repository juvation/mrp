#include <ControlBar.h>

#include <main.h>




ControlBar::ControlBar(QWidget *parent, SequencerWindow *seq_window)
  : QFrame(parent),
    seq_window(seq_window)
{

    setEraseColor(QColor(BUTTON_GRID_COLOR));

    setMargin(0);

    setLineWidth(0);

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setPaletteForegroundColor(QColor(BUTTON_GRID_COLOR));

    layout = new QHBoxLayout(this);

    layout->setSpacing(0);

    layout->addStretch(2);

    QFont font(LABEL_FONT);


    //
    //  bars
    //
    bars_label = new QLabel(this);

    bars_label->setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    bars_label->setPaletteForegroundColor(QColor(DEFAULT_FG_COLOR));

    bars_label->setFont(font);

    bars_label->setText("bars");

    bars_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    bars_label->setMargin(2);

    bars_label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    layout->addWidget(bars_label, 1);

    bars_edit = new NumEdit(this, "1", 'i');

    layout->addWidget(bars_edit, 1);

    connect((QObject *)bars_edit, SIGNAL(value_changed(double)),
            (QObject *)seq_window, SLOT(set_bars(double)));




    //
    //  steps
    //
    steps_label = new QLabel(this);

    steps_label->setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    steps_label->setPaletteForegroundColor(QColor(DEFAULT_FG_COLOR));

    steps_label->setFont(font);

    steps_label->setText("steps");

    steps_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    steps_label->setMargin(2);

    steps_label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    layout->addWidget(steps_label, 1);

    steps_edit = new NumEdit(this, "16", 'i');

    layout->addWidget(steps_edit, 1);

    connect((QObject *)steps_edit, SIGNAL(value_changed(double)),
            (QObject *)seq_window, SLOT(set_steps(double)));



    //
    //  bpm
    //
    bpm_label = new QLabel(this);

    bpm_label->setPaletteBackgroundColor(QColor(DEFAULT_BG_COLOR));

    bpm_label->setPaletteForegroundColor(QColor(DEFAULT_FG_COLOR));

    bpm_label->setFont(font);

    bpm_label->setText("bpm");

    bpm_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    bpm_label->setMargin(2);

    bpm_label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    layout->addWidget(bpm_label, 1);

    bpm_edit = new NumEdit(this, "100", 'f');

    layout->addWidget(bpm_edit, 1);

    bpm_edit->setFixedWidth(50);

    connect((QObject *)bpm_edit, SIGNAL(value_changed(double)),
            (QObject *)seq_window, SLOT(set_bpm(double)));

}



ControlBar::~ControlBar()
{
    delete bars_label;

    delete steps_label;

    delete bpm_label;

    delete bars_edit;

    delete steps_edit;

    delete bpm_edit;

    delete layout;
}




void ControlBar::set_bars(int bars)
{
    bars_edit->set_value(bars);
}




void ControlBar::set_steps(int steps)
{
    steps_edit->set_value(steps);
}




void ControlBar::set_bpm(double bpm)
{
    bpm_edit->set_value(bpm);
}








