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

#include <gtk/gtk.h>
#include "downloadui.h" 
#include "eventdata.h"

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new DownloadUI(context);
}
           }

DownloadUI::DownloadUI(FAContext *context)
{
    m_context = context;
    m_prefs = context->prefs;
    m_plm = m_context->plm;
    m_target = m_context->target;
    m_propManager = m_context->props;
    m_dlm = m_context->downloadManager;

    m_currentindex = 0;
    m_initialized = false;
    isVisible = false;
}

DownloadUI::~DownloadUI()
{
}


Error DownloadUI::Init(int32 startup_level) 
{
    if (startup_level != SECONDARY_UI_STARTUP) {
        cout << "The Download Manager cannot run as the primary ui\n"; 
        return kError_InitFailedSafely;
    }

    if (!getenv("DISPLAY"))
        return kError_InitFailedSafely;

    gtkThread = Thread::CreateThread();
    gtkThread->Create(DownloadUI::UIThreadFunc, this);

    return kError_NoErr;
}

void DownloadUI::UIThreadFunc(void *p) 
{
    assert(p);
    ((DownloadUI *)p)->GTKEventService();
}

void DownloadUI::SetRunning(void)
{
    m_context->gtkRunning = true;
}

static int download_timeout(DownloadUI *p)
{
    p->SetRunning();
    if (p->doQuitNow)
        gtk_main_quit();
}

void DownloadUI::GTKEventService(void)
{
    weAreGTK = false;
    doQuitNow = false;

    m_context->gtkLock.Acquire();
    if (!m_context->gtkInitialized) {
        m_context->gtkInitialized = true;

	g_thread_init(NULL);
	gtk_init(&m_context->argc, &m_context->argv);
	gdk_rgb_init();

	weAreGTK = true;
    }
    m_context->gtkLock.Release();

    if (weAreGTK) {
        gtk_timeout_add(250, download_timeout, this);
        gtk_main();
        gdk_threads_leave();
    }
}

int32 DownloadUI::AcceptEvent(Event *event)
{
    
    switch (event->Type()) {
        case CMD_Cleanup: {
            if (weAreGTK) 
                doQuitNow = true;

            gtkThread->Join();
           
            for (uint32 i = 0; i < downloadList.size(); i++) {
                DownloadItem *dli = downloadList[i];
                if (dli->GetState() == kDownloadItemState_Queued ||
                    dli->GetState() == kDownloadItemState_Downloading) {
                        m_dlm->CancelDownload(dli, false);
                    }
            } 
            m_target->AcceptEvent(new Event(INFO_ReadyToDieUI));

            break; }
        case CMD_ToggleDownloadUI: {
            gdk_threads_enter();
            if (m_initialized && isVisible) {
                gtk_widget_hide(m_downloadUI);
                isVisible = false;
            }
            else {
                if (m_initialized)
                    gtk_widget_show(m_downloadUI);
                else {
                    CreateDownloadUI();
                    m_initialized = true;
                }
                isVisible = true;
            }
            gdk_threads_leave();
            break; }
        case INFO_DownloadItemAdded: {
            DownloadItemAddedEvent *dliae = (DownloadItemAddedEvent *)event;
            downloadList.push_back(dliae->Item());

            if (isVisible) {
                gdk_threads_enter();
                UpdateDownloadList();
                UpdateOverallProgress();
                gdk_threads_leave();
            }
            break; }
        case INFO_DownloadItemRemoved:
        case INFO_DownloadItemProgress:
        case INFO_DownloadItemNewState: {
            if (isVisible) {
                gdk_threads_enter();
                UpdateDownloadList();
                UpdateOverallProgress();
                gdk_threads_leave();
            }
            break; }
        default:
            break;
    }
    return 0;
}

void DownloadUI::UpdateOverallProgress(void)
{
    uint32 itemCount = downloadList.size();
    uint32 totalBytes = 0, doneBytes = 0;
    uint32 totalItems = 0, doneItems = 0;
    DownloadItem *dli = NULL;
 
    if (itemCount) {
        for (uint32 i = 0; i < itemCount; i++) {
             dli = downloadList[i];
             DownloadItemState state = dli->GetState();

             if (state == kDownloadItemState_Queued ||
                 state == kDownloadItemState_Downloading) {
                 totalItems++;
                 totalBytes += dli->GetTotalBytes();
                 doneBytes += dli->GetBytesReceived();
             } 
             else if (state == kDownloadItemState_Done) {
                 doneItems++;
                 totalItems++;
                 totalBytes += dli->GetTotalBytes();
                 doneBytes += dli->GetTotalBytes();
             }
        }
    }
}

void DownloadUI::CancelEvent(void)
{
    if (downloadList.size() == 0 || !isVisible)
        return;

    DownloadItem *dli = downloadList[m_currentindex];
    if (dli == *downloadList.end())
        return;

    m_dlm->CancelDownload(dli, false);
    gtk_widget_set_sensitive(m_PauseButton, FALSE);
    gtk_widget_set_sensitive(m_CancelButton, FALSE);
    gtk_widget_set_sensitive(m_ResumeButton, TRUE);
}

void DownloadUI::PauseEvent(void)
{
    if (downloadList.size() == 0 || !isVisible)
        return;

    DownloadItem *dli = downloadList[m_currentindex];
    if (dli == *downloadList.end())
        return;

    m_dlm->CancelDownload(dli, true);
    gtk_widget_set_sensitive(m_PauseButton, FALSE);
    gtk_widget_set_sensitive(m_CancelButton, TRUE);
    gtk_widget_set_sensitive(m_ResumeButton, TRUE);
}

void DownloadUI::ResumeEvent(void)
{ 
    if (downloadList.size() == 0 || !isVisible)
        return;

    DownloadItem *dli = downloadList[m_currentindex];
    if (dli != *downloadList.end()) {
        m_dlm->QueueDownload(dli);
        gtk_widget_set_sensitive(m_PauseButton, TRUE);
        gtk_widget_set_sensitive(m_CancelButton, TRUE);
        gtk_widget_set_sensitive(m_ResumeButton, FALSE);
    }
}

void DownloadUI::SelChangeEvent(int row)
{
    m_currentindex = row;
    DownloadItem *dli = downloadList[m_currentindex];
 
    if (dli == *downloadList.end() || !isVisible)
        return;    

    switch (dli->GetState()) {
        case kDownloadItemState_Queued:
        case kDownloadItemState_Downloading: {
            gtk_widget_set_sensitive(m_PauseButton, TRUE);
            gtk_widget_set_sensitive(m_CancelButton, TRUE);
            gtk_widget_set_sensitive(m_ResumeButton, FALSE);
            break; }
        case kDownloadItemState_Cancelled:
        case kDownloadItemState_Error: {
            gtk_widget_set_sensitive(m_PauseButton, FALSE);
            gtk_widget_set_sensitive(m_CancelButton, FALSE);
            gtk_widget_set_sensitive(m_ResumeButton, TRUE);
            break; }
        case kDownloadItemState_Paused: {
            gtk_widget_set_sensitive(m_PauseButton, FALSE);
            gtk_widget_set_sensitive(m_CancelButton, TRUE);
            gtk_widget_set_sensitive(m_ResumeButton, TRUE);
            break; }
        case kDownloadItemState_Done: {
            gtk_widget_set_sensitive(m_PauseButton, FALSE);
            gtk_widget_set_sensitive(m_CancelButton, FALSE);
            gtk_widget_set_sensitive(m_ResumeButton, FALSE);
            break; }
        default:
            break;
    }
    UpdateInfo();
}
