#include "parser.h"
#include "viewer.h"

void xshow(int argc, char **argv)
{
  sViewer_container *psV_c;
  Arg wargs[10];
  int n;
    
  //fprintf(stderr,"\t-> Thread with id: %ld, internal id (iThr): %i started\n", pthread_self(), iThr);

  if (!(psV_c = (sViewer_container*) malloc(sizeof(sViewer_container))))
    {
      syslog(LOG_DEBUG,"malloc failed: sViewer_container*\n"); 
      //      fprintf(stderr, "malloc() failed: sViewer_container*\n");
      return; //pthread closed
    }
  else
    {
      iCtask = v_idle;
      psV_c->toplevel = XtVaAppInitialize(&psV_c->app_context, "Viewer", NULL, 0, &argc, argv, NULL, NULL);

      psV_c->form = XtVaCreateManagedWidget("form", formWidgetClass, psV_c->toplevel, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, D_WIN_Y_SIZE+C_WIN_Y_SIZE+50); n++;
      XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE+C_WIN_X_SIZE+50); n++;
      XtSetValues(psV_c->form, wargs, n);
      
      psV_c->draw_shell = XtVaCreateManagedWidget("draw_shell", coreWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, D_WIN_Y_SIZE); n++;
      XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE); n++;
      XtSetValues(psV_c->draw_shell, wargs, n);
      
      psV_c->color_shell = XtVaCreateManagedWidget("color_shell", coreWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, C_WIN_Y_SIZE); n++;
      XtSetArg(wargs[n], XtNwidth, C_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->draw_shell); n++;
      XtSetValues(psV_c->color_shell, wargs, n);
      
      psV_c->object_label = XtVaCreateManagedWidget("object_label",labelWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->draw_shell); n++;
      XtSetArg(wargs[n], XtNborderWidth, 0); n++;
      XtSetArg(wargs[n], XtNlabel, O_LABEL_DEFAULT); n++; 
      XtSetValues(psV_c->object_label, wargs, n);
      
      psV_c->color_label = XtVaCreateManagedWidget("color_label",labelWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, C_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->color_shell); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNborderWidth, 0); n++;
      XtSetArg(wargs[n], XtNlabel, C_LABEL_DEFAULT); n++; 
      XtSetValues(psV_c->color_label, wargs, n);
      
      psV_c->save_command = XtVaCreateManagedWidget("save_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_SAVE_COMMAND); n++;
      XtSetValues(psV_c->save_command, wargs, n);

      psV_c->prev_object_command = XtVaCreateManagedWidget("prev_object_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->save_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_PREV_OBJECT_COMMAND); n++;
      XtSetValues(psV_c->prev_object_command, wargs, n);
      
      psV_c->next_object_command = XtVaCreateManagedWidget("next_object_command", commandWidgetClass, psV_c->form, NULL);

      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->prev_object_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_OBJECT_COMMAND); n++;
      XtSetValues(psV_c->next_object_command, wargs, n);
      
      psV_c->prev_state_command = XtVaCreateManagedWidget("prev_state_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->next_object_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_PREV_STATE_COMMAND); n++;
      XtSetValues(psV_c->prev_state_command, wargs, n);
      n=0;

      psV_c->next_state_command = XtVaCreateManagedWidget("next_state_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->prev_state_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_STATE_COMMAND); n++;
      XtSetValues(psV_c->next_state_command, wargs, n);
      n=0;

      psV_c->curr_object_command = XtVaCreateManagedWidget("curr_object_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->next_state_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_CURR_COMMAND); n++;
      XtSetValues(psV_c->curr_object_command, wargs, n);
      n=0;

      psV_c->quit_command = XtVaCreateManagedWidget("quit_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->curr_object_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_QUIT_COMMAND); n++;
      XtSetValues(psV_c->quit_command, wargs, n);
      
 
      if (!(psV_c->psDraw_c = (sDraw_container*) malloc(sizeof(sDraw_container))))
	{
	  syslog(LOG_DEBUG, "malloc failed: sDraw_container*\n");
	    //	  fprintf(stderr, "malloc() failed: sDraw_container*\n");
	  return; //pthread closed
	}
      else
	{
	  if(!(v_get_draw_c(psV_c->psDraw_c,
			    psV_c->draw_shell,
			    XtDisplay(psV_c->draw_shell),
			    XtWindow(psV_c->draw_shell)
			    ))
	     ) { return; } //pthread closed 
        }
      
 
      // XtAddEventHandler(psV_c->toplevel, EnterWindowMask, FALSE, (XtEventHandler) v_event_handler, (XtPointer) psV_c);

      XtAddEventHandler(psV_c->draw_shell, ExposureMask, FALSE, (XtEventHandler) v_event_draw, (XtPointer) psV_c);

      XtAddEventHandler(psV_c->color_shell, ExposureMask, FALSE, (XtEventHandler) v_event_color, (XtPointer) psV_c);
      XtAddCallback(psV_c->save_command, XtNcallback, v_save, (XtPointer) psV_c);
      XtAddCallback(psV_c->next_object_command, XtNcallback, v_next_object, (XtPointer) psV_c);
      XtAddCallback(psV_c->prev_object_command, XtNcallback, v_prev_object, (XtPointer) psV_c);
      XtAddCallback(psV_c->next_state_command, XtNcallback, v_next_state, (XtPointer) psV_c);
      XtAddCallback(psV_c->prev_state_command, XtNcallback, v_prev_state, (XtPointer) psV_c);
      XtAddCallback(psV_c->curr_object_command, XtNcallback, v_curr_object, (XtPointer) psV_c);
      XtAddCallback(psV_c->quit_command, XtNcallback, v_quit, (XtPointer) psV_c);
      
      XtAppAddTimeOut(psV_c->app_context, 300, v_timer_handler, (XtPointer) psV_c);

      iDstate_idx=iState_idx;
      
      v_update_layout(psV_c);
      
      XtRealizeWidget(psV_c->toplevel);
      XtAppMainLoop(psV_c->app_context);
    }
}


void v_timer_handler(XtPointer client_data, XtIntervalId *id)
{
    sViewer_container *psV_c = (sViewer_container*) client_data; 
    Widget w_quit = psV_c->quit_command;
    Widget w_current = psV_c->curr_object_command;
  

    if (iCtask == v_hide)
	{
	    Display *d = XtDisplay(psV_c->toplevel);
	    Window w = XtWindow(psV_c->toplevel);

	    XUnmapWindow(d, w);
	}

    if (iCtask == v_show)
	{
	    Display *d = XtDisplay(psV_c->toplevel);
	    Window w = XtWindow(psV_c->toplevel);

	    XMapWindow(d, w);
	}

    if (iCtask == v_redraw)
	{
	    if (XtHasCallbacks(w_current, XtNcallback) == XtCallbackHasSome)
		{ XtCallCallbacks(w_current, XtNcallback, client_data); }
	}
    else if ( iCtask == v_exit)
	{
	    if (XtHasCallbacks(w_quit, XtNcallback) == XtCallbackHasSome)
		{ XtCallCallbacks(w_quit, XtNcallback, client_data); }
	}

    iCtask = v_idle;
    iCstate = v_open;
  
    psV_c->vt_h = XtAppAddTimeOut(psV_c->app_context,300, v_timer_handler, (XtPointer) psV_c);
}

void v_event_handler(Widget w, XtPointer client_data, XEvent* ev, Boolean continue_to_dispatch)
{

  sViewer_container *psV_c = (sViewer_container*) client_data; 
  if(ev->type == EnterNotify){ v_draw(psV_c);}
}

void v_event_draw(Widget w, XtPointer client_data, XExposeEvent* ev)
{
  sViewer_container *psV_c = (sViewer_container*) client_data; 

  if(!ev->count)
    v_draw(psV_c);
}


int v_get_draw_c(sDraw_container* act, Widget w, Display *display, Window window)
{
  act->last_x=0;
  act->last_y=0;

  if( DefaultVisual(display,0)->class != TrueColor)
    {
      free(act);
      return 0; 
    }
    
  act->colormap = XCopyColormapAndFree(display,DefaultColormap(display, 0));
  XInstallColormap(display,act->colormap);

  static char *v_colors[] = { "black", "white", "green", "red", "lightblue", "blue", "orange","magenta", "yellow","grey","darkgrey","grey14","cyan"};

  int clength = sizeof(v_colors)/sizeof(v_colors[0]);
    
  for (int i=0; i<clength; i++)
    {
      act->ctable[i].key=i;
      act->ctable[i].name=v_colors[i];
      act->ctable[i].value=get_color_by_name(display, act->colormap, v_colors[i]);
    }

  return 1;
}
  
void v_event_color(Widget w, XtPointer client_data, XExposeEvent* ev)
{
  sViewer_container *psV_c = (sViewer_container*) client_data; 

  if(!ev->count)
    {
      XGCValues values;
      GC gc;
       Display *display;
      Drawable window;
      char *fno;
      
      display = XtDisplay(psV_c->color_shell);
      window = XtWindow(psV_c->color_shell);

      XClearWindow(display, window);

      int y=0;
      for(int i=0; i<19; i++)
	{
	  values.line_width = 20;
	  values.foreground = psV_c->psDraw_c->ctable[i].value;
	  values.background = 1;

	  gc = XtGetGC(psV_c->color_shell, GCForeground|GCLineWidth, &values);
	  y+=20;
	  XDrawLine(display, window, gc,
		    30,
		    y,
		    C_WIN_X_SIZE-10,
		    y
		);

	  int fno_length = snprintf( NULL, 0, "%02d", psV_c->psDraw_c->ctable[i].key);

	  if(!(fno = (char*)malloc((fno_length+1)*sizeof(char))))
	    syslog(LOG_DEBUG, "malloc failed: char*\n");
	  //	    fprintf(stderr, "malloc() failed: char*\n");
	  else
	    {
	      snprintf( fno, fno_length + 1, "%02d", psV_c->psDraw_c->ctable[i].key);
	      
	      values.foreground = 0;
	      values.background = 1;
	      gc = XCreateGC(display, window, GCForeground|GCBackground, &values);
	      
	      XDrawString(display, window, gc, 10, y+3, fno, 2);
	  
	      free(fno);
	    }

	  XtReleaseGC(psV_c->color_shell, gc);
	}
    }
}


void v_set_window_attributes(Display* display, Drawable window)
{

  XWindowAttributes g_attributes;

  XGetWindowAttributes(display, window, &g_attributes);

  if(g_attributes.backing_store == 0)
    {
      XSetWindowAttributes s_attributes;
      unsigned long valuemask=CWBackingStore;

      s_attributes.backing_store=Always;
      XChangeWindowAttributes(display, window, valuemask, &s_attributes);

      XGetWindowAttributes(display, window, &g_attributes);
      //fprintf(stderr,"Backing_store set to: %i\n", g_attributes.backing_store);
    }
}


void v_draw(XtPointer client_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  XGCValues values, rvalues;
  GC gc;
  Display *display;
  Drawable window;
 

  display = XtDisplay(psV_c->draw_shell);
  window = XtWindow(psV_c->draw_shell);
  
  v_set_window_attributes(display, window);
        
  sObject* oact = get_object_by_key(iObj_idx);
  fprintf(stderr,"Displaying symbols, running state: %i, display state: %i\n", iState_idx, iDstate_idx);
  fprintf(stderr,"[\n\tObject[%i] parameters:\n\n\tObject name: %s\n", iObj_idx, oact->name);
  
  sVset *sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_foreground");
  if (sact) { values.foreground = psV_c->psDraw_c->ctable[sact->value].value; } else { values.foreground = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_background");
  if (sact) { values.background = psV_c->psDraw_c->ctable[sact->value].value; } else { values.background = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_line_width");
  if (sact) { values.line_width = sact->value; } else { values.line_width = 0; }  

  gc = XtGetGC(psV_c->draw_shell,
	       GCForeground |
	       GCBackground |
	       GCLineWidth, &values);

  
  XGetGCValues(display, gc, GCForeground|GCBackground|GCLineWidth, &rvalues);
  fprintf(stderr,"\n\tGraphic context:\n\tGCForeground: %li\n\tGCBackground: %li\n\tGCLineWidth: %d\n\n", rvalues.foreground, rvalues.background, rvalues.line_width);

  XClearWindow(display, window);
  
  // psObj->psComIni->key++; defined in cnext() --> psComIni->key is incremented for each new command
  for(int j=0; j<oact->psComIni->key; j++)
    {

      //fprintf(stderr,"j: %i, psComIni-key: %i\n",i, psComIni->key);

      sCommand* cact = get_command_by_key(oact->psComIni, oact->psComIni->next, j);

      if(cact != NULL && iState_idx == iDstate_idx)
	//	  if(cact != NULL)
	{
	  // draw all commands iState_idx = 0
	  // next_state button pressed iState_idx = 1
	  // walk through the list of commands until command state(1) is found;
	  // draw all commands until break (0)


	  fprintf(stderr,"\tCommand [%d] %s: ", cact->key, cact->name);

	  for(int k=0; k<cact->count_para;k++)
	    fprintf(stderr,"%d ", cact->para[k]);
	  fprintf(stderr,"\n");

	  if (!strcmp(cact->name, "state") && cact->count_para == 1)  //32522
	    {
	      
	      iDstate_idx = cact->para[0];

	      fprintf(stderr,"\tState switched by Symbol definition \"state()\"  iState_idx: %i, iDstate: %i\n", iState_idx, iDstate_idx);
	    }
	  if (!strcmp(cact->name, "break") && cact->count_para == 1)   //32523
	    {
	      iDstate_idx = cact->para[0];

	      fprintf(stderr,"\tState switched by Symbol definition \"break()\"  iState_idx: %i, iDstate: %i\n", iState_idx, iDstate_idx); 
	    }
	  
	  if (!strcmp(cact->name, "line") && cact->count_para == 4)  //32513
	    {
	      XDrawLine(display, window, gc,
			cact->para[0],
			cact->para[1],
			cact->para[2],
			cact->para[3]
			);
	      psV_c->psDraw_c->last_x=cact->para[2];
	      psV_c->psDraw_c->last_y=cact->para[3];
	      
	    }
	  if (!strcmp(cact->name, "MAX") && cact->count_para == 5) 
	    {
	      sCommand* mact = get_command_by_key(oact->psComIni, oact->psComIni->next, cact->para[0]);

	      if(mact != 0 && !strncmp(mact->name,"line",strlen("line")))
		{
		  Pixel mcolor = psV_c->psDraw_c->ctable[cact->para[4]].value; 
		  v_max_impl(cact, mact, 1, mcolor, display, window, gc);
		}

	    }
	  else if (!strcmp(cact->name, "lineto") && cact->count_para == 2)  //32514
	    {
	      XDrawLine(display, window, gc,
			psV_c->psDraw_c->last_x,
			psV_c->psDraw_c->last_y,
			cact->para[0],
			cact->para[1]
			);

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];
	      
	    } else if (!strcmp(cact->name, "rectangle") && cact->count_para == 4) //32515
	    {
	      XDrawRectangle(display, window, gc,
			     cact->para[0],
			     cact->para[1],
			     cact->para[2],
			     cact->para[3]
			     );

	      psV_c->psDraw_c->last_x=cact->para[0]+cact->para[2];
	      psV_c->psDraw_c->last_y=cact->para[1]+cact->para[3];

	    } else if (!strcmp(cact->name, "circle") && cact->count_para == 3)  //32516
	    {
	      XDrawArc(display, window, gc,
		       cact->para[0]-cact->para[2],
		       cact->para[1]-cact->para[2],
		       2*cact->para[2],
		       2*cact->para[2],
		       0,
		       64*360
		       );

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];

	    } else if (!strcmp(cact->name, "ellipse") && cact->count_para == 4) //32517
	    {
	      XDrawArc(display, window, gc,
		       cact->para[0],
		       cact->para[1],
		       cact->para[2],
		       cact->para[3],
		       0,
		       64*360
		       );

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];

	    } else if (!strcmp(cact->name, "arc") && cact->count_para == 5)  //32521
	    {
	      XDrawArc(display, window, gc,
		       cact->para[0]-cact->para[4],
		       cact->para[1]-cact->para[4],
		       2*cact->para[4],
		       2*cact->para[4],
		       64*cact->para[2],
		       64*cact->para[3]);

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];	  
	    }

	  else if (!strcmp(cact->name, "flowways") && cact->count_para == 4) //32520
	    {
	      XDrawArc(display, window, gc, cact->para[0]-5, cact->para[1]-5, 10, 10, 0, 64*360);
	      XDrawArc(display, window, gc, cact->para[2]-5, cact->para[3]-5, 10, 10, 0, 64*360);
	      XFillArc(display, window, gc, cact->para[0]-4, cact->para[1]-4, 8, 8, 0, 64*360);
	      XFillArc(display, window, gc, cact->para[2]-4, cact->para[3]-4, 8, 8, 0, 64*360);
	    }
	  else if (!strcmp(cact->name, "moveto") && cact->count_para == 2)  //32518
	    {
	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];

	    } else if (!strcmp(cact->name, "REM") && cact->count_para == 4) //32519
	    {

	      XDrawString(display, window, gc,
			  cact->para[0],
			  cact->para[1],
			  "REM",strlen("REM")
			  );

	    } else if (!strcmp(cact->name, "floodfill") && cact->count_para == 3) 
	    {
	      v_floodfill(display, window, gc,
			  cact->para[0],
			  cact->para[1],
			  psV_c->psDraw_c->ctable[cact->para[2]].value 
			  );

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];

	    }
	}
    }
  fprintf(stderr,"]\n");
  XtReleaseGC(psV_c->draw_shell, gc);
}


