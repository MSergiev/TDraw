#define TINYOBJLOADER_IMPLEMENTATION
#include "TDraw3D.hpp"

TDraw3D::TDraw3D( float fov, float near, float far ) {
    this->fov = fov;
    this->near = near;
    this->far = far;
	this->ratio = 0.0f;

    uS = 1; uT = 0; uR = 0;
    sV = { uS, uS, uS };
    tV = { uT, uT, uT };
    rV = { uR, uR, uR };
    
    cRV = v( 1.0f, 0.0f, 0.0f );
    cUV = v( 0.0f, 1.0f, 0.0f );
    cFV = v( 0.0f, 0.0f, 1.0f );
    cPV = v( 0.0f, 0.0f, -1.0f );

    cFPSV = v( 0.0f, 0.0f, 0.0f );

	//glewInit();
}

TDraw3D::~TDraw3D() {
}

void TDraw3D::translationMatrix( M& m, const v& v ) {
	M mat = {
		{1,0,0,v[0]},
		{0,1,0,v[1]},
		{0,0,1,v[2]},
		{0,0,0,1}
	};
	m *= mat;
}

void TDraw3D::scaleMatrix( M& m, const v& v ) {
	M mat = {
		{v[0],0,0,0},
		{0,v[1],0,0},
		{0,0,v[2],0},
		{0,0,0,1}
	};
	m *= mat;
}

void TDraw3D::rotationMatrix( M& m, const v & v ) {
	float CX = cos(v[0]), SX = sin(v[0]);
	float CY = cos(v[1]), SY = sin(v[1]);
	float CZ = cos(v[2]), SZ = sin(v[2]);

	/*M mat = {
		{CY*CZ, SX*SY*CZ+CX*SZ, -CX*SY*CZ+SX*SZ, 0},
		{-CY*SZ, -SX*SY*SZ+CX*CZ, CX*SY*SZ+SX*CZ, 0},
		{SY, -SX*CY, CX*CY, 0},
		{0,0,0,1} 
	};
	m *= mat;
	*/
	M mat = {
		{CY*CZ,-CY*SZ,SY,0},
		{SX*SY*CZ+CX*SZ,-SX*SY*SZ+CX*CZ,-SX*CY,0},
		{-CX*SY*CZ+SX*SZ,CX*SY*SZ+SX*CZ,CX*CY,0},
		{0,0,0,1} 
	};
	m *= mat;
}

void TDraw3D::modelMatrix(const v & t, const v & s, const v & r) {
	translationMatrix( tM, t );
	scaleMatrix( sM, s );
	rotationMatrix( rM, r );
	mM = tM*rM*sM;
}

void TDraw3D::viewMatrix(const v& r, const v& u, const v& f, const v& p) {
	vM = {
		{r[0],u[0],f[0],p[0]},
		{r[1],u[1],f[1],p[1]},
		{r[2],u[2],f[2],p[2]},
		{0,0,0,1}
	};
}

void TDraw3D::projectionMatrix() {
	float yScale = 1.0f/tan(radians(fov/2));
	float xScale = yScale / ratio;
	float nmf = near-far;

	pM = {
		{xScale, 0, 0, 0},
		{0, yScale, 0, 0},
		{0, 0, (near+far)/nmf, 2*far*near/nmf},
		{0, 0, -1, 0}
	};
}

void TDraw3D::FPV() {
	float cP = cos(cFPSV[0]);
	float sP = sin(cFPSV[0]);
	float cY = cos(cFPSV[1]);
	float sY = sin(cFPSV[1]);

	V x = { cY, 0, -sY, 0 };
	V y = { sY*sP, cP, cY*sP, 0 };
	V z = { sY*cP, -sP, cP*cY, 0 };

	vM = {
		{x[0],x[1],x[2], -(x*V(cPV[0], cPV[1], cPV[2], 1))},
		{y[0],y[1],y[2], -(y*V(cPV[0], cPV[1], cPV[2], 1))},
		{z[0],z[1],z[2], -(z*V(cPV[0], cPV[1], cPV[2], 1))},
		{0,0,0,1}
	};
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
	modelMatrix( tV, sV, rV );
	projectionMatrix();
	FPV();	

	mvpM = pM*vM*mM;
}

