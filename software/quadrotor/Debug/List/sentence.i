







 







 

 








 









 







 








 
 

 














 


 

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










 
typedef struct _nmeaPOS
{
    double lat;          
    double lon;          

} nmeaPOS;





 
typedef struct _nmeaSATELLITE
{
    int     id;          
    int     in_use;      
    int     elv;         
    int     azimuth;     
    int     sig;         

} nmeaSATELLITE;





 
typedef struct _nmeaSATINFO
{
    int     inuse;       
    int     inview;      
    nmeaSATELLITE sat[(12)];  

} nmeaSATINFO;






 
typedef struct _nmeaINFO
{
    int     smask;       

    nmeaTIME utc;        

    int     sig;         
    int     fix;         

    double  PDOP;        
    double  HDOP;        
    double  VDOP;        

    double  lat;         
    double  lon;         
    double  elv;         
    double  speed;       
    double  direction;   
    double  declination;  

    nmeaSATINFO satinfo;  

} nmeaINFO;

extern nmeaINFO info;
void nmea_zero_INFO(nmeaINFO *info);






 
enum nmeaPACKTYPE
{
    GPNON   = 0x0000,    
    GPGGA   = 0x0001,    
    GPGSA   = 0x0002,    
    GPGSV   = 0x0004,    
    GPRMC   = 0x0008,    
    GPVTG   = 0x0010     
};



 
typedef struct _nmeaGPGGA
{
    nmeaTIME utc;        
	double  lat;         
    char    ns;          
	double  lon;         
    char    ew;          
    int     sig;         
	int     satinuse;    
    double  HDOP;        
    double  elv;         
    char    elv_units;   
    double  diff;        
    char    diff_units;  
    double  dgps_age;    
    int     dgps_sid;    

} nmeaGPGGA;



 
typedef struct _nmeaGPGSA
{
    char    fix_mode;    
    int     fix_type;    
    int     sat_prn[(12)];  
    double  PDOP;        
    double  HDOP;        
    double  VDOP;        

} nmeaGPGSA;



 
typedef struct _nmeaGPGSV
{
    int     pack_count;  
    int     pack_index;  
    int     sat_count;   
    nmeaSATELLITE sat_data[(4)];

} nmeaGPGSV;



 
typedef struct _nmeaGPRMC
{
    nmeaTIME utc;        
    char    status;      
	double  lat;         
    char    ns;          
	double  lon;         
    char    ew;          
    double  speed;       
    double  direction;   
    double  declination;  
    char    declin_ew;   
    char    mode;        

} nmeaGPRMC;



 
typedef struct _nmeaGPVTG
{
    double  dir;         
    char    dir_t;       
    double  dec;         
    char    dec_m;       
    double  spn;         
    char    spn_n;       
    double  spk;         
    char    spk_k;       

} nmeaGPVTG;

void nmea_zero_GPGGA(nmeaGPGGA *pack);
void nmea_zero_GPGSA(nmeaGPGSA *pack);
void nmea_zero_GPGSV(nmeaGPGSV *pack);
void nmea_zero_GPRMC(nmeaGPRMC *pack);
void nmea_zero_GPVTG(nmeaGPVTG *pack);



 
 

  #pragma system_include

 
 

 

  #pragma system_include






 















 




 
 


  #pragma system_include

 
 

 

  #pragma system_include






 















 






                 
typedef _Sizet size_t;

typedef unsigned int __data_size_t;













 


  #pragma system_include


  
  

  





 


  




 




  


 

  #pragma inline=no_body
  __intrinsic __nounwind void * memcpy(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memcpy(_D, _S, _N);
    return _D;
  }

  #pragma inline=no_body
  __intrinsic __nounwind void * memmove(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memmove(_D, _S, _N);
    return _D;
  }

  #pragma inline=no_body
  __intrinsic __nounwind void * memset(void * _D, int _C, size_t _N)
  {
    __aeabi_memset(_D, _N, _C);
    return _D;
  }

  
  




                 

                 

