





 







 








 









 







 








 
 

 














 


 

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







 

double nmea_degree2radian(double val);
double nmea_radian2degree(double val);



 

double nmea_ndeg2degree(double val);
double nmea_degree2ndeg(double val);

double nmea_ndeg2radian(double val);
double nmea_radian2ndeg(double val);



 

double nmea_calc_pdop(double hdop, double vdop);
double nmea_dop2meters(double dop);
double nmea_meters2dop(double meters);



 

void nmea_info2pos(const nmeaINFO *info, nmeaPOS *pos);
void nmea_pos2info(const nmeaPOS *pos, nmeaINFO *info);

double  nmea_distance(
        const nmeaPOS *from_pos,
        const nmeaPOS *to_pos
        );

double  nmea_distance_ellipsoid(
        const nmeaPOS *from_pos,
        const nmeaPOS *to_pos,
        double *from_azimuth,
        double *to_azimuth
        );

int     nmea_move_horz(
        const nmeaPOS *start_pos,
        nmeaPOS *end_pos,
        double azimuth,
        double distance
        );

int     nmea_move_horz_ellipsoid(
        const nmeaPOS *start_pos,
        nmeaPOS *end_pos,
        double azimuth,
        double distance,
        double *end_azimuth
        );








 








 

 






 
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




int     nmea_generate(
        char *buff, int buff_sz,     
        const nmeaINFO *info,        
        int generate_mask            
        );

int     nmea_gen_GPGGA(char *buff, int buff_sz, nmeaGPGGA *pack);
int     nmea_gen_GPGSA(char *buff, int buff_sz, nmeaGPGSA *pack);
int     nmea_gen_GPGSV(char *buff, int buff_sz, nmeaGPGSV *pack);
int     nmea_gen_GPRMC(char *buff, int buff_sz, nmeaGPRMC *pack);
int     nmea_gen_GPVTG(char *buff, int buff_sz, nmeaGPVTG *pack);

void    nmea_info2GPGGA(const nmeaINFO *info, nmeaGPGGA *pack);
void    nmea_info2GPGSA(const nmeaINFO *info, nmeaGPGSA *pack);
void    nmea_info2GPRMC(const nmeaINFO *info, nmeaGPRMC *pack);
void    nmea_info2GPVTG(const nmeaINFO *info, nmeaGPVTG *pack);

int     nmea_gsv_npack(int sat_count);
void    nmea_info2GPGSV(const nmeaINFO *info, nmeaGPGSV *pack, int pack_idx);








 






 

struct _nmeaGENERATOR;

enum nmeaGENTYPE
{
    NMEA_GEN_NOISE = 0,
    NMEA_GEN_STATIC,
    NMEA_GEN_ROTATE,

    NMEA_GEN_SAT_STATIC,
    NMEA_GEN_SAT_ROTATE,
    NMEA_GEN_POS_RANDMOVE,

    NMEA_GEN_LAST
};

struct _nmeaGENERATOR * nmea_create_generator(int type, nmeaINFO *info);
void    nmea_destroy_generator(struct _nmeaGENERATOR *gen);

int     nmea_generate_from(
        char *buff, int buff_sz,     
        nmeaINFO *info,              
        struct _nmeaGENERATOR *gen,  
        int generate_mask            
        );



 

typedef int (*nmeaNMEA_GEN_INIT)(struct _nmeaGENERATOR *gen, nmeaINFO *info);
typedef int (*nmeaNMEA_GEN_LOOP)(struct _nmeaGENERATOR *gen, nmeaINFO *info);
typedef int (*nmeaNMEA_GEN_RESET)(struct _nmeaGENERATOR *gen, nmeaINFO *info);
typedef int (*nmeaNMEA_GEN_DESTROY)(struct _nmeaGENERATOR *gen);

typedef struct _nmeaGENERATOR
{
    void                *gen_data;
    nmeaNMEA_GEN_INIT    init_call;
    nmeaNMEA_GEN_LOOP    loop_call;
    nmeaNMEA_GEN_RESET   reset_call;
    nmeaNMEA_GEN_DESTROY destroy_call;
    struct _nmeaGENERATOR *next;

} nmeaGENERATOR;

int     nmea_gen_init(nmeaGENERATOR *gen, nmeaINFO *info);
int     nmea_gen_loop(nmeaGENERATOR *gen, nmeaINFO *info);
int     nmea_gen_reset(nmeaGENERATOR *gen, nmeaINFO *info);
void    nmea_gen_destroy(nmeaGENERATOR *gen);
void    nmea_gen_add(nmeaGENERATOR *to, nmeaGENERATOR *gen);










 





