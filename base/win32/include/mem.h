
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

__declspec(dllimport) void* __malloc(size_t size);

__declspec(dllimport) void __free(void* p);


#ifdef __cplusplus
}
#endif  /* __cplusplus */