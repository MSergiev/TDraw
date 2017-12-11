#define TINYOBJLOADER_IMPLEMENTATION
#include "TDraw3D.hpp"

TDraw3D::TDraw3D( float fov, float near, float far ) {
    this->fov = fov;
    this->near = near;
    this->far = far;
    this->ratio = 0.0f;
    this->amb = 10;

    uS = 1; uT = 0; uR = 0;
    sV = { uS, uS, uS, 0.0f };
    tV = { uT, uT, uT, 0.0f };
    rV = { uR, uR, uR, 0.0f };
    
    cRV = vec4( 1.0f, 0.0f, 0.0f, 0.0f );
    cUV = vec4( 0.0f, 1.0f, 0.0f, 0.0f );
    cFV = vec4( 0.0f, 0.0f, 1.0f, 0.0f );
    cPV = vec4( 0.0f, 0.0f, -5.0f, 1.0f );

    cFPSV = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
}

TDraw3D::~TDraw3D() {
}

void TDraw3D::translationMatrix( mat4& m, const vec4& v ) {
	mat4 mat = {
		{1,0,0,v[0]},
		{0,1,0,v[1]},
		{0,0,1,v[2]},
		{0,0,0,1}
	};
	m *= mat;
}

void TDraw3D::scaleMatrix( mat4& m, const vec4& v ) {
	mat4 mat = {
		{v[0],0,0,0},
		{0,v[1],0,0},
		{0,0,v[2],0},
		{0,0,0,1}
	};
	m *= mat;
}

void TDraw3D::rotationMatrix( mat4& m, const vec4& v ) {
	float CX = cos(v[0]), SX = sin(v[0]);
	float CY = cos(v[1]), SY = sin(v[1]);
	float CZ = cos(v[2]), SZ = sin(v[2]);
    
	mat4 mat = {
		{CY*CZ,-CY*SZ,SY,0},
		{SX*SY*CZ+CX*SZ,-SX*SY*SZ+CX*CZ,-SX*CY,0},
		{-CX*SY*CZ+SX*SZ,CX*SY*SZ+SX*CZ,CX*CY,0},
		{0,0,0,1} 
	};
    m *= mat;
}

void TDraw3D::modelMatrix(const vec4& t, const vec4& s, const vec4& r) {
    mM = mat4();
	scaleMatrix( mM, s );
	rotationMatrix( mM, r );
	translationMatrix( mM, t );
    
    imM = mM.inverse();
}

void TDraw3D::viewMatrix(const vec4& r, const vec4& u, const vec4& f, const vec4& p) {
	vM = {
		{r[0],u[0],f[0],p[0]},
		{r[1],u[1],f[1],p[1]},
		{r[2],u[2],f[2],p[2]},
		{0,0,0,1}
	};
}

void TDraw3D::viewportMatrix( const float x, const float y, const float w, const float h ) {
    float hw = w/2, fw = (w+x)/2;
    float hh = h/2, fh = (h+y)/2;
    float hd = (far-near)/2, fd = (far+near)/2;
    
    vpM = {
        {hw,0,0,fw},
        {0,hh,0,fh},
        {0,0,hd,fd},
        {0,0,0,1}
    };
    
    ivpM = vpM.inverse();
}

void TDraw3D::projectionMatrix() {
	float yScale = 1/tan(radians(fov/2));
	float xScale = yScale/ratio;
	float fmn = far-near;

	pM = {
		{xScale, 0, 0, 0},
		{0, yScale, 0, 0},
		{0, 0, (far+near)/fmn, -(2*far*near)/fmn},
		{0, 0, 1, 0}
	};
    
    ipM = pM.inverse();
}

void TDraw3D::FPV( vec4 eye, float pitch, float yaw ) {
	float cP = cos(pitch);
	float sP = sin(pitch);
	float cY = cos(yaw);
	float sY = sin(yaw);

    vec3 e(eye[0],eye[1],eye[2]);
	vec3 x = { cY, 0, -sY };
	vec3 y = { sY*sP, cP, cY*sP };
	vec3 z = { sY*cP, -sP, cP*cY };

	vM = {
		{x[0],x[1],x[2], -(x*e)},
		{y[0],y[1],y[2], -(y*e)},
		{z[0],z[1],z[2], -(z*e)},
		{0,0,0,1}
	};
    
    ivM = vM.inverse();
}

