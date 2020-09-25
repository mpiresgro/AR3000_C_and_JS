#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// INPUT BUFFER
typedef uint8_t T_BufferArray[35];

typedef struct
{
    uint8_t Length;
    T_BufferArray The_Buffer;
} T_Buffer;

// OUTPUT MEASUREMENTS
typedef struct
{
    float Value;
    bool Valid;
} T_Float_Value;

typedef struct
{
    int Value;
    bool Valid;
} T_Int_Value;

typedef struct
{
    bool Translation_Error;
    T_Float_Value Distance;
    T_Float_Value Velocity;
    T_Int_Value Strength;
    T_Float_Value Temperature;
} T_Measurements;

typedef enum
{
    DECIMAL,
    HEXADECIMAL,
    BINARY
} T_Format;

