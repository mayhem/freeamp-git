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

/* system headers */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "player.h"
#include "eventdata.h"
#include "GTKPreferenceWindow.h"
#include "GTKWindow.h"

GTKPreferenceWindow::GTKPreferenceWindow(FAContext *context,
                                         ThemeManager *pThemeMan) :
     PreferenceWindow(context, pThemeMan)
{     
}

GTKPreferenceWindow::~GTKPreferenceWindow(void)
{
} 

static gboolean pref_destroy(GtkWidget *widget)
{
    gtk_main_quit();
    return FALSE;
}

void GTKPreferenceWindow::ApplyInfo(void)
{
    if (proposedValues != currentValues) 
        SavePrefsValues(m_pContext->prefs, &proposedValues);
}

void pref_ok_click(GtkWidget *w, GTKPreferenceWindow *p)
{
    p->ApplyInfo();
    gtk_widget_destroy(p->mainWindow);
}

void pref_apply_click(GtkWidget *w, GTKPreferenceWindow *p)
{
    p->ApplyInfo();
}

void GTKPreferenceWindow::CancelInfo(void)
{
    if (currentValues != originalValues)
        SavePrefsValues(m_pContext->prefs, &proposedValues);
}

void pref_cancel_click(GtkWidget *w, GTKPreferenceWindow *p)
{
    p->CancelInfo();
    gtk_widget_destroy(p->mainWindow);
}

void pref_close_click(GtkWidget *w, GTKPreferenceWindow *p)
{
    gtk_widget_destroy(p->mainWindow);
}

bool GTKPreferenceWindow::Show(Window *pWindow)
{
    GetPrefsValues(m_pContext->prefs, &originalValues);

    currentValues = proposedValues = originalValues;

    gdk_threads_enter();
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(mainWindow), TRUE);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "destroy",
                       GTK_SIGNAL_FUNC(pref_destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(mainWindow), BRANDING" - Preferences");

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), vbox);
    gtk_widget_show(vbox);

    GtkWidget *notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(notebook), 5);
    gtk_widget_realize(notebook);
   
    GtkWidget *pane;
    GtkWidget *label;
/*
    label = gtk_label_new("General");
    gtk_widget_show(label);
    pane = CreatePage1();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);
*/
    label = gtk_label_new("Themes");
    gtk_widget_show(label);
    pane = CreatePage5();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Streaming");
    gtk_widget_show(label);
    pane = CreatePage2();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Plugins");
    gtk_widget_show(label);
    pane = CreatePage3();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Advanced");
    gtk_widget_show(label);
    pane = CreatePage6();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("About");
    gtk_widget_show(label);
    pane = CreateAbout();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    gtk_widget_show(notebook);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    GtkWidget *button;

    button = gtk_button_new_with_label("  Help  ");
    gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    applyButton = gtk_button_new_with_label("  Apply  ");
    gtk_signal_connect(GTK_OBJECT(applyButton), "clicked",
                       GTK_SIGNAL_FUNC(pref_apply_click), this);
    gtk_box_pack_end(GTK_BOX(hbox), applyButton, FALSE, FALSE, 0);
    gtk_widget_show(applyButton);
    gtk_widget_set_sensitive(applyButton, FALSE);

    button = gtk_button_new_with_label("  Cancel  ");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(pref_cancel_click), this);
    gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("  OK  ");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(pref_ok_click), this);
    gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    gtk_widget_show(mainWindow);

    gdk_threads_leave();

    gtk_main();

    gdk_threads_leave();
}

