//-----------------------------------------------------------------------+
// File         :  cbi_compose_bunch_pattern.c                           |
//                                                                       |
// Description  :  Primary routine (BP_compose()) and support functions  |
//                 to allow the user to easily specify arbitrary bunch   |
//                 patterns using a GUI.                                 |
//                                                                       |
// Arguments    :  Pointer to temporary instrument structure tree        |
//                   CBI_CTL_MODULE * ptr                                |
//                 Integer denoting the bunch spacing (timing) mode for  |
//                   the currently active set of instruments.            |
//                   int timing_mode                                     |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include <gtk/gtk.h>
#include "cbi_core_includes.h"


#define m4ns_NUM_BUTTONS            640
#define m4ns_BUTTONS_PER_ROW         30

#define m14ns_NUM_BUTTONS           183
#define m14ns_BUTTONS_PER_ROW        20

#define NUM_LABEL_ROWS      2


static int selection_active[CBI_MAX_BUNCHES];

static int mouse_down    = 0;
static int mouse_entered = 0;

static CBI_CTL_MODULE *mod;
static int *pattern_array_ptr;

int tmode;
int pattern_shift = 0;


#define MAX_CATEGORIES                20 
char categories[MAX_CATEGORIES][30];


typedef struct {
  GtkWidget  *combobox_ptr;
  GtkWidget  *bp_name_entry_ptr;
  GtkWidget  *statusbar_ptr;
} MULTIWIDGET;


//------------------------------
// Mouse button press callback
//------------------------------
static void press_callback( GtkWidget *widget,
			    GdkEventButton *event,
			    gpointer   data )
{
 
  GdkColor    color;
  int num_selected = (int)data;
  int state_index = num_selected - 1;

  int bunch;
  if (tmode == m14ns) {
    bunch = (num_selected * 2)-1;
  } else {
    bunch = num_selected;
  }

  mouse_down = 1;
  
  if( selection_active[state_index] == 0 ) {
    selection_active[state_index] = 1;
    //printf("Selection %d ENABLED\n", num_selected);
    cbi_set_bunch( pattern_array_ptr, bunch+pattern_shift );
    gdk_color_parse("red", &color);
  } else {
    selection_active[state_index] = 0;
    //g_print ("Selection %d disabled\n", num_selected);
    cbi_unset_bunch( pattern_array_ptr, bunch+pattern_shift );
    gdk_color_parse("grey90", &color);
  }

  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);

  //printf("Button pressed.  (mouse_down = %d)\n", mouse_down);
  gdk_pointer_ungrab(0);
 
}



//----------------------------------
// Mouse pointer enter box callback
//----------------------------------
static void enter_callback( GtkWidget *widget,
			    GdkEventButton *event,
			    gpointer   data )
{
 
  mouse_entered = 1;

  GdkColor    color;
  int num_selected = (int)data;
  int state_index = num_selected - 1;

  int bunch;
  if (tmode == m14ns) {
    bunch = (num_selected * 2)-1;
  } else {
    bunch = num_selected;
  }

  if (mouse_down == 1) {
    if( selection_active[state_index] == 0 ) {
      selection_active[state_index] = 1;
      //printf("Selection %d ENABLED\n", num_selected);
      cbi_set_bunch( pattern_array_ptr, bunch+pattern_shift );
      gdk_color_parse("red", &color);
    } else {
      selection_active[state_index] = 0;
      //g_print ("Selection %d disabled\n", num_selected);
      cbi_unset_bunch( pattern_array_ptr, bunch+pattern_shift );
      gdk_color_parse("grey90", &color);
    }
    
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
  }
  
}



//----------------------------------
// Mouse pointer leave callback
//----------------------------------
static void leave_callback( GtkWidget *widget,
			    GdkEventButton *event,
			    gpointer   data )
{
  mouse_entered = 0;
}



//----------------------------------
// Mouse button release callback
//----------------------------------
static void release_callback( GtkWidget *widget,
			      GdkEventButton *event,
			      gpointer   data )
{
  mouse_down = 0;
  //printf("Button released.  (mouse_down = %d)\n", mouse_down);

}


//------------------------------------
// Callback used to change a
// selection box to the 'deactivated'
// visual state.
//------------------------------------
static void deactivate_callback( GtkWidget *widget,
				 GdkEventButton *event,
				 gpointer   data )
{
  GdkColor    color;

  gdk_color_parse("grey90", &color);
    
  gtk_widget_modify_bg( widget, GTK_STATE_NORMAL, &color );

}