void v_prev_object(Widget w, XtPointer client_data, XtPointer call_data)
{
    sViewer_container *psV_c = (sViewer_container*) client_data;

    if(iObj_idx > 1)
	iObj_idx--;
    else
	iObj_idx = psObjIni->next->key;
    v_update_layout(psV_c);
    v_draw(psV_c);
}

void v_next_object(Widget w, XtPointer client_data, XtPointer call_data)
{
    sViewer_container *psV_c = (sViewer_container*) client_data;
  
    if(iObj_idx < psObjIni->next->key)
	iObj_idx++;
    else
	iObj_idx=1;

    v_update_layout(psV_c);
    v_draw(psV_c);
}

void v_curr_object(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  iDstate_idx=iState_idx;

  v_update_layout(psV_c);
  v_draw(psV_c);
}


void v_update_layout(XtPointer client_data) 
{
    sViewer_container *psV_c = (sViewer_container*) client_data;
    Arg wargs[10];
    int n=0;
    char *label, *buf;
    size_t label_length, buf_length;


    char* act_name = get_object_by_key(iObj_idx)->name;
    
    label_length = strlen("Name: ") + strlen(act_name) + strlen(", State: ") + 4*strlen("XXXX") + 2*strlen(" [/]")+ strlen(", DState: X") ;

    if(!(label = (char*)malloc((label_length+1)*sizeof(char))))
      syslog(LOG_DEBUG, "malloc failed: char*\n");
    //      fprintf(stderr, "malloc() failed: char*\n");
    else
	{	    
	    bzero(label, label_length);
      
	    buf_length = strlen("[XXXX/");
	    if ((buf = (char*)malloc((buf_length)*sizeof(char))))
		{
		    bzero(buf,buf_length);
		    snprintf(buf, buf_length," [%i/", iObj_idx);
		    strcat(label, buf);
		    bzero(buf,buf_length);
		    snprintf(buf, buf_length,"%i] ", psObjIni->next->key);
		    strcat(label,buf);
		    strcat(label,"Name: ");
		    strcat(label, act_name);
		    strcat(label,", State: ");

		    bzero(buf,buf_length);
		    snprintf(buf, buf_length," [%i/", iState_idx);
		    strcat(label, buf);
		    bzero(buf,buf_length);
		    snprintf(buf, buf_length,"%i]", MAX_STATE);
		    strcat(label,buf);
		    strcat(label,", dState: ");
		    bzero(buf,buf_length);
		    snprintf(buf, buf_length, "%i", iDstate_idx);
		    strcat(label, buf);
		    free(buf);
		}
	    else
	      syslog(LOG_DEBUG, "malloc failed: char*\n");
	    //	      fprintf(stderr, "malloc() failed: char*\n"); 
	}
    XtSetArg(wargs[n], XtNlabel, label); n++;
    XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
    XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE); n++;
    XtSetValues(psV_c->object_label, wargs, n);
    free(label);
}

