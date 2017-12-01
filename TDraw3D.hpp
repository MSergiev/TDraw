#ifndef TDRAW3D_H
#define TDRAW3D_H

#include "TDraw.hpp"
#include "tiny_obj_loader.h"

#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include "Matrix.h"

#include <fstream>

//#include <eigen3/Eigen/Core>
//#include <eigen3/Eigen/Geometry>

#define ORIGIN v(0,0,0)
#define OX v(1,0,0)
#define OY v(0,1,0)
#define OZ v(0,0,1)

typedef vec4 V; // World-space vector
typedef vec3 v; // Screen-space vector
typedef mat4 M; // Matrix

struct Model {
    tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
    char color;
};

// Cube vertices
const static V cubeVertices[8] = {
    V(-1,-1, 1, 1),
    V( 1,-1, 1, 1),
    V( 1, 1, 1, 1),
    V(-1, 1, 1, 1),
    V(-1,-1,-1, 1),
    V( 1,-1,-1, 1),
    V( 1, 1,-1, 1),
    V(-1, 1,-1, 1)
};

// Cube vertex drawing order
const static uint8_t cubeOrder[36] = {
    0,1,2, 0,2,3,
    1,5,6, 1,6,2,
    5,4,7, 5,7,6,
    4,0,3, 4,3,7,
    4,5,1, 4,1,0,
    7,6,2, 7,2,3
};

// Cube edge drawing order
const static uint8_t cubeOutline[24] = {
    0,1, 0,3, 0,4, 
    1,5, 1,2,
    2,6, 2,3, 3,7,
    4,5, 4,7, 5,6, 6,7
};



class TDraw3D : public TDraw {
    
private:

    // Transformation data
    M sM, tM, rM, mM, vM, pM, mvpM;
    float uS, uT, uR; // Uniform transformation
    v sV; // Scalar vector
    v tV; // Translation vector
    v rV; // Rotation vector

    v cRV; // Camera right vector
    v cUV; // Camera up vector
    v cFV; // Camera front vector
    v cPV; // Camera position vector

	V vpV; // Viewport vector

    v cFPSV;

    float fov, near, far, ratio;
    
public:
    
	TDraw3D( float fov = 90.0f, float near = 0.01, float far = 100 );
	virtual ~TDraw3D();

    void translationMatrix( M& m, const v& v );
    void scaleMatrix( M& m, const v& v );
    void rotationMatrix( M& m, const v& v );
    void modelMatrix( const v& t, const v& s, const v& r );
    void viewMatrix( const v& r, const v& u, const v& f, const v& p );
    void projectionMatrix();
	void FPV();

    void move( float x, float y, float z );
	void rotate( float x, float y, float z ); 

	virtual void init();
    virtual void draw();
    
    void cube( const v& c, float r, char color = 1 );
    void sphere( const v& c, float r, float prec = 0.2, char color = 1 );
    void triangle( const v& a, const v& b, const v& c, char color = 1 );
   
	void draw3DLine( const v& a, const v& b, char color );	
    void drawCube( const v* cube, char color = 1 ) ;
    void drawSphere( const v& c, float r, float prec = 0.2, char color = 1 );
    void drawConic( const v& p, float h, float r1, float r2, char color );
	void drawCone( const v& p, float h, float r, char color );
	void drawCylinder( const v& dir, float h, float r, char color );
	void drawTriangle( v a, v b, v c, char color = 1 );

    v getPixel( const v& p );
    void setPixel( const v& v, char color = 1);

	void loadFromFile( Model& model, std::string path );
	void drawObject( Model& model );

};

#endif /** TDRAW3D_H */
