#pragma once

#include <limits.h>
#include <stdint.h>
#include <string>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

enum
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_POP_ENDIAN = 001000302ul
};

static const union
{
    unsigned char bytes[4];
    uint32_t value;
}
o32_host_order = { { 0, 1, 2, 3 } };
#define O32_HOST_ORDER (o32_host_order.value)

namespace UtilsIO {

    enum ByteEndianness
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN,
        POP_ENDIAN
    };

    class ByteReader
    {
    public:
        ByteReader(uint8_t*);
        ~ByteReader();

        void SetEndian(ByteEndianness);

        long long unsigned int ReadUInt64();
        long long int ReadInt64();

        unsigned int ReadUInt32();
        int ReadInt32();

        unsigned short ReadUInt16();
        short ReadInt16();

        double ReadDouble();
        float ReadSingle();

        uint8_t ReadByte();
        std::string ReadString(unsigned int);
        std::string ReadUTFString();

        void StoreBytes(char*, char*, size_t);

        long unsigned int Tell();
        void Seek(long unsigned int);

    private:
        const uint8_t* data;
        ByteEndianness endian;

        template<class T> T ReadAny();

        long unsigned int pointer;
    };
}
