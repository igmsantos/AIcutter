#include <stdio.h> 
#include <math.h>
main(){
/*
	int count = 100;
	float steepness = (float)1 / count;
	//printf("%f\n", sqrt(0.223607));
	printf("%d\n", 'Z' - 'A');

	while(count >=0){
		if(!(count%25))
			printf("%f\n", sqrt(steepness*count));
		count--;
	}
	*/
	
	float s, d, y, x; 

	s = 0.0;	
	FILE *steepness_file;
	char steepness_filename[FILENAME_MAX];
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/GAUSSIAN-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");	
	
		x = -40;
		while(x <= 40){
			y = exp((-1 * x) * s * x * s);
			d = -2 * x * s * y * s;
			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.05;
	}
	
	s = 0;
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/SIGMOID_SYMMETRIC-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");	
	
		x = -40;
		while(x <= 40){
			y = tanh(s*x); 
			d = s*(1-(y*y));
			
			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.1;
	}

	s = 0;
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/ELLIOT-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");

		x = -40;
		while(x <= 40){
			y = ( (x*s) / 2 ) / ( 1 + fabs(x*s) ) + 0.5;
			d =  s * 1 / ( 2 * ( 1 + fabs(x*s)) * ( 1 + fabs(x*s)) );

			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.1;
	}

	s = 0;
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/SIGMOID-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");

		x = -40;
		while(x <= 40){
			y = 1/(1 + exp(-2*s*x));
			d = 2*s*y*(1 - y);

			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.1;
	}

	s = 0;
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/SIN-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");

		x = -40;
		while(x <= 40){
			y = sin(x*s);
			d = s*cos(x*s);
			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.1;
	}

	s = 0;
	while(s <= 1){
		sprintf(steepness_filename, "/tmp/COS-%1.2f\0",s);
		steepness_file = fopen(steepness_filename, "w");

		x = -40;
		while(x <= 40){
			y = cos(x*s);
			d = s*-sin(x*s);

			//printf("%f %1.1f %1.10f %1.10f\n", s, x, y, d);
			fprintf(steepness_file, "%1.1f %1.10f\n", x, y);
			x += 0.1;
		}
		fclose(steepness_file);
		s += 0.1;
	}

}
