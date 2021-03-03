







 

 

 
 

  #pragma system_include

 
 

 

  #pragma system_include






 















 












 






 
 


  #pragma system_include

 
 

 

  #pragma system_include






 















 





                 



  

                 













 

   



                 



























 


  #pragma system_include

 
 
 


  #pragma system_include

 

 

 

 

   

 

   #pragma system_include






 




 

 


 


 

 

 

 

 

 

 

 

 

 





 


  typedef struct
  {
    char *_Ap;
  } _VA_LIST;


















 












 




















 










 




















































































































 


 










 





















 















 













 








 












 











 










 









 











 









 









 









 









 














 














 
















 












 








 











 










 









 









 









 









 









 









 









 









 







 




 









 




 





 
















































 













 






 


   

  







 





 





 




 



 





 




 



 












 
   






 
  #pragma language = save 
  #pragma language = extended
  #pragma language = restore






 





 




 





 








                 




















 


                 

 

 
 

 

 

 

 



                 






 
 


  #pragma system_include

 
 

 

  #pragma system_include






 















 




 
 





 

                                 













                 

                 

                 

                 
                 



                 
                                 


  #pragma language=save
  #pragma language=extended
  typedef long long _Longlong;
  typedef unsigned long long _ULonglong;
  #pragma language=restore

  typedef unsigned short int _Wchart;
  typedef unsigned short int _Wintt;



                 

typedef signed int  _Ptrdifft;
typedef unsigned int     _Sizet;

 

                 
  typedef _VA_LIST __Va_list;


__intrinsic __nounwind void __iar_Atexit(void (*)(void));


  typedef struct
  {        
    unsigned int _Wchar;
    unsigned int _State;
  } _Mbstatet;



  typedef struct __FILE _Filet;


typedef struct
{        
  _Longlong _Off;     
  _Mbstatet _Wstate;
} _Fpost;




                 














 


  #pragma system_include





 






















































































 

 


  
 

   


  





  #pragma language=save 
  #pragma language=extended
  __intrinsic __nounwind void __iar_dlib_perthread_initialize(void  *);
  __intrinsic __nounwind void  *__iar_dlib_perthread_allocate(void);
  __intrinsic __nounwind void __iar_dlib_perthread_destroy(void);
  __intrinsic __nounwind void __iar_dlib_perthread_deallocate(void  *);



  #pragma segment = "__DLIB_PERTHREAD" 
  #pragma segment = "__DLIB_PERTHREAD_init" 









   
  void  *__iar_dlib_perthread_access(void  *);
  #pragma language=restore








     
  



   
    __intrinsic __nounwind void __iar_Locale_lconv_init(void);

  


  
  typedef void *__iar_Rmtx;
  
  
  __intrinsic __nounwind void __iar_system_Mtxinit(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxdst(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxlock(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxunlock(__iar_Rmtx *m);

  __intrinsic __nounwind void __iar_file_Mtxinit(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxdst(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxlock(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxunlock(__iar_Rmtx *m);

  
 
  __intrinsic __nounwind void __iar_clearlocks(void);



  



  


  typedef unsigned _Once_t;

  








                 


                 
  
  
    
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Locale(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_StaticGuard(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock(unsigned int);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Locale(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_StaticGuard(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock(unsigned int);

  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Initdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Dstdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Lockdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlockdynamicfilelock(__iar_Rmtx *);
  
  

                 


                 







 
 
 


  #pragma system_include

 
 

 

  #pragma system_include






 















 






                 
typedef _Sizet size_t;

typedef unsigned int __data_size_t;









    typedef unsigned int __time32_t;
    typedef unsigned int clock_t;

  #pragma language=save
  #pragma language=extended
  typedef signed long long __time64_t;
  #pragma language=restore


  typedef __time32_t time_t;

struct tm
{        
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  int __BSD_bug_filler1;
  int __BSD_bug_filler2;
};



        
__intrinsic __nounwind time_t time(time_t *);
__intrinsic __nounwind __time32_t __time32(__time32_t *);
  __intrinsic __nounwind __time64_t __time64(__time64_t *);


      
__intrinsic __nounwind char * asctime(const struct tm *);
__intrinsic __nounwind clock_t clock(void);
__intrinsic __nounwind char * ctime(const time_t *);
_Pragma("function_effects = no_state, no_errno") __intrinsic __nounwind double difftime(time_t, time_t);
__intrinsic __nounwind struct tm * gmtime(const time_t *);
__intrinsic __nounwind struct tm * localtime(const time_t *);
__intrinsic __nounwind time_t mktime(struct tm *);


__intrinsic __nounwind char * __ctime32(const __time32_t *);
_Pragma("function_effects = no_state, no_errno") __intrinsic __nounwind double __difftime32(__time32_t, __time32_t);
__intrinsic __nounwind struct tm * __gmtime32(const __time32_t *);
__intrinsic __nounwind struct tm * __localtime32(const __time32_t *);
__intrinsic __nounwind __time32_t __mktime32(struct tm *);
  __intrinsic __nounwind char * __ctime64(const __time64_t *);
  _Pragma("function_effects = no_state, no_errno") __intrinsic __nounwind double __difftime64(__time64_t, __time64_t);
  __intrinsic __nounwind struct tm * __gmtime64(const __time64_t *);
  __intrinsic __nounwind struct tm * __localtime64(const __time64_t *);
  __intrinsic __nounwind __time64_t __mktime64(struct tm *);
__intrinsic __nounwind size_t strftime(char *, size_t, const char *,
                             const struct tm *);

                                                         


 

  #pragma inline=forced
  time_t time(time_t *p)
  {
      return __time32(p);
  }

  #pragma inline=forced
  char * ctime(const time_t *p)
  {
      return __ctime32(p);
  }

  #pragma inline=forced
  double difftime(time_t t1, time_t t2)
  {
      return __difftime32(t1, t2);
  }

  #pragma inline=forced
  struct tm * gmtime(const time_t *p)
  {
      return __gmtime32(p);
  }

  #pragma inline=forced
  struct tm * localtime(const time_t *p)
  {
      return __localtime32(p);
  }

  #pragma inline=forced
  time_t mktime(struct tm *p)
  {
      return __mktime32(p);
  }









 








 







 








 
 

 














 


 

  #pragma system_include

 
 

 

  #pragma system_include






 















 











__intrinsic __nounwind void __aeabi_assert(char const *, char const *, int);
__intrinsic __nounwind int  __iar_ReportAssert(const char *, const char *,
                                    const char *, const char *);
__intrinsic __nounwind void __iar_EmptyStepPoint(void);
__intrinsic __nounwind void __iar_PrintAssert(const char*);




    
 











 






 
typedef struct _nmeaTIME
{
    int     year;        
    int     mon;         
    int     day;         
    int     hour;        
    int     min;         
    int     sec;         
    int     hsec;        

} nmeaTIME;



 
void nmea_time_now(nmeaTIME *t);



 
 





 


void nmea_time_now(nmeaTIME *stm)
{
    time_t lt;
    struct tm *tt;

    time(&lt);
    tt = gmtime(&lt);
    stm->year = tt->tm_year;
    stm->mon = tt->tm_mon;
    stm->day = tt->tm_mday;
    stm->hour = tt->tm_hour;
    stm->min = tt->tm_min;
    stm->sec = tt->tm_sec;
    stm->hsec = 0;
}