void TDraw3D::lookAt( vec4 eye, vec4 target, vec4 up ) {
    vec3 e(eye[0],eye[1],eye[2]);
    vec3 t(target[0],target[1],target[2]);
    vec3 u(up[0],up[1],up[2]);
	vec3 z = (e-t).normalize();
	vec3 x = (u&z).normalize();
	vec3 y = z&x;

	vM = {
		{x[0],x[1],x[2], -(x*e)},
		{y[0],y[1],y[2], -(y*e)},
		{z[0],z[1],z[2], -(z*e)},
		{0,0,0,1}
	};
    
    ivM = vM.inverse();
}

void TDraw3D::move( float x, float y, float z ) {
	cPV[0] += x;
	cPV[1] += y;
	cPV[2] += z;
}

void TDraw3D::rotate( float x, float y, float z ) {
	cFPSV[0] += x;
	cFPSV[1] += y;
	cFPSV[2] += z;
}

void TDraw3D::init() {
	TDraw::init();
	
	ratio = (float)GetScreenWidth()/GetScreenHeight();
    zDepth = (float*)malloc(GetScreenHeight()*GetScreenWidth()*sizeof(float));
    for( int i = 0; i < GetScreenHeight()*GetScreenWidth(); ++i ) {
        zDepth[i] = far;
    }
    viewportMatrix( 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() );
	projectionMatrix();
	FPV( cPV, cFPSV[0], cFPSV[1] );

// 	mvpM = pM*vM*mM;
//     imvpM = mvpM.inverse();
}

void TDraw3D::draw() {       
    for( int i = 0; i < GetScreenHeight()*GetScreenWidth(); ++i ) {
        zDepth[i] = far;
    }
	float aspect = (float)GetScreenWidth()/GetScreenHeight();
    viewportMatrix( 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() );
	if( aspect != ratio ) projectionMatrix();
	ratio = aspect;
	FPV( cPV, cFPSV[0], cFPSV[1] );
	//lookAt( cPV, ORIGIN, OY );

    
    char str[50];
    memset(str, 0, sizeof(str));
    cPV.toString(str);
    Status1(str);
    memset(str, 0, sizeof(str));
    for( int i = 0; i < 8; ++i ) {
        char sub[10];
        memset(sub, 0, sizeof(sub));
        vec4 p = cubeVertices[i];
        ModelToClip(p);
        ClipToScreen(p);
        sprintf(sub, "%.2f ", p[3]);
        strcat(str, sub);
    }
    Status2(str);
    
// 	mvpM = pM*vM*mM;
//     imvpM = mvpM.inverse();

	TDraw::draw();
    
    free(zDepth);
    zDepth = (float*)malloc(GetScreenHeight()*GetScreenWidth()*sizeof(float));
}

void TDraw3D::drawLine ( const vec4& a, const vec4& b, char color ) {
    vec4 a1 = a, b1 = b;
    int w = b1[0]-a1[0], h = b1[1]-a1[1];
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if( w < 0 ) dx1 = -1; else if( w > 0 ) dx1 = 1;
	if( w < 0 ) dx2 = -1; else if( w > 0 ) dx2 = 1;
	if( h < 0 ) dy1 = -1; else if( h > 0 ) dy1 = 1;
	
	int longest = abs(w);
	int shortest = abs(h);

	if( longest <= shortest ) {
		std::swap(longest, shortest);
		if( h < 0 ) dy2 = -1; else if( h > 0 ) dy2 = 1;
		dx2 = 0;
	}

	int num = longest/2;

	for( int i = 0; i <= longest; ++i ) {
        if( !setPixel( a1, color ) ) return;
		num += shortest;
		if( num >= longest ) { num -= longest; a1[0] += dx1; a1[1] += dy1; }
		else { a1[0] += dx2; a1[1] += dy2; }
	}
}

void TDraw3D::draw3DLine( vec4 a, vec4 b, char color ) {
    ModelToClip( a ); ModelToClip( b );
    //clipLine(a, b);
    ClipToScreen( a ); ClipToScreen( b );
    drawLine( a, b, color );
    return;
}

