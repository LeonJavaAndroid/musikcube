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
#include <core/LibraryFactory.h>
#include <core/Library/LocalDB.h>

using namespace musik::core;

LibraryFactory LibraryFactory::sInstance;

LibraryFactory::LibraryFactory(void) : currentPosition(0){
    // The first library should always be a localDB
    LibraryPtr localLibrary(new musik::core::Library::LocalDB());
    localLibrary->Startup();
    this->libraries.push_back(localLibrary);
}

LibraryFactory::~LibraryFactory(void){
}


LibraryPtr LibraryFactory::GetCurrentLibrary(){
    return Instance().GetLibrary(sInstance.currentPosition);
}

LibraryFactory::LibraryChangeEvent& LibraryFactory::OnLibraryChange(){
    return Instance().OnLibraryChangeSignal;
}

LibraryPtr LibraryFactory::GetLibrary(int position){
    if(position>=0 && position<this->libraries.size()){
        return this->libraries[position];
    }
    return LibraryPtr();
}


