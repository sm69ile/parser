#include "parser.h"
#include "viewer.h"

void xshow(int argc, char **argv)
{
  sViewer_container *psV_c;
  Arg wargs[10];
  int n;
    
  if (!(psV_c = (sViewer_container*) malloc(sizeof(sViewer_container))))
    {
      syslog(LOG_DEBUG,"malloc failed: sViewer_container*\n"); 

      return; //pthread closed
    }
  else
    {
      iCtask = v_idle;
      psV_c->toplevel = XtVaAppInitialize(&psV_c->app_context, "Viewer", NULL, 0, &argc, argv, NULL, NULL);
      n=0;
      XtSetArg(wargs[n], XtNx, DEFAULT_WIN_POS_X); n++;
      XtSetArg(wargs[n], XtNy, DEFAULT_WIN_POS_Y); n++;
      XtSetValues(psV_c->toplevel, wargs, n);  
      
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

      psV_c->next_state_command = XtVaCreateManagedWidget("next_state_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->prev_state_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_STATE_COMMAND); n++;
      XtSetValues(psV_c->next_state_command, wargs, n);
      n=0;

      psV_c->rotate_command = XtVaCreateManagedWidget("rotate_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->next_state_command); n++;
      XtSetArg(wargs[n], XtNlabel, ROTATE_COMMAND); n++;
      XtSetValues(psV_c->rotate_command, wargs, n);
      n=0;

      psV_c->curr_object_command = XtVaCreateManagedWidget("curr_object_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->rotate_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_CURR_COMMAND); n++;
      XtSetValues(psV_c->curr_object_command, wargs, n);

      psV_c->grid_mode_command = XtVaCreateManagedWidget("grid_mode_command", toggleWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->curr_object_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_GRID_MODE_COMMAND); n++;
      XtSetValues(psV_c->grid_mode_command, wargs, n);

      psV_c->quit_command = XtVaCreateManagedWidget("quit_command", commandWidgetClass, psV_c->form, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNfromVert, psV_c->object_label); n++;
      XtSetArg(wargs[n], XtNfromHoriz, psV_c->grid_mode_command); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_QUIT_COMMAND); n++;
      XtSetArg(wargs[n], XtNsensitive, false); n++;
      XtSetValues(psV_c->quit_command, wargs, n);
      
      if (!(psV_c->psDraw_c = (sDraw_container*) malloc(sizeof(sDraw_container))))
	{
	  syslog(LOG_DEBUG, "malloc failed: sDraw_container*\n");
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
      XtAddCallback(psV_c->rotate_command, XtNcallback, v_rotate, (XtPointer) psV_c);
      XtAddCallback(psV_c->grid_mode_command, XtNcallback, v_grid_mode, (XtPointer) psV_c);
      XtAddCallback(psV_c->quit_command, XtNcallback, v_quit, (XtPointer) psV_c);
      
      XtAppAddTimeOut(psV_c->app_context, 300, v_timer_handler, (XtPointer) psV_c);
      psV_c->grid_mode=false;
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
  
    psV_c->vt_h = XtAppAddTimeOut(psV_c->app_context, 300, v_timer_handler, (XtPointer) psV_c);
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

int v_count_resource(char *f)
{
  FILE *fp;
  char c;

  int i = 0;

  if ((fp = fopen(f,"r")) == NULL)
    fprintf(stderr,"%s, %d: Cannot open file: %s for reading\n", __FILE__, __LINE__, f);
  else
    {
      while((c=getc(fp)) != EOF )
	if ( c == '\n')
	  i++;

      fclose(fp);
      return i;
    }
  return -1;
}

char **v_load_resource(char *f, int k)
{
  FILE *fp;
   char c;      
  static char **r; 
  int i, j;
  
  r = (char **) calloc(k+1, sizeof(char*));
  
  i = j = 0;

  if ((fp = fopen(f,"r")) == NULL)
    fprintf(stderr,"%s, %d: Cannot open file: %s for reading\n", __FILE__, __LINE__, f);
  else
    {
      r[i] = (char*) calloc(CHAR_BUF, sizeof(char));
      while((c=getc(fp)) != EOF )
	{
	  switch(c)
	    {
	    case '\n':
	      i++; j=0;
	      r[i] = (char*) calloc(CHAR_BUF, sizeof(char));
	      break;
	      
	    default:
	      r[i][j++]=c;
	      break;
	    }
	}
      
      free(r[i]);
      fclose(fp);
    }
  return r;
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


  /* static char *v_colors[] = { "black", "white", "green", "red", "lightblue", "blue", "orange","magenta", "yellow","grey","darkgrey","grey14","cyan"}; */
  /* int clength = sizeof(v_colors)/sizeof(v_colors[0]); */

  int clength = v_count_resource(COLOR_FILE);
  char** v_colors = v_load_resource(COLOR_FILE, clength);
  
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
      for(int i=0; i<v_count_resource(COLOR_FILE); i++)
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
    }
}


void v_draw(XtPointer client_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  XGCValues values, rvalues, bvalues, gvalues, nvalues;
  GC gc, gc_border, gc_grid, gc_numb;
  Display *display;
  Drawable window;
  int grid_factor = 50;
  char b[20];
 
  display = XtDisplay(psV_c->draw_shell);
  window = XtWindow(psV_c->draw_shell);
  
  v_set_window_attributes(display, window);
        
  sObject* oact = get_object_by_key(iObj_idx);
  fprintf(stderr,"Displaying symbols, running state: %i, display state: %i\n", iState_idx, iDstate_idx);
  fprintf(stderr,"[\n\tObject[%i] parameters:\n\n\tObject name: %s\n\tObject rotation: %d", iObj_idx, oact->name, oact->angle_deg);
  
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

  // Grid numbers

  nvalues.line_style = LineSolid;
  nvalues.foreground = psV_c->psDraw_c->ctable[15].value;
  gc_numb = XtGetGC(psV_c->draw_shell,
		    GCLineStyle |
		    GCForeground, &nvalues);

  // Grid
  
  gvalues.line_style = LineOnOffDash;
  gvalues.foreground = psV_c->psDraw_c->ctable[3].value;
  gc_grid = XtGetGC(psV_c->draw_shell,
		    GCLineStyle |
		    GCForeground, &gvalues);

  // Border - must be solid because of floodfill
  
  bvalues.foreground = 1;
  bvalues.line_width = 2;
  bvalues.line_style = LineSolid;
  bvalues.fill_style = LineSolid;
  
  gc_border = XtGetGC(psV_c->draw_shell,
	       GCForeground |
	       GCLineStyle  |
	       GCFillStyle  |
	       GCLineWidth, &bvalues);

    
  //Show current graphic context
  XGetGCValues(display, gc, GCForeground|GCBackground|GCLineWidth, &rvalues);
  fprintf(stderr,"\n\tGraphic context:\n\tGCForeground: %li\n\tGCBackground: %li\n\tGCLineWidth: %d\n\n", rvalues.foreground, rvalues.background, rvalues.line_width);

  XClearWindow(display, window);
  XDrawRectangle(display, window, gc_border, 0, 0, V_WIN_X_SIZE,  V_WIN_Y_SIZE);

  
  // psObj->psComIni->key++; defined in cnext() --> psComIni->key is incremented for each new command
  for(int j=0; j<oact->psComIni->key; j++)
    {

      //fprintf(stderr,"j: %i, psComIni-key: viewe%i\n",i, psComIni->key);

      sCommand* cact = get_command_by_key(oact->psComIni, oact->psComIni->next, j);

      if(cact != NULL && iState_idx >= iDstate_idx)
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
	  else if (!strcmp(cact->name, "break") && cact->count_para == 1)   //32523
	    {
	      iDstate_idx = cact->para[0];

	      fprintf(stderr,"\tState switched by Symbol definition \"break()\"  iState_idx: %i, iDstate: %i\n", iState_idx, iDstate_idx); 
	    }
	  
	  else if (!strcmp(cact->name, "line") && cact->count_para == 4)  //32513
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
	  else if (!strcmp(cact->name, "MAX") && cact->count_para == 5) 
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
			     cact->para[2]-cact->para[0],
			     cact->para[3]-cact->para[1]
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

	    } else if (!strcmp(cact->name, "clear"))  //32524
	    {
	      XClearArea(display, window, 0, 0, V_WIN_X_SIZE, V_WIN_Y_SIZE, FALSE);
	    }
	}
    }

  if(psV_c->grid_mode)
    {
      for(int x=0; x<V_WIN_X_SIZE; x+=grid_factor)
	{
	  sprintf(b, "%i", x);
	  XDrawLine(display, window, gc_grid, x, 0, x, V_WIN_Y_SIZE+10);
	  XDrawString(display, window, gc_numb, x, V_WIN_Y_SIZE+25, b, strlen(b));
	}
      
      for(int y=0; y<V_WIN_Y_SIZE; y+=grid_factor)
	{
	  sprintf(b, "%i", y);
	  XDrawLine(display, window, gc_grid, 0, y, V_WIN_X_SIZE+10, y);
	  XDrawString(display, window, gc_numb, V_WIN_X_SIZE+25, y, b, strlen(b));
	}
    }

  fprintf(stderr,"]\n");
  XtReleaseGC(psV_c->draw_shell, gc);
  XtReleaseGC(psV_c->draw_shell, gc_numb);
  XtReleaseGC(psV_c->draw_shell, gc_grid);
  XtReleaseGC(psV_c->draw_shell, gc_border);
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

void v_grid_mode(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  if(psV_c->grid_mode == true) psV_c->grid_mode=false;
  else if (psV_c->grid_mode==false) psV_c->grid_mode = true;


  iDstate_idx=iState_idx;
  v_update_layout(psV_c);
  v_draw(psV_c);
}

void v_rotate(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  sObject* oact = get_object_by_key(iObj_idx);

  oact->angle_deg += 15;

  if(oact->angle_deg == 360)
    oact->angle_deg=0;

  oact->angle_rad = oact->angle_deg*3.14159265358979323846/180;

  fprintf(stderr, "v_rotate: angle_deg=%d\n",oact->angle_deg);

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
  XtDestroyApplicationContext(psV_c->app_context);
  
  //XtDestroyWidget(psV_c->toplevel);
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
