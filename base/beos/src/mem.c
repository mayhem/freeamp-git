#include <memory.h>


void* __malloc(size_t size)
{
    void* result = NULL;
    
    result = (void *)malloc(size);
    
    return result;
}


void __free(void* p)
{
    if(p != NULL)
    {
        free(p);
    }
}