typedef void (*nmeaTraceFunc)(const char *str, int str_size);
typedef void (*nmeaErrorFunc)(const char *str, int str_size);

typedef struct _nmeaPROPERTY
{
    nmeaTraceFunc   trace_func;
    nmeaErrorFunc   error_func;
    int             parse_buff_size;

} nmeaPROPERTY;

nmeaPROPERTY * nmea_property();

void nmea_trace(const char *str, ...);
void nmea_trace_buff(const char *buff, int buff_size);
void nmea_error(const char *str, ...);



 
 

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









 
 
 


  #pragma system_include

 
 

 

  #pragma system_include






 















 




 
 




 
#pragma rtmodel="__dlib_full_locale_support",   "1"




extern int __aeabi_MB_CUR_MAX(void);





                 



                 
  typedef _Wchart wchar_t;

typedef struct
{        
  int quot;
  int rem;
} div_t;

typedef struct
{        
  long quot;
  long rem;
} ldiv_t;

    #pragma language=save
    #pragma language=extended
    typedef struct
    {      
      _Longlong quot;
      _Longlong rem;
    } lldiv_t;
    #pragma language=restore

                 
  
__intrinsic __nounwind int atexit(void (*)(void));
  __intrinsic __noreturn __nounwind void _Exit(int) ;
__intrinsic __noreturn __nounwind void exit(int) ;
__intrinsic __nounwind char * getenv(const char *);
__intrinsic __nounwind int system(const char *);



             __intrinsic __noreturn __nounwind void abort(void) ;
_Pragma("function_effects = no_state, no_errno")     __intrinsic __nounwind int abs(int);
             __intrinsic __nounwind void * calloc(size_t, size_t);
_Pragma("function_effects = no_state, no_errno")     __intrinsic __nounwind div_t div(int, int);
             __intrinsic __nounwind void free(void *);
_Pragma("function_effects = no_state, no_errno")     __intrinsic __nounwind long labs(long);
_Pragma("function_effects = no_state, no_errno")     __intrinsic __nounwind ldiv_t ldiv(long, long);
    #pragma language=save
    #pragma language=extended
    _Pragma("function_effects = no_state, no_errno") __intrinsic __nounwind long long llabs(long long);
    _Pragma("function_effects = no_state, no_errno") __intrinsic __nounwind lldiv_t lldiv(long long, long long);
    #pragma language=restore
             __intrinsic __nounwind void * malloc(size_t);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind int mblen(const char *, size_t);
_Pragma("function_effects = no_read(1), no_write(2)") __intrinsic __nounwind size_t mbstowcs(wchar_t *, 
                                          const char *, size_t);
_Pragma("function_effects = no_read(1), no_write(2)") __intrinsic __nounwind int mbtowc(wchar_t *, const char *, 
                                     size_t);
             __intrinsic __nounwind int rand(void);
             __intrinsic __nounwind void srand(unsigned int);
             __intrinsic __nounwind void * realloc(void *, size_t);
_Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long strtol(const char *, 
                                      char **, int);
_Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind unsigned long strtoul(const char *, char **, int);
_Pragma("function_effects = no_read(1), no_write(2)") __intrinsic __nounwind size_t wcstombs(char *, 
                                          const wchar_t *, size_t);
_Pragma("function_effects = no_read(1)")    __intrinsic __nounwind int wctomb(char *, wchar_t);
    #pragma language=save
    #pragma language=extended
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long long strtoll(const char *, char **, int);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind unsigned long long strtoull(const char *, 
                                                          char **, int);
    #pragma language=restore




    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind unsigned long __iar_Stoul(const char *, char **, 
                                                        int);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind float         __iar_Stof(const char *, char **, long);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind double        __iar_Stod(const char *, char **, long);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long double   __iar_Stold(const char *, char **, 
                                                          long);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long          __iar_Stolx(const char *, char **, int, 
                                                        int *);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind unsigned long __iar_Stoulx(const char *, char **,
                                                         int, int *);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind float         __iar_Stofx(const char *, char **, 
                                                        long, int *);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind double        __iar_Stodx(const char *, char **, 
                                                        long, int *);
    _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long double   __iar_Stoldx(const char *, char **, 
                                                         long, int *);
      #pragma language=save
      #pragma language=extended
      _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind _Longlong   __iar_Stoll(const char *, char **, 
                                                        int);
      _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind _ULonglong   __iar_Stoull(const char *, char **, 
                                                          int);
      _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind _Longlong    __iar_Stollx(const char *, char **, 
                                                          int, int *);
      _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind _ULonglong   __iar_Stoullx(const char *, char **, 
                                                           int, int *);
      #pragma language=restore





