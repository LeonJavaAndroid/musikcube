//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2008, Daniel �nnerby
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////
#include "pch.hpp"
#include <core/filestreams/RegularFileStream.h>
#include <core/config.h>

//////////////////////////////////////////////////////////////////////////////
#ifdef UTF_WIDECHAR
#define UTFFopen    _wfopen
typedef fpos_t  stdioPositionType;
#else
#define UTFFopen    fopen
typedef int stdioPositionType;
#endif
//////////////////////////////////////////////////////////////////////////////


using namespace musik::core::filestreams;

//////////////////////////////////////////////////////////////////////////////
RegularFileStream::RegularFileStream()
 :file(NULL)
{

}
RegularFileStream::~RegularFileStream(){
    this->Close();
}

bool RegularFileStream::Open(const utfchar *filename,unsigned int options){
    this->file  = UTFFopen(filename,UTF("r"));
    return this->file!=NULL;
}

bool RegularFileStream::Close(){
    if(this->file){
        if(fclose(this->file)==0){
            this->file  = NULL;
            return true;
        }
    }
    return false;
}

void RegularFileStream::Destroy(){
    delete this;
}

PositionType RegularFileStream::Read(void* buffer,PositionType readBytes){
    return (PositionType)fread(buffer,1,readBytes,this->file);
}

bool RegularFileStream::SetPosition(PositionType position){
    stdioPositionType newPosition  = (stdioPositionType)position;
    return fsetpos(this->file,&newPosition)==0;
}

PositionType RegularFileStream::Position(){
    stdioPositionType currentPosition(0);
    if(fgetpos(this->file,&currentPosition)==0){
        return (PositionType)currentPosition;
    }
    return -1;
}

bool RegularFileStream::Eof(){
    return feof(this->file)!=0;
}
