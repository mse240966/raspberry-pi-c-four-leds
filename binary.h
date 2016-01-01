#ifndef BINARY_H_INCLUDED
#define BINARY_H_INCLUDED

typedef unsigned short int UINT16;

typedef struct bit
{
    UINT16 one:1;
    UINT16 two:1;
    UINT16 three:1;
    UINT16 four:1;
    UINT16 Unused:12;
} BITS;

typedef union decimal
{
    UINT16 value;
    BITS bits;
} DECIMAL;

#endif // BINARY_H_INCLUDED
