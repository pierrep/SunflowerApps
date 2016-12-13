//
// PL40.cpp
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


#include "PL40.h"
#include "Log.h"

#include <math.h>
#include <string>
#include <map>

using namespace std;

PL40::PL40(const char * portName)
{
    myPortName = new std::string(portName);
    mySerialPort = NULL;
    myVoltageMultiplier = PL40_VOLTAGE_24_X; // Default to 24V system multiplier
}

PL40::~PL40() {
    if (isConnected()) disconnect();
}

bool PL40::isConnected()
{
    return (mySerialPort != NULL);
}

void PL40::connect()
{
    if (isConnected()) {
        throw ERR_PL40_INVALID_CONNECT;
    }

    // Create and open the serial port
    mySerialPort = new SerialPort();

    try {
        mySerialPort->open(myPortName->c_str());
    } catch (int exception) {
        Log::error("PL40::connect: failed to open serial port (error %d)", exception);
        mySerialPort = NULL;
        throw;
    }

    // Send a continuous stream of zero-value bits as-per the PL40 serial spec to allow it to
    // parasitically power itself from the line.
    Log::debug("PL40::connect: Sending break to wake up PL40");
    mySerialPort->sendBreak(1);

    // Perform a loopback test to make sure we are actually talking to a PL-40
    Log::debug("Reading system reference voltage");
    // Read the system voltage to get our reference
    unsigned char volt = readRAM8(PL40_RAM_VOLT);
    volt &= 0x0F; // Mask away upper nibble

    switch (volt) {
    case PL40_VOLTAGE_12:
        Log::info("PL40 system voltage is 12V.");
        myVoltageMultiplier = PL40_VOLTAGE_12_X;
        break;

    case PL40_VOLTAGE_24:
        Log::info("PL40 system voltage is 24V.");
        myVoltageMultiplier = PL40_VOLTAGE_24_X;
        break;

    case PL40_VOLTAGE_32:
        Log::info("PL40 system voltage is 32V.");
        myVoltageMultiplier = PL40_VOLTAGE_32_X;
        break;

    case PL40_VOLTAGE_36:
        Log::info("PL40 system voltage is 36V.");
        myVoltageMultiplier = PL40_VOLTAGE_36_X;
        break;

    case PL40_VOLTAGE_48:
        Log::info("PL40 system voltage is 48V.");
        myVoltageMultiplier = PL40_VOLTAGE_48_X;
        break;

    default:
        Log::info("PL40 system voltage cannot be determined, defaulting to 24V (MAKE SURE THIS IS CORRECT).");
        myVoltageMultiplier = PL40_VOLTAGE_24_X;
        break;
    }
}

void PL40::disconnect()
{
    if (!isConnected()) {
        throw ERR_PL40_INVALID_DISCONNECT;
    }

    // Close the serial port
    mySerialPort->close();
    delete mySerialPort;
    mySerialPort = NULL;
}

void PL40::readHistory(int day, PL40History * record)
{
    // Read the history pointer
    int p = readEEPROM8(PL40_E2_HIST);


    // Calculate the starting offset for Day 1
    int d1 = 0x2E + (7 * p);

    // Calculate the offset for the requested day
    int d = d1 - (7 * ( day -  1 ));
    if (d < 0x2E) {
        d = ( d1 + 0xD2 ) - (7 * ( day - 1 ));
    }

    Log::debug("ReadHistory: p = %02x", p);
    Log::debug("ReadHistory: d1 = %02x", d1);
    Log::debug("ReadHistory: d = %02x", d);

    // Read the 7 bytes in
    unsigned char buffer[7];

    // TODO: Bypass buffer and read contents directly to structure with EEPROM8/16.
    buffer[0] = readEEPROM8(d++);
    buffer[1] = readEEPROM8(d++);
    buffer[2] = readEEPROM8(d++);
    buffer[3] = readEEPROM8(d++);
    buffer[4] = readEEPROM8(d++);
    buffer[5] = readEEPROM8(d++);
    buffer[6] = readEEPROM8(d);

    record->Vmax = buffer[0] * myVoltageMultiplier / 10;
    record->Vmin = buffer[1] * myVoltageMultiplier / 10;
    record->FTime = buffer[2];
    record->SOC = buffer[3];

    // Calculate the AH in and out
    // NOTE: The upper nibble of byte[6] is the MSB of AHout and the lower nibble is the MSB of AHin
    // giving 12 bits of data for each value
    record->AHin =  (unsigned short)(((buffer[6] & 0x0F) << 8) | buffer[4]);
    record->AHout = (unsigned short)(((buffer[6] & 0xF0) << 4) | buffer[5]);


    Log::debug("RECORD %d", day);
    Log::debug("VMAX %f", record->Vmax);
    Log::debug("VMIN %f", record->Vmin);
    Log::debug("FTime %u", record->FTime);
    Log::debug("SOC %u", record->SOC);
    Log::debug("AHin %u", record->AHin);
    Log::debug("AHout %u", record->AHout);

    // Done

}

