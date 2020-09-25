#include "decode_binary.h"

int N_Bytes_Complement(uint8_t *measurement_ptr, int size, int filling_mask)
{
    int value = 0;
    uint8_t mask_7_bits = 0b01111111;

    for (int i = 0; i < size; i++)
    {
        value = value << 7 | (int)measurement_ptr[i] & mask_7_bits;
    }
    uint8_t negative_mask = 0b01000000;
    // 7 MSB indicates if negative
    if (negative_mask & measurement_ptr[0])
    {
        return (value | filling_mask);
    }
    else
    {
        return value;
    }
}

int Two_Bytes(uint8_t *measurement_ptr, int size)
{
    int value = 0;
    return measurement_ptr[0] << 7;
}

void Decode_Binary(T_Buffer *I_Binary, T_Measurements *O_Measurements)
{
    // Sub components size
    int velocity_size = 3;
    int distance_size = 3;
    int strength_size = 1;
    int temperature_size = 2;

    // Pointers to start of sub arrays
    uint8_t *distance_ptr = NULL;
    uint8_t *velocity_ptr = NULL;
    uint8_t *strength_ptr = NULL;
    uint8_t *temperature_ptr = NULL;

    O_Measurements->Translation_Error = false;

    switch (I_Binary->Length)
    // Define Sub Components Start Position
    {
    case 3:
        // Mode = DM0;
        distance_ptr = I_Binary->The_Buffer;
        break;
    case 4:
        // Mode = DM1;
        distance_ptr = I_Binary->The_Buffer;
        strength_ptr = distance_ptr + distance_size;
        break;
    case 7:
        // Mode = VM1;
        velocity_ptr = I_Binary->The_Buffer;
        distance_ptr = velocity_ptr + velocity_size;
        strength_ptr = distance_ptr + distance_size;
        break;
    case 5:
        // Mode = DM2;
        distance_ptr = I_Binary->The_Buffer;
        temperature_ptr = distance_ptr + distance_size;
        break;
    case 8:
        // Mode = VM2;
        velocity_ptr = I_Binary->The_Buffer;
        distance_ptr = velocity_ptr + velocity_size;
        temperature_ptr = distance_ptr + distance_size;
        break;
    case 9:
        // Mode = VM3;
        velocity_ptr = I_Binary->The_Buffer;
        distance_ptr = velocity_ptr + velocity_size;
        strength_ptr = distance_ptr + distance_size;
        temperature_ptr = strength_ptr + strength_size;
        break;

    case 6:
        if ((I_Binary->The_Buffer[4] & 0b10000000) > 0)
        {
            // Mode = DM3
            distance_ptr = I_Binary->The_Buffer;
            strength_ptr = distance_ptr + distance_size;
            temperature_ptr = strength_ptr + strength_size;
        }
        else
        {
            // Mode = VM0
            velocity_ptr = I_Binary->The_Buffer;
            distance_ptr = velocity_ptr + velocity_size;
        }
        break;
    default:
        O_Measurements->Translation_Error = true;
    }

    int filling_mask_21 = 0b11111111111000000000000000000000;

    int filling_mask_14 = 0b11111111111111111100000000000000;

    // ------------- DISTANCE ---------------------------
    float distance_val;
    bool distance_exists = true;

    distance_val = N_Bytes_Complement(distance_ptr, distance_size, filling_mask_21) * 0.001;
    O_Measurements->Distance = (T_Float_Value){.Valid = distance_val, .Value = distance_val};

    // ------------- VELOCITY ---------------------------
    float velocity_val;
    bool velocity_exists;
    if (!velocity_ptr)
    {
        velocity_exists = false;
    }
    else
    {
        velocity_exists = true;
        velocity_val = N_Bytes_Complement(velocity_ptr, velocity_size, filling_mask_21) * 0.001;
    }
    O_Measurements->Velocity = (T_Float_Value){.Valid = velocity_exists, .Value = velocity_val};

    // ------------- STRENGTH ---------------------------
    int strength_val;
    bool strength_exists;
    if (!strength_ptr)
    {
        strength_exists = false;
    }
    else
    {
        strength_exists = true;
        strength_val = Two_Bytes(strength_ptr, strength_size);
    }
    O_Measurements->Strength = (T_Int_Value){.Valid = strength_exists, .Value = strength_val};

    // ------------- TEMPERATURE ---------------------------
    float temperature_val;
    bool temperature_exists;
    if (!temperature_ptr)
    {
        temperature_exists = false;
    }
    else
    {
        temperature_exists = true;
        temperature_val = N_Bytes_Complement(temperature_ptr, temperature_size, filling_mask_14) * 0.1;
    }

    O_Measurements->Temperature = (T_Float_Value){.Valid = temperature_exists, .Value = temperature_val};
}