#include <StepHeaderRow.h>

#include <main.h>
#include <Theme.h>


#include <qpainter.h>

#include <assert.h>



StepHeaderRow::StepHeaderRow(QWidget *parent, MRPSequencer &mrp_seq, int num_steps) 
    : QHBox(parent),
      mrp_seq(mrp_seq),
      num_steps(num_steps),
      current_step(0)
{
    setPaletteBackgroundColor(QColor(HEADER_BG_COLOR));

    setPaletteForegroundColor(QColor(HEADER_FG_COLOR));

    //
    //  CornerHeader and StepHeaders
    //
    corner_header = new CornerHeader(this, mrp_seq);

    step_headers = (StepHeader **)malloc(sizeof(StepHeader *) * num_steps);

    assert(step_headers);


    for (int i = 0; i < num_steps; i++) 
    {
        step_headers[i] = new StepHeader(this, mrp_seq, i);
    }


    //
    //  this allows the background color to show through,
    //  creating the vertical gridlines
    //
    setEraseColor(QColor(GRID_COLOR));

    setSpacing(g_theme->get_grid_spacing());
}



StepHeaderRow::~StepHeaderRow()
{
    for (int i = 0; i < num_steps; i++) 
    {
        delete step_headers[i];
    }
}



void StepHeaderRow::set_current_step(int step)
{

    //
    //  first unset the previous current_step
    //
    if (current_step >= 0 && current_step < num_steps)
    {
        step_headers[current_step]->set_is_current_step(false);
    }

    //
    //  now set the new current_step
    //
    current_step = step;

    if (current_step >= 0 && current_step < num_steps)
    {
        step_headers[current_step]->set_is_current_step(true);
    }
}




void StepHeaderRow::set_loop_step(int step)
{

    //
    //  first unset the previous loop_step
    //
    if (loop_step >= 0 && loop_step < num_steps)
    {
        step_headers[loop_step]->set_is_loop_step(false);
    }

    //
    //  now set the new loop_step
    //
    loop_step = step;

    if (loop_step >= 0 && loop_step < num_steps)
    {
        step_headers[loop_step]->set_is_loop_step(true);
    }
}