_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        memcmp(const void *, const void *,
                                                size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind void *     memcpy(void *, 
                                                const void *, size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind void *     memmove(void *, const void *, size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), returns 1")    __intrinsic __nounwind void *     memset(void *, int, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(2), returns 1")    __intrinsic __nounwind char *     strcat(char *, 
                                                const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        strcmp(const char *, const char *);
_Pragma("function_effects = no_write(1,2)")     __intrinsic __nounwind int        strcoll(const char *, const char *);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind char *     strcpy(char *, 
                                                const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind size_t     strcspn(const char *, const char *);
                 __intrinsic __nounwind char *     strerror(int);
_Pragma("function_effects = no_state, no_errno, no_write(1)")      __intrinsic __nounwind size_t     strlen(const char *);
_Pragma("function_effects = no_state, no_errno, no_write(2), returns 1")    __intrinsic __nounwind char *     strncat(char *, 
                                                 const char *, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        strncmp(const char *, const char *, 
                                                 size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind char *     strncpy(char *, 
                                                 const char *, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind size_t     strspn(const char *, const char *);
_Pragma("function_effects = no_write(2)")        __intrinsic __nounwind char *     strtok(char *, 
                                                const char *);
_Pragma("function_effects = no_write(2)")        __intrinsic __nounwind size_t     strxfrm(char *, 
                                                 const char *, size_t);

  _Pragma("function_effects = no_write(1)")      __intrinsic __nounwind char *   strdup(const char *);
  _Pragma("function_effects = no_write(1,2)")   __intrinsic __nounwind int      strcasecmp(const char *, const char *);
  _Pragma("function_effects = no_write(1,2)")   __intrinsic __nounwind int      strncasecmp(const char *, const char *, 
                                                   size_t);
  _Pragma("function_effects = no_state, no_errno, no_write(2)")    __intrinsic __nounwind char *   strtok_r(char *, const char *, char **);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind size_t   strnlen(char const *, size_t);



  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind void *memchr(const void *_S, int _C, size_t _N);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *strchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *strpbrk(const char *_S, const char *_P);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *strrchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *strstr(const char *_S, const char *_P);


                 

                 

_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind void *__iar_Memchr(const void *, int, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *__iar_Strchr(const char *, int);
               __intrinsic __nounwind char *__iar_Strerror(int, char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *__iar_Strpbrk(const char *, const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *__iar_Strrchr(const char *, int);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *__iar_Strstr(const char *, const char *);


                 
                 
    #pragma inline
    void *memchr(const void *_S, int _C, size_t _N)
    {
      return (__iar_Memchr(_S, _C, _N));
    }

    #pragma inline
    char *strchr(const char *_S, int _C)
    {
      return (__iar_Strchr(_S, _C));
    }

    #pragma inline
    char *strpbrk(const char *_S, const char *_P)
    {
      return (__iar_Strpbrk(_S, _P));
    }

    #pragma inline
    char *strrchr(const char *_S, int _C)
    {
      return (__iar_Strrchr(_S, _C));
    }

    #pragma inline
    char *strstr(const char *_S, const char *_P)
    {
      return (__iar_Strstr(_S, _P));
    }

  #pragma inline
  char *strerror(int _Err)
  {
    return (__iar_Strerror(_Err, 0));
  }









 

void nmea_zero_GPGGA(nmeaGPGGA *pack)
{
    memset(pack, 0, sizeof(nmeaGPGGA));
    nmea_time_now(&pack->utc);
    pack->ns = 'N';
    pack->ew = 'E';
    pack->elv_units = 'M';
    pack->diff_units = 'M';
}

void nmea_zero_GPGSA(nmeaGPGSA *pack)
{
    memset(pack, 0, sizeof(nmeaGPGSA));
    pack->fix_mode = 'A';
    pack->fix_type = (1);
}

void nmea_zero_GPGSV(nmeaGPGSV *pack)
{
    memset(pack, 0, sizeof(nmeaGPGSV));
}

void nmea_zero_GPRMC(nmeaGPRMC *pack)
{
    memset(pack, 0, sizeof(nmeaGPRMC));
    nmea_time_now(&pack->utc);
    pack->status = 'V';
    pack->ns = 'N';
    pack->ew = 'E';
    pack->declin_ew = 'E';
}

void nmea_zero_GPVTG(nmeaGPVTG *pack)
{
    memset(pack, 0, sizeof(nmeaGPVTG));
    pack->dir_t = 'T';
    pack->dec_m = 'M';
    pack->spn_n = 'N';
    pack->spk_k = 'K';
}
