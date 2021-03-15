#include <stdio.h>
#include <pthread.h>
#include "math.h"
#include "class_args.h"
#include "functions.h"

using namespace std;

void * find_max_min(void * p1)
{
	args * a = (args * ) p1;
	int p = a->p;
	//int k = a->k;
	double max = 0.0;
	double min = 0.0;
	double c_0;
	char b;

    FILE * f = fopen(a->filename, "r");
    if (f == NULL)
    {  
        a->error = -1;
        reduce_sum (p, a);
        reduce_sum_of_count (p, a);
    	return 0;
        //return 0;
    }

	if( fscanf(f,"%lf", &c_0) != 1)
    {
    	fseek (f, -1 , SEEK_CUR);
    	if( fscanf(f,"%c", &b) != 1)
        {
            fclose(f);
            a->error = -2;
            reduce_sum (p, a);
            reduce_sum_of_count (p, a);
    		return 0;
            //return 0;
        }
    	while(b == ' ' ) 
        {
            if( fscanf(f,"%c", &b) != 1)
            {
                fclose(f);
                a->error = -2;
                reduce_sum (p, a);
                reduce_sum_of_count (p, a);
    			return 0;
                //return 0;
            }
        }
    	if(b == ' ' || b == '\n')
    	{ 
    		fclose(f);
    		a->error = -2; // empty file
            reduce_sum (p, a);
            reduce_sum_of_count (p, a);
    		return 0;
            //return 0;
    	} 
    	else
    	{ 
    		fclose(f);
    		a->error = -3;	// bad - contant file
            reduce_sum (p, a);
            reduce_sum_of_count (p, a);
    		return 0;
            //return 0;
    	} 
    }

    max = c_0;
    min = c_0;

    while(!feof(f))
    {

    	if( fscanf(f,"%lf", &c_0) != 1)
   	 	{
    		fseek (f, -1 , SEEK_CUR);
    		if( fscanf(f,"%c", &b) != 1)
            {
                fclose(f);
                a->error = -10;
                reduce_sum (p, a);
                reduce_sum_of_count (p, a);
    			return 0;
                //return 0;
            }
            while(b == ' ' ) 
            {
                if( fscanf(f,"%c", &b) != 1)
                {
                   fclose(f);
                   a->error = -10;
                   reduce_sum (p, a);
                   reduce_sum_of_count (p, a);
    				return 0;
                   //return 0;
                }
            }
    		if(b == ' ' || b == '\n')
    		{ 
    			fclose(f);
    			break; // end of file
    		}
    		else 
    		{
    			fclose(f);
    			a->error = -3; 
                reduce_sum (p, a);
                reduce_sum_of_count (p, a);
    			return 0;
                //return 0;
    		}// bad - contant file
   	 	}

   	 	if( c_0 > max ) max = c_0;
    	if( c_0 < min ) min = c_0;

    }

    a->max = max;
    a->min = min;
    a->min_max_sum = max + min;

    //printf("For main file %s min_max_sum / 2 %lf\n", a->filename, a->min_max_sum / 2.0);

    reduce_sum (p, a);

    if(a->error != 0)
    {
    	reduce_sum_of_count (p, a);
    	return 0;
    }

    //args * a = (args * ) p1;
    //int p = a->p;
    //int k = a->k;
    f = fopen(a->filename, "r");
    int count = 0;
    //double c_0;
    double min_max_sum = a->min_max_sum;
   //char b;

    if( fscanf(f,"%lf", &c_0) != 1)
    {
        fseek (f, -1 , SEEK_CUR);
        if( fscanf(f,"%c", &b) != 1)
        {
            fclose(f);
            a->error = -2;
        }
        while(b == ' ' ) 
        {
            if( fscanf(f,"%c", &b) != 1)
            {
                fclose(f);
                a->error = -2;
            }
        }
        if(b == ' ' || b == '\n')
        { 
            fclose(f);
            a->error = -2; // empty file
        } 
        else
        { 
            fclose(f);
            a->error = -3;  // bad - contant file
        } 
    }

    if(c_0 > min_max_sum) count++;

    while(!feof(f))
    {

        if( fscanf(f,"%lf", &c_0) != 1)
        {
            fseek (f, -1 , SEEK_CUR);
            if( fscanf(f,"%c", &b) != 1)
            {
                fclose(f);
                a->error = -10;
            }
            while(b == ' ' ) 
            {
                if( fscanf(f,"%c", &b) != 1)
                {
                   fclose(f);
                   a->error = -10;
                }
            }
            if(b == ' ' || b == '\n')
            { 
                fclose(f);
                break; // end of file
            }
            else 
            {
                fclose(f);
                a->error = -3; 
            }// bad - contant file
        }

        if(c_0 > min_max_sum) count++;

    }

    a->count = count;

    //printf("Count for file %s for thread number is %d\n", a->filename, a->count);

    reduce_sum_of_count (p, a);

    return 0;
}


int main( int argc, char * argv[])
{
	int k, p;
	args *a;
	pthread_t *tids;
    int flag = 0;

	if (argc <= 1)
  	{
    	printf ("Usage: %s <instances>\n", argv[0]);
    	return 1;
  	}

  	p = argc - 1;
	
	a = new args[p];
	
	if(!a)
	{
		printf("Can't allocate memory \n");
		return 2;
	}
	
	for(k = 0; k < p; k++)
	{
		a[k].k = k;
		a[k].p = p;
		a[k].filename = argv[1 + k];
        a[k].max = 0.0;
        a[k].min = 0.0;
        a[k].error = 0;
        a[k].count = 0;
        a[k].min_max_sum = 0.0;
	}
	
	tids = new pthread_t[p];
	
	if(!tids)
	{
		printf("Can't allocate tids \n");
		delete [] a;
		return 3;
	}
	
	for(k = 0; k < p; k++)
	{
		flag = pthread_create(&tids[k], 0, &find_max_min, a + k);
        if( flag != 0 ) printf("Can't create thread %d for task 'find_max_min'\n", k);
	} // p + 1 potok

	for(k = 0; k < p; k ++)
	{
		if(pthread_join(tids[k], 0))
            fprintf(stderr, "Can't wait thread %d for task 'find_max_min'\n", k);
        if(a[k].error != 0)
        {
            if(a[k].error == -1) printf ("Can't open file %s\n", a[k].filename);
            if(a[k].error == -2) printf ("Empty file %s \n", a[k].filename);
            if(a[k].error == -3) printf ("File %s is bad - contant file\n", a[k].filename);
            if(a[k].error == -10) printf ("Unknown error in %s\n", a[k].filename);
            flag = a[k].error;
        }
	}

    if(flag == 0 ) printf("Total count is %d\n", a[0].count);

	delete [] a;
	delete [] tids;	

	if(flag != 0 )
    {
        return flag;
    }

    return 0;
}

// g++ file.cpp -lpthread
