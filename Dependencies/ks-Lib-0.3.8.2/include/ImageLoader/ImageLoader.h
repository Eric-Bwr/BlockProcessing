#pragma once
#include <cstdio>
#include <malloc.h>

/*
Formats
jpeg
jpg
png
bmp
tga
 */

void reverseBytes(void *start, int size) {
    unsigned char *lo = (unsigned char*)start;
    unsigned char *hi = (unsigned char*)start + size - 1;
    unsigned char swap;
    while (lo < hi) {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

void read(FILE* f, void* data, uint64_t elemCount, uint64_t elemSize, uint64_t& bytesRead) {
    uint64_t result = fread(data, elemCount, elemSize, f);
    if(result != elemSize) {
        printf("Failed to load image()");
    }

    bytesRead += result;
}

unsigned char* loadImage(const char* path, unsigned int& width, unsigned int& height, unsigned int& nrChannels, int tmp) {
    uint64_t pathSize = strlen(path);
    char first = path[pathSize - 3];
    char second = path[pathSize - 2];
    char third = path[pathSize - 1];

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        printf("Failed to open image");
        return nullptr;
    }

    fseek(f, 0, SEEK_END);
    uint64_t totalBytes = ftell(f);
    rewind(f);

    if((first == 'P' && second == 'N' && third == 'G') || (first == 'p' && second == 'n' && third == 'g')) {
        struct HEADER {
            char check;
            char name[3];
            char lineEnding[2];
            char dosStop;
            char unixLineEnding;
        };
        struct DATA_IHDR {
            unsigned int width;
            unsigned int height;
            unsigned char bitDepth;
            unsigned char colorType;
            unsigned char compressionMethod;
            unsigned char filterMethod;
            unsigned char interlaceMethod;
        };
        struct CHUNK {
            unsigned int length;
            char type[4];
            unsigned int crc;
        };

        uint64_t checkSum = 0;

        //Load HEADER
        HEADER header;
        read(f, &header, 1, sizeof(HEADER), checkSum);

        //Load CHUNKS
        CHUNK IHDR; DATA_IHDR ihdrData;
        CHUNK PLTE; unsigned char PLTEData[256][3];
        CHUNK IDAT; void* IDATData;
        CHUNK IEND; //no data
        CHUNK bKGD;
        CHUNK cHRM; unsigned int cHRMData[8];
        CHUNK dSIG;
        CHUNK eXIF;
        CHUNK gAMA; unsigned int gAMAData;
        CHUNK hIST; unsigned short hISTData[256 * 3];
        CHUNK iCCP;
        CHUNK iTXt;
        CHUNK pHYs; unsigned int pHYsData[2]; unsigned char PHysData1;
        CHUNK sBIT; unsigned char sBITData0, sBITData2_3[3], sBITData4[2], sBITData6[4];
        CHUNK sPLT;
        CHUNK sRGB; unsigned char sRGBData;
        CHUNK sTER;
        CHUNK tEXt;
        CHUNK tIME; unsigned short tIMEData; unsigned char tIMEData0[5];
        CHUNK tRNS; unsigned char tRNSData0[2][256], tRNSData2[2][256 * 3], tRNSData3[256];
        CHUNK zTXt;

        while (true) {
            CHUNK tmpChunk;
            read(f, &tmpChunk.length, 1, sizeof(tmpChunk.length), checkSum);
            reverseBytes(&tmpChunk.length, 4);
            read(f, &tmpChunk.type, 1, sizeof(tmpChunk.type), checkSum);
            std::cout << "Type: " << tmpChunk.type << " length: " << tmpChunk.length << std::endl;

            if (tmpChunk.type[0] == 'I' && tmpChunk.type[1] == 'H' && tmpChunk.type[2] == 'D' &&
                tmpChunk.type[3] == 'R') {         //IDHR
                IHDR.length = tmpChunk.length;
                memcpy(IHDR.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &ihdrData.width, 1, sizeof(ihdrData.width), checkSum);
                read(f, &ihdrData.height, 1, sizeof(ihdrData.height), checkSum);
                reverseBytes(&ihdrData.width, 4);
                reverseBytes(&ihdrData.height, 4);
                read(f, &ihdrData.bitDepth, 1, sizeof(ihdrData.bitDepth), checkSum);
                read(f, &ihdrData.colorType, 1, sizeof(ihdrData.colorType), checkSum);
                read(f, &ihdrData.compressionMethod, 1, sizeof(ihdrData.compressionMethod), checkSum);
                read(f, &ihdrData.filterMethod, 1, sizeof(ihdrData.filterMethod), checkSum);
                read(f, &ihdrData.interlaceMethod, 1, sizeof(ihdrData.interlaceMethod), checkSum);
                read(f, &IHDR.crc, 1, sizeof(IHDR.crc), checkSum);
                reverseBytes(&IHDR.crc, 4);
            } else if (tmpChunk.type[0] == 'P' && tmpChunk.type[1] == 'L' && tmpChunk.type[2] == 'T' &&
                       tmpChunk.type[3] == 'E') {  //PLTE
                uint64_t PLTEcheckSum = (tmpChunk.length / 3) * 3;
                if(PLTEcheckSum != tmpChunk.length) { fclose(f); return nullptr; }
                PLTE.length = tmpChunk.length;
                memcpy(PLTE.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &PLTEData, 1, sizeof(PLTEData), checkSum);
                read(f, &PLTE.crc, 1, sizeof(IDAT.crc), checkSum);
                reverseBytes(&PLTE.crc, 4);
            } else if (tmpChunk.type[0] == 'I' && tmpChunk.type[1] == 'D' && tmpChunk.type[2] == 'A' &&
                       tmpChunk.type[3] == 'T') {   //IDAT
                IDAT.length = tmpChunk.length;
                memcpy(IDAT.type, tmpChunk.type, sizeof(tmpChunk.type));
                IDATData = malloc(IDAT.length * sizeof(unsigned char));
                read(f, IDATData, 1, IDAT.length * sizeof(unsigned char), checkSum);
                read(f, &IDAT.crc, 1, sizeof(IDAT.crc), checkSum);
                reverseBytes(&IDAT.crc, 4);
            } else if (tmpChunk.type[0] == 'I' && tmpChunk.type[1] == 'E' && tmpChunk.type[2] == 'N' &&
                       tmpChunk.type[3] == 'D') {   //IEND
                IEND.length = tmpChunk.length;
                memcpy(IEND.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &IEND.crc, 1, sizeof(IEND.crc), checkSum);
                reverseBytes(&IEND.crc, 4);

                //TODO: real data return!
                fclose(f);
                if(checkSum == totalBytes) return (unsigned char*)IDATData;
                return nullptr;
            } else if (tmpChunk.type[0] == 'b' && tmpChunk.type[1] == 'K' && tmpChunk.type[2] == 'G' &&
                       tmpChunk.type[3] == 'D') {  //bKGD
                bKGD.length = tmpChunk.length;
                memcpy(bKGD.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 'c' && tmpChunk.type[1] == 'H' && tmpChunk.type[2] == 'R' &&
                       tmpChunk.type[3] == 'M') {   //cHRM
                cHRM.length = tmpChunk.length;
                memcpy(cHRM.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &cHRMData, 1, sizeof(cHRMData), checkSum);
                reverseBytes(&cHRMData[0], 4);
                reverseBytes(&cHRMData[1], 4);
                reverseBytes(&cHRMData[2], 4);
                reverseBytes(&cHRMData[3], 4);
                reverseBytes(&cHRMData[4], 4);
                reverseBytes(&cHRMData[5], 4);
                reverseBytes(&cHRMData[6], 4);
                reverseBytes(&cHRMData[7], 4);
                read(f, &cHRM.crc, 1, sizeof(IEND.crc), checkSum);
                reverseBytes(&cHRM.crc, 4);
            } else if (tmpChunk.type[0] == 'd' && tmpChunk.type[1] == 'S' && tmpChunk.type[2] == 'I' &&
                       tmpChunk.type[3] == 'G') {   //dSIG
                dSIG.length = tmpChunk.length;
                memcpy(dSIG.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 'e' && tmpChunk.type[1] == 'X' && tmpChunk.type[2] == 'I' &&
                       tmpChunk.type[3] == 'F') {  //eXIF
                eXIF.length = tmpChunk.length;
                memcpy(eXIF.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 'g' && tmpChunk.type[1] == 'A' && tmpChunk.type[2] == 'M' &&
                       tmpChunk.type[3] == 'A') {   //gAMA
                gAMA.length = tmpChunk.length;
                memcpy(gAMA.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &gAMAData, 1, sizeof(gAMAData), checkSum);
                reverseBytes(&gAMAData, 4);
                read(f, &gAMA.crc, 1, sizeof(gAMA.crc), checkSum);
                reverseBytes(&gAMA.crc, 4);
            } else if (tmpChunk.type[0] == 'h' && tmpChunk.type[1] == 'I' && tmpChunk.type[2] == 'S' &&
                       tmpChunk.type[3] == 'T') {   //hIST
                if(tmpChunk.length != 256 * 3) { fclose(f); return nullptr; }
                hIST.length = tmpChunk.length;
                memcpy(hIST.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &hISTData, 1, sizeof(hISTData), checkSum);
                read(f, &hIST.crc, 1, sizeof(hIST.crc), checkSum);
                reverseBytes(&hIST.crc, 4);
            } else if (tmpChunk.type[0] == 'i' && tmpChunk.type[1] == 'C' && tmpChunk.type[2] == 'C' &&
                       tmpChunk.type[3] == 'P') {  //iCCP
                iCCP.length = tmpChunk.length;
                memcpy(iCCP.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 'i' && tmpChunk.type[1] == 'T' && tmpChunk.type[2] == 'X' &&
                       tmpChunk.type[3] == 't') {   //iTXt
                iTXt.length = tmpChunk.length;
                memcpy(iTXt.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 'p' && tmpChunk.type[1] == 'H' && tmpChunk.type[2] == 'Y' &&
                       tmpChunk.type[3] == 's') {   //pHYs
                gAMA.length = tmpChunk.length;
                memcpy(pHYs.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &pHYsData, 1, sizeof(pHYsData), checkSum);
                reverseBytes(&pHYsData[0], 4);
                reverseBytes(&pHYsData[1], 4);
                read(f, &PHysData1, 1, sizeof(PHysData1), checkSum);
                read(f, &pHYs.crc, 1, sizeof(pHYs.crc), checkSum);
                reverseBytes(&pHYs.crc, 4);
            } else if (tmpChunk.type[0] == 's' && tmpChunk.type[1] == 'B' && tmpChunk.type[2] == 'I' &&
                       tmpChunk.type[3] == 'T') {  //sBIT
                sBIT.length = tmpChunk.length;
                memcpy(sBIT.type, tmpChunk.type, sizeof(tmpChunk.type));
                if(ihdrData.colorType == 0) read(f, &sBITData0, 1, sizeof(sBITData0), checkSum);
                else if(ihdrData.colorType == 2) read(f, &sBITData2_3, 1, sizeof(sBITData2_3), checkSum);
                else if(ihdrData.colorType == 3) read(f, &sBITData2_3, 1, sizeof(sBITData2_3), checkSum);
                else if(ihdrData.colorType == 4) read(f, &sBITData4, 1, sizeof(sBITData4), checkSum);
                else if(ihdrData.colorType == 3) read(f, &sBITData6, 1, sizeof(sBITData6), checkSum);
                else { fclose(f); return nullptr; }
                read(f, &sBIT.crc, 1, sizeof(pHYs.crc), checkSum);
                reverseBytes(&sBIT.crc, 4);
            } else if (tmpChunk.type[0] == 's' && tmpChunk.type[1] == 'P' && tmpChunk.type[2] == 'L' &&
                       tmpChunk.type[3] == 'T') {   //sPLT
                sPLT.length = tmpChunk.length;
                memcpy(sPLT.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 's' && tmpChunk.type[1] == 'R' && tmpChunk.type[2] == 'G' &&
                       tmpChunk.type[3] == 'B') {   //sRGB
                sRGB.length = tmpChunk.length;
                memcpy(sRGB.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &sRGBData, 1, sizeof(sRGBData), checkSum);
                read(f, &sRGB.crc, 1, sizeof(sRGB.crc), checkSum);
                reverseBytes(&sRGB.crc, 4);
            } else if (tmpChunk.type[0] == 's' && tmpChunk.type[1] == 'T' && tmpChunk.type[2] == 'E' &&
                       tmpChunk.type[3] == 'R') {  //sTER
                sTER.length = tmpChunk.length;
                memcpy(sTER.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 't' && tmpChunk.type[1] == 'E' && tmpChunk.type[2] == 'X' &&
                       tmpChunk.type[3] == 't') {   //tEXt
                tEXt.length = tmpChunk.length;
                memcpy(tEXt.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else if (tmpChunk.type[0] == 't' && tmpChunk.type[1] == 'I' && tmpChunk.type[2] == 'M' &&
                       tmpChunk.type[3] == 'E') {   //tIME
                tIME.length = tmpChunk.length;
                memcpy(tIME.type, tmpChunk.type, sizeof(tmpChunk.type));
                read(f, &tIMEData, 1, sizeof(tIMEData), checkSum);
                reverseBytes(&tIMEData, 2);
                read(f, &tIMEData0, 1, sizeof(tIMEData0), checkSum);
                read(f, &tIME.crc, 1, sizeof(tIME.crc), checkSum);
                reverseBytes(&tIME.crc, 4);
            } else if (tmpChunk.type[0] == 't' && tmpChunk.type[1] == 'R' && tmpChunk.type[2] == 'N' &&
                       tmpChunk.type[3] == 'S') {  //tRNS
                tRNS.length = tmpChunk.length;
                memcpy(tRNS.type, tmpChunk.type, sizeof(tmpChunk.type));
                if(ihdrData.colorType == 0) {

                } else if(ihdrData.colorType == 2) {

                } else if(ihdrData.colorType == 3) {

                }
            } else if (tmpChunk.type[0] == 'z' && tmpChunk.type[1] == 'T' && tmpChunk.type[2] == 'X' &&
                       tmpChunk.type[3] == 't') {  //zTXt
                zTXt.length = tmpChunk.length;
                memcpy(zTXt.type, tmpChunk.type, sizeof(tmpChunk.type));
            } else {
                break;
            }
        }
    }

    fclose(f);

    return nullptr;
}