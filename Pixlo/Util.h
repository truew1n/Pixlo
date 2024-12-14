#ifndef PIXLO_UTIL_H
#define PIXLO_UTIL_H

#include <iostream>

#define ASIZE(Array) (sizeof(Array) / sizeof(Array[0]))

namespace NMess {

    inline void PrintHex(uint8_t *Data, uint64_t Size)
    {
        for (uint64_t I = 0; I < Size; ++I) {
            printf_s("%02x", Data[I]);
        }
    }

    inline void PrintAscii(uint8_t *Data, uint64_t Size)
    {
        for (uint64_t I = 0; I < Size; ++I) {
            if (Data[I] < 32) printf_s(".");
            else printf_s("%c", Data[I]);
        }
    }
}

namespace NUtilConverter {

    template<typename T>
    inline T SwapEndian(T value) {
        static_assert(std::is_integral<T>::value, "SwapEndian requires an integral type.");


        uint8_t *bytePointer = reinterpret_cast<uint8_t *>(&value);
        T swappedValue = 0;

        for (size_t i = 0; i < sizeof(T); ++i) {
            swappedValue |= static_cast<T>(bytePointer[i]) << ((sizeof(T) - 1 - i) * 8);
        }

        return swappedValue;
    }

    inline uint16_t SwapEndian(uint16_t Number) {
        return ((Number & 0x00FF) << 8) | ((Number & 0xFF00) >> 8);
    }

    inline int16_t SwapEndian(int16_t Number) {
        return ((Number & 0x00FF) << 8) | ((Number & 0xFF00) >> 8);
    }

    inline uint32_t SwapEndian(uint32_t Number) {
        return ((Number >> 24) & 0x000000FF) |
            ((Number >> 8) & 0x0000FF00) |
            ((Number << 8) & 0x00FF0000) |
            ((Number << 24) & 0xFF000000);
    }

    inline int32_t SwapEndian(int32_t Number) {
        return ((Number >> 24) & 0x000000FF) |
            ((Number >> 8) & 0x0000FF00) |
            ((Number << 8) & 0x00FF0000) |
            ((Number << 24) & 0xFF000000);
    }

    inline uint64_t SwapEndian(uint64_t Number) {
        return ((Number >> 56) & 0x00000000000000FFULL) |
            ((Number >> 40) & 0x000000000000FF00ULL) |
            ((Number >> 24) & 0x0000000000FF0000ULL) |
            ((Number >> 8) & 0x00000000FF000000ULL) |
            ((Number << 8) & 0x000000FF00000000ULL) |
            ((Number << 24) & 0x0000FF0000000000ULL) |
            ((Number << 40) & 0x00FF000000000000ULL) |
            ((Number << 56) & 0xFF00000000000000ULL);
    }

    inline int64_t SwapEndian(int64_t Number) {
        return ((Number >> 56) & 0x00000000000000FFULL) |
            ((Number >> 40) & 0x000000000000FF00ULL) |
            ((Number >> 24) & 0x0000000000FF0000ULL) |
            ((Number >> 8) & 0x00000000FF000000ULL) |
            ((Number << 8) & 0x000000FF00000000ULL) |
            ((Number << 24) & 0x0000FF0000000000ULL) |
            ((Number << 40) & 0x00FF000000000000ULL) |
            ((Number << 56) & 0xFF00000000000000ULL);
    }
}

#endif