//----------------------------------
// Mouse button press callback
//----------------------------------
static void clear_all_callback( GtkWidget *widget,
				gpointer   data )
{ 
  int i;
  int bunch;

  gtk_container_foreach( GTK_CONTAINER(data), (gpointer)deactivate_callback, NULL);
  for (i = 0; i < CBI_MAX_BUNCHES; i++) {
    selection_active[i] = 0;
    bunch = (i-1);
    cbi_unset_bunch( pattern_array_ptr, bunch+pattern_shift );
  }

}


//----------------------------------
// Save button callback
//----------------------------------
static void save_pattern_callback( GtkWidget  *widget,
				   gpointer   data )
{ 
  int status, active_item, entry_number;
  char temp_BP_name[60];

  // Get a pointer to the structure of widgets passed in to this function.
  MULTIWIDGET *mwidget = (MULTIWIDGET*)data;

  active_item = gtk_combo_box_get_active( (gpointer)mwidget->combobox_ptr );

  entry_number = cbi_new_pattern_number( categories[active_item] );


  // Replace all space characters with underscores to standardize and allow
  // for present limitations in faccess() string handling.  TODO: refine
  strcpy(temp_BP_name, "");
  strcpy(temp_BP_name, gtk_entry_get_text( (gpointer)mwidget->bp_name_entry_ptr ) );
  replace(temp_BP_name, ' ', '_');


  // 'categories[active_item] approach used here because GTK+2.4 (what is
  // presently installed on our systems) does not have available the 
  // gtk_combo_box_get_active_text method.  (2.6 does)
  status = cbi_save_bunch_pattern( pattern_array_ptr, 
				   categories[active_item],
				   temp_BP_name,
				   entry_number );

  if (status == 100) {
    gtk_statusbar_push( (gpointer)mwidget->statusbar_ptr, 0, "Pattern NOT Saved!  One with the same name already exists in the given category.");
  }
  if (status == CBI_F_FAILURE) {
    printf("FAILED!\n");
    gtk_statusbar_push( (gpointer)mwidget->statusbar_ptr, 0, "Save failed! (file I/O problem)");
  }
  if (status == CBI_F_SUCCESS) {
    printf("SUCCESS!\n");
    gtk_statusbar_push( (gpointer)mwidget->statusbar_ptr, 0, "Pattern Saved");
  }

}



//----------------------------------
// Save button callback
//----------------------------------
static void set_spsecies_callback( GtkWidget  *widget,
				   gpointer   data )
{

  if ( (int)data == POSITRONS ) {
    pattern_shift = 0;
  }
  if ( (int)data == ELECTRONS ) {
    pattern_shift = 1;
  }

}
  




//----------------------------------
// Callback to allow graceful
// destruction of a window.
//----------------------------------
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    return FALSE;
}







