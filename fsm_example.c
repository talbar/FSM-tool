
/* Copyright (C) January 2014, Leonid Chernin & Evgeny Yoshpe
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "fsm_example.h"

#ifdef FSM_COMPILED
    
//  declaration of FSM using pseudo language

// ***********************************
stateMachine
{
lag_fsm ()

// ***********************************

events {
	start_ev          ()
	port_up_ev        (int lacp_enable,int* port_id)
	port_down_ev      ()
	lacp_up_ev        (int* port_id)
	lacp_down_ev      (int* port_id)
	mismatch_ev       (int* port_id)
	admin_on_ev       ()
	admin_off_ev      ()
}



state idle {
 default	
 transitions {
     { start_ev , NULL , admin_down , NULL }
 }
}

state admin_down {
	transitions {
	    { admin_on_ev , NULL , admin_up , on_admin_on() }
    }
    ef = admin_down
}

state admin_up {
    tm = 6000
    transitions {
        { port_down_ev , NULL , admin_up   , NULL                          }
        { port_up_ev   , NULL , IN_STATE   , on_port_up_composed_state()   }
        { lacp_down_ev , NULL , admin_up   , on_lacp_down_composed_state() }
        { lacp_up_ev   , NULL , admin_up   , on_lacp_up_composed_state()   }
        { TIMER_EVENT  , NULL , admin_down , NULL                          }
        { admin_off_ev , NULL , admin_down , on_admin_off()                }
    }
    substates( lag_down, wait_lag_up, lag_up, mismatch )
}

state lag_down {
    transitions {
	    { port_up_ev  , $ ev->lacp_enable $ , wait_lag_up , on_port_up_lacp_enable()   }
	    { port_up_ev  , $ else $            , lag_up      , on_port_up_lacp_disable()  }
	    { lacp_up_ev  , NULL                , lag_up      , on_lacp_up_in_wait_state() }
	    { mismatch_ev , NULL                , mismatch    , on_mismatch()              }
    }
    ef = lag_down
    xf = lag_down
}

state wait_lag_up {
    tm = 2000
    transitions {
        { mismatch_ev , NULL , mismatch , on_mismatch()              }
        { lacp_up_ev  , NULL , lag_up   , on_lacp_up_in_wait_state() }
        { TIMER_EVENT , NULL , lag_down , on_in_progress_tout()      }
    }
}

state lag_up {
    transitions {
	    { port_down_ev , $ fsm->lag == 0 $ , lag_down  ,  on_port_down_last_port() }
	    { mismatch_ev  , NULL              , condit1   ,  on_mismatch()            }
	    { lacp_down_ev , $ fsm->lag  $     , lag_down  ,  on_lacp_down_last_port() }
    }
}

state condit1 {
    transitions {
        {   NULL_EVENT , $ fsm->lag == 0 $ , mismatch , on_mismatch() }
	    {   NULL_EVENT , $ else $          , lag_up   , NULL          }
	}
}

state mismatch {
    transitions {
        { lacp_down_ev , $ fsm->lag $      , lag_up , on_mismatch_cleared(1) }
        { port_down_ev , $ fsm->lag == 0 $ , lag_up , on_mismatch_cleared(0) }
    }
}

}// end of stateMachine

/* *********************************** */
#endif

/*#$*/
 /* The code  inside placeholders  generated by tool and cannot be modifyed*/


/*----------------------------------------------------------------
            Generated enumerator for  Events of FSM   
---------------------------------------------------------------*/
enum lag_fsm_events_t{
    start_ev  = 0, 
    port_up_ev  = 1, 
    port_down_ev  = 2, 
    lacp_up_ev  = 3, 
    lacp_down_ev  = 4, 
    mismatch_ev  = 5, 
    admin_on_ev  = 6, 
    admin_off_ev  = 7, 
 };

/*----------------------------------------------------------------
            Generated structures for  Events of FSM   
---------------------------------------------------------------*/
struct  lag_fsm_start_ev_t
{
    int  opcode;
    const char *name;
};

struct  lag_fsm_port_up_ev_t
{
    int  opcode;
    const char *name;
    int  lacp_enable ; 
    int*  port_id ; 
};

