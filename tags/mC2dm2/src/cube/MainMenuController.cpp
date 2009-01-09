//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2007, Casey Langen
//
// Sources and Binaries of: mC2, win32cpp
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
#include <core/MessageQueue.h>
#include <cube/MainMenuController.hpp>
#include <cube/dialog/AddLibraryController.hpp>
#include <cube/dialog/HelpAboutController.hpp>
#include <cube/dialog/PreferencesController.hpp>
#include <win32cpp/Application.hpp>
#include <win32cpp/TopLevelWindow.hpp>
#include <boost/format.hpp>


//////////////////////////////////////////////////////////////////////////////

using namespace musik::cube;

//////////////////////////////////////////////////////////////////////////////

/*ctor*/    MainMenuController::MainMenuController(TopLevelWindow& mainWindow)
: mainWindow(mainWindow)
{
    this->mainWindow.Created.connect(
        this, &MainMenuController::OnMainWindowCreated);
}

MainMenuController::~MainMenuController(){
}

void        MainMenuController::OnMainWindowCreated(Window* window)
{
    this->mainWindow.SetMenu(this->CreateMenu());
}

void        MainMenuController::ConnectMenuHandlers()
{
    this->fileExit->Activated.connect(this, &MainMenuController::OnFileExit);
//    this->filePreferences->Activated.connect(this, &MainMenuController::OnFilePreferences);
    this->helpAbout->Activated.connect(this, &MainMenuController::OnHelpAbout);
    this->fileAddLibraryLocal->Activated.connect(this,&MainMenuController::OnAddLibraryLocal);
    this->fileAddLibraryRemote->Activated.connect(this,&MainMenuController::OnAddLibraryRemote);
//    this->fileNewPlaylist->Activated.connect(this,&MainMenuController::OnNewPlaylist);
}

void        MainMenuController::OnFileExit(MenuItemRef menuItem)
{
    Application::Instance().Terminate();
}

void        MainMenuController::OnFilePreferences(MenuItemRef menuItem)
{
    win32cpp::TopLevelWindow preferencesDialog(_(_T("mC2 Preferences")));
    preferencesDialog.SetMinimumSize(Size(700, 500));

    dialog::PreferencesController filePreferences(preferencesDialog);

    preferencesDialog.ShowModal(&this->mainWindow);
}

void        MainMenuController::OnAddLibraryLocal(MenuItemRef menuItem)
{
    win32cpp::TopLevelWindow popupDialog(_(_T("Add local library")));
    popupDialog.SetMinimumSize(Size(300, 150));

    dialog::AddLibraryController addLibrary(popupDialog,musik::core::LibraryFactory::LocalDB);

    popupDialog.ShowModal(&this->mainWindow);

}
void        MainMenuController::OnAddLibraryRemote(MenuItemRef menuItem)
{
    win32cpp::TopLevelWindow popupDialog(_(_T("Add remote library")));
    popupDialog.SetMinimumSize(Size(300, 220));

    dialog::AddLibraryController addLibrary(popupDialog,musik::core::LibraryFactory::Remote);

    popupDialog.ShowModal(&this->mainWindow);

}

void        MainMenuController::OnHelpAbout(MenuItemRef menuItem)
{
    win32cpp::TopLevelWindow aboutDialog(_(_T("About mC2")));
    aboutDialog.SetMinimumSize(Size(400,300));

    dialog::HelpAboutController helpAbout(aboutDialog);

    aboutDialog.ShowModal(&this->mainWindow);
}

void MainMenuController::OnNewPlaylist(MenuItemRef menuItem){
	musik::core::MessageQueue::SendMessage("NewPlaylist");
}


MenuRef     MainMenuController::CreateMenu()
{
    // main menu
    this->mainMenu  = Menu::Create();
    MenuItemCollection& mainItems = this->mainMenu->Items();
    //
    this->file = mainItems.Append(MenuItem::Create(_(_T("&File"))));
    this->view = mainItems.Append(MenuItem::Create(_(_T("&View"))));
    this->audio = mainItems.Append(MenuItem::Create(_(_T("&Audio"))));
    this->tags = mainItems.Append(MenuItem::Create(_(_T("&Tags"))));
    this->help = mainItems.Append(MenuItem::Create(_(_T("&Help"))));

    // file menu
    this->fileMenu  = Menu::Create();
    MenuItemCollection& fileItems = this->fileMenu->Items();
    //
    this->file->SetSubMenu(this->fileMenu);

    MenuItemRef addLibraryMenu  = fileItems.Append(MenuItem::Create(_(_T("&New Library"))));
    MenuRef addLibrarySubmenu   = Menu::Create();
    this->fileAddLibraryLocal   = addLibrarySubmenu->Items().Append(MenuItem::Create(_(_T("&Local library"))));
    this->fileAddLibraryRemote  = addLibrarySubmenu->Items().Append(MenuItem::Create(_(_T("&Remote library"))));
    addLibraryMenu->SetSubMenu(addLibrarySubmenu);

    this->fileNewPlaylist       = fileItems.Append(MenuItem::Create(_(_T("&New Playlist"))));

    this->filePreferences       = fileItems.Append(MenuItem::Create(_(_T("&Preferences"))));

    this->fileExit              = fileItems.Append(MenuItem::Create(_(_T("E&xit"))));

    // help menu
    this->helpMenu  = Menu::Create();
    MenuItemCollection& helpItems = this->helpMenu->Items();
    //
    this->help->SetSubMenu(this->helpMenu);
    this->helpAbout = helpItems.Append(MenuItem::Create(_(_T("&About"))));

    this->ConnectMenuHandlers();

    return this->mainMenu;
}