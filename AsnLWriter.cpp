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

#include "AsnLWriter.h"
#include "AsnLMsg.h"
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

void AsnLWriter::init() {
    _aMsg.msgLen = 0;
    _fix = 0;
}

int AsnLWriter::integer(int len, int value) {
    if (_aMsg.msgLen + 2 + len > _aMsg.msgCapacity) return -1;
    _aMsg.msg[_aMsg.msgLen] = ASNL_INT;
    _aMsg.msg[_aMsg.msgLen+1] = (unsigned char) len;
    // write value in BigEndian (MSB first)
    uint32_t v = uint32_t(value);
    for (int i = 0; i < len; i++) {
        _aMsg.msg[_aMsg.msgLen + 1 + len - i] = (unsigned char)(v % 256);
        v = v / 256;
    }
    _aMsg.msgLen += len + 2;
    return _aMsg.msgLen;
}

int AsnLWriter::uinteger(int len, unsigned int value) {
    if (_aMsg.msgLen + 2 + len > _aMsg.msgCapacity) return -1;
    _aMsg.msg[_aMsg.msgLen] = ASNL_UINT;
    _aMsg.msg[_aMsg.msgLen+1] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        _aMsg.msg[_aMsg.msgLen + 1 + len - i] = (unsigned char)(value % 256);
        value = value / 256;
    }
    _aMsg.msgLen += len + 2;
    return _aMsg.msgLen;
}

int AsnLWriter::string(String txt) {
    int len = txt.length();
    if (_aMsg.msgLen + 2 + len > _aMsg.msgCapacity) return -1;
    _aMsg.msg[_aMsg.msgLen++] = ASNL_STRING;
    _aMsg.msg[_aMsg.msgLen++] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        _aMsg.msg[_aMsg.msgLen++] = (unsigned char)(txt.charAt(i));
    }
    return _aMsg.msgLen;
}

int AsnLWriter::structure() {
    if (_aMsg.msgLen + 2 > _aMsg.msgCapacity) return -1;
    _aMsg.msg[_aMsg.msgLen++] = ASNL_STRUCT;
    // Here we use the "length" field to chain the struct tokens.
    // This will be "fixed" with the EndStruct function.
    _aMsg.msg[_aMsg.msgLen] = (unsigned char) _fix;
    _fix = _aMsg.msgLen++;
    return _aMsg.msgLen;
}

int AsnLWriter::endStructure() {
    if (_fix > 0) {
        // Fix the "length" field and pop the address of the previous
        // "struct".
        int i = (int)_aMsg.msg[_fix];
        _aMsg.msg[_fix] = (unsigned int)(_aMsg.msgLen - _fix - 1);
        _fix = i;
        return 0;
    } else {
        return -1;
    }
}

int AsnLWriter::close() {
    while (_fix > 0) {
        int i = (int)_aMsg.msg[_fix];
        _aMsg.msg[_fix] = (unsigned int)(_aMsg.msgLen - _fix - 1);
        _fix = i;
    }
    return 0;
}

int AsnLWriter::fixOk() {
    return _fix == 0;
}