void GTKPreferenceWindow::GetPrefsValues(Preferences* prefs, 
                                         PrefsStruct* values)
{
    uint32 bufferSize = 1;
    uint32 size;

    char *buffer = (char *)malloc(bufferSize);

    size = bufferSize;

    if (kError_BufferTooSmall == prefs->GetDefaultPMO(buffer, &size)) {
        buffer = (char *)realloc(buffer, size);
        prefs->GetDefaultPMO(buffer, &size);
    }
    values->defaultPMO = buffer;
    size = bufferSize;

    prefs->GetInputBufferSize(&values->inputBufferSize);
    prefs->GetOutputBufferSize(&values->outputBufferSize);
    prefs->GetPrebufferLength(&values->preBufferLength);

    prefs->GetStreamBufferInterval(&values->streamInterval);
    prefs->GetSaveStreams(&values->saveStreams);
    
    if (kError_BufferTooSmall == prefs->GetProxyServerAddress(buffer, &size)) {
        buffer = (char *)realloc(buffer, size);
        prefs->GetProxyServerAddress(buffer, &size);
    }
    values->proxyServer = buffer;

    prefs->GetUseProxyServer(&values->useProxyServer);

    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetSaveStreamsDirectory(buffer, &size)) {
        size++;
        buffer = (char*)realloc(buffer, size);
        prefs->GetSaveStreamsDirectory(buffer, &size);
    }
    values->saveStreamsDirectory = buffer;
    
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetAlternateNICAddress(buffer, &size)) {
        buffer = (char*)realloc(buffer, size);
        prefs->GetAlternateNICAddress(buffer, &size);
    }

    values->alternateIP = buffer;
    size = bufferSize;

    prefs->GetUseAlternateNIC(&values->useAlternateIP);

    prefs->GetUseDebugLog(&values->enableLogging);
    prefs->GetLogMain(&values->logMain);
    prefs->GetLogDecode(&values->logDecoder);
    prefs->GetLogInput(&values->logInput);
    prefs->GetLogOutput(&values->logOutput);
    prefs->GetLogPerformance(&values->logPerformance);

    if(kError_BufferTooSmall == prefs->GetThemeDefaultFont(buffer, &size)) {
        size++;
        buffer = (char*)realloc(buffer, size);
        prefs->GetThemeDefaultFont(buffer, &size);
    }

    values->defaultFont = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetSaveMusicDirectory(buffer, &size)) {
        buffer = (char*)realloc(buffer, size);
        prefs->GetSaveMusicDirectory(buffer, &size);
    }
    values->saveMusicDirectory = buffer;

    m_pThemeMan->GetCurrentTheme(values->currentTheme);

    if(kError_BufferTooSmall == prefs->GetSaveMusicDirectory(buffer, &size))
    {
        size++;
        bufferSize = size;
        buffer = (char*)realloc(buffer, bufferSize);
        prefs->GetSaveMusicDirectory(buffer, &size);
    }

    values->saveMusicDirectory = buffer;
    size = bufferSize;

    if(kError_BufferTooSmall == prefs->GetUsersPortablePlayers(buffer, &size)) {
        buffer = (char*)realloc(buffer, size);
        prefs->GetUsersPortablePlayers(buffer, &size);
    }

    char* cp = buffer;
    char* name = cp;

    while ((cp = strchr(cp, ';'))) {
        *cp = 0x00;
        values->portablePlayers.insert(string(name));
        cp++;
        name = cp;
    }

    if (*name) 
        values->portablePlayers.insert(string(name));

    free(buffer);
}

