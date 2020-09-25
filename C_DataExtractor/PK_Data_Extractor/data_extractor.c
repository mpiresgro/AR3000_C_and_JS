
#include "data_extractor.h"

void Data_Extractor(T_Buffer *I_Messagge, T_Measurements *O_Measurements)
{

    unsigned char first_char = I_Messagge->The_Buffer[0];
    T_Format Format;
    switch (first_char)
    // Identify format
    {
    case 'D':
        Format = DECIMAL;
        break;
    case 'H':
        Format = HEXADECIMAL;
        break;
    default:
        // TODO : Check first MSB of first int
        Format = BINARY;
    }

    switch (Format)
    // Calling specfic extractor function
    {
    case DECIMAL:
        // printf("MODE > DECIMAL\n");
        Decode_Decimal(I_Messagge, O_Measurements);
        break;
    case HEXADECIMAL:
        // printf("MODE > HEXADECIMAL\n");
        Decode_Hexadecimal(I_Messagge, O_Measurements);
        break;
    case BINARY:
        // printf("MODE > BINARY\n");
        Decode_Binary(I_Messagge, O_Measurements);
        break;
    default:
        printf("MODE > Someting wong\n");
        // Someting wong
    }
}