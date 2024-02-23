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
GtkWidget *menu, *play, *stop,*restart,*vocation,*waypoints,*clientid,*quit,*dialog;
gint res;

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
  }
  gtk_widget_destroy( dialog );
}

void open_tibia( GtkWidget *widget, gpointer data ) {
  if ( !filesystem::exists( string("/tmp/.X11-unix/X" + to_string( which ) ).c_str( ) ) ) { 
    system( string( "/opt/afk/client.sh " + to_string( which ) ).c_str( ) );
  }
}

void Quit( ) {
  system( string("rm /tmp/afk" + to_string( which )).c_str( ) );
  gtk_main_quit( );
}

void Toggle( ) {
  if ( CHARACTER != "" && WAYPOINTS != "" ) {
    running = !running;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), running ? "Stop" : "Play" );
    if ( running == true ) run();
  }
}

void Restart( ) {
  position = 0;
}

void Interface( int argc, char** argv ) {

  gtk_init( &argc, &argv );

  if ( filesystem::exists( "/tmp/afk3" ) ) {
    which = 4;
  } else if ( filesystem::exists( "/tmp/afk2" ) ) {
    which = 3;
  } else if ( filesystem::exists( "/tmp/afk1" ) ) {
    which = 2;
  }

  TIBIA = string( ":" + to_string( which ) );
  system( string( "touch /tmp/afk" + to_string( which ) ).c_str( ) );
  const char* ico = string("/opt/afk/t" + to_string( which ) + ".png").c_str( );
  
  appindicator = app_indicator_new( "AFK", ico, APP_INDICATOR_CATEGORY_APPLICATION_STATUS );

  menu = gtk_menu_new( );
  play = gtk_menu_item_new_with_label( "Play" );
  restart = gtk_menu_item_new_with_label( "Restart" );
  vocation = gtk_menu_item_new_with_label( "Character" );
  waypoints = gtk_menu_item_new_with_label( "Waypoints" );
  clientid = gtk_menu_item_new_with_label( "Client" );
  quit = gtk_menu_item_new_with_label( "Quit" );
  // submenu

  g_signal_connect( play, "activate", G_CALLBACK( Toggle ), appindicator );
  g_signal_connect( restart, "activate", G_CALLBACK( Restart ), appindicator );
  g_signal_connect( clientid, "activate", G_CALLBACK( open_tibia ), appindicator );
  g_signal_connect( vocation, "activate", G_CALLBACK( open_character ), appindicator );
  g_signal_connect( waypoints, "activate", G_CALLBACK( open_waypoints ), appindicator );
	g_signal_connect( quit, "activate", G_CALLBACK( Quit ), appindicator );

  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), play );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), restart );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), clientid );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), vocation );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), waypoints );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), quit );
  gtk_widget_show_all( menu );
  //gtk_widget_hide( stop );

  app_indicator_set_status( appindicator, APP_INDICATOR_STATUS_ACTIVE );
  app_indicator_set_attention_icon( appindicator, "dialog-warning" );
  app_indicator_set_menu( appindicator, GTK_MENU( menu ) );

  gtk_main( );
}

#endif // GUI_H