void TDraw3D::draw() {
	float aspect = (float)GetScreenWidth()/GetScreenHeight();
	if( aspect != ratio ) projectionMatrix();
	ratio = aspect;
    modelMatrix( tV, sV, rV );
	FPV();

	mvpM = pM*vM*mM;
	
	TDraw::draw();
}


void TDraw3D::cube(const v& c, float r, char color) {
	v tCube[8];
	M m = {
		{r,0,0,c[0]},
		{0,r,0,c[1]},
		{0,0,r,c[2]},
		{0,0,0,1}
	};
	
	for( int i = 0; i < 8; ++i ) {
		V tmp = m*cubeVertices[i];
		tCube[i] = getPixel( v(tmp[0],tmp[1],tmp[2]) );
	}
	drawCube( tCube, color );
}

void TDraw3D::sphere(const v& c, float r, float prec, char color) {
    drawSphere( c, r, prec, color);
}

void TDraw3D::triangle(const v& a, const v& b, const v& c, char color) {
    drawTriangle( getPixel(a), getPixel(b), getPixel(c), color );
}

void TDraw3D::draw3DLine( const v& a, const v& b, char color ) {
	v a1 = getPixel(a), b1 = getPixel(b);
	TDraw::drawLine( a1[0], a1[1], b1[0], b1[1], color );	
}

void TDraw3D::drawCube(const v * cube, char color) {
	for( int i = 0; i < 12; ++i ) {
		TDraw::drawLine( cube[cubeOutline[2*i]][0], cube[cubeOutline[2*i]][1], cube[cubeOutline[2*i+1]][0], cube[cubeOutline[2*i+1]][1], color ); 

	}
}

void TDraw3D::drawSphere(const  v& c, float r, float prec, char color) {
	v pOld;
	for( float i = 0; i < 2*M_PI; i+=prec ) {
		for( float j = 0; j < 2*M_PI; j+=prec ){
			v p = getPixel(v(r*sin(i)*cos(j)+c[0], r*sin(i)*sin(j)+c[1], r*cos(i)+c[2]));
			//drawPixel(p[0], p[1], color);
			if(j!=0) TDraw::drawLine(p[0], p[1], pOld[0], pOld[1], color);
			pOld = p;
		}
	}
}

void TDraw3D::drawConic( const v& p, float h, float r1, float r2, char color ) {
	float tH = (h*r2)/(r1-r2);
	float rad;
	v point;
	for( float i = 0; i < h; i+=0.01 ) {
		rad = (r2*(tH+i))/(tH+h); 
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + rad*cos(j);
			point[1] = p[1] + rad*sin(j);
			v pix = getPixel(point);
			drawPixel(pix[0], pix[1], color);
		}
	}
}

void TDraw3D::drawCone( const v& p, float h, float r, char color ) {	
	float rad;
	v point;
	for( float i = 0; i < h; i+=0.01 ) {
		rad = (r*(h-i))/h; 
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + rad*cos(j);
			point[1] = p[1] + rad*sin(j);
			v pix = getPixel(point);
			drawPixel(pix[0], pix[1], color);
		}
	}
}

void TDraw3D::drawCylinder( const v& p, float h, float r, char color ) {
	v point;
	for( float i = 0; i < h; i+=0.01 ) {
		point[2] = i;
		for( float j = 0; j < 2*M_PI; j+=0.1 ) {
			point[0] = p[0] + r*cos(j);
			point[1] = p[1] + r*sin(j);
			v pix = getPixel(point);
			drawPixel(pix[0], pix[1], color);
		}
	}
}

