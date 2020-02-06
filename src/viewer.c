#include "parser.h"
#include "viewer.h"

void xshow(int argc, char **argv)
{
  sViewer_container *psV_c;
  Arg wargs[10];
  int n;

  XtAppContext app_context;
  
  if (!(psV_c = (sViewer_container*) malloc(sizeof(sViewer_container))))
    quit();

  psV_c->psDraw_c = NULL;
  psV_c->psObjIni = NULL;
  
  psV_c->toplevel = XtVaAppInitialize(&app_context, "Viewer", NULL, 0, &argc, argv, NULL, NULL);

  psV_c->box = XtVaCreateManagedWidget("box", boxWidgetClass, psV_c->toplevel, NULL);

  psV_c->draw_shell=XtVaCreateManagedWidget("draw_shell", coreWidgetClass, psV_c->box, NULL);
  n=0;
  XtSetArg(wargs[n],XtNheight,WIN_Y_SIZE); n++;
  XtSetArg(wargs[n],XtNwidth,WIN_X_SIZE); n++;
  XtSetValues(psV_c->draw_shell, wargs, n);

  psV_c->object_label = XtCreateManagedWidget("object_label",labelWidgetClass, psV_c->box, NULL, 0);
  XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
  XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
  XtSetArg(wargs[n], XtNborderWidth, 0); n++;
  XtSetArg(wargs[n], XtNlabel, "Test"); n++; 
  XtSetValues(psV_c->object_label, wargs, n);

  psV_c->next_command = XtCreateManagedWidget("next_command", commandWidgetClass, psV_c->box, NULL, 0);
  n = 0;
  XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
  XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
  XtSetArg(wargs[n], XtNlabel, LABEL_NEXT_COMMAND); n++;
  XtSetValues(psV_c->next_command, wargs, n);

  psV_c->prev_command = XtCreateManagedWidget("prev_command", commandWidgetClass, psV_c->box, NULL, 0);
  n = 0;
  XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
  XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
  XtSetArg(wargs[n], XtNlabel, LABEL_PREV_COMMAND); n++;
  XtSetValues(psV_c->prev_command, wargs, n);

  psV_c->quit_command = XtVaCreateManagedWidget("quit_command", commandWidgetClass, psV_c->box, NULL);
  n = 0;
  XtSetArg(wargs[n], XtNheight, LABEL_HEIGHT); n++;
  XtSetArg(wargs[n], XtNwidth, LABEL_WIDTH); n++;
  XtSetArg(wargs[n], XtNlabel, LABEL_QUIT_COMMAND); n++;
  XtSetValues(psV_c->quit_command, wargs, n);
  n=0;


  n=0;
  XtSetArg(wargs[n], XtNlabel, "Default"); n++;
  XtSetArg(wargs[n], XtNwidth, WIN_X_SIZE); n++;
  XtSetValues(psV_c->object_label, wargs, n);  
  
  // Start global
  psV_c->psObjIni = psObjIni;
  // End global
  
  psV_c->psDraw_c = v_get_draw_c(psV_c->draw_shell,
				 (Display *) XtDisplay(psV_c->draw_shell),
				 XtWindow(psV_c->draw_shell)
				 );
  if(psV_c->psDraw_c)
    {
      XtAddEventHandler(psV_c->draw_shell, ExposureMask, FALSE, (XtEventHandler) v_ev_draw, (XtPointer) psV_c);
      XtAddCallback(psV_c->next_command, XtNcallback, v_next, (XtPointer) psV_c);
      XtAddCallback(psV_c->prev_command, XtNcallback, v_prev, (XtPointer) psV_c);
      XtAddCallback(psV_c->quit_command, XtNcallback, v_quit, (XtPointer) psV_c);

    }
  else
    exit(EXIT_FAILURE);
  
  XtRealizeWidget(psV_c->toplevel);
  XtAppMainLoop(app_context);
}

