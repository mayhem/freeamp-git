/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "config.h"
#include "freeamp-gtk.h"
#include "event.h"
#include "playlist.h"
#include "thread.h"
#include "eventdata.h"


#define stdinfd 0


GtkUI *g_pGtkUI;

extern "C" {

UserInterface *Initialize() {
    return new GtkUI();
}

	   }

GtkUI::GtkUI() {

    m_plm = NULL;
    m_playerEQ = NULL;
    
    g_pGtkUI = this;

    //cout << endl << "Command Line Interface" << endl << endl;
    //cout << " * q    Quit" << endl;
    //cout << " * +/=  Next Song" << endl;
    //cout << " * -    Prev Song" << endl;
    //cout << " * p    Pause / UnPause" << endl;
    //cout << " * s    Shuffle" << endl << endl;
}

void GtkUI::SetPlayListManager(PlayListManager *plm) {
    m_plm = plm;
}

GtkWidget *window;
GtkWidget *p_button_play;
GtkWidget *p_button_pause;
GtkWidget *p_button_quit;
GtkWidget *p_button_stop;
GtkWidget *p_button_next;
GtkWidget *p_button_prev;
GtkWidget *p_titleField;

bool playing = false;

void button_general (GtkWidget *widget, gpointer data) {
    switch ((int32)data) {
	case 1:
	    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_Play));
	    break;
	case 2:
	    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_TogglePause));
	    break;
	case 3:
	    gtk_main_quit(); // causes gtkServiceThread to exit, causing a quitplayer event to be sent to player
	    break;
	case 4:
	    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_Stop));
	    break;
	case 5:
	    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_NextMediaPiece));
	    break;
	case 6:
	    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_PrevMediaPiece));
	    break;
	default:
	    g_print("don't know what to do with %d\n",(int32) data);
    }
}

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return (TRUE);
}

/* another callback */
void destroy (GtkWidget *widget, gpointer data) {
    gtk_main_quit ();
}

void GtkUI::Init() {
    GtkWidget *theButtonBox;
    GtkWidget *outerBox;

    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);
    
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
			GTK_SIGNAL_FUNC (destroy), NULL);
    
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    p_button_play = gtk_button_new_with_label ("Play");
    p_button_pause = gtk_button_new_with_label ("Pause");
    p_button_quit = gtk_button_new_with_label ("Quit");
    p_button_stop = gtk_button_new_with_label ("Stop");
    p_button_next = gtk_button_new_with_label ("Next");
    p_button_prev = gtk_button_new_with_label ("Prev");

    gtk_signal_connect (GTK_OBJECT (p_button_play), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)1);

    gtk_signal_connect (GTK_OBJECT (p_button_pause), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)2);
    
    gtk_signal_connect (GTK_OBJECT (p_button_quit), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)3);

    gtk_signal_connect (GTK_OBJECT (p_button_stop), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)4);

    gtk_signal_connect (GTK_OBJECT (p_button_next), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)5);

    gtk_signal_connect (GTK_OBJECT (p_button_prev), "clicked",
			GTK_SIGNAL_FUNC (button_general), (gpointer)6);

    theButtonBox = gtk_hbox_new(FALSE,0);
    outerBox = gtk_vbox_new(FALSE,0);

    p_titleField = gtk_label_new("No Song Playing");

    gtk_box_pack_start(GTK_BOX(outerBox),p_titleField,TRUE,FALSE,2);
    gtk_box_pack_start(GTK_BOX(outerBox),theButtonBox,TRUE,FALSE,0);

    gtk_container_add(GTK_CONTAINER (window), outerBox);
    
    /* this packs the button into the window (a gtk container). */

    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_play, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_pause, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_stop, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_prev, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_next, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theButtonBox),p_button_quit, TRUE, FALSE, 0);

    /* and the window */
    gtk_widget_show (p_button_play);
    gtk_widget_show (p_button_pause);
    gtk_widget_show (p_button_stop);
    gtk_widget_show (p_button_prev);
    gtk_widget_show (p_button_next);
    gtk_widget_show (p_button_quit);
    
    gtk_widget_show (p_titleField);
    gtk_widget_show (theButtonBox);
    gtk_widget_show (outerBox);
    gtk_widget_show (window);
    
    //gtk_container_disable_resize(GTK_CONTAINER(window));
    
    gtkListenThread = Thread::CreateThread();
    gtkListenThread->Create(GtkUI::gtkServiceFunction,this);
    
}


GtkUI::~GtkUI() {
    //cout << "GtkUI: begin deleted..." << endl;
    if (gtkListenThread) {
	gtkListenThread->Destroy();
	delete gtkListenThread;
	gtkListenThread = NULL;
    }
}

void GtkUI::gtkServiceFunction(void *pclcio) {
    gtk_main();
    g_pGtkUI->m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
}

int32 GtkUI::AcceptEvent(Event *e) {
    if (e) {
	//cout << "GtkUI: processing event " << e->Type() << endl;
	switch (e->Type()) {
	    case INFO_PlayListDonePlay: {
		//Event *e = new Event(CMD_QuitPlayer);
		//m_playerEQ->AcceptEvent(e);
		break; }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieUI);
		m_playerEQ->AcceptEvent(e);
		break; }
	    case INFO_MediaInfo: {
		MediaInfoEvent *pmvi = (MediaInfoEvent *)e;
		if (pmvi) {
		    //cout << "Playing: " << pmvi->m_songTitle << endl;
		    //g_print("Playing: %s\n",pmvi->m_songTitle);
		    gtk_label_set(GTK_LABEL(p_titleField), pmvi->m_filename);
		    //cout << "set label..." << endl;
		    //g_print("set label...\n");
		    while (gtk_events_pending())
			gtk_main_iteration(); // redraw
		    //cout << "tried to redraw" << endl;
		    //g_print("tried to redraw\n");
		    //if (pmvi->m_tagInfo.m_containsInfo) {
		    //cout << "Title  : " << pmvi->m_tagInfo.m_songName << endl;
		    //cout << "Artist : " << pmvi->m_tagInfo.m_artist << endl;
		    //cout << "Album  : " << pmvi->m_tagInfo.m_album << endl;
		    //cout << "Year   : " << pmvi->m_tagInfo.m_year << endl;
		    //cout << "Comment: " << pmvi->m_tagInfo.m_comment << endl;
		    //}
		}
		break; }
	    default:
		break;
	}
    }
    return 0;
}

void GtkUI::SetArgs(int argc, char **argv) {
    
    gtk_init(&argc, &argv);

    char *pc = NULL;
    for(int i=1;i<argc;i++) {
	//cout << "Adding arg " << i << ": " << argv[i] << endl;
	pc = argv[i];
	if (pc[0] == '-') {
	    processSwitch(&(pc[0]));
	} else {
	    m_plm->Add(pc,0);
	}
    }
    m_plm->SetFirst();
}

void GtkUI::processSwitch(char *pc) {
    return;
}




