//
// PL40.h
//
// This file is part of the SunflowerDataService application.
//
// SunflowerDataService is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SunflowerDataService is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SunflowerDataService.  If not, see <http://www.gnu.org/licenses/>.
//
// Created by Makina on behalf of Southern Cross University.
// Copyright (c) 2014 Makina. All rights reserved.
//

#ifndef __SunflowerDataService__PL40__
#define __SunflowerDataService__PL40__

#include <utility>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

#include "SerialPort.h"

using namespace std;

/*
 * PL-40 Serial Protocol Definitions
 *
 * SERIAL SPECIFICATIONS:
 * 9600, 8, N, 1 (Must set jumpers on PL40 for 9600)
 *
 * REQUEST PACKET FORMAT (4 BYTES):
 * [CMD] [ADDR] [WRITE]|[0] [~CMD]
 * (Byte 3 must be 0 if reading)
 *
 * RESPONSE PACKET FORMAT (1 OR 2 BYTES):
 * [ERR/TEST]
 * OR
 * [C8H]
 *
 * IMPLEMENTATION NOTES:
 * - PL40 serial port is powered parasitically by host, so send all 0x00's for about 100ms to feed it.
 * - All voltages returned are relative to 12V! So if the system is running @ 24V, the data is V/2. @ 48V, V/4

 */

struct PL40History {
    float Vmax;
    float  Vmin;
    unsigned char FTime;
    unsigned char SOC;
    unsigned short AHin;
    unsigned short AHout;
};

class PL40
{

public:
    PL40(const char * portName);
    ~PL40();

    bool isConnected();
    void connect();
    void disconnect();
    void poll(unordered_map<string, string> * results);

    unsigned char readRAM8(unsigned char address);
    unsigned short readRAM16(unsigned char msb, unsigned char lsb);
    unsigned char readEEPROM8(unsigned char address);
    unsigned short readEEPROM16(unsigned char msb, unsigned char lsb);

    void readHistory(int day, PL40History * record);
    bool loopBack();

protected:
    unsigned char readUInt8(unsigned char command, unsigned char address);
    unsigned short readUInt16(unsigned char source, unsigned char msb, unsigned char lsb);


private:
    SerialPort * mySerialPort;
    std::string * myPortName;
    float myVoltageMultiplier;
};

//
// ERROR definitions
//
#define ERR_PL40_INVALID_CONNECT        0x2001
#define ERR_PL40_INVALID_DISCONNECT     0x2002
#define ERR_PL40_INVALID_RESPONSE       0x2009
#define ERR_PL40_NO_RESPONSE            0x200A

//
// PL40 Voltage Definitions
//
#define PL40_VOLTAGE_12     0
#define PL40_VOLTAGE_12_X   1
#define PL40_VOLTAGE_24     1
#define PL40_VOLTAGE_24_X   2
#define PL40_VOLTAGE_32     2
#define PL40_VOLTAGE_32_X   2.66666666
#define PL40_VOLTAGE_36     3
#define PL40_VOLTAGE_36_X   3
#define PL40_VOLTAGE_48     4
#define PL40_VOLTAGE_48_X   4

//
// PL40 Command Definitions
//
#define PL40_CMD_READ_RAM       0x14
#define PL40_CMD_READ_E2PROM    0x48
#define PL40_CMD_WRITE_RAM      0x98
#define PL40_CMD_WRITE_E2       0xCA
#define PL40_CMD_LOOPBACK       0xBB

//
// PL40 Format / Error Definitions
//
#define PL40_RX_LOOPBACK        0x80
#define PL40_RX_SOH             0xC8
#define PL40_RX_ERR_TIMEOUT     0x81
#define PL40_RX_ERR_CHECKSUM    0x82
#define PL40_RX_ERR_COMMAND     0x83
#define PL40_RX_ERR_NOREPLY     0x85
#define PL40_RX_ERR_BADREPLY    0x86

//
// PL40 RAM Addresses
//

// The version of the PL controller
// NOTE: 0-127 = PL20, 128-191 = PL40, 192-255 = PL60
#define PL40_RAM_VERSION        0x00


#define PL40_RAM_VDIV           0x20
#define PL40_RAM_BCALS          0x21
#define PL40_RAM_BCAL12         0x22
#define PL40_RAM_BCAL24         0x23
#define PL40_RAM_BCAL48         0x24
#define PL40_RAM_NIGHT          0x25
#define PL40_RAM_GENDEL         0x26
#define PL40_RAM_DUTYCYC        0x27
#define PL40_RAM_LCOUNT         0x28
#define PL40_RAM_DTIME          0x29
#define PL40_RAM_SEC            0x2E
#define PL40_RAM_MIN            0x2F
#define PL40_RAM_HOUR           0x30
#define PL40_RAM_DAY            0x31
#define PL40_RAM_BATV           0x32
#define PL40_RAM_BATTEMP        0x34
#define PL40_RAM_SOLV           0x35
#define PL40_RAM_PROT           0x36
#define PL40_RAM_SERCON         0x37
#define PL40_RAM_LINKCNT        0x38
#define PL40_RAM_POS1           0x3E
#define PL40_RAM_BSTDAY         0x3F
#define PL40_RAM_GON            0x40
#define PL40_RAM_GDEL           0x41
#define PL40_RAM_GOFF           0x42
#define PL40_RAM_GEXF           0x43
#define PL40_RAM_GRUN           0x44
#define PL40_RAM_LOFF           0x45
#define PL40_RAM_LON            0x46
#define PL40_RAM_LDEL           0x47
#define PL40_RAM_ASET           0x48
#define PL40_RAM_BSTFREQ        0x49
#define PL40_RAM_ATIM           0x4A
#define PL40_RAM_HYST           0x4B
#define PL40_RAM_BRET           0x4C
#define PL40_RAM_CURLIM         0x4D
#define PL40_RAM_BAT2           0x4E
#define PL40_RAM_ESET1          0x4F
#define PL40_RAM_ESET2          0x50
#define PL40_RAM_ESET3          0x51
#define PL40_RAM_EQFREQ         0x52
#define PL40_RAM_ETIM           0x53
#define PL40_RAM_ABSV           0x54
#define PL40_RAM_EMAX           0x55
#define PL40_RAM_FLTV           0x56