void GTKPreferenceWindow::SavePrefsValues(Preferences* prefs, 
                                          PrefsStruct* values)
{
    prefs->SetDefaultPMO(values->defaultPMO.c_str());
    prefs->SetInputBufferSize(values->inputBufferSize);
    prefs->SetOutputBufferSize(values->outputBufferSize);
    prefs->SetPrebufferLength(values->preBufferLength);

    prefs->SetStreamBufferInterval(values->streamInterval);
    prefs->SetSaveStreams(values->saveStreams);
    prefs->SetSaveStreamsDirectory(values->saveStreamsDirectory.c_str());
    prefs->SetProxyServerAddress(values->proxyServer.c_str());
    prefs->SetUseProxyServer(values->useProxyServer);
    prefs->SetAlternateNICAddress(values->alternateIP.c_str());
    prefs->SetUseAlternateNIC(values->useAlternateIP);

    prefs->SetUseDebugLog(values->enableLogging);
    prefs->SetLogMain(values->logMain);
    prefs->SetLogDecode(values->logDecoder);
    prefs->SetLogInput(values->logInput);
    prefs->SetLogOutput(values->logOutput);
    prefs->SetLogPerformance(values->logPerformance);

    prefs->SetThemeDefaultFont(values->defaultFont.c_str());

    map<string, string>::iterator i;
    int32 iLoop = 0;

    for (i = m_oThemeList.begin(); i != m_oThemeList.end(); i++, iLoop++) {
         if (iLoop == values->listboxIndex)
             values->currentTheme = (*i).first;
    }
    m_pThemeMan->UseTheme(m_oThemeList[values->currentTheme]);

    prefs->SetSaveMusicDirectory(values->saveMusicDirectory.c_str());

    PortableSet::const_iterator j = values->portablePlayers.begin();
    string portableList;

    for (; j != values->portablePlayers.end(); j++) {
        portableList += *j;
        portableList += ";";
    }
    prefs->SetUsersPortablePlayers(portableList.c_str());

    Registry *pmo = m_pContext->player->GetPMORegistry();
    int32 k = 0;
    RegistryItem *item;

    while (pmo && (item = pmo->GetItem(k++))) {
        if (values->outputIndex == (k-1))
            values->defaultPMO = item->Name();
    }
    prefs->SetDefaultPMO(values->defaultPMO.c_str());
    
    if (*values != currentValues) {
        m_pContext->target->AcceptEvent(new Event(INFO_PrefsChanged));
        currentValues = proposedValues = *values;
    }
}

GtkWidget *GTKPreferenceWindow::CreatePage1(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    return pane;
}

void GTKPreferenceWindow::SetStreamInterval(int newvalue)
{
    proposedValues.streamInterval = newvalue;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void stream_interval_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    int newdata = atoi(text);

    p->SetStreamInterval(newdata);
}

void GTKPreferenceWindow::SaveLocalToggle(int active)
{
    gtk_widget_set_sensitive(saveStreamLabel, active);
    gtk_widget_set_sensitive(saveStreamBox, active);
//    gtk_widget_set_sensitive(saveBrowseBox, active);
    proposedValues.saveStreams = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void save_local_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->SaveLocalToggle(i);
}

void GTKPreferenceWindow::ProxyToggle(int active)
{
    gtk_widget_set_sensitive(proxyAddyBox, active);
    gtk_widget_set_sensitive(proxyPortBox, active);
    gtk_widget_set_sensitive(proxyColon, active);
    gtk_widget_set_sensitive(proxyAddyLabel, active);
    gtk_widget_set_sensitive(proxyPortLabel, active);
    proposedValues.useProxyServer = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void proxy_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->ProxyToggle(i);
}

void GTKPreferenceWindow::AltIPToggle(int active)
{
    gtk_widget_set_sensitive(ipLabel, active);
    gtk_widget_set_sensitive(ipOneBox, active);
    gtk_widget_set_sensitive(ipPeriod1, active);
    gtk_widget_set_sensitive(ipTwoBox, active);
    gtk_widget_set_sensitive(ipPeriod2, active);
    gtk_widget_set_sensitive(ipThreeBox, active);
    gtk_widget_set_sensitive(ipPeriod3, active);
    gtk_widget_set_sensitive(ipFourBox, active);
    proposedValues.useAlternateIP = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void alt_ip_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->AltIPToggle(i);
}

void GTKPreferenceWindow::SaveLocalSet(char *newpath)
{
    proposedValues.saveStreamsDirectory = newpath;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void save_stream_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    p->SaveLocalSet(text);
}

