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
    SavePrefsValues(m_pContext->prefs, &currentValues);
    m_pContext->target->AcceptEvent(new Event(INFO_PrefsChanged));
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

void pref_cancel_click(GtkWidget *w, GTKPreferenceWindow *p)
{
    gtk_widget_destroy(p->mainWindow);
}

bool GTKPreferenceWindow::Show(Window *pWindow)
{
    GetPrefsValues(m_pContext->prefs, &currentValues);

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

    label = gtk_label_new("General");
    gtk_widget_show(label);
    pane = CreatePage1();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Streaming");
    gtk_widget_show(label);
    pane = CreatePage2();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Debug");
    gtk_widget_show(label);
    pane = CreatePage3();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("About");
    gtk_widget_show(label);
    pane = CreateAbout();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    label = gtk_label_new("Themes");
    gtk_widget_show(label);
    pane = CreatePage5();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pane, label);

    gtk_widget_show(notebook);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    GtkWidget *button;

    button = gtk_button_new_with_label("  OK  ");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(pref_ok_click), this);
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

    gtk_widget_show(mainWindow);

    gdk_threads_leave();

    gtk_main();

    gdk_threads_leave();
}

void GTKPreferenceWindow::GetPrefsValues(Preferences* prefs, 
                                         PrefsStruct* values)
{
    uint32 size = 256;

    values->prefs = prefs;

    prefs->GetDefaultPMO(values->defaultPMO, &size);
    size = 256;
    prefs->GetDefaultUI(values->defaultUI, &size);
    prefs->GetInputBufferSize(&values->inputBufferSize);
    prefs->GetOutputBufferSize(&values->outputBufferSize);
    prefs->GetPrebufferLength(&values->preBufferLength);

    prefs->GetStreamBufferInterval(&values->streamInterval);
    prefs->GetSaveStreams(&values->saveStreams);
    size = 256;
    prefs->GetProxyServerAddress(values->proxyServer, &size);
    prefs->GetUseProxyServer(&values->useProxyServer);
    size = _MAX_PATH;
    prefs->GetSaveStreamsDirectory(values->saveStreamsDirectory, &size);
    size = 16;
    prefs->GetAlternateNICAddress(values->alternateIP, &size);
    prefs->GetUseAlternateNIC(&values->useAlternateIP);

    prefs->GetUseDebugLog(&values->enableLogging);
    prefs->GetLogMain(&values->logMain);
    prefs->GetLogDecode(&values->logDecoder);
    prefs->GetLogInput(&values->logInput);
    prefs->GetLogOutput(&values->logOutput);
    prefs->GetLogPerformance(&values->logPerformance);

    size = 256;
    prefs->GetThemeDefaultFont(values->defaultFont, &size);
    m_pThemeMan->GetCurrentTheme(values->currentTheme);
}

void GTKPreferenceWindow::SavePrefsValues(Preferences* prefs, 
                                          PrefsStruct* values)
{
    prefs = values->prefs;

    prefs->SetDefaultPMO(values->defaultPMO);
    prefs->SetDefaultUI(values->defaultUI);
    prefs->SetInputBufferSize(values->inputBufferSize);
    prefs->SetOutputBufferSize(values->outputBufferSize);
    prefs->SetPrebufferLength(values->preBufferLength);

    prefs->SetStreamBufferInterval(values->streamInterval);
    prefs->SetSaveStreams(values->saveStreams);
    prefs->SetSaveStreamsDirectory(values->saveStreamsDirectory);
    prefs->SetProxyServerAddress(values->proxyServer);
    prefs->SetUseProxyServer(values->useProxyServer);
    prefs->SetAlternateNICAddress(values->alternateIP);
    prefs->SetUseAlternateNIC(values->useAlternateIP);

    prefs->SetUseDebugLog(values->enableLogging);
    prefs->SetLogMain(values->logMain);
    prefs->SetLogDecode(values->logDecoder);
    prefs->SetLogInput(values->logInput);
    prefs->SetLogOutput(values->logOutput);
    prefs->SetLogPerformance(values->logPerformance);

    prefs->SetThemeDefaultFont(values->defaultFont);

    map<string, string>::iterator i;
    int32 iLoop = 0;

    for (i = m_oThemeList.begin(); i != m_oThemeList.end(); i++, iLoop++) {
         if (iLoop == currentValues.listboxIndex)
             currentValues.currentTheme = (*i).first;
    }
    m_pThemeMan->UseTheme(m_oThemeList[values->currentTheme]);
}

