#ifndef DEBUG_HPP
#define DEBUG_HPP
#include <cstdio>
#include <cstdlib>
//#define DEBUG
#ifdef DEBUG
#define deb(arg,arg2) fprintf(stderr,(arg),(arg2));
#define line fprintf(stderr,"I: %s : %d\n",__FILE__,__LINE__);
#define Eline(arg) fprintf(stderr,"E: %s : %d : %s \n",__FILE__,__LINE__,arg);
#define Eline2(arg,arg2) fprintf(stderr,"E: %s : %d : %s %s\n",__FILE__,__LINE__,arg,arg2);
#define info(arg) fprintf(stderr,"I: %s : %d : %s\n",__FILE__,__LINE__,arg);
#define info2(arg,arg2) fprintf(stderr,"I: %s : %d : %s => %s\n",__FILE__,__LINE__,arg,arg2);
#else
#define info(arg)
#define deb(arg,arg2)
#define line
#define Eline
#define Eline2(arg1,arg2);
#define info2(arg,arg2)
#endif

#endif//DEBUG_HPP