void TDraw3D::drawCube( const vec4& t, const vec4& s, const vec4& r, bool fill, char color ) {
    modelMatrix(t, s, r);
	
	for( int i = 0; i < 12; ++i ) {
        if(fill) drawTriangle( cubeVertices[cubeOrder[3*i]], cubeVertices[cubeOrder[3*i+1]], cubeVertices[cubeOrder[3*i+2]], true, color );
		else draw3DLine( cubeVertices[cubeOutline[2*i]], cubeVertices[cubeOutline[2*i+1]], color ); 
	}
}

void TDraw3D::drawSphere(const  vec4& c, float r, float prec, char color) {
	vec4 pOld;
	for( float i = 0; i < 2*M_PI; i+=prec ) {
		for( float j = 0; j < 2*M_PI; j+=prec ){
            vec4 p(r*sin(i)*cos(j)+c[0], r*sin(i)*sin(j)+c[1], r*cos(i)+c[2], 1);
			if(j!=0) draw3DLine(p, pOld, color);
			pOld = p;
		}
	}
}

void TDraw3D::drawConic( const vec4& p, float h, float r1, float r2, char color ) {
	float tH = (h*r2)/(r1-r2);
	float rad;
	vec4 point;
	for( float i = 0; i < h; i+=0.01 ) {
		rad = (r2*(tH+i))/(tH+h); 
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + rad*cos(j);
			point[1] = p[1] + rad*sin(j);
			setPixel(getPixel(point), color);
		}
	}
}

void TDraw3D::drawCone( const vec4& p, float h, float r, char color ) {
	float rad;
	vec4 point;
	for( float i = 0; i < h; i+=0.01 ) {
		rad = (r*(h-i))/h; 
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + rad*cos(j);
			point[1] = p[1] + rad*sin(j);
			setPixel(getPixel(point), color);
		}
	}
}

void TDraw3D::drawCylinder( const vec4& p, float h, float r, char color ) {
	vec4 point;
	for( float i = 0; i < h; i+=0.01 ) {
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + r*cos(j);
			point[1] = p[1] + r*sin(j);
			setPixel(getPixel(point), color);
		}
	}
}

void TDraw3D::drawTriangle( vec4 a, vec4 b, vec4 c, bool fill, char color ) {
    if( fill ) {
        ModelToClip(a); ModelToClip(b); ModelToClip(c); 
        //clipLine(a, b); clipLine(a, c); clipLine(b, c);
        ClipToScreen(a); ClipToScreen(b); ClipToScreen(c); 
//         a = getPixel(a);
//         b = getPixel(b);
//         c = getPixel(c);
		vec4* s[3];
		if( a[1] < b[1] ){
			if( a[1] < c[1] ){
				s[0] = &a;
				if( b[1] < c[1] ) { s[1]=&b; s[2]=&c; }
			   	else { s[1]=&c; s[2]=&b; }
			} else {
				s[0] = &c;
				if( b[1] < a[1] ) { s[1] = &b; s[2] = &a; }
                else { s[1] = &a; s[2] = &b; }
			}
		} else {
			if ( b[1] < c[1] ){
				s[0] = &b;
				if( a[1] < c[1] ) { s[1] = &a; s[2] = &c; }
				else { s[1] = &c; s[2] = &a; }
			} else {
				s[0]=&c;
				if( a[1] < b[1] ) { s[1] = &a; s[2] = &b; }
				else { s[1] = &b; s[2] = &a; }
			}
		}

		if( (*s[1])[0] > (*s[2])[0] ) std::swap( s[1], s[2] );

		if( (*s[1])[1] == (*s[2])[1] ) {
            fillTriangle( s[0], s[1], s[2], color );
        } else if( (*s[0])[1] == (*s[1])[1] ) {
            fillTriangle( s[2], s[0], s[1], color );
        } else {
			vec4 a1, a2, a3, a4;
			a1 = (*s[0]);
			if( (*s[1])[1] < (*s[2])[1] ) { 
                a2=(*s[2]); 
                a3=(*s[1]);
                
            } else {
                a2=(*s[1]); 
                a3=(*s[2]);
            }
			a4 = vec4( GetScreenWidth(), a3[1], 1, 1 );
			int denom = ( (a1[0] - a2[0]) * (a3[1] - a4[1] ) - ( a1[1] - a2[1] ) * ( a3[0] - a4[0] ) );
			int x = GetScreenWidth() / 2, y = GetScreenHeight() / 2;
			if( denom != 0 ) {
				x = ( (a1[0] * a2[1]-a1[1] * a2[0]) * (a3[0]-a4[0]) - (a1[0]-a2[0]) * (a3[0] * a4[1]-a3[1] * a4[0]) ) / denom;
				y = ( (a1[0] * a2[1]-a1[1] * a2[0]) * (a3[1]-a4[1]) - (a1[1]-a2[1]) * (a3[0] * a4[1]-a3[1] * a4[0]) ) / denom;
			}
			vec4 m( x, y, a1[2], 1 );
			fillTriangle( &a1, &a3, &m, color );
			fillTriangle( &a2, &a3, &m, color );
		}
	} else {
		draw3DLine( a, b, color );
		draw3DLine( b, c, color );
		draw3DLine( c, a, color );
	}
}

