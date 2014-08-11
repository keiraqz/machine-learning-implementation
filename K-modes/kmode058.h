/**
 * @modified
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */
 
#ifndef KMODE058_H
#define KMODE058_H

class Kmode
{ 
	public:
		Kmode();
		~Kmode();
		void clustr ( double x[], double d[], double dev[], int b[], double f[], int e[], 
	 	 int i, int j, int n, int nz, int k );
		void timestamp ( );

};


#endif
