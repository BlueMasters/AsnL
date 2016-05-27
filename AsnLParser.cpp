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
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

void AsnLParser::Init() {
    pos = 0;
    fix = 0;
}

int AsnLParser::NextToken() {
    if (fix > 0 && pos > (fix + msg[fix])) {
        // We reached the end of a structure. So we "pop" the address
        // of the previous "struct".
        int i = (int)msg[fix-1];
        msg[fix-1] = ASNL_STRUCT;
        fix = i;
        return ASNL_END_STRUCT;
    }
    if (pos >= msgLen) {
        return ASNL_NIL;
    }

    int type = (int)msg[pos++];
    int len = (int)msg[pos++];
    valPtr = pos;
    switch(type) {
    case ASNL_INT:
        pos += len;
        break;
    case ASNL_STRING:
        pos += len;
        break;
    case ASNL_STRUCT:
        // We use the "type" byte (ASNL_STRUCT) to chain the "structs"
        msg[pos-2] = (unsigned char)fix;
        fix = pos-1;
        break;
    }
    return type;
}

int AsnLParser::ReadInt(int* value) {
    if (valPtr <= 0 || valPtr >= msgCapacity) return -1;
    int len = msg[valPtr-1];
    if (valPtr + len > msgCapacity) return -1;
    uint32_t v = 0;
    for (int i = 0; i < len; i++) {
        v = v * 256 + msg[valPtr+i];
    }
    *value = (int)v;
    return 0;
}

int AsnLParser::ReadString(char* buffer, int bufferLen) {
    if (valPtr <= 0 || valPtr >= msgCapacity) return -1;
    int len = msg[valPtr-1];
    if (valPtr + len > msgCapacity) {
        return -1;
    }
    if (bufferLen < len+1) {
        return -1;
    }
    memcpy((void*)buffer, (void*)&msg[valPtr], len);
    buffer[len] = 0;
    return len;
}

void AsnLParser::Abort() {
    while (fix > 0) {
        int i = (int)msg[fix-1];
        msg[fix-1] = ASNL_STRUCT;
        fix = i;
    }
}