sDraw_container *v_get_draw_c(Widget w, Display *display, Window window)
{
  sDraw_container *psDraw_c;
  
  if (!(psDraw_c = (sDraw_container*) malloc(sizeof(sDraw_container))))
    return NULL;

  psDraw_c->obj_idx=1;
  psDraw_c->last_x=0;
  psDraw_c->last_y=0;
  
  if( DefaultVisual(display,0)->class != TrueColor)
    {
      free(psDraw_c);
      return NULL;
    }

  psDraw_c->colormap = XCopyColormapAndFree(display,DefaultColormap(display, 0));
  XInstallColormap(display,psDraw_c->colormap);

  return psDraw_c;
}

void v_ev_draw(Widget w, XtPointer client_data, XExposeEvent* ev)
{
  sViewer_container *psV_c = (sViewer_container*) client_data; 
  if(!ev->count)
    v_draw(psV_c);
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

  sObject* oact = get_object_by_key(psV_c->psObjIni, psV_c->psObjIni->next, psV_c->psDraw_c->obj_idx);
  printf("[\n\tObject parameters:\n\tObject name: %s\n", oact->name);

  sVset *sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_foreground");
  if (sact) { values.foreground = sact->value; } else { values.foreground = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_background");
  if (sact) { values.background = sact->value; } else { values.background = 0; }  

  sact = get_setting_by_name(oact->psVsetIni, oact->psVsetIni->next,"v_line_width");
  if (sact) { values.line_width = sact->value; } else { values.line_width = 0; }  

  gc = XCreateGC(display, window,
		 GCForeground |
		 GCBackground |
		 GCLineWidth, &values);


  XGetGCValues(display, gc, GCForeground|GCBackground|GCLineWidth, &rvalues);
  printf("\nGraphic context:\nGCForeground: %li\nGCBackground: %li\nGCLineWidth: %d\n", rvalues.foreground, rvalues.background, rvalues.line_width);

  XClearWindow(display, window);
  for(int j=0; j<oact->psComIni->key; j++)
    {
      sCommand* cact = get_command_by_key(oact->psComIni, oact->psComIni->next, j);
      if(cact != NULL)
	{
	  printf("\tCommand [%d] %s: ", j,cact->name);

	  for(int k=0; k<cact->count_para;k++)
	    printf("%d ", cact->para[k]);
	  printf("\n");
	  
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
	      
	    } else if (!strcmp(cact->name, "lineto") && cact->count_para == 2) 
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

	    }
	}
    }
  printf("]\n");
  XFreeGC(display, gc);
}


void v_next(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(psV_c->psDraw_c->obj_idx < psV_c->psObjIni->next->key-1)
    psV_c->psDraw_c->obj_idx++;
  else
    psV_c->psDraw_c->obj_idx = 1;
  

  v_draw(psV_c);
}


void v_prev(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(psV_c->psDraw_c->obj_idx > 1)
    psV_c->psDraw_c->obj_idx--;
  else
    psV_c->psDraw_c->obj_idx = psV_c->psObjIni->next->key-1;
  
  v_draw(psV_c);
}


void v_quit(Widget w, XtPointer client_data, XtPointer call_data)
{

  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  free(psV_c->psDraw_c);
  free(psV_c);
  quit();
};


sObject* get_object_by_key(sObject* first, sObject* last, int key)
{
  sObject* act = last;
  do
    {
      sObject* next = act->next;
      if(act->key == key)
	return act;
      act=next;
    }
  while(act != first);

  return NULL;
}


sCommand* get_command_by_key(sCommand* first, sCommand* last, int key)
{
  sCommand* act = last;
  do
    {
      sCommand* next = act->next;
      if(act->key == key)
	return act;
      act=next;
    }
  while(act != first);

  return NULL;
}


sVset* get_setting_by_name(sVset* first, sVset* last, const char* para)
{
  sVset* act = last;
  do
    {
      sVset* next = act->next;
      if (!strcmp(act->para, para))
	{
	  fprintf(stderr,"Found %s, returning %lu\n",act->para, act->value);
	  return act;
	}
      act=next;
    }
  while(act != first);

  return NULL;
}