void GTKPreferenceWindow::ProxyAddySet()
{
    char *name = gtk_entry_get_text(GTK_ENTRY(proxyAddyBox));
    char *port = gtk_entry_get_text(GTK_ENTRY(proxyPortBox));

    proposedValues.proxyServer = name;
    if (*port) {
        proposedValues.proxyServer += string(":") + string(port);
    }
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void proxy_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    p->ProxyAddySet();
}

void GTKPreferenceWindow::AltIPSet()
{
    char *one = gtk_entry_get_text(GTK_ENTRY(ipOneBox));
    char *two = gtk_entry_get_text(GTK_ENTRY(ipTwoBox));
    char *three = gtk_entry_get_text(GTK_ENTRY(ipThreeBox));
    char *four = gtk_entry_get_text(GTK_ENTRY(ipFourBox));
    if (*one)
        proposedValues.alternateIP = one;
    else
        proposedValues.alternateIP = "0";
    proposedValues.alternateIP += ".";
    if (*two)
        proposedValues.alternateIP += two;
    else
        proposedValues.alternateIP += "0";
    proposedValues.alternateIP += ".";
    if (*three)
        proposedValues.alternateIP += three;
    else
        proposedValues.alternateIP += "0";
    proposedValues.alternateIP += ".";
    if (*four)
        proposedValues.alternateIP += four;
    else
        proposedValues.alternateIP += "0";
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void ip_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    p->AltIPSet();
}

