#include <iostream>
#include "Matrix.h"

int main() {

	vec3 v;
	v = {1,2,3};

	std::cout << v[2] << std::endl;

	mat2 a;

	a = {
		{1,2},
		{2,3}
	};

	mat4 m, n;
	
	m = { 
		{2,3,4,2}, 
		{1,2,7,1}, 
		{3,4,5,6}, 
		{9,8,7,8} 
	};

	n = { 
		{4,5,1,2}, 
		{2,7,8,6}, 
		{9,1,2,4}, 
		{7,1,1,2} 
	};

	char str[200];

	memset(str, 0, sizeof(str));
	(a).toString(str,1);
	std::cout << str << std::endl << std::endl;
	
    
	std::cout << "Matrices: " << std::endl;
	memset(str, 0, sizeof(str));
	(m).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	memset(str, 0, sizeof(str));
	(n).toString(str,1);
	std::cout << str << std::endl << std::endl;

    mat4 add = m+n;
	std::cout << "Addition: " << std::endl;
	memset(str, 0, sizeof(str));
	(add).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	
	std::cout << "Matrices: " << std::endl;
	memset(str, 0, sizeof(str));
	(m).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	memset(str, 0, sizeof(str));
	(n).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	std::cout << "Multiplication: " << std::endl;
	memset(str, 0, sizeof(str));
	(m*n).toString(str,1);
	std::cout << str << std::endl << std::endl;

    
	std::cout << "Matrices: " << std::endl;
	memset(str, 0, sizeof(str));
	(m).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	memset(str, 0, sizeof(str));
	(n).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
    
	std::cout << "Transposition: " << std::endl;
	memset(str, 0, sizeof(str));
	(~(m*n)).toString(str,1);
	std::cout << str << std::endl << std::endl;
	
    
	std::cout << "Matrices: " << std::endl;
	memset(str, 0, sizeof(str));
	(m).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	memset(str, 0, sizeof(str));
	(n).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	std::cout << "Identity: " << std::endl;
	memset(str, 0, sizeof(str));
	(mat4()).toString(str,1);
	std::cout << str << std::endl << std::endl;
    
	std::cout << "Matrix * Vector " << std::endl;
	memset(str, 0, sizeof(str));
	(m*n[0]).toString(str,1);
	std::cout << str << std::endl << std::endl;
	
	vec3 v1(m[0][0],m[0][1],m[0][2]);
	vec3 v2(n[0][0],n[0][1],n[0][2]);

	std::cout << "Vector & Vector " << std::endl;
	memset(str, 0, sizeof(str));
	(v1).toString(str,1);
	std::cout << str << std::endl << std::endl;
	memset(str, 0, sizeof(str));
	(v2).toString(str,1);
	std::cout << str << std::endl << std::endl;
	memset(str, 0, sizeof(str));
	(v1&v2).toString(str,1);
	std::cout << str << std::endl << std::endl;
	
	std::cout << "Vector Normal " << std::endl;
	memset(str, 0, sizeof(str));
	(v1.normalize()).toString(str);
	std::cout << str << std::endl << std::endl;
	
	return 0;
}