typedef int _Cmpfun(const void *, const void *);

_Pragma("function_effects = no_write(1,2)") __intrinsic void * bsearch(const void *, 
                                                   const void *, size_t,
                                                   size_t, _Cmpfun *);
             __intrinsic void qsort(void *, size_t, size_t, 
                                               _Cmpfun *);
             __intrinsic void __qsortbbl(void *, size_t, size_t, 
                                                    _Cmpfun *);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind double atof(const char *);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind int atoi(const char *);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind long atol(const char *);
    #pragma language=save
    #pragma language=extended
    _Pragma("function_effects = no_write(1)") __intrinsic __nounwind long long atoll(const char *);
    #pragma language=restore
  _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind float strtof(const char *, 
                                         char **);
  _Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind long double strtold(const char *, char **);
_Pragma("function_effects = no_write(1), no_read(2)") __intrinsic __nounwind double strtod(const char *, 
                                        char **);
                                        
                                        
               __intrinsic __nounwind size_t __iar_Mbcurmax(void);

  _Pragma("function_effects = no_state, no_errno")     __intrinsic __nounwind int __iar_DLib_library_version(void);
  




  
  typedef void _Atexfun(void);
  

                 
    #pragma inline=no_body
    double atof(const char *_S)
    {       
      return (__iar_Stod(_S, 0, 0));
    }

    #pragma inline=no_body
    int atoi(const char *_S)
    {       
      return ((int)__iar_Stoul(_S, 0, 10));
    }

    #pragma inline=no_body
    long atol(const char *_S)
    {       
      return ((long)__iar_Stoul(_S, 0, 10));
    }

        #pragma language=save
        #pragma language=extended
        #pragma inline=no_body
        long long atoll(const char *_S)
        {       
            return ((long long)__iar_Stoull(_S, 0, 10));
        }
        #pragma language=restore

    #pragma inline=no_body
    double strtod(const char * _S, char ** _Endptr)
    {       
      return (__iar_Stod(_S, _Endptr, 0));
    }

      #pragma inline=no_body
      float strtof(const char * _S, char ** _Endptr)
      {       
        return (__iar_Stof(_S, _Endptr, 0));
      }

      #pragma inline=no_body
      long double strtold(const char * _S, char ** _Endptr)
      {       
        return (__iar_Stold(_S, _Endptr, 0));
      }

    #pragma inline=no_body
    long strtol(const char * _S, char ** _Endptr, 
                int _Base)
    {       
      return (__iar_Stolx(_S, _Endptr, _Base, 0));
    }

    #pragma inline=no_body
    unsigned long strtoul(const char * _S, char ** _Endptr, 
                          int _Base)
    {       
      return (__iar_Stoul(_S, _Endptr, _Base));
    }

        #pragma language=save
        #pragma language=extended
        #pragma inline=no_body
        long long strtoll(const char * _S, char ** _Endptr,
                          int _Base)
        {       
            return (__iar_Stoll(_S, _Endptr, _Base));
        }

        #pragma inline=no_body
        unsigned long long strtoull(const char * _S, 
                                    char ** _Endptr, int _Base)
        {       
            return (__iar_Stoull(_S, _Endptr, _Base));
        }
        #pragma language=restore


  #pragma inline=no_body
  int abs(int i)
  {       
    return (i < 0 ? -i : i);
  }

  #pragma inline=no_body
  long labs(long i)
  {       
    return (i < 0 ? -i : i);
  }

      #pragma language=save
      #pragma language=extended
      #pragma inline=no_body
      long long llabs(long long i)
      {       
        return (i < 0 ? -i : i);
      }
      #pragma language=restore








 


double nmea_random(double min, double max)
{
    static double rand_max = 0x3fffffff;
    double rand_val = rand();
    double bounds = max - min;
    return min + (rand_val * bounds) / rand_max;
}



 

int nmea_gen_init(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int RetVal = 1; int smask = info->smask;
    nmeaGENERATOR *igen = gen;

    nmea_zero_INFO(info);
    info->smask = smask;

    info->lat = (5001.2621);
    info->lon = (3613.0595);

    while(RetVal && igen)
    {
        if(igen->init_call)
            RetVal = (*igen->init_call)(igen, info);
        igen = igen->next;
    }

    return RetVal;
}