GtkWidget *GTKPreferenceWindow::CreatePage2(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *frame = gtk_frame_new("Buffer Sizes");
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), hbox);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_widget_show(hbox);

    GtkWidget *label = gtk_label_new("Buffer Streams For ");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_widget_show(label);

    int value;
    char tempstr[256];

    GtkWidget *entry = gtk_entry_new();
    value = originalValues.streamInterval;
    sprintf(tempstr, "%d", value);
    gtk_entry_set_text(GTK_ENTRY(entry), tempstr);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
    gtk_widget_set_usize(entry, 32, 0);
    gtk_signal_connect(GTK_OBJECT(entry), "changed",
                       GTK_SIGNAL_FUNC(stream_interval_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, FALSE, 0);
    gtk_widget_show(entry);

    label = gtk_label_new(" Seconds");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_widget_show(label);

    GtkWidget *check = gtk_check_button_new_with_label("Save Streams Locally");
    gtk_container_add(GTK_CONTAINER(pane), check);
    gtk_signal_connect(GTK_OBJECT(check), "toggled",
                       GTK_SIGNAL_FUNC(save_local_toggle), this);
    if (originalValues.saveStreams)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
    gtk_widget_show(check);

    frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    gtk_widget_show(vbox);

    saveStreamLabel = gtk_label_new("Save Location:");
    gtk_misc_set_alignment(GTK_MISC(saveStreamLabel), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), saveStreamLabel, FALSE, FALSE, 0);
    gtk_widget_show(saveStreamLabel);

    char copys[256];

    strncpy(copys, originalValues.saveStreamsDirectory.c_str(), 256);
    saveStreamBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(saveStreamBox), copys); 
    gtk_entry_set_max_length(GTK_ENTRY(saveStreamBox), 64);
    gtk_signal_connect(GTK_OBJECT(saveStreamBox), "changed",
                       GTK_SIGNAL_FUNC(save_stream_change), this);
    gtk_box_pack_start(GTK_BOX(vbox), saveStreamBox, FALSE, FALSE, 0);
    gtk_widget_show(saveStreamBox);

    if (!originalValues.saveStreams) {
        gtk_widget_set_sensitive(saveStreamLabel, FALSE);
        gtk_widget_set_sensitive(saveStreamBox, FALSE);
//        gtk_widget_set_sensitive(saveBrowseBox, FALSE);
    }

    check = gtk_check_button_new_with_label("Use Proxy Server");
    gtk_container_add(GTK_CONTAINER(pane), check);
    gtk_signal_connect(GTK_OBJECT(check), "toggled",
                       GTK_SIGNAL_FUNC(proxy_toggle), this);
    if (originalValues.useProxyServer)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
    gtk_widget_show(check);

    frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    gtk_widget_show(vbox);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 0);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    proxyAddyLabel = gtk_label_new("Proxy Address:");
    gtk_box_pack_start(GTK_BOX(hbox), proxyAddyLabel, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(proxyAddyLabel), 0.0, 0.5);
    gtk_widget_show(proxyAddyLabel);

    proxyPortLabel = gtk_label_new("Port:");
    gtk_box_pack_end(GTK_BOX(hbox), proxyPortLabel, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(proxyPortLabel), 0.9, 0.5);
    gtk_widget_show(proxyPortLabel);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 0);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    char *port = NULL;
    strncpy(tempstr, originalValues.proxyServer.c_str(), 256);
    port = strrchr(tempstr, ':');
    if (port) {
        *port = '\0';
        port++;
    }

    proxyAddyBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(proxyAddyBox), tempstr);
    gtk_signal_connect(GTK_OBJECT(proxyAddyBox), "changed",
                       GTK_SIGNAL_FUNC(proxy_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), proxyAddyBox, FALSE, TRUE, 0);
    gtk_widget_show(proxyAddyBox);

    proxyColon = gtk_label_new(":");
    gtk_box_pack_start(GTK_BOX(hbox), proxyColon, FALSE, FALSE, 5);
    gtk_widget_show(proxyColon);

    proxyPortBox = gtk_entry_new();
    if (port)
        gtk_entry_set_text(GTK_ENTRY(proxyPortBox), port);
    gtk_entry_set_max_length(GTK_ENTRY(proxyPortBox), 5);
    gtk_widget_set_usize(proxyPortBox, 60, 0);
    gtk_signal_connect(GTK_OBJECT(proxyPortBox), "changed",
                       GTK_SIGNAL_FUNC(proxy_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), proxyPortBox, FALSE, FALSE, 0);
    gtk_widget_show(proxyPortBox);

    if (!originalValues.useProxyServer) {
        gtk_widget_set_sensitive(proxyAddyBox, FALSE);
        gtk_widget_set_sensitive(proxyPortBox, FALSE);
        gtk_widget_set_sensitive(proxyColon, FALSE);
        gtk_widget_set_sensitive(proxyAddyLabel, FALSE);
        gtk_widget_set_sensitive(proxyPortLabel, FALSE);
    }

    check = gtk_check_button_new_with_label("Use Alternate IP");
    gtk_container_add(GTK_CONTAINER(pane), check);
    gtk_signal_connect(GTK_OBJECT(check), "toggled",
                       GTK_SIGNAL_FUNC(alt_ip_toggle), this);
    if (originalValues.useAlternateIP)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
    gtk_widget_show(check);

    frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);
    gtk_container_add(GTK_CONTAINER(frame), hbox);
    gtk_widget_show(hbox);

    ipLabel = gtk_label_new("IP Address: ");
    gtk_box_pack_start(GTK_BOX(hbox), ipLabel, FALSE, FALSE, 0);
    gtk_widget_show(ipLabel);

    char *dot = NULL;
    char *ip[4];
    int32 i = 1;

    if (originalValues.alternateIP.length() == 0) {
        ip[0] = ip[1] = ip[2] = ip[3] = "0";
    }
    else {
        strncpy(tempstr, originalValues.alternateIP.c_str(), 256);
        ip[0] = tempstr;
        dot = tempstr;

        while ((dot = strchr(dot, '.'))) {
            *dot = 0x00;
            ip[i++] = ++dot;
            if (i == 4)
                break;
        }
    }

    ipOneBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(ipOneBox), ip[0]);
    gtk_entry_set_max_length(GTK_ENTRY(ipOneBox), 3);
    gtk_widget_set_usize(ipOneBox, 32, 0);
    gtk_signal_connect(GTK_OBJECT(ipOneBox), "changed",
                       GTK_SIGNAL_FUNC(ip_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), ipOneBox, TRUE, FALSE, 0);
    gtk_widget_show(ipOneBox);

    ipPeriod1 = gtk_label_new(".");
    gtk_container_add(GTK_CONTAINER(hbox), ipPeriod1);
    gtk_widget_show(ipPeriod1);

    ipTwoBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(ipTwoBox), ip[1]);
    gtk_entry_set_max_length(GTK_ENTRY(ipTwoBox), 3);
    gtk_widget_set_usize(ipTwoBox, 32, 0);
    gtk_signal_connect(GTK_OBJECT(ipOneBox), "changed",
                       GTK_SIGNAL_FUNC(ip_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), ipTwoBox, TRUE, FALSE, 0);
    gtk_widget_show(ipTwoBox);

    ipPeriod2 = gtk_label_new(".");
    gtk_container_add(GTK_CONTAINER(hbox), ipPeriod2);
    gtk_widget_show(ipPeriod2);

    ipThreeBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(ipThreeBox), ip[2]);
    gtk_entry_set_max_length(GTK_ENTRY(ipThreeBox), 3);
    gtk_widget_set_usize(ipThreeBox, 32, 0);
    gtk_signal_connect(GTK_OBJECT(ipThreeBox), "changed",
                       GTK_SIGNAL_FUNC(ip_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), ipThreeBox, TRUE, FALSE, 0);
    gtk_widget_show(ipThreeBox);

    ipPeriod3 = gtk_label_new(".");
    gtk_container_add(GTK_CONTAINER(hbox), ipPeriod3);
    gtk_widget_show(ipPeriod3);

    ipFourBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(ipFourBox), ip[3]);
    gtk_entry_set_max_length(GTK_ENTRY(ipFourBox), 3);
    gtk_widget_set_usize(ipFourBox, 32, 0);
    gtk_signal_connect(GTK_OBJECT(ipFourBox), "changed",
                       GTK_SIGNAL_FUNC(ip_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), ipFourBox, TRUE, FALSE, 0);
    gtk_widget_show(ipFourBox);

    if (!originalValues.useAlternateIP) {
        gtk_widget_set_sensitive(ipLabel, FALSE);
        gtk_widget_set_sensitive(ipOneBox, FALSE);
        gtk_widget_set_sensitive(ipPeriod1, FALSE);
        gtk_widget_set_sensitive(ipTwoBox, FALSE);
        gtk_widget_set_sensitive(ipPeriod2, FALSE);
        gtk_widget_set_sensitive(ipThreeBox, FALSE);
        gtk_widget_set_sensitive(ipPeriod3, FALSE);
        gtk_widget_set_sensitive(ipFourBox, FALSE);
    }

    return pane;
}

