#include "parser.h"
#include "viewer.h"

int fops;

void xshow(int argc, char **argv)
{
  sViewer_container *psV_c;
  Arg wargs[10];
  int n;
    
  fprintf(stderr,"\t-> Thread with id: %ld, internal id (iThr): %i started\n", pthread_self(), iThr);

  if (!(psV_c = (sViewer_container*) malloc(sizeof(sViewer_container))))
    {
      fprintf(stderr, "malloc() failed: sViewer_container*\n");
      return; //pthread closed
    }
  else
    {
      psV_c-> v_Pthread = pthread_self();
      psV_c->v_iThr= iThr;
      fprintf(stderr,"Stored Thread id: %ld, internal id (iThr): %i in memory\n", psV_c->v_Pthread, psV_c->v_iThr);

      //Global
      psV_c->psObjIni = psObjIni;  
      //

      psV_c->toplevel = XtVaAppInitialize(&psV_c->app_context, "Viewer", NULL, 0, &argc, argv, NULL, NULL);

      psV_c->box = XtVaCreateManagedWidget("box", boxWidgetClass, psV_c->toplevel, NULL);
      n=0;
      XtSetArg(wargs[n],XtNheight,D_WIN_Y_SIZE+C_WIN_Y_SIZE+50); n++;
      XtSetArg(wargs[n],XtNwidth,D_WIN_X_SIZE+C_WIN_X_SIZE+50); n++;
      XtSetValues(psV_c->box, wargs, n);
      
      psV_c->draw_shell = XtVaCreateManagedWidget("draw_shell", coreWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n],XtNheight,D_WIN_Y_SIZE); n++;
      XtSetArg(wargs[n],XtNwidth,D_WIN_X_SIZE); n++;
      XtSetValues(psV_c->draw_shell, wargs, n);
      
      psV_c->color_shell = XtVaCreateManagedWidget("color_shell", coreWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n],XtNheight,C_WIN_Y_SIZE); n++;
      XtSetArg(wargs[n],XtNwidth,C_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNwidth, C_WIN_X_SIZE); n++;
      XtSetValues(psV_c->color_shell, wargs, n);
      
      psV_c->object_label = XtVaCreateManagedWidget("object_label",labelWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNborderWidth, 0); n++;
      XtSetArg(wargs[n], XtNlabel, O_LABEL_DEFAULT); n++; 
      XtSetValues(psV_c->object_label, wargs, n);
      
      psV_c->color_label = XtVaCreateManagedWidget("color_label",labelWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNborderWidth, 0); n++;
      XtSetArg(wargs[n], XtNwidth, C_WIN_X_SIZE); n++;
      XtSetArg(wargs[n], XtNlabel, C_LABEL_DEFAULT); n++; 
      XtSetValues(psV_c->color_label, wargs, n);
      
      psV_c->prev_object_command = XtVaCreateManagedWidget("prev_object_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_PREV_OBJECT_COMMAND); n++;
      XtSetValues(psV_c->prev_object_command, wargs, n);
      
      psV_c->next_object_command = XtVaCreateManagedWidget("next_object_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_OBJECT_COMMAND); n++;
      XtSetValues(psV_c->next_object_command, wargs, n);
      
      psV_c->prev_state_command = XtVaCreateManagedWidget("prev_state_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_PREV_STATE_COMMAND); n++;
      XtSetValues(psV_c->prev_state_command, wargs, n);
      n=0;

      psV_c->next_state_command = XtVaCreateManagedWidget("next_state_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_STATE_COMMAND); n++;
      XtSetValues(psV_c->next_state_command, wargs, n);
      n=0;

      psV_c->curr_object_command = XtVaCreateManagedWidget("curr_object_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_CURR_COMMAND); n++;
      XtSetValues(psV_c->curr_object_command, wargs, n);
      n=0;

      psV_c->quit_command = XtVaCreateManagedWidget("quit_command", commandWidgetClass, psV_c->box, NULL);
      n=0;
      XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
      XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
      XtSetArg(wargs[n], XtNlabel, LABEL_QUIT_COMMAND); n++;
      XtSetValues(psV_c->quit_command, wargs, n);
      
      if (!(psV_c->psDraw_c = (sDraw_container*) malloc(sizeof(sDraw_container))))
	{
	  fprintf(stderr, "malloc() failed: sDraw_container*\n");
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
      XtAddCallback(psV_c->next_object_command, XtNcallback, v_next_object, (XtPointer) psV_c);
      XtAddCallback(psV_c->prev_object_command, XtNcallback, v_prev_object, (XtPointer) psV_c);
      XtAddCallback(psV_c->next_state_command, XtNcallback, v_next_state, (XtPointer) psV_c);
      XtAddCallback(psV_c->prev_state_command, XtNcallback, v_prev_state, (XtPointer) psV_c);
      XtAddCallback(psV_c->curr_object_command, XtNcallback, v_curr_object, (XtPointer) psV_c);


      XtAddCallback(psV_c->quit_command, XtNcallback, v_quit, (XtPointer) psV_c);
      
      // XtAppAddTimeOut(psV_c->app_context,1000, v_timer_draw, (XtPointer) psV_c);
      
      v_update_layout(psV_c);
      
      XtRealizeWidget(psV_c->toplevel);
      XtAppMainLoop(psV_c->app_context);
    }
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

void v_timer_draw(XtPointer client_data, XtIntervalId *id)
{
  sViewer_container *psV_c = (sViewer_container*) client_data; 

  v_update_layout(psV_c);
  v_draw(psV_c);

  XtAppAddTimeOut(psV_c->app_context, 1000, v_timer_draw, (XtPointer) psV_c);
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

      int y=10;
      for(int i=0; i<20; i++)
	{
	  values.line_width = 20;
	  values.foreground = psV_c->psDraw_c->ctable[i].value;
	  values.background = 1;
	  gc = XCreateGC(display, window, GCForeground|GCLineWidth, &values);
	  y+=20;
	  XDrawLine(display, window, gc,
		    30,
		    y,
		    C_WIN_X_SIZE-10,
		    y
		);

	  int fno_length = snprintf( NULL, 0, "%02d", psV_c->psDraw_c->ctable[i].key);

	  if(!(fno = (char*)malloc((fno_length+1)*sizeof(char))))
	    fprintf(stderr, "malloc() failed: char*\n");
	  else
	    {
	      snprintf( fno, fno_length + 1, "%02d", psV_c->psDraw_c->ctable[i].key);
	      
	      values.foreground = 0;
	      values.background = 1;
	      gc = XCreateGC(display, window, GCForeground|GCBackground, &values);
	      
	      XDrawString(display, window, gc, 10, y+3, fno, 2);
	  
	      free(fno);
	    }
	  XFreeGC(display, gc);
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
      fprintf(stderr,"Backing_store set to: %i\n", g_attributes.backing_store);
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
        
  sObject* oact = get_object_by_key(psV_c->psObjIni, psV_c->psObjIni->next, iObj_idx);

  fprintf(stderr,"Displaying symbols in running state: %i\n", iState_idx);
  fprintf(stderr,"[\n\tObject[%i] parameters:\n\n\tObject name: %s\n", iObj_idx, oact->name);
  
  sVset *sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_foreground");
  if (sact) { values.foreground = psV_c->psDraw_c->ctable[sact->value].value; } else { values.foreground = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_background");
  if (sact) { values.background = psV_c->psDraw_c->ctable[sact->value].value; } else { values.background = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_line_width");
  if (sact) { values.line_width = sact->value; } else { values.line_width = 0; }  

  gc = XCreateGC(display, window,
		 GCForeground |
		 GCBackground |
		 GCLineWidth, &values);

  XGetGCValues(display, gc, GCForeground|GCBackground|GCLineWidth, &rvalues);
  fprintf(stderr,"\n\tGraphic context:\n\tGCForeground: %li\n\tGCBackground: %li\n\tGCLineWidth: %d\n\n", rvalues.foreground, rvalues.background, rvalues.line_width);

  XClearWindow(display, window);

  for(int j=0; j<oact->psComIni->key; j++)
    {
      sCommand* cact = get_command_by_key(oact->psComIni, oact->psComIni->next, j);
      if(cact != NULL)
	{
	  fprintf(stderr,"\tCommand [%d] %s: ", cact->key, cact->name);

	  for(int k=0; k<cact->count_para;k++)
	    fprintf(stderr,"%d ", cact->para[k]);
	  fprintf(stderr,"\n");
	  
	  if (!strcmp(cact->name, "line") && cact->count_para == 4) 
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
	  else if (!strcmp(cact->name, "lineto") && cact->count_para == 2) 
	    {
	      XDrawLine(display, window, gc,
			psV_c->psDraw_c->last_x,
			psV_c->psDraw_c->last_y,
			cact->para[0],
			cact->para[1]
			);

	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];
	      
	    } else if (!strcmp(cact->name, "rectangle") && cact->count_para == 4) 
	    {
	      XDrawRectangle(display, window, gc,
		       cact->para[0],
		       cact->para[1],
		       cact->para[2],
		       cact->para[3]
		       );

	      psV_c->psDraw_c->last_x=cact->para[0]+cact->para[2];
	      psV_c->psDraw_c->last_y=cact->para[1]+cact->para[3];

	    } else if (!strcmp(cact->name, "circle") && cact->count_para == 3) 
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

	    } else if (!strcmp(cact->name, "ellipse") && cact->count_para == 4) 
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

	    } else if (!strcmp(cact->name, "moveto") && cact->count_para == 2) 
	    {
	      psV_c->psDraw_c->last_x=cact->para[0];
	      psV_c->psDraw_c->last_y=cact->para[1];

	    } else if (!strcmp(cact->name, "REM") && cact->count_para == 2) 
	    {

	      XDrawString(display, window, gc,
			  cact->para[0],
			  cact->para[1],
			  "REM", 3
			  );

	    } else if (!strcmp(cact->name, "floodfill") && cact->count_para == 3) 
	    {
	      fops=0;
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
  XFreeGC(display, gc);
}


void v_prev_object(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  if(iObj_idx > 1)
    iObj_idx--;
  else
    iObj_idx = psV_c->psObjIni->next->key;

  v_update_layout(psV_c);
  v_draw(psV_c);
}

void v_next_object(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(iObj_idx < psV_c->psObjIni->next->key)
    iObj_idx++;
  else
    iObj_idx=1;

  v_update_layout(psV_c);
  v_draw(psV_c);
}

void v_curr_object(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

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

  label_length = strlen(get_object_by_key(psV_c->psObjIni, psV_c->psObjIni->next, iObj_idx)->name) + 2*strlen("XX")+strlen(" [/]");

  if(!(label = (char*)malloc((label_length+1)*sizeof(char))))
    fprintf(stderr, "malloc() failed: char*\n");
  else
    {	    

      bzero(label, label_length);
      strcat(label,get_object_by_key(psV_c->psObjIni, psV_c->psObjIni->next, iObj_idx)->name);

      buf_length = strlen(" [XX/");
      if ((buf = (char*)malloc((buf_length)*sizeof(char))))
	  {
	    snprintf(buf, buf_length," [%i/", iState_idx);
	    strcat(label, buf);
	    bzero(buf,buf_length);
	    snprintf(buf, buf_length,"%i]", MAX_STATE);
	    strcat(label,buf);
	    free(buf);
	  }
    }
  XtSetArg(wargs[n], XtNlabel, label); n++;
  XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
  XtSetArg(wargs[n], XtNwidth, D_WIN_X_SIZE); n++;
  XtSetValues(psV_c->object_label, wargs, n);
  free(label);
}



void v_quit(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;

  pthread_t v_Pthread =psV_c->v_Pthread;
  int v_iThr = psV_c->v_iThr;

  fprintf(stderr,"Need to close Thread id: %ld, internal id (iThr): %i\n", v_Pthread, v_iThr);  

  XFreeColormap(XtDisplay(psV_c->draw_shell),psV_c->psDraw_c->colormap);

  XtDestroyWidget(psV_c->toplevel);
  free(psV_c->psDraw_c);
  free(psV_c);

  return;
};


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

	  fops=0;	  
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
	  fops=0;
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

	  fops=0;	  
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

	  fops=0;	  
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

  if( image != NULL)
    {
      bcp = XGetPixel(image, x, y);

      image = floodfill(image, x, y, fcp, bcp, D_WIN_X_SIZE, D_WIN_Y_SIZE);  
      
      XPutImage(display, window, gc, image, 0, 0, 0, 0, D_WIN_X_SIZE, D_WIN_Y_SIZE);
      XDestroyImage(image);
    }
  else
    fprintf(stderr,"Problem occured with XGetImage()\n");
}


XImage* floodfill(XImage* image, int x, int y, Pixel fcp, Pixel bcp, int max_x, int max_y)
{
  Pixel actp;
  
  if(x<max_x && x>0 && y<max_y && y>0 && (actp=XGetPixel(image, x, y)) == bcp && fops<MAX_FLOODFILL)
    {
      if(fops++ < MAX_FLOODFILL)
	{
	XPutPixel(image, x, y, fcp);
	floodfill(image, x+1, y, fcp, bcp, max_x, max_y);
	floodfill(image, x, y+1, fcp, bcp, max_x, max_y);
	floodfill(image, x-1, y, fcp, bcp, max_x, max_y);
	floodfill(image, x, y-1, fcp, bcp, max_x, max_y);
	}
    }

  return image;
};
