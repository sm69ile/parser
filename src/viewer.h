#if !defined viewer_h
#define _viewer_h

#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>


#define WIN_X_SIZE 400
#define WIN_Y_SIZE 400
#define LABEL_HEIGHT 20
#define LABEL_WIDTH 60
#define LABEL_QUIT_COMMAND "Quit"
#define LABEL_NEXT_COMMAND "Next"
#define LABEL_PREV_COMMAND "Prev"

typedef struct Draw_container{
  Colormap colormap;
  int curr_obj;
  int last_x;
  int last_y;

}sDraw_container;
  
typedef struct Viewer_container{
  Widget toplevel;
  Widget box;
  Widget draw_shell;
  Widget quit_command;
  Widget next_command;
  Widget prev_command;

  sDraw_container *psDraw_c;



  //  sObject *psObj_c;
  
  /*
  Widget curr_object_label;
  Widget label;

  Widget curr_command;


  Widget state_command;
  Widget rescan_command;
  Widget save_command;

  */

}sViewer_container;


extern void quit();
extern void olist();

sDraw_container *v_set_vdraw_attributes(Widget w, Display *display, Window window);

void v_ev_draw(Widget, XtPointer, XExposeEvent*);
void v_draw(XtPointer);
void v_next(Widget, XtPointer, XtPointer);
void v_prev(Widget, XtPointer, XtPointer);
void v_quit(Widget, XtPointer, XtPointer);

sDraw_container *v_set_vdraw_attributes(Widget w, Display *display, Window window);

//Global
sObject *get_object_by_key(int);
sCommand *get_command_by_key(sObject*, int);
int get_last_object_key();
  
#endif