void v_save(Widget w, XtPointer client_data, XtPointer call_data)
{
    save();
}

void v_quit(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  XFreeColormap(XtDisplay(psV_c->draw_shell),psV_c->psDraw_c->colormap);
  XtRemoveTimeOut(psV_c->vt_h);
  XtRemoveEventHandler(psV_c->toplevel ,EnterWindowMask, FALSE, (XtEventHandler) v_event_handler, (XtPointer) psV_c);
  XtDestroyWidget(psV_c->toplevel);

  iCstate = v_close;
  
  free(psV_c->psDraw_c);
  free(psV_c);
  quit(); 
}

void v_prev_state(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(iState_idx > 0)
    iState_idx--;
  else
    iState_idx=MAX_STATE;

  v_update_layout(psV_c);
  v_draw(psV_c);
}

void v_next_state(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(iState_idx < MAX_STATE)
    iState_idx++;
  else
    iState_idx=0;

  v_update_layout(psV_c);
  v_draw(psV_c);
}


Pixel get_color_by_name(Display* display, Colormap cm, char* cname)
{
  XColor color, ignore;

  if (XAllocNamedColor(display, cm, cname, &color, &ignore))
    return color.pixel;
  return 0;
}

void v_max_impl(sCommand* cact, sCommand* mact, int state, Pixel mcolor, Display* display, Drawable window , GC gc)
{

    if (cact->para[1] == 6)
    {
      int y = mact->para[1];
      for(int k=0; k < iState_idx && k < cact->para[3]; k++)
	{
	  y+=cact->para[2]; 
	  XDrawLine(display, window, gc,
		    mact->para[0],
		    y,
		    mact->para[2],
		    y
		    );

	  int x=mact->para[2]-mact->para[0];
	  if (x<0)
	    x=mact->para[0]-mact->para[2];

	  v_floodfill(display, window, gc,
		      x,
		      y-cact->para[2]/2,
		      mcolor
		      );
	}
    }
  if (cact->para[1] == 12)
    {
      int y = mact->para[1];
      for(int k=0; k < iState_idx && k < cact->para[3]; k++)
	{
	  y-=cact->para[2]; 
	  XDrawLine(display, window, gc,
		    mact->para[0],
		    y,
		    mact->para[2],
		    y
		    );

	  int x=mact->para[2]-mact->para[0];
	  if (x< 0)
	    x=mact->para[0]-mact->para[2];

	  v_floodfill(display, window, gc,
		      x,
		      y+cact->para[2]/2,
		      mcolor
		      );
	}
    }

  if (cact->para[1] == 3)
    {
      int x = mact->para[0];
      for(int k=0; k < iState_idx && k < cact->para[3]; k++)
	{
	  x+=cact->para[2]; 
	  XDrawLine(display, window, gc,
		    x,
		    mact->para[1],
		    x,
		    mact->para[3]
		    );

	  int y=mact->para[1]-mact->para[3];
	  if (y<0)
	    y=mact->para[3]-mact->para[1];


	  v_floodfill(display, window, gc,
		      x-cact->para[2]/2,
		      y,
		      mcolor
		      );
	}
    }

  if (cact->para[1] == 9)
    {
      int x = mact->para[0];
      for(int k=0; k < iState_idx && k < cact->para[3]; k++)
	{
	  x-=cact->para[2]; 
	  XDrawLine(display, window, gc,
		    x,
		    mact->para[1],
		    x,
		    mact->para[3]
		    );

	  int y=mact->para[1]-mact->para[3];
	  if (y<0)
	    y=mact->para[3]-mact->para[1];

	  v_floodfill(display, window, gc,
		      x+cact->para[2]/2,
		      y,
		      mcolor
		      );
	}
    }
}


