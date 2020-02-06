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
  
  psV_c->toplevel = XtVaAppInitialize(&app_context, "Viewer", NULL, 0, &argc, argv, NULL, NULL);

  psV_c->box = XtVaCreateManagedWidget("box", boxWidgetClass, psV_c->toplevel, NULL);

  psV_c->draw_shell=XtVaCreateManagedWidget("draw_shell", coreWidgetClass, psV_c->box, NULL);
  n=0;
  XtSetArg(wargs[n],XtNheight,WIN_Y_SIZE); n++;
  XtSetArg(wargs[n],XtNwidth,WIN_X_SIZE); n++;
  XtSetValues(psV_c->draw_shell, wargs, n);

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

  psV_c->psDraw_c = v_set_vdraw_attributes(psV_c->draw_shell,
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

  XtRealizeWidget(psV_c->toplevel);
  XtAppMainLoop(app_context);
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
  
  XGCValues values;
  GC gc;
  Display *display;
  Drawable window;

  display = XtDisplay(psV_c->draw_shell);
  window = XtWindow(psV_c->draw_shell);

  values.background = 1; 
  values.foreground = 0;
  values.line_width = 2;
    
  gc = XCreateGC(display, window, GCForeground | GCLineWidth | GCBackground, &values);
  
  XClearWindow(display, window);

  sObject* oact = get_object_by_key(psV_c->psDraw_c->curr_obj);

  printf("\n[\n\tObject parameters:\n\tObject name: %s\n", oact->name);

  for(int j=0; j<oact->psComIni->key; j++)
    {
      sCommand* cact = get_command_by_key(oact,j);
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
}


void v_next(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(psV_c->psDraw_c->curr_obj < get_last_object_key()-1)
    psV_c->psDraw_c->curr_obj++;
  else
    psV_c->psDraw_c->curr_obj = 1;
  
  v_draw(psV_c);
}

void v_prev(Widget w, XtPointer client_data, XtPointer call_data)
{
  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  if(psV_c->psDraw_c->curr_obj > 1)
    psV_c->psDraw_c->curr_obj--;
  else
    psV_c->psDraw_c->curr_obj = get_last_object_key()-1;
  
  v_draw(psV_c);
}



void v_quit(Widget w, XtPointer client_data, XtPointer call_data)
{

  sViewer_container *psV_c = (sViewer_container*) client_data;
  
  free(psV_c->psDraw_c);
  free(psV_c);
  quit();
};


sDraw_container *v_set_vdraw_attributes(Widget w, Display *display, Window window)
{
  /*
    XSetWindowAttributes winattrib;
    unsigned long valuemask;
    Visual *v;
    int i,j,class;
  */
  
  sDraw_container *psDraw_c;
  
  if (!(psDraw_c = (sDraw_container*) malloc(sizeof(sDraw_container))))
    return NULL;

  psDraw_c->curr_obj=1;
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

// Global

sObject* get_object_by_key(int key)
{
  sObject* last = psObjIni->next;
  sObject* first = psObjIni;
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

int get_last_object_key()
{
  return psObjIni->next->key;
}

sCommand* get_command_by_key(sObject* psObj, int key)
{
  sCommand* last = psObj->psComIni->next;
  sCommand* first = psObj->psComIni;
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

