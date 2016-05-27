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

#include "AsnL.h"
#include <stdlib.h>
#include <Arduino.h>

AsnL::AsnL(int bufferSize) {
    msgCapacity = bufferSize;
    msg = (unsigned char*) malloc(bufferSize);
    msgLen = 0;
    fix = 0;
}

AsnL::~AsnL() {
    if (msg != 0) {
        free(msg);
    }
}

int AsnL::FixOk() {
    return fix == 0;
}

void AsnL::Dump() {
    for (int i = 0; i < msgLen; i++) {
        int x = msg[i];
        int xh = (x / 16) % 16;
        int xl = x % 16;
        Serial.print("0x");
        Serial.print((char)(xh < 10 ? ('0' + xh) : ('a' - 10 + xh)));
        Serial.print((char)(xl < 10 ? ('0' + xl) : ('a' - 10 + xl)));
        Serial.print(' ');
        if (i % 8 == 7) {
            Serial.println();
        }
    }
    Serial.println();
}

int AsnL::FromCharArray(unsigned char* buffer, int bufferLen) {
    if (msgCapacity < bufferLen) return -1;
    msgLen = bufferLen;
    memcpy((void*)msg, (void*)buffer, msgLen);
    return msgLen;
}

int AsnL::ToCharArray(unsigned char* buffer, int bufferLen) {
    if (bufferLen < msgLen) return -1;
    memcpy((void*)buffer, (void*)msg, msgLen);
    return msgLen;
}

int AsnL::FromAsnL(AsnL a) {
    if (msgCapacity < a.msgLen) return -1;
    msgLen = a.msgLen;
    memcpy((void*)msg, (void*)a.msg, msgLen);
    return msgLen;
}
