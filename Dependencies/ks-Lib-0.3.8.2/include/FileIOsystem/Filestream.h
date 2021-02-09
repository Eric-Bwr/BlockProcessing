// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include "../EventSystem/EventSystem.h"
#include "../AudioSystem/AudioBuffer.h"
#include "../CallbackSystem/Callback.h"
#include "../Util/String.h"
#include <cstdint>
#include <cstdio>
#include <malloc.h>
#include <vector>
#include <cstring>
#include <cctype>
#include <tchar.h>

namespace fios {
    void linkCallback(cs::BasicCallback<void, es::Event &> *callback);
    char *readFile(const char *filePath);
    bool writeFile(char *buffer, const char *filePath);
    bool writeToFile(char *buffer, const char *filePath);
    Vector<String *> splitToVector(char *data);
    namespace binary {
        enum _Binary_buffer_error {
            BBE_OK = 0,
            BBE_CANT_FIND_FILE,
            BBE_CANT_ALLOC_MEM
        };
        typedef enum _Binary_buffer_error BinaryBufferError;
        typedef enum _Binary_buffer_error& BinaryBufferErrorReference;
        typedef enum _Binary_buffer_error const BinaryBufferErrorConst;
        typedef enum _Binary_buffer_error* BinaryBufferErrorPointer;
        typedef enum _Binary_buffer_error* const BinaryBufferErrorConstPointer;

        struct _Binary_buffer {
            uint8_t* _byte_array;
            uint64_t _bytes;
            uint64_t _bytes_off;
        };
        typedef struct _Binary_buffer BinaryBuffer;
        typedef struct _Binary_buffer& BinaryBufferReference;
        typedef struct _Binary_buffer const BinaryBufferConst;
        typedef struct _Binary_buffer* BinaryBufferPointer;
        typedef struct _Binary_buffer* const BinaryBufferConstPointer;

        void binaryBufferCreate(BinaryBufferPointer buffer, uint64_t alloc_size);
        void binaryBufferDestroy(BinaryBufferPointer buffer);
        uint8_t binaryBufferPop8(BinaryBufferPointer buffer);
        uint16_t binaryBufferPop16(BinaryBufferPointer buffer);
        uint32_t binaryBufferPop32(BinaryBufferPointer buffer);
        uint64_t binaryBufferPop64(BinaryBufferPointer buffer);
        float binaryBufferPopFloat(BinaryBufferPointer buffer);
        const char* binaryBufferPopString(BinaryBufferPointer buffer, uint64_t string_size);
        const char* binaryBufferPopStringWithTermination(BinaryBufferPointer buffer, uint64_t string_size);
        void binaryBufferPush8(BinaryBufferPointer buffer, uint8_t u8);
        void binaryBufferPush16(BinaryBufferPointer buffer, uint16_t u16);
        void binaryBufferPush32(BinaryBufferPointer buffer, uint32_t u32);
        void binaryBufferPush64(BinaryBufferPointer buffer, uint64_t u64);
        void binaryBufferPushFloat(BinaryBufferPointer buffer, float f32);
        void binaryBufferPush_String(BinaryBufferPointer buffer, const char* string);

        typedef const char* Path;

        BinaryBufferError binaryBufferReadFromFile(BinaryBufferPointer buffer, Path file_path);
        BinaryBufferError binaryBufferWriteToFile(BinaryBufferPointer buffer, Path file_path);
    }
}