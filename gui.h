#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

bool running = false;

AppIndicator* appindicator;
GtkWidget* menu;
GtkWidget* play;
GtkWidget* stop;
GtkWidget* quit;

void Quit( ) {
  gtk_main_quit( );
}

void Toggle( ) {
  if ( !running ) {
    running = true;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), "Stop" );
  } else {
    running = false;
    gtk_menu_item_set_label( GTK_MENU_ITEM( play ), "Play" );
  }
}

void Interface( int argc, char** argv, void ( *callback )() ) {

  gtk_init( &argc, &argv );

  appindicator = app_indicator_new( "AFK", "/opt/tibia/tibia.ico", APP_INDICATOR_CATEGORY_APPLICATION_STATUS );

  menu = gtk_menu_new( );
  play = gtk_menu_item_new_with_label( "Play" );
  quit = gtk_menu_item_new_with_label( "Quit" );

  g_signal_connect( play, "activate", G_CALLBACK( Toggle ), appindicator );
	g_signal_connect( quit, "activate", G_CALLBACK( Quit ), appindicator );

  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), play );
  gtk_menu_shell_append( GTK_MENU_SHELL( menu ), quit );
  gtk_widget_show_all( menu );
  //gtk_widget_hide( stop );

  app_indicator_set_status( appindicator, APP_INDICATOR_STATUS_ACTIVE );
  app_indicator_set_attention_icon( appindicator, "dialog-warning" );
  app_indicator_set_menu( appindicator, GTK_MENU( menu ) );

  callback( );

  gtk_main( );
}

#endif // GUI_H