#if !defined viewer_h
#define _viewer_h

#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>

/*
#define GCFG "GCForegound"
#define GCBG "GCBackground"
#define GCLW "GCLineWidth"
*/

#define D_WIN_X_SIZE 400
#define D_WIN_Y_SIZE 400
#define C_WIN_X_SIZE 200
#define C_WIN_Y_SIZE 400
#define MAX_FLOODFILL 100000

#define LABEL_HEIGHT 20
#define LABEL_WIDTH 60

#define O_LABEL_DEFAULT "default"
#define C_LABEL_DEFAULT "Colormap"

#define LABEL_NEXT_COMMAND "Next"
#define LABEL_PREV_COMMAND "Prev"
#define LABEL_STATE_COMMAND "State +"
#define LABEL_QUIT_COMMAND "Close"

typedef struct Colortable{
  int key;
  char* name;
  Pixel value;
}sColortable;

typedef struct Draw_container{
  int obj_idx;
  int last_x;
  int last_y;
  Colormap colormap;
  sColortable ctable[256];

}sDraw_container;
  
typedef struct Viewer_container{
  Widget toplevel;
  Widget box;
  Widget draw_shell;
  Widget color_shell;
  Widget object_label; 
  Widget color_label;
  Widget next_command;
  Widget prev_command;
  Widget state_command;
  Widget quit_command;
  
  sDraw_container *psDraw_c;
  sObject *psObjIni;
  
  /*
  Widget curr_command;
  Widget state_command;
  Widget rescan_command;
  Widget save_command;
  */

}sViewer_container;

extern void quit();
extern void olist();
extern int iState;

void v_ev_draw(Widget, XtPointer, XExposeEvent*);
void v_ev_color(Widget, XtPointer, XExposeEvent*);
void v_draw(XtPointer);
void v_next(Widget, XtPointer, XtPointer);
void v_prev(Widget, XtPointer, XtPointer);
void v_update_layout(XtPointer); 
void v_state(Widget, XtPointer, XtPointer);
void v_quit(Widget, XtPointer, XtPointer);

void v_get_draw_c(sDraw_container*, Widget, Display*, Window);

void v_max_impl(sCommand*, sCommand*, int, Pixel, Display*, Drawable, GC);
void v_floodfill(Display*, Drawable, GC, int, int, unsigned long);
XImage* floodfill(XImage*, int, int, Pixel, Pixel, int, int);

unsigned long get_color_by_name(Display*, Colormap, char*);
sObject *get_object_by_key(sObject*, sObject*, int);
sCommand *get_command_by_key(sCommand*, sCommand*, int);
sVset *get_setting_by_name(sVset*, sVset*, const char*);

#endif
