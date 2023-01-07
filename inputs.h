#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <tuple>

using namespace std;

class Inputs {
public:
   int id;
   void mouse( int, int, int );
   void mouseMove( int, int );
   void mouseDrag( int, int );
   tuple < int, int > mouseLocation( );
   void keyboard( std::string );
   void keywith( const char* );
   void shiftDown( );
   void shiftUp( );
};

#endif // INPUTS_H