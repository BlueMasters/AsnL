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

#ifndef ASN_L__H
#define ASN_L__H

// ASN Types
#define  ASNL_INT           'I'
#define  ASNL_UINT          'U'
#define  ASNL_STRING        '"'
#define  ASNL_STRUCT        '{'

// Tokens
#define  ASNL_END_STRUCT    '}'
#define  ASNL_NIL           '0'

class AsnL {

public:
    AsnL(int bufferSize);
    ~AsnL();
    int FixOk();
    void Dump();
    int FromCharArray(unsigned char* buffer, int bufferLen);
    int ToCharArray(unsigned char* buffer, int bufferLen);
    int FromAsnL(AsnL a);
    int ReadFromSerial();
    int WriteToSerial();


protected:
    unsigned char* msg;
    int msgCapacity;
    int msgLen;
    int fix;

};

#endif
