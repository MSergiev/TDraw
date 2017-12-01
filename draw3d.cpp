#include "TDraw3D.hpp"

#define FPS 20

TDraw3D draw;

int main() {

	draw.init();

    //Model cat, tree;
	//draw.loadFromFile( cat, "cat.obj" );
	//draw.loadFromFile( tree, "tree.obj" );

	while( true ) {
        
        //draw.drawObject( cat );
        //draw.drawObject( tree );

		
 		double rad = 0.3;
		int lim = 50;
		for( int i = -lim; i < lim; i+=10 ) {
			for( int j = -lim; j < lim; j+=10 ) {
				draw.cube( v(i*rad, j*rad, 10), rad*2, (i+j)%3 + 2 );
			}
		}
	
		//draw.drawConic( ORIGIN, 0.2, 0.5, 0.2, 3 );	
		//draw.draw3DLine( ORIGIN, OX, 1);
		//draw.draw3DLine( ORIGIN, OY, 2);
		//draw.draw3DLine( ORIGIN, OZ, 3);
		//draw.draw3DLine( ORIGIN, V(1,0,0,-1), 1);
		//draw.draw3DLine( ORIGIN, V(0,1,0,-1), 2);		
		//draw.draw3DLine( ORIGIN, V(0,0,1,-1), 3);		

		//draw.cube( ORIGIN, 0.2, 4 );
		/*draw.sphere(z, rad*2, 0.2, 4);
		draw.cube( a, rad, 2 );
 		draw.cube( b, rad, 3 );
 		draw.cube( c, rad, 1 );
 		draw.cube( d, rad, 1 );
		*/
		draw.draw();
			
		int input = getch();

		if( input == 'q' or input == 'Q' or input == KEY_EXIT ) break;
		else {
			double f = 0.1;
			if( input == KEY_LEFT )		draw.move( f, 0, 0 ); 
			if( input == KEY_RIGHT )	draw.move(-f, 0, 0 ); 
			if( input == KEY_UP )		draw.move( 0, 0, f );
			if( input == KEY_DOWN )		draw.move( 0, 0,-f );
			if( input == 'r' )			draw.move( 0, f, 0 );
			if( input == 'f' )			draw.move( 0,-f, 0 );

			f/=10;
			if( input == 'w' ) draw.rotate(-f, 0, 0 );
			if( input == 's' ) draw.rotate( f, 0, 0 );
			if( input == 'a' ) draw.rotate( 0, f, 0 );
			if( input == 'd' ) draw.rotate( 0,-f, 0 );
		}
	}

	draw.deinit();

	return 0;
}
