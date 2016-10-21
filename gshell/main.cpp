#include <SequencerWindow.h>
#include <main.h>

#include <qapplication.h>



QMacStyle *g_mac_style;

Theme     *g_theme;




int main(int argc, char **argv)
{
    QApplication app(argc, argv);


    g_mac_style = new QMacStyle();

    app.setStyle(g_mac_style);


    g_theme = new Theme();


    SequencerWindow *seq_window = new SequencerWindow();
  
    app.setMainWidget(seq_window);


    return app.exec();
}