struct  lag_fsm_port_down_ev_t
{
    int  opcode;
    const char *name;
};

struct  lag_fsm_lacp_up_ev_t
{
    int  opcode;
    const char *name;
    int*  port_id ; 
};

struct  lag_fsm_lacp_down_ev_t
{
    int  opcode;
    const char *name;
    int*  port_id ; 
};

struct  lag_fsm_mismatch_ev_t
{
    int  opcode;
    const char *name;
    int*  port_id ; 
};

struct  lag_fsm_admin_on_ev_t
{
    int  opcode;
    const char *name;
};

struct  lag_fsm_admin_off_ev_t
{
    int  opcode;
    const char *name;
};

/*----------------------------------------------------------------
           State entry/exit functions prototypes  
---------------------------------------------------------------*/
static int admin_down_entry_func(lag_fsm *fsm, struct fsm_event_base *ev);
static int lag_down_entry_func(lag_fsm *fsm, struct fsm_event_base *ev);
static int lag_down_exit_func(lag_fsm *fsm, struct fsm_event_base *ev);
/*----------------------------------------------------------------
            Generated functions for  Events of FSM   
---------------------------------------------------------------*/
int   lag_fsm_start_ev(struct lag_fsm  * fsm)
{
     struct lag_fsm_start_ev_t ev;
     ev.opcode  =  start_ev ;
     ev.name    = "start_ev" ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_port_up_ev(struct lag_fsm  * fsm,  int  lacp_enable,  int*  port_id)
{
     struct lag_fsm_port_up_ev_t ev;
     ev.opcode  =  port_up_ev ;
     ev.name    = "port_up_ev" ;
     ev.lacp_enable = lacp_enable ;
     ev.port_id = port_id ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_port_down_ev(struct lag_fsm  * fsm)
{
     struct lag_fsm_port_down_ev_t ev;
     ev.opcode  =  port_down_ev ;
     ev.name    = "port_down_ev" ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_lacp_up_ev(struct lag_fsm  * fsm,  int*  port_id)
{
     struct lag_fsm_lacp_up_ev_t ev;
     ev.opcode  =  lacp_up_ev ;
     ev.name    = "lacp_up_ev" ;
     ev.port_id = port_id ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_lacp_down_ev(struct lag_fsm  * fsm,  int*  port_id)
{
     struct lag_fsm_lacp_down_ev_t ev;
     ev.opcode  =  lacp_down_ev ;
     ev.name    = "lacp_down_ev" ;
     ev.port_id = port_id ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_mismatch_ev(struct lag_fsm  * fsm,  int*  port_id)
{
     struct lag_fsm_mismatch_ev_t ev;
     ev.opcode  =  mismatch_ev ;
     ev.name    = "mismatch_ev" ;
     ev.port_id = port_id ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_admin_on_ev(struct lag_fsm  * fsm)
{
     struct lag_fsm_admin_on_ev_t ev;
     ev.opcode  =  admin_on_ev ;
     ev.name    = "admin_on_ev" ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
int   lag_fsm_admin_off_ev(struct lag_fsm  * fsm)
{
     struct lag_fsm_admin_off_ev_t ev;
     ev.opcode  =  admin_off_ev ;
     ev.name    = "admin_off_ev" ;

     return fsm_handle_event(&fsm->base, (struct fsm_event_base *)&ev);
}
/*----------------------------------------------------------------
                 Reactions of FSM    
---------------------------------------------------------------*/
static  int on_admin_on (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_port_up_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_lacp_down_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_lacp_up_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_admin_off (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_port_up_lacp_enable (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_port_up_lacp_disable (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_lacp_up_in_wait_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_mismatch (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_in_progress_tout (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_port_down_last_port (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_lacp_down_last_port (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);
static  int on_mismatch_cleared (lag_fsm  * fsm, int parameter, struct fsm_event_base *event);


/*----------------------------------------------------------------
           State Dispatcher function  
---------------------------------------------------------------*/
static int lag_fsm_state_dispatcher( lag_fsm  * fsm, uint16 state, struct fsm_event_base *evt, struct fsm_state_base** target_state );
/*----------------------------------------------------------------
           Initialized  State classes of FSM  
---------------------------------------------------------------*/
 struct fsm_state_base            state_lag_fsm_idle; 
 struct fsm_state_base            state_lag_fsm_admin_down; 
 struct fsm_state_base            state_lag_fsm_admin_up; 
 struct fsm_state_base            state_lag_fsm_lag_down; 
 struct fsm_state_base            state_lag_fsm_wait_lag_up; 
 struct fsm_state_base            state_lag_fsm_lag_up; 
 struct fsm_state_base            state_lag_fsm_condit1; 
 struct fsm_state_base            state_lag_fsm_mismatch; 


 struct fsm_state_base      state_lag_fsm_idle ={ "idle", lag_fsm_idle , SIMPLE, NULL, NULL, &lag_fsm_state_dispatcher, NULL, NULL };

 struct fsm_state_base      state_lag_fsm_admin_down ={ "admin_down", lag_fsm_admin_down , SIMPLE, NULL, NULL, &lag_fsm_state_dispatcher, admin_down_entry_func , NULL };

 struct fsm_state_base      state_lag_fsm_admin_up ={ "admin_up", lag_fsm_admin_up ,  COMPOSED, NULL, &state_lag_fsm_lag_down, &lag_fsm_state_dispatcher, NULL, NULL };

 struct fsm_state_base      state_lag_fsm_lag_down ={ "lag_down", lag_fsm_lag_down , SIMPLE, &state_lag_fsm_admin_up, NULL, &lag_fsm_state_dispatcher, lag_down_entry_func , lag_down_exit_func };

 struct fsm_state_base      state_lag_fsm_wait_lag_up ={ "wait_lag_up", lag_fsm_wait_lag_up , SIMPLE, &state_lag_fsm_admin_up, NULL, &lag_fsm_state_dispatcher, NULL, NULL };

 struct fsm_state_base      state_lag_fsm_lag_up ={ "lag_up", lag_fsm_lag_up , SIMPLE, &state_lag_fsm_admin_up, NULL, &lag_fsm_state_dispatcher, NULL, NULL };

 struct fsm_state_base      state_lag_fsm_condit1 ={ "condit1", lag_fsm_condit1 , CONDITION, NULL, NULL, &lag_fsm_state_dispatcher, NULL, NULL };

 struct fsm_state_base      state_lag_fsm_mismatch ={ "mismatch", lag_fsm_mismatch , SIMPLE, &state_lag_fsm_admin_up, NULL, &lag_fsm_state_dispatcher, NULL, NULL };

static struct fsm_static_data static_data= {0,0,1,0,0 ,{ &state_lag_fsm_idle,  &state_lag_fsm_admin_down,  &state_lag_fsm_admin_up, 
                                                  &state_lag_fsm_lag_down,  &state_lag_fsm_wait_lag_up,  &state_lag_fsm_lag_up, 
                                                  &state_lag_fsm_condit1,  &state_lag_fsm_mismatch, }
};

 static struct fsm_state_base * default_state = &state_lag_fsm_idle ;
/*----------------------------------------------------------------
           StateDispatcher of FSM  
---------------------------------------------------------------*/

static int lag_fsm_state_dispatcher( lag_fsm  * fsm, uint16 state, struct fsm_event_base *evt, struct fsm_state_base** target_state )
{
  int err=0;
  struct fsm_timer_event *event = (  struct fsm_timer_event * )evt;
  switch(state) 
  {

   case lag_fsm_idle : 
    {
      if ( event->opcode == start_ev )
      {
         SET_EVENT(lag_fsm , start_ev)
         {/*tr00:*/
              /* Source line = 51*/ 
            *target_state =  &state_lag_fsm_admin_down;
            return err;
         }
      }

    }break;

   case lag_fsm_admin_down : 
    {
      if ( event->opcode == admin_on_ev )
      {
         SET_EVENT(lag_fsm , admin_on_ev)
         {/*tr10:*/
            err = on_admin_on( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 57*/ 
            *target_state =  &state_lag_fsm_admin_up;
            return err;
         }
      }

    }break;

   case lag_fsm_admin_up : 
    {
      if ( event->opcode == port_down_ev )
      {
         SET_EVENT(lag_fsm , port_down_ev)
         {/*tr20:*/
              /* Source line = 65*/ 
            *target_state =  &state_lag_fsm_admin_up;
            return err;
         }
      }

      else if ( event->opcode == port_up_ev )
      {
         SET_EVENT(lag_fsm , port_up_ev)
         {/*tr21:*/
           fsm->base.reaction_in_state = 1;
           err = on_port_up_composed_state( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 66*/ 
            *target_state =  &state_lag_fsm_admin_up;
            return err;
         }
      }

      else if ( event->opcode == lacp_down_ev )
      {
         SET_EVENT(lag_fsm , lacp_down_ev)
         {/*tr22:*/
            err = on_lacp_down_composed_state( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 67*/ 
            *target_state =  &state_lag_fsm_admin_up;
            return err;
         }
      }

      else if ( event->opcode == lacp_up_ev )
      {
         SET_EVENT(lag_fsm , lacp_up_ev)
         {/*tr23:*/
            err = on_lacp_up_composed_state( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 68*/ 
            *target_state =  &state_lag_fsm_admin_up;
            return err;
         }
      }

      else if (( event->opcode == TIMER_EVENT )&& (event->id == lag_fsm_admin_up) )
      {
         {/*tr24:*/
              /* Source line = 69*/ 
            *target_state =  &state_lag_fsm_admin_down;
            return err;
         }
      }

      else if ( event->opcode == admin_off_ev )
      {
         SET_EVENT(lag_fsm , admin_off_ev)
         {/*tr25:*/
            err = on_admin_off( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 70*/ 
            *target_state =  &state_lag_fsm_admin_down;
            return err;
         }
      }

    }break;

   case lag_fsm_lag_down : 
    {
      if ( event->opcode == port_up_ev )
      {
         SET_EVENT(lag_fsm , port_up_ev)
         if(  ev->lacp_enable  )
         {/*tr30:*/
            err = on_port_up_lacp_enable( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 77*/ 
            *target_state =  &state_lag_fsm_wait_lag_up;
            return err;
         }
         else
         {/*tr31:*/
            err = on_port_up_lacp_disable( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 78*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

      else if ( event->opcode == lacp_up_ev )
      {
         SET_EVENT(lag_fsm , lacp_up_ev)
         {/*tr32:*/
            err = on_lacp_up_in_wait_state( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 79*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

      else if ( event->opcode == mismatch_ev )
      {
         SET_EVENT(lag_fsm , mismatch_ev)
         {/*tr33:*/
            err = on_mismatch( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 80*/ 
            *target_state =  &state_lag_fsm_mismatch;
            return err;
         }
      }

    }break;

   case lag_fsm_wait_lag_up : 
    {
      if ( event->opcode == mismatch_ev )
      {
         SET_EVENT(lag_fsm , mismatch_ev)
         {/*tr40:*/
            err = on_mismatch( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 89*/ 
            *target_state =  &state_lag_fsm_mismatch;
            return err;
         }
      }

      else if ( event->opcode == lacp_up_ev )
      {
         SET_EVENT(lag_fsm , lacp_up_ev)
         {/*tr41:*/
            err = on_lacp_up_in_wait_state( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 90*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

      else if (( event->opcode == TIMER_EVENT )&& (event->id == lag_fsm_wait_lag_up) )
      {
         {/*tr42:*/
            err = on_in_progress_tout( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 91*/ 
            *target_state =  &state_lag_fsm_lag_down;
            return err;
         }
      }

    }break;

   case lag_fsm_lag_up : 
    {
      if ( event->opcode == port_down_ev )
      {
         SET_EVENT(lag_fsm , port_down_ev)
         if(  fsm->lag == 0  )
         {/*tr50:*/
            err = on_port_down_last_port( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 97*/ 
            *target_state =  &state_lag_fsm_lag_down;
            return err;
         }
      }

      else if ( event->opcode == mismatch_ev )
      {
         SET_EVENT(lag_fsm , mismatch_ev)
         {/*tr51:*/
            err = on_mismatch( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 98*/ 
            *target_state =  &state_lag_fsm_condit1;
            return err;
         }
      }

      else if ( event->opcode == lacp_down_ev )
      {
         SET_EVENT(lag_fsm , lacp_down_ev)
         if(  fsm->lag   )
         {/*tr52:*/
            err = on_lacp_down_last_port( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 99*/ 
            *target_state =  &state_lag_fsm_lag_down;
            return err;
         }
      }

    }break;

   case lag_fsm_condit1 : 
    {
      {
         if(  fsm->lag == 0  )
         {/*tr60:*/
            err = on_mismatch( fsm, DEFAULT_PARAMS_D, evt);  /* Source line = 105*/ 
            *target_state =  &state_lag_fsm_mismatch;
            return err;
         }
         else
         {/*tr61:*/
              /* Source line = 106*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

    }break;

   case lag_fsm_mismatch : 
    {
      if ( event->opcode == lacp_down_ev )
      {
         SET_EVENT(lag_fsm , lacp_down_ev)
         if(  fsm->lag  )
         {/*tr70:*/
            err = on_mismatch_cleared( fsm, 1, evt);  /* Source line = 112*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

      else if ( event->opcode == port_down_ev )
      {
         SET_EVENT(lag_fsm , port_down_ev)
         if(  fsm->lag == 0  )
         {/*tr71:*/
            err = on_mismatch_cleared( fsm, 0, evt);  /* Source line = 113*/ 
            *target_state =  &state_lag_fsm_lag_up;
            return err;
         }
      }

    }break;

   default:
   break;

  }
  return FSM_NOT_CONSUMED;

}
/*----------------------------------------------------------------
              Constructor of FSM   
---------------------------------------------------------------*/
int lag_fsm_init(struct lag_fsm *fsm, fsm_user_trace user_trace, void * sched_func, void * unsched_func)
 {
       fsm->state_timer[lag_fsm_idle] = 0 ;
       fsm->state_timer[lag_fsm_admin_down] = 0 ;
       fsm->state_timer[lag_fsm_admin_up] = 6000 ;
       fsm->state_timer[lag_fsm_lag_down] = 0 ;
       fsm->state_timer[lag_fsm_wait_lag_up] = 2000 ;
       fsm->state_timer[lag_fsm_lag_up] = 0 ;
       fsm->state_timer[lag_fsm_condit1] = 0 ;
       fsm->state_timer[lag_fsm_mismatch] = 0 ;

       fsm_init(&fsm->base,  default_state, 8, fsm->state_timer, user_trace, sched_func, unsched_func, &static_data);

      return 0;
 }
 /*  per state functions*/
/*----------------------------------------------------------------
              Getters for each State   
---------------------------------------------------------------*/
tbool lag_fsm_idle_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_idle);
}
tbool lag_fsm_admin_down_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_admin_down);
}
tbool lag_fsm_admin_up_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_admin_up);
}
tbool lag_fsm_lag_down_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_lag_down);
}
tbool lag_fsm_wait_lag_up_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_wait_lag_up);
}
tbool lag_fsm_lag_up_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_lag_up);
}
tbool lag_fsm_condit1_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_condit1);
}
tbool lag_fsm_mismatch_in(struct lag_fsm *fsm){
    return fsm_is_in_state(&fsm->base, lag_fsm_mismatch);
}
/*----------------------------------------------------------------
                 Reactions of FSM  (within comments . user may paste function body outside placeholder region)  
---------------------------------------------------------------*/

/*static  int on_admin_on (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , admin_on_ev) ; 
 }
static  int on_port_up_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , port_up_ev) ; 
 }
static  int on_lacp_down_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , lacp_down_ev) ; 
 }
static  int on_lacp_up_composed_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , lacp_up_ev) ; 
 }
static  int on_admin_off (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , admin_off_ev) ; 
 }
static  int on_port_up_lacp_enable (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , port_up_ev) ; 
 }
static  int on_port_up_lacp_disable (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , port_up_ev) ; 
 }
static  int on_lacp_up_in_wait_state (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , lacp_up_ev) ; 
 }
static  int on_mismatch (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , mismatch_ev) ; 
 }
static  int on_in_progress_tout (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , TIMER_EVENT) ; 
 }
static  int on_port_down_last_port (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , port_down_ev) ; 
 }
static  int on_lacp_down_last_port (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , lacp_down_ev) ; 
 }
static  int on_mismatch_cleared (lag_fsm  * fsm, int parameter, struct fsm_event_base *event)
 {
   SET_EVENT(lag_fsm , lacp_down_ev) ; 
 }


*/
/*   19359*/

/*#$*/

/* Below code written by user */

/* ---------------------------------------------------------------------- */
/*                        on_admin_on */
/* Purpose:         called when FSM passes to adminUp state */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_admin_on(lag_fsm  * theFsmP, int parameter, struct fsm_event_base *event )
{
    /* SET_EVENT(lag_fsm , AdminOnEv) ; */
    /* lag->HandleAdminStatusChange(); */
    /* CalculateAlarm(LAG_ADMIN_UP); */
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                       on_admin_off */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_admin_off( lag_fsm  * theFsmP, int parameter, struct fsm_event_base *event )
{
    /* lag->HandleAdminStatusChange(); */
    /* CalculateAlarm(LAG_ADMIN_DOWN); */
    return 0;
}


/* ---------------------------------------------------------------------- */
/*                       on_port_up_composed_state */
/* Purpose:          called in Composed state */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */

int
on_port_up_composed_state( lag_fsm *fsm, int parameter, struct fsm_event_base *event )
{
    SET_EVENT(lag_fsm, port_up_ev);
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                        on_port_up_lacp_enable */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_port_up_lacp_enable( lag_fsm  * fsm, int parameter, struct fsm_event_base *event )
{
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                        on_port_up_lacp_disable */
/* Purpose:          called when passes to Lag Up state */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_port_up_lacp_disable( lag_fsm  * fsm, int parameter , struct fsm_event_base *event)
{
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                      on_port_down_last_port */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */

int
on_port_down_last_port( lag_fsm  * theFsmP, int parameter, struct fsm_event_base *event )
{
    return 0;
}


/* ---------------------------------------------------------------------- */
/*                       on_in_progress_tout */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */

int
on_in_progress_tout(lag_fsm  * theFsmP,  int parameter, struct fsm_event_base *event )
{
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                        on_mismatch */
/* Purpose:          called when passes to Mismatch state */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_mismatch(lag_fsm  * fsm,  int parameter, struct fsm_event_base *event )
{
    SET_EVENT(lag_fsm, mismatch_ev);
    return 0;
}


/* ---------------------------------------------------------------------- */
/*                        on_lacp_up_in_wait_state */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_lacp_up_in_wait_state(lag_fsm  * fsm, int parameter, struct fsm_event_base *event )
{
    SET_EVENT(lag_fsm, lacp_up_ev);
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                        on_lacp_down_last_port */
/* Purpose:          called in Lag Up state .  pases to Lag down */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_lacp_down_last_port(lag_fsm  * fsm, int parameter , struct fsm_event_base *event)
{
    SET_EVENT(lag_fsm, lacp_down_ev);
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                        on_lacp_down_composed_state */
/* Purpose:          called when passes to adminDownState */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_lacp_down_composed_state(lag_fsm  * theFsmP, int parameter , struct fsm_event_base *event)
{
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                       on_lacp_up_composed_state */
/* Purpose:          called in Composed state */
/* Input  :      none */
/* Output :      none */
/* Precondition: */
/*  */
/* Returns:  none */
/* ----------------------------------------------------------------------- */
int
on_lacp_up_composed_state(lag_fsm  * theFsmP, int parameter , struct fsm_event_base *event)
{
    return 0;
}

static int
admin_down_entry_func(lag_fsm *pFsm, struct fsm_event_base *ev )
{
    printf("called %s\n", __FUNCTION__);
    return 0;
}
static int
lag_down_entry_func(lag_fsm *pFsm , struct fsm_event_base *ev)
{
    printf("called %s\n", __FUNCTION__);
    return 0;
}
static int
lag_down_exit_func(lag_fsm *pFsm , struct fsm_event_base *ev)
{
    printf("called %s\n", __FUNCTION__);
    return 0;
}

static int
on_mismatch_cleared(lag_fsm *fsm, int parameter, struct fsm_event_base *event)
{
    SET_EVENT(lag_fsm, lacp_down_ev);
    return 0;
}