void pmo_select(GtkWidget *item, GTKPreferenceWindow *p)
{
}

void ui_select(GtkWidget *item, GTKPreferenceWindow *p)
{
}

void GTKPreferenceWindow::SetInputBufferSize(int newvalue)
{
    currentValues.inputBufferSize = newvalue;
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
    currentValues.outputBufferSize = newvalue;
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
    currentValues.preBufferLength = newvalue;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void prestream_buffer_change(GtkWidget *w, GTKPreferenceWindow *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    int newdata = atoi(text);

    p->SetPreBufferLength(newdata);
}

GtkWidget *GTKPreferenceWindow::CreatePage1(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *frame = gtk_frame_new("Plug-In");
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    GtkWidget *table = gtk_table_new(2, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table); 
    gtk_widget_show(table);

    Registry *pmo = m_pContext->player->GetPMORegistry();

    int32 i = 0;
    int32 pos = 0;
    RegistryItem *item;

    GtkWidget *label = gtk_label_new("Output");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                     5, 0);
    gtk_widget_show(label);

    GtkWidget *optionmenu = gtk_option_menu_new();
    GtkWidget *menu = gtk_menu_new();

    while (pmo && (item = pmo->GetItem(i++))) {
        GtkWidget *menuitem = gtk_menu_item_new_with_label(item->Name());
        gtk_signal_connect(GTK_OBJECT(menuitem), "activate", 
                           GTK_SIGNAL_FUNC(pmo_select), this);
        gtk_widget_show(menuitem);

        gtk_menu_append(GTK_MENU(menu), menuitem);
        if (!strcmp(currentValues.defaultPMO, item->Name()))
            pos = i;
    }
    gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);
    gtk_table_attach(GTK_TABLE(table), optionmenu, 1, 2, 1, 2, GTK_FILL, 
                     GTK_FILL, 5, 5);
    gtk_widget_show(optionmenu);
    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), pos - 1);

    frame = gtk_frame_new("Buffer Sizes");
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    int32 value;
    char tempstr[256];

    label = gtk_label_new("Input Buffer Size (Kilobytes)");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    GtkWidget *entry = gtk_entry_new();
    value = currentValues.inputBufferSize;
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
    value = currentValues.outputBufferSize;
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
    value = currentValues.preBufferLength;
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

void GTKPreferenceWindow::SetStreamInterval(int newvalue)
{
    currentValues.streamInterval = newvalue;
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
    currentValues.saveStreams = active;
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
    currentValues.useProxyServer = active;
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
    currentValues.useAlternateIP = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void alt_ip_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->AltIPToggle(i);
}

