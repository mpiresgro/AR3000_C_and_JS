#include "decode_decimal.h"

int Signed_Decimal_Value(uint8_t *measurement_ptr, int size)
{
    int value = 0;
    for (int i = 0; i < size; i++)
    {
        if (measurement_ptr[i] == '.' || measurement_ptr[i] == '-' || measurement_ptr[i] == ' ')
            continue;
        value += (measurement_ptr[i] - '0');
        if (i != size - 1)
            value *= 10;
    }
    if (measurement_ptr[0] == '-')
        value *= -1;

    return value;
}

void Decode_Decimal(T_Buffer *I_Decimal, T_Measurements *O_Measurements)
{
    // Sub components size
    int velocity_size = 9;
    int distance_size = 9;
    int strength_size = 5;
    int temperature_size = 5;
    int space = 1;

    // Pointers to start of sub arrays
    uint8_t *distance_ptr = NULL;
    uint8_t *velocity_ptr = NULL;
    uint8_t *strength_ptr = NULL;
    uint8_t *temperature_ptr = NULL;

    O_Measurements->Translation_Error = false;

    switch (I_Decimal->Length)
    // Define Sub Components Start Position
    {
    case 10:
        // Mode = DM0;
        distance_ptr = I_Decimal->The_Buffer + 1;
        break;
    case 20:
        // Mode = VM0;
        velocity_ptr = I_Decimal->The_Buffer + 1;
        distance_ptr = velocity_ptr + velocity_size + space;
        break;
    case 22:
        // Mode = DM3;
        distance_ptr = I_Decimal->The_Buffer + 1;
        strength_ptr = distance_ptr + space + distance_size;
        temperature_ptr = strength_ptr + space + strength_size;
        break;
    case 32:
        // Mode = VM3;
        velocity_ptr = I_Decimal->The_Buffer + 1;
        distance_ptr = velocity_ptr + velocity_size + space;
        strength_ptr = distance_ptr + space + distance_size;
        temperature_ptr = strength_ptr + space + strength_size;
        break;
    case 16:
        if (I_Decimal->The_Buffer[14] != 46)
        {
            // Mode = DM1;
            distance_ptr = I_Decimal->The_Buffer + 1;
            strength_ptr = distance_ptr + space + distance_size;
        }
        else
        {
            // Mode = DM2;
            distance_ptr = I_Decimal->The_Buffer + 1;
            temperature_ptr = distance_ptr + space + distance_size;
        }
        break;
    case 26:
        // Position 24 has char '.'
        if (I_Decimal->The_Buffer[24] != 46)
        {
            // Mode = VM1;
            velocity_ptr = I_Decimal->The_Buffer + 1;
            distance_ptr = velocity_ptr + velocity_size + space;
            strength_ptr = distance_ptr + space + distance_size;
        }
        else
        {
            // Mode = VM2;

            velocity_ptr = I_Decimal->The_Buffer + 1;
            distance_ptr = velocity_ptr + velocity_size + space;
            temperature_ptr = distance_ptr + space + distance_size;
        }
        break;
    default:
        O_Measurements->Translation_Error = true;
    }

    // ------------- DISTANCE ---------------------------
    float distance_val;
    bool distance_exists = true;
    distance_val = Signed_Decimal_Value(distance_ptr, distance_size) * 0.001;
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
        velocity_val = Signed_Decimal_Value(velocity_ptr, velocity_size) * 0.001;
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
        strength_val = Signed_Decimal_Value(strength_ptr, strength_size);
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
        temperature_val = Signed_Decimal_Value(temperature_ptr, temperature_size) * 0.1;
    }
    O_Measurements->Temperature = (T_Float_Value){.Valid = temperature_exists, .Value = temperature_val};
}
