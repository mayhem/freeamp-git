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
#include "musicbrowser.h"

void infoeditorUI::DoApplyInfoEdit(void)
{
    gchar *text;

    MetaData newmeta;

    if (!changed)
        return;
    gtk_widget_set_sensitive(m_okButton, FALSE);
    gtk_widget_set_sensitive(m_applyButton, FALSE);
    text = gtk_entry_get_text(GTK_ENTRY(m_titleEntry));
    newmeta.SetTitle(strdup_new(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_artistEntry));
    newmeta.SetArtist(strdup_new(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_albumEntry));
    newmeta.SetAlbum(strdup_new(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_yearEntry));
    newmeta.SetYear(atoi(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_commentEntry));
    newmeta.SetComment(strdup_new(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_genreEntry));
    newmeta.SetGenre(strdup_new(text));
    text = gtk_entry_get_text(GTK_ENTRY(m_trackEntry));
    newmeta.SetTrack(atoi(text));

    MetaData oldmeta = m_playlistItem->GetMetaData();

    newmeta.SetTime(oldmeta.Time());
    newmeta.SetSize(oldmeta.Size());

    m_playlistItem->SetMetaData(&newmeta);

    m_context->browser->m_catalog->UpdateSong(m_playlistItem);
}

gint info_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    return FALSE;
}

void info_destroy(GtkWidget *widget, infoeditorUI *p)
{
    delete p;
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

void text_changed_event(GtkWidget * widget, infoeditorUI *p)
{
   gtk_widget_set_sensitive(p->m_okButton, TRUE);
   gtk_widget_set_sensitive(p->m_applyButton, TRUE);
   p->changed = true;
}

infoeditorUI::infoeditorUI(FAContext *context, PlaylistItem *editee)
{
    m_context = context;
    m_playlistItem = editee;
}

void infoeditorUI::DisplayInfo(void)
{
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *table;
   GtkWidget *label;
   GtkWidget *separator;
   GtkWidget *close_button;

   MetaData songmeta = m_playlistItem->GetMetaData();

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
   if (songmeta.Title().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_titleEntry), songmeta.Title().c_str());
   gtk_signal_connect(GTK_OBJECT(m_titleEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_titleEntry, 1, 2, 0, 1);
   gtk_widget_show(m_titleEntry);

   /* Artist entry */
   label = gtk_label_new("Artist:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_artistEntry = gtk_entry_new();
   if (songmeta.Artist().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_artistEntry), songmeta.Artist().c_str());
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
   if (songmeta.Album().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_albumEntry), songmeta.Album().c_str());
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
   if (songmeta.Year() != 0) {
       char tempstr[10];
       sprintf(tempstr, "%d", songmeta.Year());
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
   if (songmeta.Genre().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_genreEntry), songmeta.Genre().c_str());
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
   if (songmeta.Track() != 0) {
      char strtemp[5];
      sprintf(strtemp, "%d", songmeta.Track());
      gtk_entry_set_text(GTK_ENTRY(m_trackEntry), strtemp);
   }
   gtk_signal_connect(GTK_OBJECT(m_trackEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_trackEntry, 1, 2, 5, 6);
   gtk_widget_show(m_trackEntry);

   /* Comment entry */
   label = gtk_label_new("Comments:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 6, 7, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   m_commentEntry = gtk_entry_new();
   if (songmeta.Comment().c_str())
      gtk_entry_set_text(GTK_ENTRY(m_commentEntry), songmeta.Comment().c_str());
   gtk_signal_connect(GTK_OBJECT(m_commentEntry), "changed",
                      GTK_SIGNAL_FUNC(text_changed_event), this);
   gtk_table_attach_defaults(GTK_TABLE(table), m_commentEntry, 1, 2, 6, 7);
   gtk_widget_show(m_commentEntry);

   /* Length display */
   label = gtk_label_new("Length:");
   gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
   gtk_table_attach(GTK_TABLE(table), label, 0, 1, 7, 8, GTK_FILL, GTK_FILL,
                    10, 1);
   gtk_widget_show(label);

   {
      gchar *length;
      char minutes[5];
      char seconds[5];
      int min, sec;

      min = songmeta.Time() / 60;
      sec = songmeta.Time() % 60;
      sprintf(minutes, "%02d", min);
      sprintf(seconds, "%02d", sec);
      length = g_strconcat((gchar *) minutes, ":", (gchar *) seconds, NULL);
      GtkWidget *time_label;
      time_label = gtk_label_new(length);
      gtk_table_attach_defaults(GTK_TABLE(table), time_label, 1, 2, 7, 8);
      gtk_widget_show(time_label);
   }

   /* Control buttons at the bottom */
   separator = gtk_hseparator_new();
   gtk_container_add(GTK_CONTAINER(vbox), separator);
   gtk_widget_show(separator);

   hbox = gtk_hbox_new(FALSE, 10);
   gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_widget_show(hbox);

   m_okButton = gtk_button_new_with_label("Okay");
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
}
