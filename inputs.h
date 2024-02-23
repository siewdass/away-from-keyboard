#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <tuple>

using namespace std;

class Inputs {
public:
   void load( const char* );
   void mouse( int, int, int );
   void mouseMove( int, int );
   void mouseDrag( int, int );
   tuple < int, int > mouseLocation( );
   void keyboard( std::string );
   void keywith( const char* );
   void shiftDown( );
   void shiftUp( );
   ~Inputs( );
};

#endif // INPUTS_H