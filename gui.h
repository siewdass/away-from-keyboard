#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <filesystem>
#include "global.h"

typedef void ( *Function )( );
bool Pause = false;
bool running = true;
bool Stop = false;

AppIndicator* appindicator;
GtkWidget *menu;
GtkWidget* play;
GtkWidget* stop;
GtkWidget* restart;
GtkWidget* quit;
Function func;

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

  func = callback;

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
  quit = gtk_menu_item_new_with_label( "Quit" );

  g_signal_connect( play, "activate", G_CALLBACK( Toggle ), appindicator );
  g_signal_connect( restart, "activate", G_CALLBACK( Restart ), appindicator );
	g_signal_connect( quit, "activate", G_CALLBACK( Quit ), appindicator );

  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), play );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), restart );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), quit );
  gtk_widget_show_all( menu );
  //gtk_widget_hide( stop );

  app_indicator_set_status( appindicator, APP_INDICATOR_STATUS_ACTIVE );
  app_indicator_set_attention_icon( appindicator, "dialog-warning" );
  app_indicator_set_menu( appindicator, GTK_MENU( menu ) );

  func( );

  gtk_main( );
}

#endif // GUI_H