void GTKPreferenceWindow::SetPMO(int newsel)
{
    proposedValues.outputIndex = newsel;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void pmo_select(GtkWidget *item, GTKPreferenceWindow *p)
{
    int i = 0;
    if (!GTK_WIDGET_MAPPED(item))
        return;

    GSList *glist = gtk_radio_menu_item_group((GtkRadioMenuItem *)
                        (((GtkOptionMenu *)p->pmoOptionMenu)->menu_item));
    while (glist && !((GtkCheckMenuItem *)(glist->data))->active) {
        glist = glist->next;
        i++;
    }
 
    p->SetPMO(p->numPMOs - i);
}

GtkWidget *GTKPreferenceWindow::CreatePage3(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *frame = gtk_frame_new("Audio");
    gtk_box_pack_start(GTK_BOX(pane), frame, TRUE, FALSE, 0);
    gtk_widget_show(frame);

    GtkWidget *table = gtk_table_new(2, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    Registry *pmo = m_pContext->player->GetPMORegistry();
    int32 i = 0;
    RegistryItem *item;

    GtkWidget *label = gtk_label_new("Audio Output");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                     5, 0);
    gtk_widget_show(label);

    pmoOptionMenu = gtk_option_menu_new();
    pmoMenu = gtk_menu_new();
    GSList *group = NULL;
    GtkWidget *menuitem;

    while (pmo && (item = pmo->GetItem(i++))) {
        menuitem = gtk_radio_menu_item_new_with_label(group, item->Name());
        gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                           GTK_SIGNAL_FUNC(pmo_select), this);
        group = gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(menuitem));
        gtk_menu_append(GTK_MENU(pmoMenu), menuitem);
        if (originalValues.defaultPMO == item->Name()) {
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);

            proposedValues.outputIndex = i - 1;
        }
        gtk_widget_show(menuitem);
    }

    numPMOs = i - 2;
    gtk_option_menu_set_menu(GTK_OPTION_MENU(pmoOptionMenu), pmoMenu);
    gtk_table_attach(GTK_TABLE(table), pmoOptionMenu, 1, 2, 1, 2, GTK_FILL,
                     GTK_FILL, 5, 5);
    gtk_option_menu_set_history(GTK_OPTION_MENU(pmoOptionMenu),
                                proposedValues.outputIndex);
    gtk_widget_show(pmoOptionMenu);

    return pane;
}

