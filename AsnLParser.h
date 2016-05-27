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

#include "AsnL.h"

class AsnLParser : public AsnL {

public:
    AsnLParser(int bufferSize) : AsnL(bufferSize) {};

    void Init();
    int Init(char* buffer, int len);
    int NextToken();
    int ReadInt(int* value);
    int ReadString(char* buffer, int len);
    void Abort();

private:
    int pos;
    int valPtr;

};

#endif