void TDraw3D::fillTriangle( vec4* t, vec4* l, vec4* r, char color ) {
    if( (*l)[0] > (*r)[0] ) std::swap( l ,r );
	int h = std::abs( (*l)[1] - (*t)[1] ); 
	if( h != 0 ) {
		int lL = (*l)[0] - (*t)[0];
		int rL = (*r)[0] - (*t)[0];
		vec4 lT = *t, rT = *t;
		int lOff, rOff;
		for( int i = 0; i<=h; ++i ) {
			if( (*t)[1] < (*l)[1] ){
				lOff = lL * i / h;
                rOff = rL * i / h;
				lT[1] = rT[1] = ( i + (*t)[1] );
			} else {
				lOff = lL * (h-i) / h;
                rOff = rL * (h-i) / h;
				lT[1] = rT[1] = ( (*t)[1] - h + i );
			}
			lT[0] = (*t)[0] + lOff;
            rT[0] = (*t)[0] + rOff + 1; 
            char c1 = mapTo( (cPV-lT).length(), 0, amb, 8, 1 );
            char c2 = mapTo( (cPV-rT).length(), 0, amb, 8, 1 );
			drawLine( lT, rT, c1 );
		}
	}
}


bool TDraw3D::isInFrustum( const vec4& a ) {
    bool isInside = true;
    isInside &= (a[0] < std::abs(a[3]));
    isInside &= (a[1] < std::abs(a[3]));
    isInside &= (a[2] < std::abs(a[3]));
    isInside &= (a[3] > near);
    return isInside;
}


void TDraw3D::ModelToClip( vec4& a ) {
    a = mM*a;
    a = vM*a;
    a = pM*a;
}

void TDraw3D::ScreenToWorld( vec4& a ) {
    float w = a[3];
    a[3]=1.0f;
    a = ivpM*a;
    a *= w;
    a = ipM*a;
    a = ivM*a;
}

void TDraw3D::ClipToScreen( vec4& a ) {
    float w = a[3];
    a /= w;
    a = vpM*a;
    a[3] = w;
}

void TDraw3D::clipLine( vec4& a, vec4& b ) {
    if( isInFrustum(a) != isInFrustum(b) ) {
        float n = (a[3] - near) / (a[3] - b[3]);
        float xc = (n * a[0]) + ((1-n) * b[0]);
        float yc = (n * a[1]) + ((1-n) * b[1]);
        float zc = (n * a[2]) + ((1-n) * b[2]);
        b = {xc, yc, zc, near};
    }
}

vec4 TDraw3D::getPixel( vec4 p ) {
    ModelToClip(p);
    ClipToScreen(p);
	return p;
}

bool TDraw3D::setPixel( vec4 p, char color ) {
    vec4 p1 = p;
    ScreenToWorld(p1);
//     if( !isInFrustum(p1) ) return false;
    if( p[3] < 1 ) return false;
    if( p[0] < 0 or p[0] > GetScreenWidth()-1 or p[1] < 0 or p[1] > GetScreenHeight()-1 ) return true;
    int id = (int)p[0]*GetScreenHeight() + (int)p[1];
    float depth = (cPV-p1).length();
    //if( zDepth[id] > depth ) {
        //zDepth[id] = depth;
        drawPixel( p[0], p[1], mapTo( depth, 0, amb, 7, 1 ) );
    //}
    return true;
}