bool PL40::loopBack()
{

    unsigned char response = readUInt8(PL40_CMD_LOOPBACK, 0x00);

    // Test for a valid loopback response
    return (response == PL40_RX_LOOPBACK);
}

unsigned char PL40::readUInt8(unsigned char command, unsigned char address)
{

    /*
     * NOTE:
     * In Linux, for some reason you need to send any command and get an
     * 'INVALID RESPONSE'. After that, comms behaves (more or less) as normal.
     * In this implementation, we allow [RetryCount] failures before we throw an exception
     * and force the comms port to reconnect.
     */

    const int CommandLength = 4;
    const int ResponseLength = 2;
    const int RetryCount = 5;
    const int CommandWaitInterval = 100000; // uS
    const int ReadWaitInterval = 20000; // uS
    const int MaxReadWaitCount = 50; // uS
    const int RetryInterval = 250000; // uS

    // Create the command
    unsigned char cmd[CommandLength];
    cmd[0] = command;
    cmd[1] = address;
    cmd[2] = 0x00;
    cmd[3] = ~command;

    int retries = 0;

    while (retries < RetryCount) {

        // Write the command
        mySerialPort->write(cmd, CommandLength);
        // Wait for processing to occur
        usleep(CommandWaitInterval);

        // Read the response
        unsigned char rsp[ResponseLength];
        int len;

        int maxWaitCount = 0;
        while ( (len = mySerialPort->read(rsp, ResponseLength)) < 0 ) {
            usleep(ReadWaitInterval);

            maxWaitCount++;
            if (maxWaitCount >= MaxReadWaitCount) {
                throw ERR_PL40_NO_RESPONSE;
            }
        }

        // Validate the response
        if (len == 2 && rsp[0] == PL40_RX_SOH) {
            // We have a valid packet
            return rsp[1];
        } else if (len == 1 && rsp[0] == PL40_RX_LOOPBACK) {
            // We have a loopback response
            return rsp[0];
        }

        // Error handling, increment the retries counter
        retries++;

        if (retries > RetryCount) {
            Log::error("PL40::readUint8: Command failed");
            throw ERR_PL40_NO_RESPONSE;
        }

        // Debugging help
        if (len == 0) {
            // No response data
            Log::debug("PL40::readUInt8: Zero length response, retry %d of %d ...", retries, RetryCount);
        } else if (len == 1) {
            // Unknown single-byte response
            Log::debug("PL40::readUInt8: Invalid response, retry %d of %d ...", retries, RetryCount);
        } else if (len == 2) {
            // Invalid SOH value
            Log::debug("PL40::readUInt8: Invalid response format, retry %d of %d ...", retries, RetryCount);
        } else {
            // Invalid packet length
            Log::debug("PL40::readUInt8: Invalid length (expected 2, got %d) retry %d of %d ...", len, retries, RetryCount);
        }

        // Sleep before the next retry
        usleep(RetryInterval);
    }

    // We have reached the maximum of our retry count.
    throw ERR_PL40_INVALID_RESPONSE;
}

unsigned short PL40::readUInt16(unsigned char source, unsigned char msb, unsigned char lsb)
{

    unsigned char msbBuffer = readUInt8(source, msb);
    unsigned char lsbBuffer = readUInt8(source, lsb);

    unsigned short result = (unsigned short)msbBuffer << 8;
    result |= lsbBuffer;
    return result;
}

unsigned char PL40::readRAM8(unsigned char address)
{
    return readUInt8(PL40_CMD_READ_RAM, address);
}

unsigned short PL40::readRAM16(unsigned char msb, unsigned char lsb)
{
    return readUInt16(PL40_CMD_READ_RAM, msb, lsb);
}