void GTKPreferenceWindow::SaveLocalSet(char *newpath)
{
    strcpy(currentValues.saveStreamsDirectory, newpath);
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

    strcpy(currentValues.proxyServer, name);
    if (*port) {
        strcat(currentValues.proxyServer, ":");
        strcat(currentValues.proxyServer, port);
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
        strcpy(currentValues.alternateIP, one);
    else
        strcpy(currentValues.alternateIP, "0");
    strcat(currentValues.alternateIP, ".");
    if (*two)
        strcpy(currentValues.alternateIP, two);
    else
        strcpy(currentValues.alternateIP, "0");
    strcat(currentValues.alternateIP, ".");
    if (*three)
        strcpy(currentValues.alternateIP, three);
    else
        strcpy(currentValues.alternateIP, "0");
    strcat(currentValues.alternateIP, ".");
    if (*four)
        strcpy(currentValues.alternateIP, four);
    else
        strcpy(currentValues.alternateIP, "0");
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
    value = currentValues.streamInterval;
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
    if (currentValues.saveStreams)
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

    strncpy(copys, currentValues.saveStreamsDirectory, 256);
    saveStreamBox = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(saveStreamBox), copys); 
    gtk_entry_set_max_length(GTK_ENTRY(saveStreamBox), 64);
    gtk_signal_connect(GTK_OBJECT(saveStreamBox), "changed",
                       GTK_SIGNAL_FUNC(save_stream_change), this);
    gtk_box_pack_start(GTK_BOX(vbox), saveStreamBox, FALSE, FALSE, 0);
    gtk_widget_show(saveStreamBox);

    if (!currentValues.saveStreams) {
        gtk_widget_set_sensitive(saveStreamLabel, FALSE);
        gtk_widget_set_sensitive(saveStreamBox, FALSE);
//        gtk_widget_set_sensitive(saveBrowseBox, FALSE);
    }

    check = gtk_check_button_new_with_label("Use Proxy Server");
    gtk_container_add(GTK_CONTAINER(pane), check);
    gtk_signal_connect(GTK_OBJECT(check), "toggled",
                       GTK_SIGNAL_FUNC(proxy_toggle), this);
    if (currentValues.useProxyServer)
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
    strncpy(tempstr, currentValues.proxyServer, 256);
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
    gtk_entry_set_text(GTK_ENTRY(proxyPortBox), port);
    gtk_entry_set_max_length(GTK_ENTRY(proxyPortBox), 5);
    gtk_widget_set_usize(proxyPortBox, 60, 0);
    gtk_signal_connect(GTK_OBJECT(proxyPortBox), "changed",
                       GTK_SIGNAL_FUNC(proxy_change), this);
    gtk_box_pack_start(GTK_BOX(hbox), proxyPortBox, FALSE, FALSE, 0);
    gtk_widget_show(proxyPortBox);

    if (!currentValues.useProxyServer) {
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
    if (currentValues.useAlternateIP)
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

    strncpy(tempstr, currentValues.alternateIP, 256);
    ip[0] = tempstr;
    dot = tempstr;

    while ((dot = strchr(dot, '.'))) {
        *dot = 0x00;
        ip[i++] = ++dot;
        if (i == 4)
            break;
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

    if (!currentValues.useAlternateIP) {
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

void GTKPreferenceWindow::LogToggle(int active)
{
    gtk_widget_set_sensitive(logGeneral, active);
    gtk_widget_set_sensitive(logPMI, active);
    gtk_widget_set_sensitive(logPMO, active);
    gtk_widget_set_sensitive(logLMC, active);
    gtk_widget_set_sensitive(logPerf, active);
    currentValues.enableLogging = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void log_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->LogToggle(i);
}

void GTKPreferenceWindow::GeneralToggle(int active)
{
    currentValues.logMain = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void general_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->GeneralToggle(i);
}
void GTKPreferenceWindow::PMIToggle(int active)
{
    currentValues.logInput = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void pmi_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->PMIToggle(i);
}

void GTKPreferenceWindow::PMOToggle(int active)
{
    currentValues.logOutput = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void pmo_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->PMOToggle(i);
}

void GTKPreferenceWindow::LMCToggle(int active)
{
    currentValues.logDecoder = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void lmc_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->LMCToggle(i);
}

void GTKPreferenceWindow::PerfToggle(int active)
{
    currentValues.logPerformance = active;
    gtk_widget_set_sensitive(applyButton, TRUE);
}

void perf_toggle(GtkWidget *w, GTKPreferenceWindow *p)
{
    int i = GTK_TOGGLE_BUTTON(w)->active;
    p->PerfToggle(i);
}

GtkWidget *GTKPreferenceWindow::CreatePage3(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_misc_set_alignment(GTK_MISC(textlabel), (gfloat)0.0, (gfloat)0.5);
    gtk_label_set_text(GTK_LABEL(textlabel), "If you are experiencing a problem with "BRANDING" it is possible to log what the program is doing in order to help us track down the cause.  When you enable logging a file is created in your home directory called freeamp.log.  You should send us the log file when you report a bug.");
    gtk_container_add(GTK_CONTAINER(pane), textlabel);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_misc_set_alignment(GTK_MISC(textlabel), (gfloat)0.0, (gfloat)0.5);
    gtk_label_set_text(GTK_LABEL(textlabel), "You have the ability to narrow the logging to a particular portion of "BRANDING" by selecting the various logging options.  If you are not sure in which portion of the program the problem is occuring you can simply enable all the logging options.");
    gtk_container_add(GTK_CONTAINER(pane), textlabel);
    gtk_widget_show(textlabel);

    GtkWidget *check = gtk_check_button_new_with_label("Enable Logging");
    gtk_container_add(GTK_CONTAINER(pane), check);
    gtk_signal_connect(GTK_OBJECT(check), "toggled",
                       GTK_SIGNAL_FUNC(log_toggle), this);
    if (currentValues.enableLogging)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
    gtk_widget_show(check);

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(pane), frame);
    gtk_widget_show(frame);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    gtk_widget_show(vbox);

    logGeneral = gtk_check_button_new_with_label(
                      "Log general player errors (e.g. Loading plugins)");
    gtk_container_add(GTK_CONTAINER(vbox), logGeneral);
    gtk_signal_connect(GTK_OBJECT(logGeneral), "toggled",
                       GTK_SIGNAL_FUNC(general_toggle), this);
    if (currentValues.logMain)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logGeneral), TRUE);
    gtk_widget_show(logGeneral);

    logPMI = gtk_check_button_new_with_label(
                   "Log input from PMIs (e.g. File, HTTP, ShoutCAST)");
    gtk_container_add(GTK_CONTAINER(vbox), logPMI);
    gtk_signal_connect(GTK_OBJECT(logPMI), "toggled",
                       GTK_SIGNAL_FUNC(pmi_toggle), this);
    if (currentValues.logInput)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logPMI), TRUE);
    gtk_widget_show(logPMI);

    logPMO = gtk_check_button_new_with_label(
                   "Log output from PMOs (e.g. Soundcard, EsounD, ALSA)");
    gtk_container_add(GTK_CONTAINER(vbox), logPMO);
    gtk_signal_connect(GTK_OBJECT(logPMO), "toggled",
                       GTK_SIGNAL_FUNC(pmo_toggle), this);
    if (currentValues.logOutput)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logPMO), TRUE);
    gtk_widget_show(logPMO);

    logLMC = gtk_check_button_new_with_label(
                   "Log decoding process (e.g. MP3, CDDA)");
    gtk_container_add(GTK_CONTAINER(vbox), logLMC);
    gtk_signal_connect(GTK_OBJECT(logLMC), "toggled",
                       GTK_SIGNAL_FUNC(lmc_toggle), this);
    if (currentValues.logDecoder)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logLMC), TRUE);
    gtk_widget_show(logLMC);

    logPerf = gtk_check_button_new_with_label(
               "Log performance issues (e.g. Timing, Buffer Under/Overflows)");
    gtk_container_add(GTK_CONTAINER(vbox), logPerf);
    gtk_signal_connect(GTK_OBJECT(logPerf), "toggled",
                       GTK_SIGNAL_FUNC(perf_toggle), this);
    if (currentValues.logPerformance)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logPerf), TRUE);
    gtk_widget_show(logPerf);

    return pane;
}

