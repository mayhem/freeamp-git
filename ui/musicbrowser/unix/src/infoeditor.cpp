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

#include "utility.h"
#include "infoeditor.h"
#include "metadata.h"
#include "musiccatalog.h"

void infoeditorUI::DoApplyInfoEdit(void)
{
    if (!changed)
        return;

    gtk_widget_set_sensitive(m_okButton, FALSE);
    gtk_widget_set_sensitive(m_applyButton, FALSE);

    vector<PlaylistItem *>::iterator i = m_itemlist->begin();
    for (; i != m_itemlist->end(); i++) {
        MetaData oldmeta, newmeta;
        gchar *text;

        oldmeta = newmeta = (*i)->GetMetaData();

        if (title_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_titleEntry));
            newmeta.SetTitle(text);
        }
        if (artist_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_artistEntry));
            newmeta.SetArtist(text);
        }
        if (album_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_albumEntry));
            newmeta.SetAlbum(text);
        }
        if (year_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_yearEntry));
            newmeta.SetYear(atoi(text));
        }
        if (comment_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_commentEntry));
            newmeta.SetComment(text);
        }
        if (genre_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_genreEntry));
            newmeta.SetGenre(text);
        }
        if (track_change) {
            text = gtk_entry_get_text(GTK_ENTRY(m_trackEntry));
            newmeta.SetTrack(atoi(text));
        }

        newmeta.SetTime(oldmeta.Time());
        newmeta.SetSize(oldmeta.Size());

        if (newmeta != oldmeta) {
            (*i)->SetMetaData(&newmeta);
            if (m_plm)
                m_plm->UpdateTrackMetaData(*i, true);
            m_context->catalog->UpdateSong(*i);
        }
    }
}

gint info_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    return FALSE;
}

void info_ok_button_event(GtkWidget *widget, infoeditorUI *p)
{
    p->DoApplyInfoEdit();
    delete p;
}

void info_close_button_event(GtkWidget *widget, infoeditorUI *p)
{
    delete p;
}

void info_apply_button_event(GtkWidget * widget, infoeditorUI *p)
{
    p->DoApplyInfoEdit();
}

void text_changed_event(GtkWidget *widget, infoeditorUI *p)
{
   gtk_widget_set_sensitive(p->m_okButton, TRUE);
   gtk_widget_set_sensitive(p->m_applyButton, TRUE);
   p->changed = true;
   p->CheckWidget(widget);
}

void infoeditorUI::CheckWidget(GtkWidget *widget)
{
   if (widget == m_titleEntry)
       title_change = true;
   else if (widget == m_artistEntry)
       artist_change = true;
   else if (widget == m_albumEntry)
       album_change = true;
   else if (widget == m_yearEntry)
       year_change = true;
   else if (widget == m_genreEntry)
       genre_change = true;
   else if (widget == m_trackEntry)
       track_change = true;
   else if (widget == m_commentEntry)
       comment_change = true;
}

infoeditorUI::infoeditorUI(FAContext *context, PlaylistManager *plm,
                           vector<PlaylistItem *> *itemlist)
{
    m_context = context;
    m_itemlist = itemlist;
    m_plm = plm;
    title_change = false;
    artist_change = false;
    album_change = false;
    genre_change = false;
    year_change = false;
    comment_change = false;
    track_change = false;
}