unsigned char PL40::readEEPROM8(unsigned char address)
{
    return readUInt8(PL40_CMD_READ_E2PROM, address);
}

unsigned short PL40::readEEPROM16(unsigned char msb, unsigned char lsb)
{
    return readUInt16(PL40_CMD_READ_E2PROM, msb, lsb);
}

typedef pair<string, string> KV;


void PL40::poll(unordered_map<string, string> * results)
{
    try {
        unsigned char BATV = readRAM8(PL40_RAM_BATV);
        unsigned short CEAH = readRAM16(PL40_RAM_CEAHH, PL40_RAM_CEAHL);
        unsigned short CIAH = readRAM16(PL40_RAM_CIAHH, PL40_RAM_CIAHL);
        unsigned short LEAH = readRAM16(PL40_RAM_LEAHH, PL40_RAM_LEAHL);
        unsigned short LIAH = readRAM16(PL40_RAM_LIAHH, PL40_RAM_LIAHL);
        unsigned char EXTF = readRAM8(PL40_RAM_EXTF);
        unsigned char CEXT = readRAM8(PL40_RAM_CEXT);
        unsigned char CINT = readRAM8(PL40_RAM_CINT);
        unsigned char LEXT = readRAM8(PL40_RAM_LEXT);
        unsigned char LINT = readRAM8(PL40_RAM_LINT);
        unsigned char DSOC = readRAM8(PL40_RAM_DSOC);

        Log::debug("\n\n----------------------------------------------------");

        // Account for the charge/load scaling factor
        float fCEXT, fCINT, fLEXT, fLINT;

        if ((EXTF & 0x04)) {
            fCEXT = ((EXTF & 0x01) == 0) ? CEXT * 0.1 : CEXT;
        } else {
            fCEXT = 0;
        }

        if ((EXTF & 0x08) != 0) {
            fLEXT = ((EXTF & 0x02) == 0) ? LEXT * 0.1 : LEXT;
        } else {
            fLEXT = 0;
        }

        fCINT = CINT * 0.2;
        fLINT = LINT * 0.1;

        // Limit battery % to 100
        DSOC = (DSOC > 100) ? 100 : DSOC;

        // Produce totals
        float CTOT = (fCEXT + fCINT);
        float LTOT = (fLEXT + fLINT);
        int CDAYTOT = CIAH + CEAH;
        int LDAYTOT = LIAH + LEAH;

        float CTOTW = CTOT * 24;
        float LTOTW = LTOT * 24;
        int CDAYTOTW = CDAYTOT * 24;
        int LDAYTOTW = LDAYTOT * 24;

        float fBATV = BATV;
        fBATV = fBATV * myVoltageMultiplier;
        fBATV = fBATV / 10;

        Log::debug("CEAH=%d, CIAH=%d, LEAH=%d, LIAH=%d", CEAH, CIAH, LEAH, LIAH);
        Log::debug("CEXT=%f, CINT=%f, LEXT=%f, LINT=%f, EXTF=%.2x", fCEXT, fCINT, fLEXT, fLINT, EXTF);
        Log::debug("CTOT=%f, LTOT=%f, CDAYTOT=%d, LDAYTOT=%d", CTOT, LTOT, CDAYTOT, LDAYTOT);
        Log::debug("CTOTW=%f, LTOTW=%f, CDAYTOTW=%d, LDAYTOTW=%d", CTOTW, LTOTW, CDAYTOTW, LDAYTOTW);

        Log::debug("DOSC=%d", DSOC);
        Log::debug("----------------------------------------------------\n");



        // Process and populate the data

        // BatteryPercent (limit to 100)
        results->insert(KV("BatteryPercent", to_string(DSOC)));

        // LoadW
        results->insert(KV("LoadW", to_string((int)LDAYTOTW)));

        // InstantLoadW
        results->insert(KV("InstantLoadW", to_string((int)LTOTW)));

        // InstantChargeW
        results->insert(KV("InstantChargeW", to_string((int)CTOTW)));

        // ChargeW
        results->insert(KV("ChargeW", to_string((int)CDAYTOTW)));

        // BatteryVoltage
        results->insert(KV("BatteryVoltage", to_string(fBATV)));
    }
    catch (int error)
    {
        Log::error("Error polling PL40 values.");
        results->clear();
        return;
    }
}

