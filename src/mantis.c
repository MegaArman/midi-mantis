// object that increments and outputs a mantis when it gets banged
#include "../m_pd.h"
#include <math.h>

static t_class *mantis_class;

typedef struct _mantis {
  t_object x_obj;
  t_int i_count;
  t_inlet *in_pitch, *in_env;
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

void mantis_onSet_pitch(t_mantis *x, t_floatarg f)
{
  post("pitch %d", (int)round(f));
}

void mantis_onSet_env(t_mantis *x, t_floatarg f)
{
  post("env %d", (int)round(f));
}

//constructor
void *mantis_new(t_floatarg f)
{
  t_mantis *x = (t_mantis *) pd_new(mantis_class);
  
  //set initial mantis value
  x->i_count = f;

  //inlets
  //inlet_new(internal object, internal objects pd ref, type inlet recieves, message to forward to handle the data 
  x->in_pitch = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_pitch"));
  x->in_env = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_env"));
  
  //create a new outlet of floating-point values
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}

void polybang_free(t_mantis *x) 
{
  inlet_free(x->in_pitch);
  inlet_free(x->in_env);
}


void mantis_setup(void) 
{
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
    
    //
    class_addmethod(mantis_class, 
                    (t_method) mantis_onSet_pitch,
                    gensym("in_pitch"),
                    A_DEFFLOAT,
                    0);
    
    class_addmethod(mantis_class, 
                    (t_method) mantis_onSet_env,
                    gensym("in_env"),
                    A_DEFFLOAT,
                    0);
  }
