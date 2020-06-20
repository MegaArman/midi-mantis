// object that takes audio to midi input from sigmund but rounds pitch & amp and accounts for noteoffs
#include "../m_pd.h"
#include <math.h>

static t_class *mantis_class;

typedef struct _mantis {
  t_object x_obj;
  t_float held_pitch;
  t_float new_amp;
  t_inlet *in_pitch, *in_env;
  t_outlet *out_pitch, *out_env;
} t_mantis;

void mantis_onSet_pitch(t_mantis *x, t_floatarg f)
{
  int rounded_pitch =  (int)round(f);

  //turn off last note in case of legato
  outlet_float(x->out_env, 0);
  outlet_float(x->out_pitch, x->held_pitch);

  //output new note
  x->held_pitch = rounded_pitch;
  outlet_float(x->out_env, x->new_amp);
  outlet_float(x->out_pitch, x->held_pitch);
  
}

void mantis_onSet_env(t_mantis *x, t_floatarg f)
{
  int rounded_amp = f < 50 ? 0 : (int)round(f);
  if (rounded_amp != 0) 
  {
    x->new_amp = rounded_amp;
  }
  
  outlet_float(x->out_env, rounded_amp);
  
  //to turn note off when it naturally dies
  (rounded_amp == 0) ? 
  outlet_float(x->out_pitch, x->held_pitch) 
  :
  NULL;
}

//constructor
void *mantis_new(t_floatarg f)
{
  t_mantis *x = (t_mantis *) pd_new(mantis_class);
  x->held_pitch = 0;

  //createt inlets
  x->in_pitch = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_pitch"));
  x->in_env = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("in_env"));
  
  //create outlets
  x->out_pitch = outlet_new(&x->x_obj, &s_float);
  x->out_env = outlet_new(&x->x_obj, &s_float);

  return (void *)x;
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
