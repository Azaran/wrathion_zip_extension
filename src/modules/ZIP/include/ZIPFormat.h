/* 
 * Copyright (C) 2014 Jan Schmied, 2016 Vojtech Vecera
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 */

#ifndef ZIPFORMAT_H
#define	ZIPFORMAT_H

#include "FileFormat.h"
#include <vector>
#include <bitset>

/**
 * Type of file encryption
 */
enum ZIPEncType{
    PKSTREAM,
    WZAES,	// WinZIP AES
    SAES,	// AES from APPNOTE.TXT
    TDES,
    CDENC,	// Central Directory is encrypted
    NONE
};

struct ZIPInitData{
    ZIPInitData();
    ZIPInitData(const ZIPInitData &orig);
    ZIPEncType type;
    uint32_t crc32;
    uint32_t dataLen;
    uint32_t uncompressedSize;
    uint16_t keyLength;
    uint16_t compression;
    uint16_t encSize;
    uint16_t erdSize;
    uint16_t ivSize;
    uint8_t *encData;
    uint8_t *erdData;
    uint8_t *ivData;
    uint8_t saltLen;
    uint8_t salt[16];
    uint8_t verifier[2];
    uint8_t authCode[10];
    uint8_t streamBuffer[12];
};

/**
 * Class for reading ZIP file
 */
class ZIPFormat: public FileFormat {
public:
    ZIPFormat();
    ZIPFormat(const ZIPFormat& orig);
    virtual ~ZIPFormat();
    virtual void init(std::string& filename);
    virtual CrackerFactory* getCPUCracker();
    virtual CrackerFactory* getGPUCracker();


protected:
    /**
     * Read one file in stream from PK\x03\x04 signature position
     * @param stream
     * @return 
     */
    ZIPInitData readOneFile(std::ifstream *stream);
    /**
     * Reads Strong Encryption Header from the strean
     * @param stream, data
     * @return 
     */
    uint16_t readStongEncHdr(std::ifstream *stream, ZIPInitData *data);
    /**
     * Removes all files, except the smallest one
     */
    void filterData();
    
private:
    std::vector<ZIPInitData> data;

};

#endif	/* ZIPFORMAT_H */

