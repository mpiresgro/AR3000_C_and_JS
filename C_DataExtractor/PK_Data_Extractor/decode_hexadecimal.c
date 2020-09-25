#include "decode_hexadecimal.h"

int Signed_Hexadecimal_Value(uint8_t *measurement_ptr, int size)
{
    // Initializing base value to 1, i.e 16^0
    int base = 1;
    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i = size - 1; i >= 0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (measurement_ptr[i] >= '0' && measurement_ptr[i] <= '9')
        {
            dec_val += (measurement_ptr[i] - 48) * base;
            // incrementing base by power
            base = base * 16;
        }
        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (measurement_ptr[i] >= 'A' && measurement_ptr[i] <= 'F')
        {
            dec_val += (measurement_ptr[i] - 55) * base;
            // incrementing base by power
            base = base * 16;
        }
    }
    return dec_val;
}

void Decode_Hexadecimal(T_Buffer *I_Hexadecimal, T_Measurements *O_Measurements)
{
    // Sub components size
    int velocity_size = 6;
    int distance_size = 6;
    int strength_size = 5;
    int temperature_size = 5;
    int space = 1;

    // Pointers to start of sub arrays
    uint8_t *distance_ptr = NULL;
    uint8_t *velocity_ptr = NULL;
    uint8_t *strength_ptr = NULL;
    uint8_t *temperature_ptr = NULL;

    O_Measurements->Translation_Error = false;
    O_Measurements->Distance.Valid = false;
    O_Measurements->Velocity.Valid = false;
    O_Measurements->Strength.Valid = false;
    O_Measurements->Temperature.Valid = false;

    switch (I_Hexadecimal->Length)
    // Define Sub Components Start Position
    {
    case 7:
        // Mode = DM0;
        distance_ptr = I_Hexadecimal->The_Buffer + 1;
        break;
    case 12:
        // Mode = DM1 and DM2;
        printf("Unable to identify operation mode! DM1 - DM2 - ");
        O_Measurements->Translation_Error = true;
        return;
    case 14:
        // Mode = VM0;
        velocity_ptr = I_Hexadecimal->The_Buffer + 1;
        distance_ptr = velocity_ptr + velocity_size + space;
        break;
    case 17:
        // Mode = DM3;
        distance_ptr = I_Hexadecimal->The_Buffer + 1;
        strength_ptr = distance_ptr + space + distance_size;
        temperature_ptr = strength_ptr + space + strength_size;
        break;
    case 19:
        // Mode = VM1 and VM2;
        O_Measurements->Translation_Error = true;
        printf("Unable to identify operation mode! VM1 - VM2 - ");
        return;
    case 24:
        // Mode = VM3;
        velocity_ptr = I_Hexadecimal->The_Buffer + 1;
        distance_ptr = velocity_ptr + velocity_size + space;
        strength_ptr = distance_ptr + space + distance_size;
        temperature_ptr = strength_ptr + strength_size;
        break;
    default:
        O_Measurements->Translation_Error = true;
    }

    // ------------- DISTANCE ---------------------------

    float distance_val;
    bool distance_exists = true;
    distance_val = Signed_Hexadecimal_Value(distance_ptr, distance_size);
    if (distance_ptr[0] > '7')
    {
        // pow(2, 24) = 16777216;
        distance_val -= 16777216;
    }
    distance_val *= 0.001;
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
        velocity_val = Signed_Hexadecimal_Value(velocity_ptr, velocity_size);
        if (velocity_ptr[0] > '7')
        {
            // pow(2, 24) = 16777216;
            velocity_val -= 16777216;
        }
        velocity_val *= 0.001;
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
        strength_val = Signed_Hexadecimal_Value(strength_ptr, strength_size);
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
        temperature_val = Signed_Hexadecimal_Value(temperature_ptr, temperature_size);
        if (temperature_ptr[0] > '7')
        {
            // pow(2, 16) = 65536
            temperature_val -= 65536;
        }
        temperature_val *= 0.1;
    }

    O_Measurements->Temperature = (T_Float_Value){.Valid = temperature_exists, .Value = temperature_val};
}