void v_floodfill(Display* display, Drawable window, GC gc, int x, int y, Pixel fcp)
{
  Pixel  bcp;
  XImage* image = XGetImage(display, window, 0, 0, D_WIN_X_SIZE, D_WIN_Y_SIZE, AllPlanes, ZPixmap);
  fops=0;	  
    
  if( image != NULL)
    {
      bcp = XGetPixel(image, x, y);

      image = floodfill(image, x, y, fcp, bcp, D_WIN_X_SIZE, D_WIN_Y_SIZE);  
      
      XPutImage(display, window, gc, image, 0, 0, 0, 0, D_WIN_X_SIZE, D_WIN_Y_SIZE);
      XDestroyImage(image);
    }
  else
    syslog(LOG_DEBUG, "Problem occured with XGetImage()\n");
  //  fprintf(stderr,"Problem occured with XGetImage()\n");
}


XImage* floodfill(XImage* image, int x, int y, Pixel fcp, Pixel bcp, int max_x, int max_y)
{
    Pixel actp;

    if(x<max_x && x>0 && y<max_y && y>0 && (actp=XGetPixel(image, x, y)) == bcp && fops++<MAX_FLOODFILL)
	{
	    XPutPixel(image, x, y, fcp);
	    floodfill(image, x+1, y, fcp, bcp, max_x, max_y);
	    floodfill(image, x, y+1, fcp, bcp, max_x, max_y);
	    floodfill(image, x-1, y, fcp, bcp, max_x, max_y);
	    floodfill(image, x, y-1, fcp, bcp, max_x, max_y);

	}

    return image;
}
