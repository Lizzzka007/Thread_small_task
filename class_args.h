#ifndef __CLASS_ARGS_H__
#define __CLASS_ARGS_H__
using namespace std;

class args{
	public:
        int p = 0;
        int k = 0;
        int count = 0;
        double max = 0.0;
        double min = 0.0;
        double min_max_sum = 0.0;
        int error = 0;
        const char * filename = "a.txt";
	args() = default;
};
 #endif 
