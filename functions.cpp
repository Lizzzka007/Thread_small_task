#include "class_args.h"
#include "functions.h"
#include <stdio.h>
#include <pthread.h>
using namespace std;



void reduce_sum (int p, args *a)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static args *p_a = 0;
    
    pthread_mutex_lock(&m);
    if(!p_a) 
    {
        p_a = a;
        p_a->min_max_sum = a->min_max_sum;
        //printf("head thread number %d\n", a->k);
    }
    else 
    {
        p_a->min_max_sum += a->min_max_sum;
        //printf("another one thread number %d \n", a->k);
    }
    t_in ++;
    if(t_in >= p)
    {
        t_out = 0;
        pthread_cond_broadcast(&c_in);

        if(t_in == p)
        {
        	//printf("p_a->min_max_sum is %lf and (2 * p) is %d\n\n", p_a->min_max_sum, (2 * p));
            p_a->min_max_sum = p_a->min_max_sum / (2 * p);
            //printf("p_a->min_max_sum is %lf \n\n", p_a->min_max_sum);
        }
    }
    else while(t_in < p) pthread_cond_wait(&c_in, &m);

    //printf("a->min_max_sum = p_a->min_max_sum is %lf \n\n", p_a->min_max_sum);
    
    a->min_max_sum = p_a->min_max_sum;
    
    t_out ++;
    
    if(t_out >= p)
    {
        t_in = 0;
        p_a = 0;
        pthread_cond_broadcast(&c_out);
    }
    else while(t_out < p) pthread_cond_wait(&c_out, &m);
    
    pthread_mutex_unlock(&m);
    
    
}



void reduce_sum_of_count (int p, args *a)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static args *p_a = 0;
    
    pthread_mutex_lock(&m);

    if(!p_a) 
    {
        p_a = a;
    }
    else 
    {
        p_a->count += a->count;
    }
    t_in ++;
    if(t_in >= p)
    {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    }
    else while(t_in < p) pthread_cond_wait(&c_in, &m);
    
    a->count = p_a->count;
    
    t_out ++;
    
    if(t_out >= p)
    {
        t_in = 0;
        p_a = 0;
        pthread_cond_broadcast(&c_out);
    }
    else while(t_out < p) pthread_cond_wait(&c_out, &m);
    
    pthread_mutex_unlock(&m);
    
    
}

