/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999 EMusic.com

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

        $Id$
____________________________________________________________________________*/

#ifndef INCLUDED_INTROWIZARD_H_
#define INCLUDED_INTROWIZARD_H_

#include "config.h"

#include <gtk/gtk.h>

#include "facontext.h"

class MusicBrowserUI;

class IntroWizardUI {
 public:
    IntroWizardUI(FAContext *, MusicBrowserUI *parent);
   ~IntroWizardUI();
  
    void Show(bool runMain = true);

    Error AcceptEvent(Event *);
 
    void SetSearchPath(string &newpath) { searchPath = newpath; }
    void SetSearchPath(const char *newpath) { searchPath = newpath; }

    void UpdateEntry(void);

    void StartSearch();
    void EndSearch();
    void GoToPage1();
    void GoToPage2();
    void GoToPage3();
    void GoToPage4();
    void GoToPage5();
    void GoToPage6();
    void DeleteEvent();
    void Close();
   
    void CheckCreateProfile();
    void SetMBPref(void);
    void SetBitziPref(void);

    unsigned int page;
    
    bool custom;
    bool searchInProgress;
    bool searchDone;
    bool done;

    bool skipRelatableTwo;
    bool bitziEnabled;
    bool mbEnabled;

    GtkWidget *m_window;
	FAContext *GetContext(void) { return m_context; }
    
 protected:
    FAContext *m_context;

 private:
    bool m_main;
 
    GtkWidget *m_backButton;
    GtkWidget *m_searchButton;
    GtkWidget *textEntry;
    GtkWidget *browseButton;
    GtkWidget *buttonLabel;

    GtkWidget *profileEntry;
    
    GtkWidget *page1;
    GtkWidget *page2;
    GtkWidget *page3;
    GtkWidget *page4;
    GtkWidget *page5;
    GtkWidget *page6;
   
    string searchPath;

    GtkWidget *IntroPage(void);
    GtkWidget *RelatablePage(void);
    GtkWidget *RelatableTwoPage(void);
    GtkWidget *MusicBrainzPage(void);
    GtkWidget *BitziPage(void);
    GtkWidget *SearchPage(void);

    MusicBrowserUI *m_parent;
};
   
#endif
