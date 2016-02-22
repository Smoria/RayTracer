#pragma once

#ifndef SAFE_POINTER_DELETE
    #define SAFE_POINTER_DELETE(pointer)\
            if(pointer != nullptr)\
            {\
                delete pointer;\
                pointer = nullptr;\
            }
#else
    #error This macrodefinitions already defined
#endif

#ifndef SAFE_ARRAY_DELETE
    #define SAFE_ARRAY_DELETE(pointer)\
            if(pointer != nullptr)\
            {\
                delete [] pointer;\
                pointer = nullptr;\
            }
#else
    #error This macrodefinitions already defined
#endif