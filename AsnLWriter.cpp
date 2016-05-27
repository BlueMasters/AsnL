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
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

void AsnLWriter::Init() {
    msgLen = 0;
    fix = 0;
}

int AsnLWriter::Int(int len, int value) {
    if (msgLen + 2 + len > msgCapacity) return -1;
    msg[msgLen] = ASNL_INT;
    msg[msgLen+1] = (unsigned char) len;
    // write value in BigEndian (MSB first)
    uint32_t v = uint32_t(value);
    for (int i = 0; i < len; i++) {
        msg[msgLen + 1 + len - i] = (unsigned char)(v % 256);
        v = v / 256;
    }
    msgLen += len + 2;
    return msgLen;
}

int AsnLWriter::Uint(int len, unsigned int value) {
    if (msgLen + 2 + len > msgCapacity) return -1;
    msg[msgLen] = ASNL_UINT;
    msg[msgLen+1] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        msg[msgLen + 1 + len - i] = (unsigned char)(value % 256);
        value = value / 256;
    }
    msgLen += len + 2;
    return msgLen;
}

int AsnLWriter::Str(String txt) {
    int len = txt.length();
    if (msgLen + 2 + len > msgCapacity) return -1;
    msg[msgLen++] = ASNL_STRING;
    msg[msgLen++] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        msg[msgLen++] = (unsigned char)(txt.charAt(i));
    }
    return msgLen;
}

int AsnLWriter::Struct() {
    if (msgLen + 2 > msgCapacity) return -1;
    msg[msgLen++] = ASNL_STRUCT;
    // Here we use the "length" field to chain the struct tokens.
    // This will be "fixed" with the EndStruct function.
    msg[msgLen] = (unsigned char) fix;
    fix = msgLen++;
    return msgLen;
}

int AsnLWriter::EndStruct() {
    if (fix > 0) {
        // Fix the "length" field and pop the address of the previous
        // "struct".
        int i = (int)msg[fix];
        msg[fix] = (unsigned int)(msgLen - fix - 1);
        fix = i;
        return 0;
    } else {
        return -1;
    }
}

int AsnLWriter::Close() {
    while (fix > 0) {
        int i = (int)msg[fix];
        msg[fix] = (unsigned int)(msgLen - fix - 1);
        fix = i;
    }
    return 0;
}
