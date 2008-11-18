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

#include <core/GenericTrack.h>

//////////////////////////////////////////////////////////////////////////////

using namespace musik::core;

//////////////////////////////////////////////////////////////////////////////

GenericTrack::GenericTrack(void)
{
}

GenericTrack::GenericTrack(const utfchar *uri)
{
    if(uri){
        this->uri   = uri;
    }
}

GenericTrack::~GenericTrack(void){
}

const utfchar* GenericTrack::GetValue(const char* metakey){
    if(metakey){
        MetadataMap::iterator metavalue = this->metadata.find(metakey);
        if(metavalue!=this->metadata.end()){
            return metavalue->second.c_str();
        }
    }
    return NULL;
}

void GenericTrack::SetValue(const char* metakey,const utfchar* value){
    if(metakey && value){
        this->metadata.insert(std::pair<std::string,utfstring>(metakey,value));
    }
}

void GenericTrack::ClearValue(const char* metakey){
    this->metadata.erase(metakey);
}



void GenericTrack::SetThumbnail(const char *data,long size){
}

const utfchar* GenericTrack::URI(){
    if(!this->uri.empty()){
        return this->uri.c_str();
    }
    return NULL;
}

const utfchar* GenericTrack::URL(){
    if(!this->uri.empty()){
        return this->uri.c_str();
    }
    return NULL;
}

Track::MetadataIteratorRange GenericTrack::GetValues(const char* metakey){
    return this->metadata.equal_range(metakey);
}

Track::MetadataIteratorRange GenericTrack::GetAllValues(){
    return Track::MetadataIteratorRange(this->metadata.begin(),this->metadata.end());
}




TrackPtr GenericTrack::Copy(){
    return TrackPtr(new GenericTrack(this->uri.c_str()));
}

