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

#ifndef _LIST_VIEW_H_
#define _LIST_VIEW_H_

#include "config.h"
#include "view.h"
#include "listitem.h"
#include "list.h"
#include "thread.h"


typedef enum {
	SINGLE_SELECTION_LIST,
	MULTIPLE_SELECTION_LIST
}list_view_type;

class ListView : public View {
 public:
    ListView(   HWND hwnd, 
                View* parent, 
                HRGN viewRegion,
                list_view_type type);

    ListView(   HWND hwnd, 
                View* parent, 
                RECT* viewRect,
                list_view_type type);

    virtual     ~ListView();

    

    // Deal with mouse stuff
    virtual void MouseMove(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDoubleClick(int32 x, int32 y, int32 modifiers);

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);

    // Allow the view to get timer messages
    virtual	void KeyPressed(int32 keyCode);

    virtual	void TargetedByScrollView(ScrollView *scroller);
	virtual	void ScrollTo(int32 index);
            void ScrollToSelection();


    virtual	bool AddItem(ListItem* item);
    virtual bool AddItem(ListItem* item, int32 index);
    virtual bool AddList(List<ListItem*>* items);
    virtual bool AddList(List<ListItem*>* items, int32 index);

    virtual bool        RemoveItem(ListItem* item);
    virtual ListItem*   RemoveItem(int32 index);
    virtual bool        RemoveItems(int32 index, int32 count);
    virtual bool        RemoveAll();

    virtual	void SetListType(list_view_type type);

            list_view_type	ListType() const;

            ListItem*   ItemAt(int32 index) const;
            int32	    IndexOf(int32 x, int32 y) const;
		    int32	    IndexOf(ListItem* item) const;

		    ListItem*   FirstItem() const;
		    ListItem*   LastItem() const;

		    bool	    HasItem(ListItem* item) const;
		    int32	    CountItems() const;

    virtual	void	    MakeEmpty();
		    bool	    IsEmpty() const;

		    void	    DoForEach(bool (*func)(ListItem*));
		    void	    DoForEach(bool (*func)(ListItem*, void*), void*);

    const   ListItem**  Items() const;
		    void        InvalidateItem(int32 index);

		    void        Select(int32 index, bool extend = false);
		    void	    Select(int32 from, int32 to, bool extend = false);
		    bool	    IsItemSelected(int32 index) const;
		    int32	    CurrentSelection(int32 index = 0) const;
    virtual	void        Invoke();

		    void	    DeselectAll();
		    void	    DeselectExcept(int32 except_from, int32 except_to);
		    void	    Deselect(int32 index);

    virtual void	    SelectionChanged();

		    void	    SortItems(int (*cmp)(const ListItem*, const ListItem*));

 protected:
            int32       CheckIndex(int32 index);
            int32       DetermineFirstSelected(int32 after);
            int32       DetermineLastSelected(int32 before);

            bool        InternalDeselect(int32 index);
            bool        InternalDeselectAll(int32 except_from, int32 except_to);

            bool        InternalSelect(int32 index, bool extend);
            bool        InternalSelect(int32 from, int32 to, bool extend);

            void        UpdateScrollBar();

    static  void        thread_function(void*);
            void        ThreadFunction();

    
 private:
    list_view_type      m_type;
    List<ListItem*>*    m_list;
    int32               m_firstVisible;
    int32			    m_firstSelected;
    int32			    m_lastSelected;
    int32               m_anchorIndex;
    int32               m_command;
    bool                m_pressed;
    int32               m_pressedX;
    int32               m_pressedY;
    bool                m_dragging;
    int32               m_yInsert;
    ScrollView*         m_scroller;
    Thread*             m_thread;
    int32               m_increment;
    bool                m_autoscroll;
  
    
};

#endif /* _LIST_VIEW_H_ */