#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

//////////////////////////////////////////////////

/** MAT 2x2 */
struct mat2 {
    static const uint8 size = vec2::size;
    
	vec2 mat[size];

	mat2() { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i][i]=1; 
        }
	}

	mat2( vec2 a, vec2 b ) { 
		mat[0]=a;
		mat[1]=b; 
	}

	inline mat2& operator~ () {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = i+1; j < size; ++j ) {
                swap(mat[i][j], mat[j][i]);
            }
        }
		return *this;
	};

	inline void operator= ( const mat2& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]=a.mat[i]; 
		}
   	}
	
	inline vec2& operator[] ( uint8 index ) { 
		return mat[index];
   	}

	inline mat2& operator+= ( const mat2& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]+=a.mat[i]; 
		}
		return *this;	
	}

	inline mat2& operator*= ( mat2 a ) {
		mat2 p;
		~a;
		for( uint8 i = 0; i < size; ++i ) {
			for( uint8 j = 0; j < size; ++j ) {
				p[i][j] = mat[i]*a.mat[j];
			}
		}
		for( uint8  i = 0; i < size; ++i ) { 
			mat[i] = p[i];
		}
		return *this;
	}

	inline void toString( char in[], bool integer = false ) {
		for( int i = 0; i < size; ++i ){
			mat[i].toString(in, integer);
			if(i<(size-1)) strcat( in, "\n" );
		}
	}
};

inline mat2 operator+ ( mat2 a, mat2& b ) {
		return a+=b;
}

inline mat2 operator* ( mat2 a, mat2& b ) {
	return a*=b;
}

inline vec2 operator* ( mat2& a, vec2 b ) {
    vec2 tmp;
    for( uint8 i = 0; i < vec2::size; ++i ) {
        tmp[i] = a[i]*b;
    }
	return tmp;
}

//////////////////////////////////////////////////

/** MAT 3x3 */
struct mat3 {
    static const uint8 size = vec3::size;
    
	vec3 mat[size];

	mat3() { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i][i]=1; 
        }
	}

	mat3( vec3 a, vec3 b, vec3 c ) { 
		mat[0]=a;
		mat[1]=b; 
		mat[2]=c; 
	}

	inline mat3& operator~ () {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = i+1; j < size; ++j ) {
                swap(mat[i][j], mat[j][i]);
            }
        }
		return *this;
	};

	inline void operator= ( const mat3& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]=a.mat[i]; 
		}
   	}
	
	inline vec3& operator[] ( uint8 index ) { 
		return mat[index];
   	}

	inline mat3& operator+= ( const mat3& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]+=a.mat[i]; 
		}
		return *this;	
	}

	inline mat3& operator*= ( mat3 a ) {
		mat3 p;
		~a;
		for( uint8 i = 0; i < size; ++i ) {
			for( uint8 j = 0; j < size; ++j ) {
				p[i][j] = mat[i]*a.mat[j];
			}
		}
		for( uint8  i = 0; i < size; ++i ) { 
			mat[i] = p[i];
		}
		return *this;
	}

	inline void toString( char in[], bool integer = false ) {
		for( int i = 0; i < size; ++i ){
			mat[i].toString(in, integer);
			if(i<(size-1)) strcat( in, "\n" );
		}
	}
};

inline mat3 operator+ ( mat3 a, mat3& b ) {
		return a+=b;
}

inline mat3 operator* ( mat3 a, mat3& b ) {
	return a*=b;
}

inline vec3 operator* ( mat3& a, vec3 b ) {
    vec3 tmp;
    for( uint8 i = 0; i < vec3::size; ++i ) {
        tmp[i] = a[i]*b;
    }
	return tmp;
}

//////////////////////////////////////////////////

/** MAT 4x4 */
struct mat4 {
    static const uint8 size = vec4::size;
    
	vec4 mat[size];

	mat4() { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i][i]=1; 
        }
	}

	mat4( vec4 a, vec4 b, vec4 c, vec4 d ) { 
		mat[0]=a;
		mat[1]=b; 
		mat[2]=c; 
		mat[3]=d; 
	}

	inline mat4& operator~ () {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = i+1; j < size; ++j ) {
                swap(mat[i][j], mat[j][i]);
            }
        }
		return *this;
	};

	inline void operator= ( const mat4& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]=a.mat[i]; 
		}
   	}
	
	inline vec4& operator[] ( uint8 index ) { 
		return mat[index];
   	}

	inline mat4& operator+= ( const mat4& a ) { 
		for( uint8 i = 0; i < size; ++i ) {
			mat[i]+=a.mat[i]; 
		}
		return *this;	
	}

	inline mat4& operator*= ( const mat4& a ) {
		mat4 p = *this, tmp = a;
        ~tmp;
		for( uint8 i = 0; i < size; ++i ) {
			for( uint8 j = 0; j < size; ++j ) {
				mat[i][j] = p[i]*tmp.mat[j];
			}
		}
		return *this;
	}

	inline void toString( char in[], bool integer = false ) {
		for( int i = 0; i < size; ++i ) {
			mat[i].toString(in, integer);
			if(i<(size-1)) strcat( in, "\n" );
		}
	}
};

inline mat4 operator+ ( mat4 a, mat4& b ) {
		return a+=b;
}

inline mat4 operator* ( mat4 a, mat4& b ) {
	return a*=b;
}

inline vec4 operator* ( mat4& a, vec4 b ) {
    vec4 tmp;
    for( uint8 i = 0; i < vec4::size; ++i ) {
        tmp[i] = a[i]*b;
    }
	return tmp;
}

#endif /** MATRIX_H */
