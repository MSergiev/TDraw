#ifndef TDRAW3D_H
#define TDRAW3D_H

#include "TDraw.hpp"
#include "tiny_obj_loader.h"

#include "Matrix.h"

#define ORIGIN vec4(0,0,0,1)
#define OX vec4(1,0,0,1)
#define OY vec4(0,1,0,1)
#define OZ vec4(0,0,1,1)

struct Model {
    tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
    char color;
};

// Cube vertices
const static vec4 cubeVertices[8] = {
    vec4(-1,-1, 1, 1), // FTL
    vec4( 1,-1, 1, 1), // FTR
    vec4( 1, 1, 1, 1), // FBR
    vec4(-1, 1, 1, 1), // FBL
    vec4(-1,-1,-1, 1), // RTL
    vec4( 1,-1,-1, 1), // RTR
    vec4( 1, 1,-1, 1), // RBR
    vec4(-1, 1,-1, 1)  // RBL
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
    1,5, 1,2, 2,6, 
    2,3, 3,7, 4,5, 
    4,7, 5,6, 6,7
};



class TDraw3D : public TDraw {
    
private:

    // Transformation data
    mat4 sM, tM, rM, mM, vM, pM, mvpM, vpM;
    mat4 isM, itM, irM, imM, ivM, ipM, imvpM, ivpM;
    float uS, uT, uR; // Uniform transformation
    vec4 sV; // Scalar vector
    vec4 tV; // Translation vector
    vec4 rV; // Rotation vector

    vec4 cRV; // Camera right vector
    vec4 cUV; // Camera up vector
    vec4 cFV; // Camera front vector
    vec4 cPV; // Camera position vector

    vec4 cFPSV; // Camera rotation vector

    float fov, near, far, ratio;
    int amb;
    
    // Screen pixel depth
	float* zDepth;
    
public:
    
	TDraw3D( float fov = 90.0f, float near = 0.01, float far = 100 );
	virtual ~TDraw3D();

    void translationMatrix( mat4& m, const vec4& v );
    void scaleMatrix( mat4& m, const vec4& v );
    void rotationMatrix( mat4& m, const vec4& v );
    void modelMatrix( const vec4& t, const vec4& s, const vec4& r );
    void viewMatrix( const vec4& r, const vec4& u, const vec4& f, const vec4& p );
    void viewportMatrix( const float x, const float y, const float w, const float h );
    void projectionMatrix();
	void lookAt( vec4 eye, vec4 target, vec4 up );
	void FPV( vec4 eye, float pitch, float yaw );

    void move( float x, float y, float z );
	void rotate( float x, float y, float z ); 

	virtual void init();
    virtual void draw();
    virtual void drawLine ( const vec4& a, const vec4& b, char color = 1 );
   
	void draw3DLine( vec4 a, vec4 b, char color = 1 );
    void drawCube( const vec4& t, const vec4& s, const vec4& r, bool fill = 0, char color = 1 ) ;
    void drawSphere( const vec4& c, float r, float prec = 0.2, char color = 1 );
    void drawConic( const vec4& p, float h, float r1, float r2, char color = 1 );
	void drawCone( const vec4& p, float h, float r, char color = 1 );
	void drawCylinder( const vec4& dir, float h, float r, char color = 1 );
	void drawTriangle( vec4 a, vec4 b, vec4 c, bool fill = 0, char color = 1 );
    void fillTriangle( vec4* t, vec4* l, vec4* r, char col );

    bool isInFrustum( const vec4& a );
    
    void ModelToClip( vec4& a );
    void ScreenToWorld( vec4& a );
    void ClipToScreen( vec4& a );
    
    void clipLine( vec4& a, vec4& b );
    vec4 getPixel( vec4 p );
    bool setPixel( vec4 v, char color = 1);
    
    void dumpTransformation( vec4 p = vec4(1,1,1,1) );

	void loadFromFile( Model& model, std::string path );
	void drawObject( Model& model );

};

#endif /** TDRAW3D_H */