void GTKPreferenceWindow::SetInputBufferSize(int newvalue)
{
    proposedValues.inputBufferSize = newvalue;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void input_buffer_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    int newdata = atoi(text);
    p->SetInputBufferSize(newdata);
}

void GTKPreferenceWindow::SetOutputBufferSize(int newvalue)
{
    proposedValues.outputBufferSize = newvalue;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void output_buffer_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    int newdata = atoi(text);
    p->SetOutputBufferSize(newdata);
}

void GTKPreferenceWindow::SetPreBufferLength(int newvalue)
{
    proposedValues.preBufferLength = newvalue;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void prestream_buffer_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    int newdata = atoi(text);
    p->SetPreBufferLength(newdata);
}

GtkWidget *GTKPreferenceWindow::CreatePage6(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *frame = gtk_frame_new("Buffer Sizes");
    gtk_box_pack_start(GTK_BOX(pane), frame, TRUE, FALSE, 0);
    gtk_widget_show(frame);

    GtkWidget *table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    int32 value;
    char tempstr[256];

    GtkWidget *label = gtk_label_new("Input Buffer Size (Kilobytes)");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    GtkWidget *entry = gtk_entry_new();
    value = originalValues.inputBufferSize;
    sprintf(tempstr, "%d", value);
    gtk_entry_set_text(GTK_ENTRY(entry), tempstr);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 4);
    gtk_signal_connect(GTK_OBJECT(entry), "changed",
                       GTK_SIGNAL_FUNC(input_buffer_change), this);
    gtk_table_attach(GTK_TABLE(table), entry, 1, 2, 0, 1, GTK_FILL, GTK_FILL,
                     10, 3);
    gtk_widget_show(entry);

    label = gtk_label_new("Output Buffer Size (Kilobytes)");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    entry = gtk_entry_new();
    value = originalValues.outputBufferSize;
    sprintf(tempstr, "%d", value);
    gtk_entry_set_text(GTK_ENTRY(entry), tempstr);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 4);
    gtk_signal_connect(GTK_OBJECT(entry), "changed",
                       GTK_SIGNAL_FUNC(output_buffer_change), this);
    gtk_table_attach(GTK_TABLE(table), entry, 1, 2, 1, 2, GTK_FILL, GTK_FILL,
                     10, 3);
    gtk_widget_show(entry);

    label = gtk_label_new("Prebuffer Streams (Seconds)");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    entry = gtk_entry_new();
    value = originalValues.preBufferLength;
    sprintf(tempstr, "%d", value);
    gtk_entry_set_text(GTK_ENTRY(entry), tempstr);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
    gtk_signal_connect(GTK_OBJECT(entry), "changed",
                       GTK_SIGNAL_FUNC(prestream_buffer_change), this);
    gtk_table_attach(GTK_TABLE(table), entry, 1, 2, 2, 3, GTK_FILL, GTK_FILL,
                     10, 3);
    gtk_widget_show(entry);

    return pane;
}

