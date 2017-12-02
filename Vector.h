#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#define STR_LEN 100
#define PI 3.141526

typedef unsigned char uint8;

inline void swap( float& a, float& b ) {
	float tmp = a; 
	a=b; 
	b=tmp; 
};

inline float radians( float deg ) {
    return deg*PI/180.0f;
}

//////////////////////////////////////////////////

/** VEC2 */
struct vec2 {
    static const uint8 size = 2;
    
	float vec[size];

	vec2() {
		memset( vec, 0, sizeof(vec) );	
	}

	vec2( float x, float y ) { 
		vec[0] = x;	
		vec[1] = y;
	}
	
	inline void operator= ( const vec2& a ) { 
		memcpy(vec,a.vec,sizeof(vec)); 
	}

	inline float& operator[] ( uint8 index ) { 
		return vec[index]; 
	}
	
	inline float operator[] ( uint8 index ) const { 
		return vec[index]; 
	}

	inline vec2& operator+= ( const vec2& a ) { 
		for( int i = 0; i < size; ++i ) {
			vec[i]+=a.vec[i];
		}
		return *this;
	}

	inline float operator*= ( const vec2& a ) {
		float dot = 0;
		for( int i = 0; i < size; ++i ) {
			dot += vec[i]*a.vec[i];
		}	
		return dot;
   	}
   	
    inline vec2& operator/= ( const float a ) {
		for( int i = 0; i < size; ++i ) {
			vec[i]/=a;
		}	
		return *this;
   	}

	inline float length() {
		return sqrt(vec[0]*vec[0] + vec[1]*vec[1]);
	}

	inline vec2& normalize() {
		*this/=length();
		return *this;	
	}

	inline void toString ( char in[], bool integer = false ) {
		char str[STR_LEN];
		memset( str, 0, sizeof(str) );        
        if( integer ) 
            sprintf(str, "| %.0f %.0f |", vec[0], vec[1] );
        else
            sprintf(str, "| %.2f %.2f |", vec[0], vec[1] );
		strcat( in, str );
	}
};

inline vec2 operator+ ( vec2 a, const vec2& b ) {
		return a+=b;
}

inline float operator* ( vec2 a, const vec2& b ) {
	return a*=b;
}


//////////////////////////////////////////////////

/** VEC3 */
struct vec3 {
    static const uint8 size = 3;
    
	float vec[size];

	vec3() {
		memset( vec, 0, sizeof(vec) );	
	}

	vec3( float x, float y, float z ) { 
		vec[0] = x;	
		vec[1] = y;	
		vec[2] = z;	
	}
	
	inline void operator= ( const vec3& a ) { 
		memcpy(vec,a.vec,sizeof(vec)); 
	}

	inline float& operator[] ( uint8 index ) { 
		return vec[index]; 
	}

	inline float operator[] ( uint8 index ) const { 
		return vec[index]; 
	}

	inline vec3& operator+= ( const vec3& a ) { 
		for( int i = 0; i < size; ++i ) {
			vec[i]+=a.vec[i];
		}
		return *this;	
	}

	inline float operator*= ( const vec3& a ) {
		float dot = 0;
		for( int i = 0; i < size; ++i ) {
			dot += vec[i]*a.vec[i];
		}	
		return dot;
   	}

	inline vec3& operator&= ( const vec3& a ) {
		vec3 v;
		v[0] = vec[1]*a.vec[2] - vec[2]*a.vec[1];
		v[1] = vec[2]*a.vec[0] - vec[0]*a.vec[2];
		v[2] = vec[0]*a.vec[1] - vec[1]*a.vec[0];
		*this = v;
		return *this;
	}

    inline vec3& operator/= ( const float a ) {
		for( int i = 0; i < size; ++i ) {
			vec[i]/=a;
		}	
		return *this;
   	}
	
	inline float length() {
		return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
	}

	inline vec3& normalize() {
		*this/=length();
		return *this;	
	}

	inline void toString ( char in[], bool integer = false ) {
		char str[STR_LEN];
		memset( str, 0, sizeof(str) );
        if( integer ) 
            sprintf(str, "| %.0f %.0f %.0f |", vec[0], vec[1], vec[2]);
        else
            sprintf(str, "| %.2f %.2f %.2f |", vec[0], vec[1], vec[2]);
		strcat( in, str );
	}
};

inline vec3 operator+ ( vec3 a, const vec3& b ) {
		return a+=b;
}

inline float operator* ( vec3 a, const vec3& b ) {
	return a*=b;
}

inline vec3 operator& ( vec3 a, const vec3& b ) {
	return a&=b;
}

//////////////////////////////////////////////////

/** VEC4 */
struct vec4 {
    static const uint8 size = 4;
    
	float vec[size];

	vec4() {
		memset( vec, 0, sizeof(vec) );	
	}

	vec4( float x, float y, float z, float w ) { 
		vec[0] = x;	
		vec[1] = y;	
		vec[2] = z;	
		vec[3] = w;	
	}
	
	inline void operator= ( const vec4& a ) { 
		memcpy(vec,a.vec,sizeof(vec)); 
	}

	inline float& operator[] ( uint8 index ) { 
		return vec[index]; 
	}
	
	inline float operator[] ( uint8 index ) const { 
		return vec[index]; 
	}

	inline vec4& operator+= ( const vec4& a ) { 
		for( int i = 0; i < size; ++i ) {
			vec[i]+=a.vec[i];
		}
		return *this;	
	}

	inline float operator*= ( const vec4& a ) {
		float dot = 0;
		for( int i = 0; i < size; ++i ) {
			dot += vec[i]*a.vec[i];
		}
		return dot;
   	}
   	
    inline vec4& operator/= ( const float a ) {
		for( int i = 0; i < size; ++i ) {
			vec[i]/=a;
		}	
		return *this;
   	}

	inline float length() {
		return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] + vec[3]*vec[3]);
	}

	inline vec4& normalize() {
		*this/=length();
		return *this;	
	}
   	
	inline void toString ( char in[], bool integer = false  ) {
		char str[STR_LEN];
		memset( str, 0, sizeof(str) );
        if( integer ) 
            sprintf(str, "| %.0f %.0f %.0f %.0f |", vec[0], vec[1], vec[2], vec[3]);
        else
            sprintf(str, "| %.2f %.2f %.2f %.2f |", vec[0], vec[1], vec[2], vec[3]);
		strcat( in, str );
	}
};

inline vec4 operator+ ( vec4 a, const vec4& b ) {
		return a+=b;
}

inline float operator* ( vec4 a, const vec4& b ) {
	return a*=b;
}


#endif /** VECTOR_H */