void TDraw3D::drawTriangle(v a, v b, v c, char color) {
// 	if(fill){
// 		v* s[3];
// 		if(a[1]<b[1]){
// 			if(a[1]<c[1]){
// 				s[0]=&a;
// 				if(b[1]<c[1]){s[1]=&b; s[2]=&c;}
// 			   	else {s[1]=&c; s[2]=&b;}
// 			} else {
// 				s[0]=&c;
// 				if(b[1]<a[1]){s[1]=&b; s[2]=&a;}
// 			   	else {s[1]=&a; s[2]=&b;}
// 			}
// 		} else {
// 			if(b[1]<c[1]){
// 				s[0]=&b;
// 				if(a[1]<c[1]){s[1]=&a; s[2]=&c;}
// 				else {s[1]=&c; s[2]=&a;}
// 			} else {
// 				s[0]=&c;
// 				if(a[1]<b[1]){s[1]=&a; s[2]=&b;}
// 				else {s[1]=&b; s[2]=&a;}
// 			}
// 		}
// 
// 
// 		if((*s[1])[0]>(*s[2])[0]) std::swap(s[1], s[2]);
// 
// 		if((*s[1])[1]==(*s[2])[1]) fillTriangle(s[0], s[1], s[2], col);
// 		else if((*s[0])[1]==(*s[1])[1]) fillTriangle(s[2], s[0], s[1], col);
// 		else {
// 			v a1,a2, a3,a4;
// 			a1 = (*s[0]);
// 			if((*s[1])[1]<(*s[2])[1]){ a2=(*s[2]); a3=(*s[1]);} else {a2=(*s[1]); a3=(*s[2]);}
// 			a4 = v(WIDTH,a3[1],1,1);
// 			int denom = ((a1[0]-a2[0])*(a3[1]-a4[1])-(a1[1]-a2[1])*(a3[0]-a4[0]));
// 			int x=WIDTH/2,y=HEIGHT/2;
// 			if(denom!=0){
// 				x = ((a1[0]*a2[1]-a1[1]*a2[0])*(a3[0]-a4[0])-(a1[0]-a2[0])*(a3[0]*a4[1]-a3[1]*a4[0]))/denom;
// 				y = ((a1[0]*a2[1]-a1[1]*a2[0])*(a3[1]-a4[1])-(a1[1]-a2[1])*(a3[0]*a4[1]-a3[1]*a4[0]))/denom;
// 			}
// 			v m(x,y,a1[2],1);
// 			fillTriangle(&a1, &a3, &m, col);
// 			fillTriangle(&a2, &a3, &m, col);
// 		}
// 	} else {
		TDraw::drawLine( a[0], a[1], b[0], b[1], color );
		TDraw::drawLine( b[0], b[1], c[0], c[1], color );
		TDraw::drawLine( c[0], c[1], a[0], a[1], color );
// 	}
}

v TDraw3D::getPixel(const v& p) {
	V a(p[0],p[1],p[2],1);
	a = mvpM*a;
	v b(a[0],a[1],a[2]);
	b/=a[3];
	b[0] = GetScreenWidth()/2*b[0] + GetScreenWidth()/2;
	b[1] = GetScreenHeight()/2*b[1] + GetScreenHeight()/2;
	b[2] = (far-near)/2*b[2] + (far+near)/2;
	return b;
}

void TDraw3D::setPixel(const v & v, char color) {
	//if( v[0] < 0 or v[0] > GetScreenWidth() or v[1] < 0 or v[1] > GetScreenHeight() or v[2] < near or v[2] > far) return;
	drawPixel( (int)v[0], (int)v[1], color );
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
		std::vector<v> shape;
		for( size_t f = 0; f < model.shapes[s].mesh.num_face_vertices.size(); ++f ){
			size_t fv = model.shapes[s].mesh.num_face_vertices[f];
			for( size_t v = 0; v < fv ; ++v ){
				/*tinyobj::index_t idx = model.shapes[s].mesh.indices[index_offset+v];
				float vx = model.attrib.vertices[3*idx.vertex_index+0];
				float vy = model.attrib.vertices[3*idx.vertex_index+1];
				float vz = model.attrib.vertices[3*idx.vertex_index+2];
				V vert(vx,vy,vz,1.0);
				v ver = getPixel( V(vx,vy,vz,1.0) );
				shape.push_back( ver );
				setPixel( ver, model.color );
				setPixel(V( ver[0]+1, ver[1], ver[2], ver[3]), model.color );
				setPixel(V( ver[0]-1, ver[1], ver[2], ver[3]), model.color );
				setPixel(V( ver[0], ver[1]+1, ver[2], ver[3]), model.color );
				setPixel(V( ver[0], ver[1]-1, ver[2], ver[3]), model.color );
			*/}
			for(size_t sh=0; sh<shape.size()/2; ++sh)
				TDraw::drawLine( shape[sh][0], shape[sh][1], shape[sh+1][0], shape[sh+1][1], model.color);
			index_offset+=fv;
		}
	}

}