// Dump transformation data to file
void TDraw3D::dumpTransformation( vec4 p ) {
    
    char str[200];
    
    std::ofstream file("data.txt");
    
    memset( str, 0, sizeof(str) );
    mM.toString(str);
    file << "Model matrix:\n" << str << std::endl;    
    
    memset( str, 0, sizeof(str) );
    imM.toString(str);
    file << "Model inverse:\n" << str << std::endl;      
    
    memset( str, 0, sizeof(str) );
    vM.toString(str);
    file << "View matrix:\n" << str << std::endl;      
    
    memset( str, 0, sizeof(str) );
    ivM.toString(str);
    file << "View inverse:\n" << str << std::endl;     
    
    memset( str, 0, sizeof(str) );
    pM.toString(str);
    file << "Projection matrix:\n" << str << std::endl;     
    
    memset( str, 0, sizeof(str) );
    ipM.toString(str);
    file << "Projection inverse:\n" << str << std::endl;    
    
    memset( str, 0, sizeof(str) );
    vpM.toString(str);
    file << "Viewport matrix:\n" << str << std::endl;     
    
    memset( str, 0, sizeof(str) );
    ivpM.toString(str);
    file << "Viewport inverse:\n" << str << std::endl;  
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "\nInitial vector: " << str << std::endl;
    
    p = mM*p;  
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Model transformation: " << str << std::endl;
    
    p = vM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "View transformation: " << str << std::endl;
    
    p = pM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Projection transformation: " << str << std::endl;
    
    float w = p[3];
    p/=p[3];
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Perspective divide: " << str << std::endl;
    
    p = vpM*p;    
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Viewport transformation: " << str << std::endl;
    
    p = ivpM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Viewport inverse: " << str << std::endl;
    
    p *= w;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Perspective divide inverse: " << str << std::endl;
    
    p = ipM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Projection inverse: " << str << std::endl;
    
    p = ivM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "View inverse: " << str << std::endl;
    
    p = imM*p;
    
    memset( str, 0, sizeof(str) );
    p.toString(str);
    file << "Model inverse: " << str << std::endl;
    
    file.close();
}

//Load .OBJ file
void TDraw3D::loadFromFile( Model& model, std::string path ) {
	std::string err;
	tinyobj::LoadObj( &model.attrib, &model.shapes, &model.materials, &err, path.c_str() );
	if( !err.empty() ) std::cerr << err << std::endl;
}

//Draw loaded .OBJ model
void TDraw3D::drawObject( Model& model ) {
	//int scale = 50;
	for( size_t s = 0;s < model.shapes.size(); ++s ){
		size_t index_offset = 0;
		std::vector<vec4> shape;
		for( size_t f = 0; f < model.shapes[s].mesh.num_face_vertices.size(); ++f ) {
			size_t fv = model.shapes[s].mesh.num_face_vertices[f];
			for( size_t ve = 0; ve < fv ; ++ve ){
				tinyobj::index_t idx = model.shapes[s].mesh.indices[index_offset+ve];
				float vx = model.attrib.vertices[3*idx.vertex_index+0];
				float vy = model.attrib.vertices[3*idx.vertex_index+1];
				float vz = model.attrib.vertices[3*idx.vertex_index+2];
				vec4 ver(vx,vy,vz,1);
				shape.push_back( ver );
				setPixel( getPixel(ver), 7 );
				setPixel(getPixel(vec4( ver[0]+1, ver[1], ver[2], 1 )), 7 );
				setPixel(getPixel(vec4( ver[0]-1, ver[1], ver[2], 1 )), 7 );
				setPixel(getPixel(vec4( ver[0], ver[1]+1, ver[2], 1 )), 7 );
				setPixel(getPixel(vec4( ver[0], ver[1]-1, ver[2], 1 )), 7 );
			}
			for(size_t sh=0; sh<shape.size()/2; ++sh)
				drawLine( shape[sh], shape[sh+1][0], 7);
			index_offset+=fv;
		}
	}
}
