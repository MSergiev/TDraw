#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

//////////////////////////////////////////////////

/** MAT 2x2 */
struct mat2 {
    // Constant size
    static const uint8 size = vec2::size;

    // Array
    vec2 mat[size];

    // Default constructor
    mat2() { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i][i]=1; 
        }
    }

    // Braced constructor
    mat2( vec2 a, vec2 b ) { 
        mat[0] = a;
        mat[1] = b; 
    }

    // Uniform constructor
    mat2( float a ) { 
        mat[0] = vec2(a);
        mat[1] = vec2(a); 
    }

    // Transposition operator
    inline mat2& operator~ () {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = i+1; j < size; ++j ) {
                swap(mat[i][j], mat[j][i]);
            }
        }
        return *this;
    };

    // Copy operator
    inline void operator= ( const mat2& a ) { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i]=a.mat[i]; 
        }
    }

    // Square brace operator
    inline vec2& operator[] ( uint8 index ) { 
        return mat[index];
    }

    // Addition operator
    inline mat2& operator+= ( const mat2& a ) { 
        for( uint8 i = 0; i < size; ++i ) {
            mat[i]+=a.mat[i]; 
        }
        return *this;	
    }

    // Multiplication operator
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

    // Division operator
    inline mat2& operator/= ( float a ) {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                mat[i][j] /= a;
            }
        }
        return *this;
    }

    // Matrix minor composer
    inline float reduce( uint8 x, uint8 y ) {
        return mat[(size-1)-x][(size-1)-y];
    }

    // Determinant calculator
    inline float determinant() {
        return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
    }

    // Inverse matrix calculator
    inline mat2 inverse() {
        mat2 m;
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                m[i][j] = reduce(i,j);
                m[i][j] *= ( (i+j)%2 ? -1 : 1 );
            }
        }
        ~m;
        m/=determinant();
        return m;
    }

    // String formatter
    inline void toString( char in[], bool integer = false ) {
        for( int i = 0; i < size; ++i ){
            mat[i].toString(in, integer);
            if(i<(size-1)) strcat( in, "\n" );
        }
    }
};

// Binary addition operator
inline mat2 operator+ ( mat2 a, mat2& b ) {
		return a+=b;
}

// Binary subtraction operator
inline mat2 operator* ( mat2 a, mat2& b ) {
	return a*=b;
}

// Binary multiplication operator
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
		mat[0] = a;
		mat[1] = b; 
		mat[2] = c; 
	}
	
	mat3( float a ) { 
		mat[0] = vec3(a);
		mat[1] = vec3(a); 
		mat[2] = vec3(a); 
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

    inline mat3& operator/= ( float a ) {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                mat[i][j] /= a;
            }
        }
        return *this;
    }
    
	inline mat2 reduce( uint8 x, uint8 y ) {
        mat2 res(0);
        uint8 yIdx = 0;
        if( x >= size || y >= size ) return res;
        for( uint8 i = 0; i < size; ++i ) {
            vec2 tmp;
            uint8 xIdx = 0;
            for( uint8 j = 0; j < size; ++j ) {
                if( i != x && j != y ) {
                    tmp[xIdx] = mat[i][j];
                    xIdx++;
                }
            }
            if( i != x ) {
                res[yIdx] = tmp;
                yIdx++;
            }
        }
        return res;
    }
    
    inline float determinant() {
        float det = 0;
        for( uint8 i = 0; i < size; ++i ) {
            det += (i%2 ? -1 : 1) * mat[0][i] * reduce(0,i).determinant();
        }
        return det;
    }
    
    inline mat3 inverse() {
        mat3 m;
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                m[i][j] = reduce(i,j).determinant();
                m[i][j] *= ( (i+j)%2 ? -1 : 1 );
            }
        }
        ~m;
        m/=determinant();
        return m;
    }
    
	inline void toString( char in[], bool integer = false ) {
		for( int i = 0; i < size; ++i ){
			mat[i].toString(in, integer);
			if(i<(size-1)) strcat( in, "\n" );
		}
	}
	
	inline mat3 operator!() {
        
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
		mat[0] = a;
		mat[1] = b; 
		mat[2] = c; 
		mat[3] = d; 
	}

	mat4( float a ) { 
		mat[0] = vec4(a);
		mat[1] = vec4(a); 
		mat[2] = vec4(a); 
		mat[3] = vec4(a); 
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

    inline mat4& operator/= ( float a ) {
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                mat[i][j] /= a;
            }
        }
        return *this;
    }
	
	inline mat3 reduce( uint8 x, uint8 y ) {
        mat3 res(0);
        uint8 yIdx = 0;
        if( x >= size || y >= size ) return res;
        for( uint8 i = 0; i < size; ++i ) {
            vec3 tmp;
            uint8 xIdx = 0;
            for( uint8 j = 0; j < size; ++j ) {
                if( i != x && j != y ) {
                    tmp[xIdx] = mat[i][j];
                    xIdx++;
                }
            }
            if( i != x ) {
                res[yIdx] = tmp;
                yIdx++;
            }
        }
        return res;
    }
    
    inline float determinant() {
        float det = 0;
        for( uint8 i = 0; i < size; ++i ) {
            det += (i%2 ? -1 : 1) * mat[0][i] * reduce(0,i).determinant();
        }
        return det;
    }
    
    inline mat4 inverse() {
        mat4 m;
        for( uint8 i = 0; i < size; ++i ) {
            for( uint8 j = 0; j < size; ++j ) {
                m[i][j] = reduce(i,j).determinant();
                m[i][j] *= ( (i+j)%2 ? -1 : 1 );
            }
        }
        ~m;
        m/=determinant();
        return m;
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