int nmea_gen_loop(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int RetVal = 1;

    if(gen->loop_call)
        RetVal = (*gen->loop_call)(gen, info);

    if(RetVal && gen->next)
        RetVal = nmea_gen_loop(gen->next, info);

    return RetVal;
}

int nmea_gen_reset(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int RetVal = 1;

    if(gen->reset_call)
        RetVal = (*gen->reset_call)(gen, info);

    return RetVal;
}

void nmea_gen_destroy(nmeaGENERATOR *gen)
{
    if(gen->next)
    {
        nmea_gen_destroy(gen->next);
        gen->next = 0;
    }

    if(gen->destroy_call)
        (*gen->destroy_call)(gen);

    free(gen);
}

void nmea_gen_add(nmeaGENERATOR *to, nmeaGENERATOR *gen)
{
    if(to->next)
        nmea_gen_add(to->next, gen);
    else
        to->next = gen;
}

int nmea_generate_from(
    char *buff, int buff_sz,
    nmeaINFO *info,
    nmeaGENERATOR *gen,
    int generate_mask
    )
{
    int retval;

    if(0 != (retval = nmea_gen_loop(gen, info)))
        retval = nmea_generate(buff, buff_sz, info, generate_mask);

    return retval;
}



 

int nmea_igen_noise_init(nmeaGENERATOR *gen, nmeaINFO *info)
{
    return 1;
}

int nmea_igen_noise_loop(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int it;
    int in_use;

    info->sig = (int)nmea_random(1, 3);
    info->PDOP = nmea_random(0, 9);
    info->HDOP = nmea_random(0, 9);
    info->VDOP = nmea_random(0, 9);
    info->fix = (int)nmea_random(2, 3);
    info->lat = nmea_random(0, 100);
    info->lon = nmea_random(0, 100);
    info->speed = nmea_random(0, 100);
    info->direction = nmea_random(0, 360);
    info->declination = nmea_random(0, 360);
    info->elv = (int)nmea_random(-100, 100);

    info->satinfo.inuse = 0;
    info->satinfo.inview = 0;

    for(it = 0; it < 12; ++it)
    {
        info->satinfo.sat[it].id = it;
        info->satinfo.sat[it].in_use = in_use = (int)nmea_random(0, 3);
        info->satinfo.sat[it].elv = (int)nmea_random(0, 90);
        info->satinfo.sat[it].azimuth = (int)nmea_random(0, 359);
        info->satinfo.sat[it].sig = (int)(in_use?nmea_random(40, 99):nmea_random(0, 40));

        if(in_use)
            info->satinfo.inuse++;
        if(info->satinfo.sat[it].sig > 0)
            info->satinfo.inview++;
    }

    return 1;
}

int nmea_igen_noise_reset(nmeaGENERATOR *gen, nmeaINFO *info)
{
    return 1;
}



 

int nmea_igen_static_loop(nmeaGENERATOR *gen, nmeaINFO *info)
{
    nmea_time_now(&info->utc);
    return 1;
};

int nmea_igen_static_reset(nmeaGENERATOR *gen, nmeaINFO *info)
{
    info->satinfo.inuse = 4;
    info->satinfo.inview = 4;

    info->satinfo.sat[0].id = 1;
    info->satinfo.sat[0].in_use = 1;
    info->satinfo.sat[0].elv = 50;
    info->satinfo.sat[0].azimuth = 0;
    info->satinfo.sat[0].sig = 99;

    info->satinfo.sat[1].id = 2;
    info->satinfo.sat[1].in_use = 1;
    info->satinfo.sat[1].elv = 50;
    info->satinfo.sat[1].azimuth = 90;
    info->satinfo.sat[1].sig = 99;

    info->satinfo.sat[2].id = 3;
    info->satinfo.sat[2].in_use = 1;
    info->satinfo.sat[2].elv = 50;
    info->satinfo.sat[2].azimuth = 180;
    info->satinfo.sat[2].sig = 99;

    info->satinfo.sat[3].id = 4;
    info->satinfo.sat[3].in_use = 1;
    info->satinfo.sat[3].elv = 50;
    info->satinfo.sat[3].azimuth = 270;
    info->satinfo.sat[3].sig = 99;

    return 1;
}

int nmea_igen_static_init(nmeaGENERATOR *gen, nmeaINFO *info)
{
    info->sig = 3;
    info->fix = 3;

    nmea_igen_static_reset(gen, info);

    return 1;
}



 

