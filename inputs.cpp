#include "inputs.h"
extern "C" {
  #include <xdo.h>
}
#include <iostream>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


xdo_t * xdo = xdo_new( NULL );

void Inputs::mouse( int x, int y, int button ) {
  xdo_move_mouse( xdo, x, y, CURRENTWINDOW );
  xdo_mouse_down( xdo, CURRENTWINDOW, button );
  xdo_mouse_up( xdo, CURRENTWINDOW, button );
  //usleep( 500 );
  //xdo_move_mouse( xdo, 50, 500, CURRENTWINDOW );
}

void Inputs::mouseMove( int x, int y ) {
  xdo_move_mouse( xdo, x, y, CURRENTWINDOW );
}

void Inputs::mouseDrag( int x, int y ) {
  xdo_move_mouse( xdo, x, y, CURRENTWINDOW );
  xdo_mouse_down( xdo, CURRENTWINDOW, 1 );
  xdo_move_mouse( xdo, 497, 260, CURRENTWINDOW );
  xdo_mouse_up( xdo, CURRENTWINDOW, 1 );
}

tuple < int, int > Inputs::mouseLocation( ) {
  int X, Y, SCREEN;
  xdo_get_mouse_location( xdo, &X, &Y, &SCREEN );
  return make_tuple( X, Y );
}

void Inputs::keyboard( std::string key ) {
  //std::cout << key << std::endl;
  xdo_send_keysequence_window_down( xdo, CURRENTWINDOW, key.c_str( ), 0 );
  xdo_send_keysequence_window_up( xdo, CURRENTWINDOW, key.c_str( ), 0 );
}

void Inputs::keywith( const char* key ) {
  xdo_send_keysequence_window( xdo, CURRENTWINDOW, key, 12000 );
}

void Inputs::shiftDown( ) {
  xdo_send_keysequence_window_down( xdo, CURRENTWINDOW, "shift", 0 );
}

void Inputs::shiftUp ( ) {
  xdo_send_keysequence_window_up( xdo, CURRENTWINDOW, "shift", 0 );
}