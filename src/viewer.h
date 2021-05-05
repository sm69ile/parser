#if !defined viewer_h
#define _viewer_h

#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>

#define V_WIN_X_SIZE 400
#define V_WIN_Y_SIZE 400
#define D_WIN_X_SIZE 600
#define D_WIN_Y_SIZE 600
#define C_WIN_X_SIZE 200
#define C_WIN_Y_SIZE 600
#define MAX_FLOODFILL D_WIN_X_SIZE * D_WIN_Y_SIZE
#define DCB 32513
#define LABEL_HEIGHT 20
#define LABEL_WIDTH 60
#define CHAR_BUF 80

#define O_LABEL_DEFAULT "default"
#define C_LABEL_DEFAULT "Colormap"

#define LABEL_SAVE_COMMAND "Save"
#define LABEL_PREV_OBJECT_COMMAND "< Object"
#define LABEL_NEXT_OBJECT_COMMAND "Object >"
#define LABEL_PREV_STATE_COMMAND "< State"
#define LABEL_NEXT_STATE_COMMAND "State >"
#define LABEL_CURR_COMMAND "Reload"
#define LABEL_QUIT_COMMAND "Quit"


typedef struct Colortable{
  int key;
  char* name;
  Pixel value;
}sColortable;

typedef struct Draw_container{
  int last_x;
  int last_y;
  Colormap colormap;
  sColortable ctable[256];

}sDraw_container;
 
typedef struct Viewer_container{
  XtAppContext app_context;

  Widget toplevel;
  Widget form;
  Widget draw_shell;
  Widget color_shell;
  Widget object_label; 
  Widget color_label;
  Widget save_command;
  Widget prev_object_command;
  Widget next_object_command;
  Widget prev_state_command;
  Widget next_state_command;
  Widget curr_object_command;
  Widget quit_command;

  XtIntervalId vt_h;
  
  sDraw_container *psDraw_c;
  
}sViewer_container;

int fops;
int iDstate_idx;

void v_timer_handler(XtPointer, XtIntervalId *);
void v_event_handler(Widget, XtPointer, XEvent*, Boolean);
void v_event_draw(Widget, XtPointer, XExposeEvent*);
void v_event_color(Widget, XtPointer, XExposeEvent*);

int v_count_resource(char *f);
char **v_load_resource(char *f, int k);
void v_draw(XtPointer);
void v_next_object(Widget, XtPointer, XtPointer);
void v_prev_object(Widget, XtPointer, XtPointer);
void v_update_layout(XtPointer);
void v_set_window_attributes(Display*, Drawable);

void v_prev_state(Widget, XtPointer, XtPointer);
void v_next_state(Widget, XtPointer, XtPointer);
void v_curr_object(Widget, XtPointer, XtPointer);
void v_quit(Widget, XtPointer, XtPointer);
void v_save(Widget, XtPointer, XtPointer);

int v_get_draw_c(sDraw_container*, Widget, Display*, Window);

void v_max_impl(sCommand*, sCommand*, int, Pixel, Display*, Drawable, GC);
void v_floodfill(Display*, Drawable, GC, int, int, unsigned long);
XImage* floodfill(XImage*, int, int, Pixel, Pixel, int, int);
Pixel get_color_by_name(Display*, Colormap, char*);

#endif