GtkWidget *GTKPreferenceWindow::CreateAbout(void)
{
    GtkWidget *pane = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane), 5);
    gtk_widget_show(pane);

    GtkWidget *textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), BRANDING);
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(pane), textlabel);
    gtk_widget_show(textlabel);

    textlabel = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(textlabel), TRUE);
    gtk_label_set_text(GTK_LABEL(textlabel), "version "FREEAMP_VERSION);
    gtk_label_set_justify(GTK_LABEL(textlabel), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(pane), textlabel);
    gtk_widget_show(textlabel);

    return pane;
}

void GTKPreferenceWindow::SelectTheme(int row)
{
    currentValues.listboxIndex = row;
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
    currentValues.listboxIndex = 0;

    m_pThemeMan->GetCurrentTheme(currentValues.currentTheme);
    m_oThemeList.clear();

    m_pThemeMan->GetThemeList(m_oThemeList);
    for (i = m_oThemeList.begin(); i != m_oThemeList.end(); i++, iLoop++) {
         char *Text[1];
         Text[0] = (char *)((*i).first.c_str());
         gtk_clist_append(GTK_CLIST(list), Text);
         if ((*i).first == currentValues.currentTheme)
             currentValues.listboxIndex = iLoop;
    }

    gtk_clist_select_row(GTK_CLIST(list), currentValues.listboxIndex, 1);

    return pane;
}

