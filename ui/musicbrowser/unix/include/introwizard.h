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

class IntroWizardUI {
 public:
    IntroWizardUI(FAContext *);
   ~IntroWizardUI();
  
    void Show(bool runMain = true);

    int32 AcceptEvent(Event *);
 
    void SetSearchPath(string &newpath) { searchPath = newpath; }
    void SetSearchPath(char *newpath) { searchPath = newpath; }

    void UpdateEntry(void);

    void StartSearch();
    void EndSearch();
    void GoToPage1();
    void GoToPage2();
    
    unsigned int page;
    
    bool custom;
    bool searchInProgress;
    bool searchDone;
    bool m_main;
    bool done;

    GtkWidget *m_window;
    
 protected:
    FAContext *m_context;

 private:
    GtkWidget *m_backButton;
    GtkWidget *m_searchButton;
    GtkWidget *textEntry;
    GtkWidget *browseButton;
    GtkWidget *buttonLabel;

    GtkWidget *page1;
    GtkWidget *page2;
    
    string searchPath;

    GtkWidget *IntroPage(void);
    GtkWidget *SearchPage(void);
};
   
#endif