void freeamp_press(GtkWidget *w, GTKPreferenceWindow *p)
{
    LaunchBrowser("http://www.freeamp.org/");    
}

void emusic_press(GtkWidget *w, GTKPreferenceWindow *p)
{
    LaunchBrowser("http://www.emusic.com/");
}

GtkWidget *GTKPreferenceWindow::CreateAbout(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 0);
    gtk_widget_show(pane);

    GtkWidget *textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), BRANDING);
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "version "FREEAMP_VERSION);
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "FreeAmp is an Open Source effort to build the best digital audio player available.  In the interest of supporting the free software community, while at the same time fostering the growth of the online delivery of music, EMusic.com, THE Source for Downloadable Music, is funding both the FreeAmp.org domain and the efforts of the FreeAmp team.  The FreeAmp team consists of: Mark B. Elrod, Robert Kaye, Isaac Richards, Brett Thomas, and Jason Woodward.");
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "Other people have also contributed to FreeAmp:");
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "William Bull, Alan Cutter, Gabor Fleischer, Jean-Michel HERVE, Hiromasa Kato, Michael Bruun Petersen, Sylvain Rebaud, The Snowblind Alliance, Tom Spindler, Valters Vingolds, Bill Yuan.");
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "FreeAmp is being released under the terms of the GPL.  As is provided by the GPL, all of EMusic.com's and your efforts toward FreeAmp will be released back to the community at large.");
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(pane), textlabel, FALSE, FALSE, 0);
    gtk_widget_show(textlabel);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_container_add(GTK_CONTAINER(pane), hbox);
    gtk_widget_show(hbox);

    GtkWidget *button;

    button = gtk_button_new_with_label(" Visit FreeAmp.org ");
    gtk_container_add(GTK_CONTAINER(hbox), button);
    gtk_signal_connect(GTK_OBJECT(button), "clicked", 
                       GTK_SIGNAL_FUNC(freeamp_press), this);
    gtk_widget_show(button);

    button = gtk_button_new_with_label(" Visit EMusic.com ");
    gtk_container_add(GTK_CONTAINER(hbox), button);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(emusic_press), this);
    gtk_widget_show(button);
   
    return pane;
}

void GTKPreferenceWindow::SelectTheme(int row)
{
    proposedValues.listboxIndex = row;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void theme_click(GtkWidget *w, int row, int column, GdkEventButton *button,
                 GTKPreferenceWindow *p)
{
    p->SelectTheme(row);
}

GtkWidget *GTKPreferenceWindow::CreatePage5(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *frame = gtk_frame_new("Theme Selection");
    gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    GtkWidget *listwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(listwindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(frame), listwindow);
    gtk_widget_set_usize(listwindow, 200, 200);
    gtk_widget_show(listwindow);

    GtkWidget *list = gtk_clist_new(1);
    gtk_signal_connect(GTK_OBJECT(list), "select_row",
                       GTK_SIGNAL_FUNC(theme_click), this);
    gtk_container_add(GTK_CONTAINER(listwindow), list);
    gtk_widget_show(list);

    int iLoop = 0;
    map<string, string>::iterator i;
    proposedValues.listboxIndex = 0;

    m_pThemeMan->GetCurrentTheme(originalValues.currentTheme);
    m_oThemeList.clear();

    m_pThemeMan->GetThemeList(m_oThemeList);
    for (i = m_oThemeList.begin(); i != m_oThemeList.end(); i++, iLoop++) {
         char *Text[1];
         Text[0] = (char *)((*i).first.c_str());
         gtk_clist_append(GTK_CLIST(list), Text);
         if ((*i).first == originalValues.currentTheme)
             proposedValues.listboxIndex = currentValues.listboxIndex = iLoop;
    }

    gtk_clist_select_row(GTK_CLIST(list), proposedValues.listboxIndex, 1);

    return pane;
}