void infoeditorUI::DisplayInfo(void)
{
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *table;
   GtkWidget *label;
   GtkWidget *separator;
   GtkWidget *close_button;

   if (!m_itemlist || m_itemlist->size() == 0) 
       return;

   m_listsize = m_itemlist->size();
   m_artists = true;
   m_albums = true;
   m_years = true;
   m_genres = true;

   MetaData firstdata = (*(m_itemlist->begin()))->GetMetaData();
   vector<PlaylistItem *>::iterator i = m_itemlist->begin();
   i++;
   for (; i != m_itemlist->end(); i++) {
       MetaData compare = (*i)->GetMetaData();
       if (firstdata.Artist() != compare.Artist())
           m_artists = false;
       if (firstdata.Album() != compare.Album())
           m_albums = false;
       if (firstdata.Year() != compare.Year())
           m_years = false;
       if (firstdata.Genre() != compare.Genre())
           m_genres = false;
   }

   m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(m_window), BRANDING" - Information Editor");
   gtk_signal_connect(GTK_OBJECT(m_window), "delete_event",
                      GTK_SIGNAL_FUNC(info_delete_event), this);
   gtk_container_set_border_width(GTK_CONTAINER(m_window), 5);

   vbox = gtk_vbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(m_window), vbox);
   gtk_widget_show(vbox);

   table = gtk_table_new(10, 2, FALSE);
   gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);
   gtk_widget_show(table);

   /* Song title entry */
   label = gtk_label_new("Song Title:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_titleEntry = gtk_entry_new();
   if (m_listsize > 1) 
      gtk_entry_set_text(GTK_ENTRY(m_titleEntry), "<Multiple Tracks Selected>");
   else if (firstdata.Title().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_titleEntry), firstdata.Title().c_str());
   gtk_signal_connect(GTK_OBJECT(m_titleEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_titleEntry, 1, 2, 0, 1);
   gtk_widget_show(m_titleEntry);

   if (m_listsize > 1) {
       gtk_widget_set_sensitive(label, FALSE);
       gtk_widget_set_sensitive(m_titleEntry, FALSE);
   }

   /* Artist entry */
   label = gtk_label_new("Artist:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_artistEntry = gtk_entry_new();
   if (!m_artists) 
      gtk_entry_set_text(GTK_ENTRY(m_artistEntry), "<Multiple Artists Selected>");
   else if (firstdata.Artist().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_artistEntry), firstdata.Artist().c_str());
   gtk_signal_connect(GTK_OBJECT(m_artistEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_artistEntry, 1, 2, 1, 2);
   gtk_widget_show(m_artistEntry);

   /* Album entry */
   label = gtk_label_new("Album:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_albumEntry = gtk_entry_new();
   if (!m_albums)
      gtk_entry_set_text(GTK_ENTRY(m_albumEntry), "<Multiple Albums Selected>");
   else if (firstdata.Album().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_albumEntry), firstdata.Album().c_str());
   gtk_signal_connect(GTK_OBJECT(m_albumEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_albumEntry, 1, 2, 2, 3);
   gtk_widget_show(m_albumEntry);

   /* Year entry */
   label = gtk_label_new("Year:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 3, 4, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_yearEntry = gtk_entry_new();
   if (!m_years) 
       gtk_entry_set_text(GTK_ENTRY(m_yearEntry), "Unknown");
   else if (firstdata.Year() != 0) {
       char tempstr[10];
       sprintf(tempstr, "%d", firstdata.Year());
       gtk_entry_set_text(GTK_ENTRY(m_yearEntry), tempstr);
   }
   gtk_signal_connect(GTK_OBJECT(m_yearEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_yearEntry, 1, 2, 3, 4);
   gtk_widget_show(m_yearEntry);

   /* Genre entry */
   label = gtk_label_new("Genre:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 4, 5, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_genreEntry = gtk_entry_new();
   
   if (!m_genres) 
      gtk_entry_set_text(GTK_ENTRY(m_genreEntry), "<Multiple Genres Selected>");
   else if (firstdata.Genre().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_genreEntry), firstdata.Genre().c_str());
   gtk_signal_connect(GTK_OBJECT(m_genreEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_genreEntry, 1, 2, 4, 5);
   gtk_widget_show(m_genreEntry);

   /* Track Entry */
   label = gtk_label_new("Track:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 5, 6, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_trackEntry = gtk_entry_new();
   if (m_listsize > 1) 
      gtk_entry_set_text(GTK_ENTRY(m_trackEntry), "<Multiple>");
   else if (firstdata.Track() != 0) {
      char strtemp[5];
      sprintf(strtemp, "%d", firstdata.Track());
      gtk_entry_set_text(GTK_ENTRY(m_trackEntry), strtemp);
   }
   gtk_signal_connect(GTK_OBJECT(m_trackEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_trackEntry, 1, 2, 5, 6);
   gtk_widget_show(m_trackEntry);

   if (m_listsize > 1) {
       gtk_widget_set_sensitive(m_trackEntry, FALSE);
       gtk_widget_set_sensitive(label, FALSE);
   }

   /* Comment entry */
   label = gtk_label_new("Comments:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 6, 7, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_commentEntry = gtk_entry_new();
   if (m_listsize > 1)
      gtk_entry_set_text(GTK_ENTRY(m_commentEntry), "<Multiple Tracks Selected>");
   if (firstdata.Comment().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_commentEntry), firstdata.Comment().c_str());
   gtk_signal_connect(GTK_OBJECT(m_commentEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_commentEntry, 1, 2, 6, 7);
   gtk_widget_show(m_commentEntry);

   if (m_listsize > 1) {
       gtk_widget_set_sensitive(m_commentEntry, FALSE);
       gtk_widget_set_sensitive(label, FALSE);
   }

   /* Length display */
   label = gtk_label_new("Length:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 7, 8, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   GtkWidget *time_label;
   if (m_listsize > 1) {
       time_label = gtk_label_new("<Multiple Tracks Selected>");
   }
   else {
       gchar *length;
       char minutes[5];
       char seconds[5];
       int min, sec;

       min = firstdata.Time() / 60;
       sec = firstdata.Time() % 60;
       sprintf(minutes, "%02d", min);
       sprintf(seconds, "%02d", sec);
       length = g_strconcat((gchar *) minutes, ":", (gchar *) seconds, NULL);
       time_label = gtk_label_new(length);
   }
   gtk_table_attach_defaults(GTK_TABLE(table), time_label, 1, 2, 7, 8);
   gtk_widget_show(time_label);
   
   /* location */
   label = gtk_label_new("Location:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 8, 9, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   GtkWidget *location_label;
   if (m_listsize > 1)
       location_label = gtk_label_new("<Multiple Tracks Selected>");
   else
       location_label = gtk_label_new((*(m_itemlist->begin()))->URL().c_str());
   gtk_table_attach_defaults(GTK_TABLE(table), location_label, 1, 2, 8, 9);
   gtk_widget_show(location_label);

   /* Control buttons at the bottom */
   separator = gtk_hseparator_new();
   gtk_container_add(GTK_CONTAINER(vbox), separator);
   gtk_widget_show(separator);

   hbox = gtk_hbox_new(FALSE, 10);
   gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_widget_show(hbox);

   m_okButton = gtk_button_new_with_label("OK");
   gtk_signal_connect(GTK_OBJECT(m_okButton), "clicked",
                      GTK_SIGNAL_FUNC(info_ok_button_event), this);
   gtk_container_add(GTK_CONTAINER(hbox), m_okButton);
   gtk_widget_show(m_okButton);

   m_applyButton = gtk_button_new_with_label("Apply");
   gtk_signal_connect(GTK_OBJECT(m_applyButton), "clicked",
                      GTK_SIGNAL_FUNC(info_apply_button_event), this);
   gtk_container_add(GTK_CONTAINER(hbox), m_applyButton);
   gtk_widget_show(m_applyButton);

   close_button = gtk_button_new_with_label("Close");
   gtk_signal_connect(GTK_OBJECT(close_button), "clicked",
                      GTK_SIGNAL_FUNC(info_close_button_event), this);
   gtk_container_add(GTK_CONTAINER(hbox), close_button);
   gtk_widget_show(close_button);

   gtk_widget_show(m_window);

   gtk_widget_set_sensitive(m_okButton, FALSE);
   gtk_widget_set_sensitive(m_applyButton, FALSE);

   changed = FALSE;
}

infoeditorUI::~infoeditorUI()
{
   gtk_widget_destroy(m_window);
   delete m_itemlist;
}
