#ifndef GUI_H
#define GUI_H

#pragma once

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <filesystem>
#include "global.h"

typedef void ( *Function )( );
bool Pause = false;
bool running = true;
bool Stop = false;

AppIndicator* appindicator;
GtkWidget *menu, *play, *stop,*restart,*vocation,*waypoints,*clientid,*quit,*dialog;
Function func;
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

void choiceClient( GtkWidget *widget, gpointer data ) {
  const int MAX_BUFFER = 2048;
  char buffer[ MAX_BUFFER ];
  FILE *stream = popen( "./window" , "r" );
  if ( stream ) {
    while ( !feof( stream ) ) {
      if ( fgets( buffer, MAX_BUFFER, stream ) != NULL) { }
    }
    pclose( stream );
  }
  string v{ buffer };
  v.pop_back( );
  console.log(v);
}

void Quit( ) {
  gtk_main_quit( );
}

void Toggle( ) {
  if ( !Pause ) {
    Pause = true;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), "Stop" );
  } else {
    Pause = false;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), "Play" );
  }
}

void Restart( ) {
  position = 0;
}

void Interface( int argc, char** argv, Function callback ) {

  gtk_init( &argc, &argv );
  int n = 0;

  bool Bot1 = std::filesystem::exists( "/tmp/afk1" );
  bool Bot2 = std::filesystem::exists( "/tmp/afk2" );
  bool Bot3 = std::filesystem::exists( "/tmp/afk3" );
  bool Bot4 = std::filesystem::exists( "/tmp/afk4" );

  const char* ico = "";

  if ( Bot4 ) {
    ico = "/opt/afk/t4.png";
  } else if ( Bot3 ) {
    ico = "/opt/afk/t3.png";
  } else if ( Bot2 ) {
    ico = "/opt/afk/t2.png";
  } else {
    ico = "/opt/afk/t1.png";
  }

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
  g_signal_connect( clientid, "activate", G_CALLBACK( choiceClient ), appindicator );
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

  //callback( );

  gtk_main( );
}

#endif // GUI_H