// The Boost-Charge maximum voltage level
#define PL40_RAM_BMAX           0x57


#define PL40_RAM_LGSET          0x58
#define PL40_RAM_PWME           0x59
#define PL40_RAM_SSTOP          0x5A
#define PL40_RAM_ETMOD          0x5B
#define PL40_RAM_GMOD           0x5C
#define PL40_RAM_VOLT           0x5D
#define PL40_RAM_BCAP           0x5E
#define PL40_RAM_EQCNT          0x5F
#define PL40_RAM_GENEXD         0x60
#define PL40_RAM_EXTMOUT        0x61
#define PL40_RAM_ECON           0x62
#define PL40_RAM_MTMOUT         0x63
#define PL40_RAM_RCOUNT         0x64
#define PL40_RAM_RSTATE         0x65
#define PL40_RAM_DSTATE         0x66
#define PL40_RAM_RMODE          0x68
#define PL40_RAM_VREG           0x69
#define PL40_RAM_ERUNC          0x6C
#define PL40_RAM_EREPTC         0x6D
#define PL40_RAM_SETCNT         0x6F
#define PL40_RAM_AHBALH         0x71
#define PL40_RAM_CONOUT         0x73
#define PL40_RAM_BMINL          0x7C
#define PL40_RAM_BMAXL          0x7D
#define PL40_RAM_DTEMP          0xB4
#define PL40_RAM_DSOC           0xB5
#define PL40_RAM_DVMAX          0xB6
#define PL40_RAM_DVMIN          0xB7
#define PL40_RAM_DFLTIM         0xB8
#define PL40_RAM_CIACC1         0xB9
#define PL40_RAM_CIACC2         0xBA
#define PL40_RAM_CIACC3         0xBB
#define PL40_RAM_CIAHL          0xBC
#define PL40_RAM_CIAHH          0xBD
#define PL40_RAM_CEACC1         0xBE
#define PL40_RAM_CEACC2         0xBF
#define PL40_RAM_CEACC3         0xC0
#define PL40_RAM_CEAHL          0xC1
#define PL40_RAM_CEAHH          0xC2
#define PL40_RAM_LIACC1         0xC3
#define PL40_RAM_LIACC2         0xC4
#define PL40_RAM_LAICC3         0xC5
#define PL40_RAM_LIAHL          0xC6
#define PL40_RAM_LIAHH          0xC7
#define PL40_RAM_LEACC1         0xC8
#define PL40_RAM_LEACC2         0xC9
#define PL40_RAM_LEACC3         0xCA
#define PL40_RAM_LEAHL          0xCB
#define PL40_RAM_LEAHH          0xCC
#define PL40_RAM_CEXT           0xCD
#define PL40_RAM_LEXT           0xCE
#define PL40_RAM_EXTF           0xCF
#define PL40_RAM_VEXT           0xD0
#define PL40_RAM_AHBALL         0xD3
#define PL40_RAM_CHARGL         0xD4
#define PL40_RAM_CINT           0xD5
#define PL40_RAM_CHARGG         0xD7
#define PL40_RAM_LOADL          0xD8
#define PL40_RAM_LINT           0xD9
#define PL40_RAM_LOADG          0xDB
#define PL40_RAM_BATVL          0xDC
#define PL40_RAM_VBAT           0xDD
#define PL40_RAM_BATVG          0xDF
#define PL40_RAM_BRDTEML        0xE0
#define PL40_RAM_TBRD           0xE1
#define PL40_RAM_E2_PROT        0xE2
#define PL40_RAM_BRDTEMG        0xE3
#define PL40_RAM_BATTEML        0xE3
#define PL40_RAM_TBATL          0xE5
#define PL40_RAM_BATTEMG        0xE7
#define PL40_RAM_SOLVL          0xE8
#define PL40_RAM_VSOL           0xE9
#define PL40_RAM_SOLVG          0xEB
#define PL40_RAM_BATSENL        0xEC
#define PL40_RAM_VSEN           0xED
#define PL40_RAM_BATSENG        0xEF

//
// PL40 EEPROM (NV) Addresses
//
#define PL40_E2_HIST            0x2D

//#define PL40_E2_        0x


#endif /* defined(__SunflowerDataService__PL40__) */
