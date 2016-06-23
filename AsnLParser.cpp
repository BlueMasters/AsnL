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

#include "AsnLParser.h"
#include "AsnLMsg.h"
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

void AsnLParser::init() {
    _pos = 0;
    _fix = 0;
}

int AsnLParser::nextToken() {
    if (_fix > 0 && _pos > (_fix + _aMsg.msg[_fix])) {
        // We reached the end of a structure. So we "pop" the address
        // of the previous "struct".
        int i = (int)_aMsg.msg[_fix-1];
        _aMsg.msg[_fix-1] = ASNL_STRUCT;
        _fix = i;
        return ASNL_END_STRUCT;
    }
    if (_pos >= _aMsg.msgLen) {
        return ASNL_NIL;
    }

    int type = (int)_aMsg.msg[_pos++];
    int len = (int)_aMsg.msg[_pos++];
    _valPtr = _pos;
    switch(type) {
    case ASNL_INT:
    case ASNL_UINT:
    case ASNL_STRING:
        _pos += len;
        break;
    case ASNL_STRUCT:
        // We use the "type" byte (ASNL_STRUCT) to chain the "structs"
        _aMsg.msg[_pos-2] = (unsigned char)_fix;
        _fix = _pos-1;
        break;
    }
    return type;
}

int AsnLParser::readInt(int32_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 4) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readInt(int16_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 2) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readInt(int8_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 1) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readUInt(uint32_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 4) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readUInt(uint16_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 2) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readUInt(uint8_t* value) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (len > 1) return -1;
    if (_valPtr + len > _aMsg.msgCapacity) return -1;
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + _aMsg.msg[_valPtr+i];
    }
    return 0;
}

int AsnLParser::readString(char* buffer, int bufferLen) {
    if (_valPtr <= 0 || _valPtr >= _aMsg.msgCapacity) return -1;
    int len = _aMsg.msg[_valPtr-1];
    if (_valPtr + len > _aMsg.msgCapacity) {
        return -1;
    }
    if (bufferLen < len+1) {
        return -1;
    }
    memcpy((void*)buffer, (void*)&_aMsg.msg[_valPtr], len);
    buffer[len] = 0;
    return len;
}

void AsnLParser::close() {
    while (_fix > 0) {
        int i = (int)_aMsg.msg[_fix-1];
        _aMsg.msg[_fix-1] = ASNL_STRUCT;
        _fix = i;
    }
}

int AsnLParser::fixOk() {
    return _fix == 0;
}
