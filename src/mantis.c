// object that increments and outputs a mantis when it gets banged
#include "../m_pd.h"

static t_class *mantis_class;

typedef struct _mantis {
  t_object x_obj;
  t_int i_count;
} t_mantis;

//called whenever a bang is sent to this object
void mantis_bang(t_mantis *x) 
{
  //convert it to float for outputting
  t_float f = x->i_count;
  //increment mantis
  x->i_count++;
  //send old-mantis to 1st outlet of the object
  outlet_float(x->x_obj.ob_outlet, f);
}

//constructor
void *mantis_new(t_floatarg f)
{
  t_mantis *x = (t_mantis *) pd_new(mantis_class);
  
  //set initial mantis value
  x->i_count = f; 
  
  //create a new outlet of floating-point values
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}

void mantis_setup(void) {
    mantis_class = class_new(gensym("mantis"),
                              (t_newmethod)mantis_new,
                              0, 
                              sizeof(t_mantis),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 
                              0); 
                              /* the object takes one argument which is a floating-point and defaults to 0 */

    /* call a function when object gets banged */
    class_addbang(mantis_class, mantis_bang);
  }
