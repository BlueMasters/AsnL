/****************************************************************************
 * Copyright 2016 BlueMasters
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef ASN_L_PARSER__H
#define ASN_L_PARSER__H

#include "AsnLMsg.h"
#include <Arduino.h>

class AsnLParser  {

public:
    AsnLParser(AsnLMsg &aMsg) : _aMsg(aMsg) {};

    void init();
    int nextToken();
    int readInt(int8_t* value);
    int readInt(int16_t* value);
    int readInt(int32_t* value);
    int readUInt(uint8_t* value);
    int readUInt(uint16_t* value);
    int readUInt(uint32_t* value);
    int readString(char* buffer, int len);
    void close();
    int fixOk();

private:
    AsnLMsg &_aMsg;
    int _pos;
    int _valPtr;
    int _fix;

};

#endif
