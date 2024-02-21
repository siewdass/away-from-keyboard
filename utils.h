#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <thread>
#include <jsoncpp/json/json.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace Jsoncpp = Json;
#define Json _Json
#define time _time
#define Array std::vector

using namespace std;

class Time {
	public:
		void sleep( int ms );
		double clock( );
};

class {
public:
	template < typename ...Args >
	void log( Args... args ) {
		( ( cout << args << " " ), ... );
		cout << endl;
	}
} console;

class Json {
	public:

	Jsoncpp::Reader reader; 
	Jsoncpp::Value data; 
	const char* last;

	std::string str;
	string type;
    
	void open( const char* f ) {
		std::ifstream file ( f );
		if ( file.is_open( ) ) {
			string txt;
			while ( getline ( file, txt ) ) {
				str += txt;
			}
		}
		file.close( );
		reader.parse( str, data );
	}
		
	void save( const char* f ) {
		Jsoncpp::StyledWriter writer;
		string to = writer.write( data );
		ofstream save;
		save.open( f );
		save << to;
		save.close( );
	}

	int readInteger( ) {
		return data.asInt( );
	}
   
	bool readBoolean( ) {
		return data.asBool( );
	}
     
	string readString( ) {
		return data.asString( );
	}

	int readInteger( const char* key ) {
		return data[ key ].asInt( );
	}

	bool readBoolean( const char* key ) {
		return data[ key ].asBool( );
	}
        
	string readString( const char* key ) {
		return data[ key ].asString( );
	}
    
	Json readObject( const char* key ) {
		Json tmp;
		tmp.data = data[ key ];
		return tmp;
	}

	vector < Json > readArray( const char* key ) {
		vector < Json > vtmp;
    	
		Json tmp;
		tmp.data = data[ key ];
    	
		for ( int i = 0; i < tmp.data.size( ); i++ ) { 
			Json t;
			t.data = tmp.data[ i ];
			vtmp.push_back( t );
		}
			
		return vtmp;
	}

	vector < Json > readArray( ) {
		vector < Json > vtmp;

		for ( int i = 0; i < data.size( ); i++ ) {
			Json tmp;
			tmp.data = data[ i ];
			vtmp.push_back( tmp );
		}
			
		return vtmp;
	}

	void writeInteger( const char* key, int value ) {
		last = key;
		type = "integer";
		data[ key ] = value;
	}
    
	void writeBoolean( const char* key, bool value ) {
		last = key;
		type = "boolean";
		data[ key ] = value;
	}
		
	void writeString( const char* key, const char* value ) {
		last = key;
		type = "string";
		data[ key ] = value;
	}
		
	void writeObject( const char* key, Json value ) {
		Jsoncpp::Value item; 
		if ( value.type == "integer" ) {	
			int v = value.readInteger( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		} else if ( value.type == "boolean" ) {
			bool v = value.readBoolean( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		} else if ( value.type == "string" ) {
			string v = value.readString( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		}
	}
		
	void writeArray( const char* key, Json value ) {
		Jsoncpp::Value item; 
		if ( value.type == "integer" ) {
			int v = value.readInteger( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		} else if ( value.type == "boolean" ) {
			bool v = value.readBoolean( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		} else if ( value.type == "string" ) {
			string v = value.readString( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		}
	}

	void writeArray( const char* key, int value ) {
		data[ key ].append( value );
	}

	void writeArray( const char* key, bool value ) {
		data[ key ].append( value );
	}
	
	void writeArray( const char* key, const char* value ) {
		string tmp = value;
		data[ key ].append( tmp );
	}
};
#endif // UTILS_H