int nmea_igen_rotate_loop(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int it;
    int count = info->satinfo.inview;
    double deg = 360 / (count?count:1);
    double srt = (count?(info->satinfo.sat[0].azimuth):0) + 5;

    nmea_time_now(&info->utc);

    for(it = 0; it < count; ++it)
    {
        info->satinfo.sat[it].azimuth =
            (int)((srt >= 360)?srt - 360:srt);
        srt += deg;
    }

    return 1;
};

int nmea_igen_rotate_reset(nmeaGENERATOR *gen, nmeaINFO *info)
{
    int it;
    double deg = 360 / 8;
    double srt = 0;

    info->satinfo.inuse = 8;
    info->satinfo.inview = 8;

    for(it = 0; it < info->satinfo.inview; ++it)
    {
        info->satinfo.sat[it].id = it + 1;
        info->satinfo.sat[it].in_use = 1;
        info->satinfo.sat[it].elv = 5;
        info->satinfo.sat[it].azimuth = (int)srt;
        info->satinfo.sat[it].sig = 80;
        srt += deg;
    }

    return 1;
}

int nmea_igen_rotate_init(nmeaGENERATOR *gen, nmeaINFO *info)
{
    info->sig = 3;
    info->fix = 3;

    nmea_igen_rotate_reset(gen, info);

    return 1;
}



 

int nmea_igen_pos_rmove_init(nmeaGENERATOR *gen, nmeaINFO *info)
{    
    info->sig = 3;
    info->fix = 3;
    info->direction = info->declination = 0;
    info->speed = 20;
    return 1;
}

int nmea_igen_pos_rmove_loop(nmeaGENERATOR *gen, nmeaINFO *info)
{
    nmeaPOS crd;

    info->direction += nmea_random(-10, 10);
    info->speed += nmea_random(-2, 3);

    if(info->direction < 0)
        info->direction = 359 + info->direction;
    if(info->direction > 359)
        info->direction -= 359;

    if(info->speed > 40)
        info->speed = 40;
    if(info->speed < 1)
        info->speed = 1;

    nmea_info2pos(info, &crd);
    nmea_move_horz(&crd, &crd, info->direction, info->speed / 3600);
    nmea_pos2info(&crd, info);

    info->declination = info->direction;

    return 1;
};

int nmea_igen_pos_rmove_destroy(nmeaGENERATOR *gen)
{
    return 1;
};



 

nmeaGENERATOR * __nmea_create_generator(int type, nmeaINFO *info)
{
    nmeaGENERATOR *gen = 0;

    switch(type)
    {
    case NMEA_GEN_NOISE:
        if(0 == (gen = malloc(sizeof(nmeaGENERATOR))))
            nmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(nmeaGENERATOR));
            gen->init_call = &nmea_igen_noise_init;
            gen->loop_call = &nmea_igen_noise_loop;
            gen->reset_call = &nmea_igen_noise_reset;
        }
        break;
    case NMEA_GEN_STATIC:
    case NMEA_GEN_SAT_STATIC:
        if(0 == (gen = malloc(sizeof(nmeaGENERATOR))))
            nmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(nmeaGENERATOR));
            gen->init_call = &nmea_igen_static_init;
            gen->loop_call = &nmea_igen_static_loop;
            gen->reset_call = &nmea_igen_static_reset;
        }
        break;
    case NMEA_GEN_SAT_ROTATE:
        if(0 == (gen = malloc(sizeof(nmeaGENERATOR))))
            nmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(nmeaGENERATOR));
            gen->init_call = &nmea_igen_rotate_init;
            gen->loop_call = &nmea_igen_rotate_loop;
            gen->reset_call = &nmea_igen_rotate_reset;
        }
        break;
    case NMEA_GEN_POS_RANDMOVE:
        if(0 == (gen = malloc(sizeof(nmeaGENERATOR))))
            nmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(nmeaGENERATOR));
            gen->init_call = &nmea_igen_pos_rmove_init;
            gen->loop_call = &nmea_igen_pos_rmove_loop;
            gen->destroy_call = &nmea_igen_pos_rmove_destroy;
        }
        break;
    case NMEA_GEN_ROTATE:
        gen = __nmea_create_generator(NMEA_GEN_SAT_ROTATE, info);
        nmea_gen_add(gen, __nmea_create_generator(NMEA_GEN_POS_RANDMOVE, info));
        break;
    };

    return gen;
}

nmeaGENERATOR * nmea_create_generator(int type, nmeaINFO *info)
{
    nmeaGENERATOR *gen = __nmea_create_generator(type, info);

    if(gen)
        nmea_gen_init(gen, info);

    return gen;
}

void nmea_destroy_generator(nmeaGENERATOR *gen)
{
    nmea_gen_destroy(gen);
}

