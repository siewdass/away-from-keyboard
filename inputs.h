#ifndef INPUTS_H
#define INPUTS_H
#include <string>

class Inputs {
   public:
      void mouse( int, int, int );
      void mouseMove( int, int );
      void mouseDrag( int, int );
      void keyboard( std::string );
      void keywith( const char* );
      void shiftDown( );
      void shiftUp( );
};

#endif // INPUTS_H