//int cbi_compose_bunch_pattern( CBI_CTL_MODULE * ptr, int timing_mode ) {
int cbi_compose_bunch_pattern( int pattern_array[], int timing_mode ) {


  printf("\n\n\n\nTIMING MODE = %d \n\n\n\n", timing_mode);

  int gui_capable = TRUE;

  if (!gui_capable) {
    printf("Your terminal does not support the drawing of a graphical user interface.\n");
    printf("A possible cause for this is simply a missing '-X' option in the ssh command\n");
    printf("used to connect to the host where this application is running.\n\n");
    printf("Please check that your windowing environment (DISPLAY environment variable, etc.)\n");
    printf("is set up correctly and try again.\n\n");
  }

  tmode = timing_mode;

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *event_box;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *vbox1, *statusbar;
  GSList    *group;

  GtkRcStyle  *rc_style;
  GdkColor    color;

  int i;
  for (i = 0; i < CBI_MAX_BUNCHES; i++) {
    selection_active[i] = 0;
  }


  pattern_array_ptr = pattern_array;
  

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  char pattern_spacings[5][CBI_MAX_TIMING_MODES] =
    {"4ns", "14ns"};

  int num_buttons[CBI_MAX_TIMING_MODES] =
    { m4ns_NUM_BUTTONS,
      m14ns_NUM_BUTTONS };

  int buttons_per_row[CBI_MAX_TIMING_MODES] =
    { m4ns_BUTTONS_PER_ROW,
      m14ns_BUTTONS_PER_ROW };
  
  
  
  char text[50] = {""};
  sprintf(text, "Compose %s Bunch Pattern", pattern_spacings[timing_mode]);
  gtk_window_set_title(GTK_WINDOW(window), text);



  // Set a handler for delete_event that immediately
  // halts the GTK event loop.
  g_signal_connect (G_OBJECT (window), "delete_event",
		    G_CALLBACK (delete_event), NULL);



  /* Sets the border width of the window. */
  gtk_container_set_border_width (GTK_CONTAINER (window), 3);
  

  // Create a VBOX for packing other widgets
  vbox1 = gtk_vbox_new(FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox1);

  /* Create a table */
  table = gtk_table_new (14, buttons_per_row[timing_mode], TRUE);
  
  /* Put the table in the main window */
  //gtk_container_add (GTK_CONTAINER (window), table);
  gtk_container_add(GTK_CONTAINER(vbox1), table);

  char lbl[80];

  
  // Create label for displaying instructions
  label = gtk_label_new("Click below to toggle bunches for measurement.    Click and drag to toggle collections of bunches.");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, buttons_per_row[timing_mode], 0, 1);
  gtk_widget_show(label);


  
  //------------------------------
  /* Create grid of event boxes */
  //------------------------------
  int row = 0;
  int col = 0;
  int bunch;
  
  for (i = 0; i < num_buttons[timing_mode]; i++) {

    bunch = i + 1;
    event_box = gtk_event_box_new();
    
    strcpy(lbl, "");
    sprintf(lbl, "%d", i+1);
    label = gtk_label_new(lbl);
    gtk_container_add(GTK_CONTAINER (event_box), label);
    gtk_widget_show(label);
    
    
    gtk_widget_set_events(event_box, 
			  GDK_BUTTON_PRESS_MASK & 
			  GDK_ENTER_NOTIFY_MASK & 
			  GDK_LEAVE_NOTIFY_MASK &
			  GDK_BUTTON_RELEASE_MASK );


    // Attach callbacks to signals we are interested in processing.
    g_signal_connect (G_OBJECT (event_box), "button_press_event",
    		      G_CALLBACK (press_callback), (gpointer) bunch);
    
    g_signal_connect (G_OBJECT (event_box), "enter_notify_event",
    	      G_CALLBACK (enter_callback), (gpointer) bunch);

    g_signal_connect (G_OBJECT (event_box), "leave_notify_event",
		      G_CALLBACK (leave_callback), (gpointer) bunch);

    g_signal_connect (G_OBJECT (event_box), "button_release_event",
		      G_CALLBACK (release_callback), (gpointer) bunch);

    row = (int)(i / buttons_per_row[timing_mode]);
    col = i % buttons_per_row[timing_mode];
    gtk_table_attach_defaults (GTK_TABLE (table), event_box, col, col+1, row+NUM_LABEL_ROWS, row+NUM_LABEL_ROWS+1);
    gtk_widget_realize(event_box);
    
    gtk_widget_show (event_box);
  }
  
 

  //----------------------------------------------------------
  // Create and display the statusbar
  //----------------------------------------------------------
  statusbar = gtk_statusbar_new();
  gtk_container_add(GTK_CONTAINER(vbox1), statusbar);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, "Bunch pattern has not been saved.");
  gtk_widget_show(statusbar);
  gtk_widget_show(vbox1);


  //----------------------------------------------------------
  // Create and populate the drop-down list of bunch pattern 
  // categories.
  //----------------------------------------------------------
  label = gtk_label_new("Category: ");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 3, row+1+NUM_LABEL_ROWS, row+2+NUM_LABEL_ROWS);
  gtk_widget_show(label);

  GtkWidget *combobox, *menu_item;
  GList     *items  =  NULL;

  FILE *fp;

  combobox = gtk_combo_box_entry_new_text();


  fp = fopen( appconfig.bunpat_def_file, "r" );
  int retcode = 0;
  int count = 0;
  char incoming_string[50] = {""};
  while( retcode == 0 ) {
    strcpy( incoming_string, "" );
    retcode = faccess( READ, fp, NULL, NULL, 1, 1, STRING, incoming_string );
    if ( strcmp(incoming_string, "") == 0 ) {
      break;
    }
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox), incoming_string);
    strcpy( categories[count], incoming_string );
    printf("%2d)  %s\n", count+1, incoming_string);
    count++; 
  }


  gtk_table_attach_defaults (GTK_TABLE (table), combobox, 3, 9, row+1+NUM_LABEL_ROWS, row+2+NUM_LABEL_ROWS);

  gtk_combo_box_set_active( (gpointer)combobox, 0);
  gtk_widget_show (combobox);


  //-----------------------------------------------------------
  // Create the Name entry field for the bunch pattern to save
  //-----------------------------------------------------------
  label = gtk_label_new("Pattern Name: ");
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 3, row+2+NUM_LABEL_ROWS, row+3+NUM_LABEL_ROWS);
  gtk_widget_show(label);

  GtkWidget  *bp_name_entry;
  bp_name_entry = gtk_entry_new();
  gtk_table_attach_defaults (GTK_TABLE (table), bp_name_entry, 3, 9, row+2+NUM_LABEL_ROWS, row+3+NUM_LABEL_ROWS);
  gtk_widget_show(bp_name_entry);


  //-----------------------
  // Create "Save" button
  //-----------------------
  button = gtk_button_new_with_label ("Save");
  
  MULTIWIDGET two_widgets;

  two_widgets.combobox_ptr      = combobox;
  two_widgets.bp_name_entry_ptr = bp_name_entry;
  two_widgets.statusbar_ptr     = statusbar;


  g_signal_connect (G_OBJECT (button), "clicked",
  		    G_CALLBACK (save_pattern_callback), (gpointer)&two_widgets );

  // Insert the button into the table
  gtk_table_attach_defaults( GTK_TABLE (table), button, 9, 10, row+2+NUM_LABEL_ROWS, row+3+NUM_LABEL_ROWS);
  
  gtk_button_set_relief( (gpointer)button, GTK_RELIEF_HALF);
  gtk_button_set_focus_on_click( (gpointer)button, 0);

  gtk_widget_show (button);


  //----------------------------------------
  // Create species selection radio buttons
  //----------------------------------------
  button = gtk_radio_button_new_with_label (NULL, "(e+)");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (clear_all_callback), (gpointer) table );

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (set_spsecies_callback), (gpointer)0 );

  gtk_table_attach_defaults( GTK_TABLE (table), button, 11, 13, row+1+NUM_LABEL_ROWS, row+2+NUM_LABEL_ROWS);

  if (tmode == m14ns) {
    gtk_widget_show (button);
  } 


  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
  button = gtk_radio_button_new_with_label (group, "(e-)");

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (clear_all_callback), (gpointer) table );

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (set_spsecies_callback), (gpointer)1 );

  gtk_table_attach_defaults( GTK_TABLE (table), button, 11, 13, row+2+NUM_LABEL_ROWS, row+3+NUM_LABEL_ROWS);

  if (tmode == m14ns) {
    gtk_widget_show (button);
  } 


  //-----------------------
  // Create "Clear" button
  //-----------------------
  button = gtk_button_new_with_label ("Clear All");
  
  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (clear_all_callback), (gpointer) table );
  
  // Insert the button into the table
  gtk_table_attach_defaults (GTK_TABLE (table), button, 13, 15, row+1+NUM_LABEL_ROWS, row+2+NUM_LABEL_ROWS);
  
  gtk_button_set_relief( (gpointer)button, GTK_RELIEF_HALF);
  gtk_button_set_focus_on_click( (gpointer)button, 0);

  gtk_widget_show (button);




  //-----------------------
  // Create "Quit" button 
  //-----------------------
  button = gtk_button_new_with_label ("Close");
  

  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));


  g_signal_connect_swapped (G_OBJECT(button), "clicked",
			    G_CALLBACK(gtk_main_quit), NULL);
  
  // Insert the quit button into the table
  gtk_table_attach_defaults (GTK_TABLE (table), button, 18, 19, row+1+NUM_LABEL_ROWS, row+2+NUM_LABEL_ROWS);
  

  gtk_button_set_relief( (gpointer)button, GTK_RELIEF_HALF);
  gtk_button_set_focus_on_click( (gpointer)button, 0);

  gtk_widget_show (button);
  
  
  gtk_widget_show(table);
  gtk_widget_show(window);



  // Signal handlers for the window decoration destroy button. "X"
  g_signal_connect_swapped(G_OBJECT(window), "destroy",
			   G_CALLBACK(delete_event), NULL);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
  			   G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();


  return CBI_F_SUCCESS;

}
