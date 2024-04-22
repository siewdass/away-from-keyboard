#ifndef GUI_H
#define GUI_H

#pragma once

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <filesystem>
#include "global.h"

bool Pause = false;
bool Stop = false;
int which = 1;

AppIndicator* appindicator;
GtkWidget *menu, *play, *stop, *restart, *vocation, *waypoints, *clientid, *quit, *dialog, *xyz;
gint res;

gboolean update_label( gpointer data ) {
  GtkWidget *label_child = gtk_bin_get_child(GTK_BIN(xyz));
  gtk_label_set_text(GTK_LABEL(label_child), g_strdup_printf("X: %d | Y: %d | Z: %d", values.x, values.y, values.z));
  return TRUE;
}

void open_character( GtkWidget* widget, gpointer data ) {
  dialog = gtk_file_chooser_dialog_new( "Choose Character json file.", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL );
  res = gtk_dialog_run( GTK_DIALOG( dialog ) );
  if ( res == GTK_RESPONSE_ACCEPT ) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER( dialog );
    CHARACTER = string( gtk_file_chooser_get_filename( chooser ) );
  }
  gtk_widget_destroy( dialog );
}

void open_waypoints( GtkWidget* widget, gpointer data ) {
  dialog = gtk_file_chooser_dialog_new( "Choose Waypoint json file.", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL );
  res = gtk_dialog_run( GTK_DIALOG( dialog ) );
  if ( res == GTK_RESPONSE_ACCEPT ) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER( dialog );
    WAYPOINTS = string( gtk_file_chooser_get_filename( chooser ) );
    //console.log( WAYPOINTS );
    position = 0;
  }
  gtk_widget_destroy( dialog );
}

void open_tibia( GtkWidget *widget, gpointer data ) {
  if ( !filesystem::exists( string("/tmp/.X11-unix/X" + to_string( which ) ).c_str( ) ) ) { 
    system( string( "/opt/afk/client.sh " + to_string( which ) ).c_str( ) );
  }
}

void Quit( ) {
  gtk_main_quit( );
}

void Toggle( ) {
  if ( CHARACTER != "" && WAYPOINTS != "" && loaded ) {
    running = !running;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), running ? "Stop" : "Play" );
    if ( running == true ) {
      Stop = false;
      run();
    }
  }
}

void Restart( ) {
  position = 0;
}

void Clipboard( ) {
  system( string( "echo '{ \"x\": "+ to_string( values.x ) +", \"y\": "+ to_string( values.y ) +", \"z\": "+ to_string( values.z ) +", \"action\": \"\", \"optional\": null }' | xclip -selection clipboard" ).c_str() );
}

void Interface( int argc, char** argv ) {

  gtk_init( &argc, &argv );

  //#include <sys/prctl.h>
  //prctl(PR_SET_NAME, "afk1");

	string count = client.process( "ps aux | grep afk | wc -l" );

  if ( count == "6" ) {
    which = 4;
  } else if ( count == "5" ) {
    which = 3;
  } else if ( count == "4" ) {
    which = 2;
  }

  TIBIA = string( ":" + to_string( which ) );
  const char* ico = string( "/opt/afk/t" + to_string( which ) + ".png" ).c_str( );
  
  appindicator = app_indicator_new( "AFK", ico, APP_INDICATOR_CATEGORY_APPLICATION_STATUS );

  menu = gtk_menu_new( );
  play = gtk_menu_item_new_with_label( "Play" );
  restart = gtk_menu_item_new_with_label( "Restart" );
  clientid = gtk_menu_item_new_with_label( "Client" );
  vocation = gtk_menu_item_new_with_label( "Character" );
  waypoints = gtk_menu_item_new_with_label( "Waypoints" );
  xyz = gtk_menu_item_new_with_label( "X: 0 | Y: 0 | Z: 0" );
  quit = gtk_menu_item_new_with_label( "Quit" );

  g_signal_connect( play, "activate", G_CALLBACK( Toggle ), appindicator );
  g_signal_connect( restart, "activate", G_CALLBACK( Restart ), appindicator );
  g_signal_connect( clientid, "activate", G_CALLBACK( open_tibia ), appindicator );
  g_signal_connect( vocation, "activate", G_CALLBACK( open_character ), appindicator );
  g_signal_connect( waypoints, "activate", G_CALLBACK( open_waypoints ), appindicator );
  g_signal_connect( xyz, "activate", G_CALLBACK( Clipboard ), appindicator );
	g_signal_connect( quit, "activate", G_CALLBACK( Quit ), appindicator );

  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), play );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), restart );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), clientid );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), vocation );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), waypoints );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), xyz );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), quit );
  gtk_widget_show_all( menu );

  app_indicator_set_status( appindicator, APP_INDICATOR_STATUS_ACTIVE );
  app_indicator_set_attention_icon( appindicator, "dialog-warning" );
  app_indicator_set_menu( appindicator, GTK_MENU( menu ) );

  g_timeout_add_seconds(1, update_label, NULL);

  gtk_main( );
}

#endif