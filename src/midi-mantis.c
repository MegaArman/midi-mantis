// object that increments and outputs a counter when it gets banged
#include "../m_pd.h"

static t_class *counter_class;

typedef struct _counter {
  t_object x_obj;
  t_int i_count;
} t_counter;

//called whenever a bang is sent to this object
void counter_bang(t_counter *x) 
{
  //convert it to float for outputting
  t_float f = x->i_count;
  //increment counter
  x->i_count++;
  //send old-counter to 1st outlet of the object
  outlet_float(x->x_obj.ob_outlet, f);
}

//constructor
void *counter_new(t_floatarg f)
{
  t_counter *x = (t_counter *) pd_new(counter_class);
  
  //set initial counter value
  x->i_count = f; 
  
  //create a new outlet of floating-point values
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}

void counter_setup(void) {
    counter_class = class_new(gensym("counter"),
                              (t_newmethod)counter_new,
                              0, 
                              sizeof(t_counter),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 0); /* the object takes one argument which is a floating-point and defaults to 0 */

    /* call a function when object gets banged */
    class_addbang(counter_class, counter_bang);
  }
