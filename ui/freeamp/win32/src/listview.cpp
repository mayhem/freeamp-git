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

/* system headers */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "listview.h"
#include "renderer.h"
#include "scrollview.h"


ListView::
ListView(   HWND hwnd, 
            View* parent, 
            HRGN viewRegion,
            list_view_type type):
View(hwnd, parent, viewRegion)
{
    m_type = type;
    m_firstVisible = 0;
    m_firstSelected = -1;
    m_lastSelected = -1;
    m_anchorIndex = -1;
    m_command = -1;
    m_pressed = false;
    m_pressedX = 0;
    m_pressedY = 0;
    m_dragging = false;
    m_list = new List<ListItem*>();
    m_scroller = NULL;

    m_thread = NULL;
    m_autoscroll = false;

}

ListView::
ListView(   HWND hwnd, 
            View* parent, 
            RECT* viewRect,
            list_view_type type):
View(hwnd, parent, viewRect)
{
    m_type = type;
    m_firstVisible = 0;
    m_firstSelected = -1;
    m_lastSelected = -1;
    m_anchorIndex = -1;
    m_command = -1;
    m_pressed = false;
    m_pressedX = 0;
    m_pressedY = 0;
    m_dragging = false;
    m_list = new List<ListItem*>();
    m_scroller = NULL;

    m_thread = NULL;
    m_autoscroll = false;
}

ListView::
~ListView()
{
    m_autoscroll = false;
    m_list->DeleteAll();
    delete m_list;
}

void 
ListView::
Pulse()
{
    
}

void 
ListView::
MouseMove(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed)
    {
        if(m_dragging || (x != m_pressedX || y != m_pressedY))
        {
            m_dragging = true;
            RECT bounds;

            Bounds(&bounds);

            // auto-scrolling
            if(y < bounds.top)
            {
                m_increment = 1;
                m_autoscroll = true;

                if(m_thread == NULL)
                {
                    m_thread = Thread::CreateThread();
                    m_thread->Create(thread_function, this);
                }
            }
            else if(y > bounds.bottom)
            {
                m_increment = -1;
                m_autoscroll = true;

                if(m_thread == NULL)
                {
                    m_thread = Thread::CreateThread();
                    m_thread->Create(thread_function, this);
                }
            }
            else
            {
                m_autoscroll = false;

                int32 index = IndexOf(x,y);

                //char buffer[256];
                //wsprintf(buffer, "index: %d\r\n",index);
                //OutputDebugString(buffer);

                if(index >= 0)
                {
                    ListItem* item = ItemAt(index);
   
                    // figure out where in the item this
                    // point is located
                    int32 count = m_firstVisible;
                    RECT itemRect;

                    Bounds(&itemRect);
                    OffsetRect(&itemRect, 0, 2);
            
                    for(count; count < index; count++)
                    {
                        ListItem* i = ItemAt(count);

                        if(!i)
                            break;

                        itemRect.bottom = itemRect.top + i->Height();

                        OffsetRect(&itemRect, 0, i->Height() + 2);
                    }      

                    itemRect.bottom = itemRect.top + item->Height();

                    //if(itemRect.bottom < bounds.bottom)
                    {
                        int32 middle = itemRect.top + item->Height()/2;

                        if(y <= middle)
                        {
                            m_yInsert = itemRect.top - 2;
                        }
                        else
                        {
                            m_yInsert = itemRect.bottom;
                        }

                        Invalidate();
                    }
                }
            }
        }
    }
}

void 
ListView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{

    bool extend =   (m_type == MULTIPLE_SELECTION_LIST) && 
		            (modifiers & MK_SHIFT) != 0;

	bool disjoint = (m_type == MULTIPLE_SELECTION_LIST) && 
			        (modifiers & MK_CONTROL) != 0;

    bool select = true;
    bool update = false;

    m_pressedX = x;
    m_pressedY = y;

    int32 index = IndexOf(x,y);
    int32 lastindex = CountItems() - 1;

    if(index >= 0)
    {
        m_pressed = true;
    }

    if(extend) 
    {
		int32 i = (index != -1) ? index : lastindex;

		if(i < m_firstSelected)
			m_anchorIndex = m_firstSelected;
        else if(i > m_lastSelected)
            m_anchorIndex = m_lastSelected;
		else 
			m_anchorIndex = m_lastSelected;

		if(m_anchorIndex == -1)
			m_anchorIndex = index;
	} 
    else 
    {
		m_anchorIndex = (index != -1) ? index : lastindex;

		if (disjoint && (index != -1)) 
        {
			ListItem* item = ItemAt(index);
			select = !item->IsSelected();
		}
	}

    // try to initate a drag
	if (!extend) 
    {
		ListItem* item = ItemAt(index);

		if(item && item->IsSelected())
        {
            //m_dragging = true;
			return;
        }
	}

    if(extend)
    {
        if(InternalSelect(m_anchorIndex, index, false))
            update = true;
    }
    else if(disjoint)
    {
        if(select) 
        {
            if(InternalSelect(index, true))
                update = true;
        } 
        else 
        {
            if(InternalDeselect(index))
                update = true;
        }

        m_anchorIndex = index;
    }
    else
    {
        if(InternalSelect(index, false))
                update = true;

        m_anchorIndex = index;
    }

    if(update)
    {
        SelectionChanged();
    }
}

void 
ListView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed)
    {
        bool extend =   (m_type == MULTIPLE_SELECTION_LIST) && 
		                (modifiers & MK_SHIFT) != 0;

        bool disjoint = (m_type == MULTIPLE_SELECTION_LIST) && 
			            (modifiers & MK_CONTROL) != 0;

        if(!extend && !disjoint && !m_dragging)
        {
            int32 index = IndexOf(x,y);

            if(InternalSelect(index, false))
            {
                SelectionChanged();
            }

            m_anchorIndex = index;
        }
        else if(m_dragging)
        {
            int32 index = IndexOf(x,y);

            if(index == -1)
                index = CountItems() - 1;

            if(index >= 0)
            {
                ListItem*   item = ItemAt(index);
                int32		i = 0;
                int32       count = m_firstVisible;
                RECT        itemRect;

                Bounds(&itemRect);
                OffsetRect(&itemRect, 0, 2);

                for(count; count < index; count++)
                {
                    ListItem* i = ItemAt(count);

                    if(!i)
                        break;

                    itemRect.bottom = itemRect.top + i->Height();

                    OffsetRect(&itemRect, 0, i->Height() + 2);
                }      

                itemRect.bottom = itemRect.top + item->Height();

                int32 middle = itemRect.top + item->Height()/2;

                if(y > middle)
                {
                    index++;
                }
                
                count = 0;

                while(i < index) 
                {
		            if(ItemAt(i++)->IsSelected())
                    {
                        count++;
                    }
	            }

                index -= count;

                char buffer[256];
                wsprintf(buffer, "index: %d\r\n",index);
                OutputDebugString(buffer);

                // remove selected items from list
                List<ListItem*>* selectList = new List<ListItem*>();

	            i = m_lastSelected;

	            while(item = ItemAt(i--)) 
                {
		            if(item->IsSelected())
                    {
                        RemoveItem(item);
                        //item->Select(); // remove deselects
                        selectList->InsertAt(0, item);
                    }
	            }

                if(index > CountItems())
                    index = CountItems();

                AddList(selectList, index);

                delete selectList;
            }
        }

        m_pressed = false;
        m_dragging = false;
        m_autoscroll = false;
        Invalidate();
    }
}

void 
ListView::
LeftButtonDoubleClick(int32 x, int32 y, int32 modifiers)
{

}

void 
ListView::
Draw(DIB* canvas, RECT* invalidRect)
{
    RECT tempRect, viewRect, parentRect, drawRect;

    assert(canvas);

    // what is our rect?
    Bounds(&viewRect);

    if(invalidRect && !IsRectEmpty(&drawRect))
    {
        // what is the smallest portion of us inside the dirty rect
        IntersectRect(&tempRect, invalidRect, &viewRect);
    }
    else
    {
        CopyRect(&tempRect, &viewRect);
    }

    if(Parent())
    {
        Parent()->Bounds(&parentRect);

        // clip ourselves to our parent view
        IntersectRect(&drawRect, &tempRect, &parentRect);
    }
    else
    {
        CopyRect(&drawRect, &tempRect);
    }

    if(!IsRectEmpty(&drawRect))
    {
        // render the draw rect to the canvas

        // first we need to translate the coordinates

        int32 src_x, src_y; // starting point in source
        int32 dest_x, dest_y; // starting point in destination
        int32 width, height; // lengths in source

        src_x = abs(drawRect.left - viewRect.left);
        src_y = abs(drawRect.top - viewRect.top);
        dest_x = drawRect.left;
        dest_y = drawRect.top;
        width = drawRect.right - drawRect.left;
        height = drawRect.bottom - drawRect.top;

        // second we need to blit the bits

        RECT itemRect;
        ListItem* item = NULL;
        int32 count = m_firstVisible;

        SetRect(&itemRect,  viewRect.left, 
                            viewRect.top, 
                            viewRect.right, 
                            viewRect.top);

        OffsetRect(&itemRect, 0, 2);

        while(itemRect.top <= drawRect.bottom)
        {
            item = ItemAt(count++);

            if(!item)
                break;

            itemRect.bottom = itemRect.top + item->Height();

            // clip
            if(itemRect.bottom > drawRect.bottom)
                itemRect.bottom = drawRect.bottom;            

            item->DrawItem(canvas, &itemRect);

            OffsetRect(&itemRect, 0, item->Height() + 2);
        }

        if(m_dragging)
        {
            Renderer::Fill( canvas,
                            dest_x, 
                            m_yInsert,     
                            width,      
                            2, 
                            0,    
                            0,
                            0);
        }
    }
}

void 
ListView::
TargetedByScrollView(ScrollView *scroller)
{
    m_scroller = scroller;

    m_scroller->SetRange(0, CountItems() - 1);
    m_scroller->SetPosition(m_firstVisible);

    if(CountItems() - 1 < 7)
        m_scroller->SetProportion(CountItems() - 1);
    else
        m_scroller->SetProportion(7);
}

void 
ListView::
ScrollTo(int32 index)
{
    index = CheckIndex(index);

    if(index >= 0)
    {
        m_firstVisible = index;

        Invalidate();
    }
}

void        
ListView::
ScrollToSelection()
{
    if(m_firstSelected >= 0)
    {
        m_firstVisible = m_firstSelected;

        if(m_scroller)
            m_scroller->SetPosition(m_firstVisible);

        Invalidate();
    }
}


bool 
ListView::
AddItem(ListItem* item)
{
    bool result = false;

    if(m_list->Insert(item))
    {
		result = true;
	}

    return result;
}

bool 
ListView::
AddItem(ListItem* item, int32 index)
{
    bool result = false;

    if(m_list->InsertAt(index, item))
    {
		result = true;

        // adjust selection range if need be
	    if(m_firstSelected != -1 && 
            index <= m_firstSelected)
        {
		    m_firstSelected++;
        }

	    if(m_lastSelected != -1 && 
            index <= m_lastSelected)
        {
		    m_lastSelected++;
        }
	}

    return result;
}

bool 
ListView::
AddList(List<ListItem*>* items)
{
    bool result = false;

    result = AddList(items, CountItems());

    return result;
}

bool 
ListView::
AddList(List<ListItem*>* items, int32 index)
{
    bool result = false;

    result = m_list->AddListAt(*items, index);

    if(result)
    {
        int32 count = items->NumElements();

        // adjust selection range if need be
	    if(m_firstSelected != -1 && 
            index <= m_firstSelected)
        {
		    m_firstSelected += count;
        }

	    if(m_lastSelected != -1 && 
            index <= m_lastSelected)
        {
		    m_lastSelected += count;
        }
    }

    return result;
}


bool 
ListView::
RemoveItem(ListItem* item)
{
    bool result = false;

    result = RemoveItem(IndexOf(item)) != NULL;

    return result;
}

ListItem* 
ListView::
RemoveItem(int32 index)
{
    ListItem* result = NULL;

    result = ItemAt(index);

    if(result)
    {
        if(result->IsSelected())
        {
		    Deselect(index);
        }

        if(result && m_list->RemoveElementAt(index)) 
        {

		    if(m_firstSelected != -1 && 
                index < m_firstSelected)
            {
			    m_firstSelected--;
            }

		    if(m_lastSelected != -1 && 
                index < m_lastSelected)
            {
			    m_lastSelected--;
            }
	    }
    }

    return result;
}

bool 
ListView::
RemoveItems(int32 index, int32 count)
{
    bool result = false;

    index = CheckIndex(index);

    if(index >= 0 )
    {
        while(count--)
        {
		    RemoveItem(index);
        }

        result = true;
    }

    return result;
}


void 
ListView::
SetListType(list_view_type type)
{
    if( type == SINGLE_SELECTION_LIST &&
		m_type == MULTIPLE_SELECTION_LIST)
    {
        DeselectAll();
    }

	m_type = type;
}


list_view_type 
ListView::
ListType() const
{
    return m_type;
}


ListItem*   
ListView::
ItemAt(int32 index) const
{
    ListItem* result = NULL;

    result = m_list->ElementAt(index);

    return result;
}

int32 
ListView::
IndexOf(int32 x, int32 y) const
{
    int32       result = -1;
    ListItem*   item;
	int32		i = m_firstVisible;
    int32		cur_y;
    RECT        bounds;

    Bounds(&bounds);

	cur_y = bounds.top + 2;

	if (y >= 0)
    {
	    while (item = ItemAt(i++)) 
        {
		    cur_y += item->Height();

		    if (cur_y >= y)
            {
			    result = i - 1;
                break;
            }
            else if(cur_y > bounds.bottom)
            {
                break;
            }

		    cur_y+=2;
	    }
    }

    return result;
}

int32 
ListView::
IndexOf(ListItem* item) const
{
    int32 result = 0;

    result = m_list->IndexOf(item);

    return result;
}

ListItem*  
ListView::
FirstItem() const
{
    ListItem* result = NULL;

    result = m_list->FirstElement();

    return result;
}

ListItem*  
ListView::
LastItem() const
{
    ListItem* result = NULL;

    result = m_list->LastElement();

    return result;
}

bool	    
ListView::
HasItem(ListItem* item) const
{
    bool result = false;

    result = m_list->HasElement(item);

    return result;
}

int32	    
ListView::
CountItems() const
{
    int32 result = 0;

    result = m_list->NumElements();

    return result;
}

void	    
ListView::
MakeEmpty()
{
    m_list->DeleteAll();
}

bool	    
ListView::
IsEmpty() const
{
    bool result = false;

    result = m_list->IsEmpty();

    return result;
}

void	    
ListView::
DoForEach(bool (*func)(ListItem*))
{
    int32 count = CountItems();

	for(int32 i = 0; i < count; i++)
    {
		if((*func)((ListItem *) m_list->ElementAt(i)))
        {
			break;
        }
    }
}

void	    
ListView::
DoForEach(bool (*func)(ListItem*, void*), void* cookie)
{
    int32 count = CountItems();

	for(int32 i = 0; i < count; i++)
    {
		if((*func)((ListItem *) m_list->ElementAt(i), cookie))
        {
			break;
        }
    }
}

const ListItem**  
ListView::
Items() const
{
    return (const ListItem**)(m_list->Elements());
}

void        
ListView::
InvalidateItem(int32 index)
{

}

void
ListView::
Select(int32 index, bool extend)
{
    if(InternalSelect(index, extend)) 
    {
		SelectionChanged();
	}
}

bool
ListView::
InternalSelect(int32 index, bool extend)
{
    bool result = false;

    index = CheckIndex(index);

    if(index >= 0)
    {
        ListItem* item = ItemAt(index);

        if(item->IsEnabled()) 
        {
            if(!extend) 
            {
		        if(InternalDeselectAll(index, index))
                {
                    result = true;
                }

                m_firstSelected = m_lastSelected = index;
	        }
            else
            {
		        if (m_firstSelected == -1) 
                {
			        m_firstSelected = m_lastSelected = index;
		        } 
                else 
                {
			        if (index < m_firstSelected)
                    {
				        m_firstSelected = index;
                    }

			        if (index > m_lastSelected)
                    {
				        m_lastSelected = index;
                    }
		        }
	        }

            if (!item->IsSelected()) 
            {
			    item->Select();
                result = true;
		    }
        }
    }

    return result;
}

void	    
ListView::
Select(int32 from, int32 to, bool extend)
{
	if(InternalSelect(from, to, extend)) 
    {
		SelectionChanged();
	}
}

bool	    
ListView::
InternalSelect(int32 from, int32 to, bool extend)
{
    bool result = false;

    from = CheckIndex(from);
	to = CheckIndex(to);

    if(from > to) 
    {
		int32 swap = from;
		from = to;
		to = swap;
	}

    if(from != -1 && to != -1)
    {
        if (!extend) 
        {
		    if(!InternalDeselectAll(from, to))
            {
                result = true;
            }
	    }

        for (int32 i = from; i <= to; i++) 
        {
		    ListItem* item = ItemAt(i);

		    if (item->IsEnabled() == false)
			    continue;

		    if (!item->IsSelected()) 
            {
			    item->Select();
                result = true;
		    }
	    }

	    if(result) 
        {
		    m_firstSelected = DetermineFirstSelected(0);
		    m_lastSelected = DetermineLastSelected(CountItems() - 1);
	    }
    }

    return result;
}

bool	    
ListView::
IsItemSelected(int32 index) const
{
    bool result = false;

    ListItem* item = m_list->ElementAt(index);
	result = item ? item->IsSelected() : false;

    return result;
}

int32	    
ListView::
CurrentSelection(int32 index) const
{
    int32 result = -1;

	if (m_firstSelected != -1)
    {
	    if(index == 0)
        {
		    result = m_firstSelected;
        }
	    else 
        {
		    int32	n = 0;
		    int32	i = 0;

		    for (i = m_firstSelected; i <= m_lastSelected; i++) 
            {
			    ListItem* item = ItemAt(i);

			    if (item->IsSelected()) 
                {
				    if (n == index)
                    {
					    result = i;
                        break;
                    }

				    n++;
			    }
		    }
	    }
    }

    return result;
}

void        
ListView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
ListView::
DeselectExcept(int32 except_from, int32 except_to)
{
	except_from = CheckIndex(except_from);
	except_to = CheckIndex(except_to);

	if (except_from > except_to) 
    {
		long swap = except_from;
		except_from = except_to;
		except_to = swap;
	}

	if (except_from != -1 && except_to != -1)
    {
	    if (InternalDeselectAll(except_from, except_to)) 
        {
		    SelectionChanged();
        }
	}
}

void	    
ListView::
DeselectAll()
{
    if(InternalDeselectAll(-1, -1)) 
    {
		SelectionChanged();
	}
}

bool	    
ListView::
InternalDeselectAll(int32 except_from, int32 except_to)
{
    bool result = false;

    if(m_firstSelected != -1)
    {
        for(int32 i = m_firstSelected; i <= m_lastSelected; i++) 
        {
            ListItem* item = ItemAt(i);

            if( item->IsSelected() && 
                !((except_from <= i) && (i <= except_to))) 
            {
			    item->Deselect();
                result = true;
            }
        }   

        m_firstSelected = DetermineFirstSelected(m_firstSelected);
	    m_lastSelected = DetermineFirstSelected(m_lastSelected);
    }

    return result;
}

void	    
ListView::
Deselect(int32 index)
{
    if(InternalDeselect(index)) 
    {
		SelectionChanged();
	}
}

bool 
ListView::
InternalDeselect(int32 index)
{
    bool result = false;

    index = CheckIndex(index);

    if(index >= 0 )
    {
        ListItem* item = ItemAt(index);

        if(item->IsSelected()) 
        {
            item->Deselect();

            if (index == m_firstSelected && index == m_lastSelected)
            {
		        m_firstSelected = m_lastSelected = -1;
            }
	        else if (index == m_firstSelected)
            {
		        m_firstSelected = DetermineFirstSelected(index + 1);
            }
	        else if (index == m_lastSelected)
            {
		        m_lastSelected = DetermineLastSelected(index - 1);
            }
        }

        result = true;
    }

    return result;
}

void	    
ListView::
SelectionChanged()
{
    Invalidate();
}

void	    
ListView::
SortItems(int (*cmp)(const ListItem*, const ListItem*))
{
    DeselectAll();
    //m_list->SortItems(cmp);
	Invalidate();
}


inline 
int32 
ListView::
CheckIndex(int32 index)
{
	// If we're dealing with a bogus index then set it to -1.
	if (index < 0 || index >= CountItems())
    {
		index = -1;
    }

	return index;
}

int32 
ListView::
DetermineFirstSelected(int32 after)
{
    int32       result = -1;
	ListItem*   item;
	int32		count = CountItems();

	for(int32 i = after; i < count; i++) 
    {
		item = ItemAt(i);

		if (item->IsSelected())
        {
			result = i;
            break;
        }
	}

	return result;
}

int32 
ListView::
DetermineLastSelected(int32 before)
{
    int32       result = -1;
	ListItem*   item;

	for(int32 i = before; i >= 0; i--)
    {
		item = ItemAt(i);

		if (item->IsSelected())
        {
			result = i;
            break;
        }
	}

	return result;
}

void
ListView::
ThreadFunction()
{
    do
    {
        if(m_increment > 0)
        {
            if(m_firstVisible > 0)
            {
                ScrollTo(--m_firstVisible);

                if(m_scroller)
                    m_scroller->SetPosition(m_firstVisible);
            }
        }
        else if(m_increment < 0)
        {
            if(m_firstVisible < CountItems() - 8)
            {
                ScrollTo(++m_firstVisible); 

                if(m_scroller)
                    m_scroller->SetPosition(m_firstVisible);
            }
        }

        Sleep(100);

    }while(m_autoscroll);

    m_thread = NULL;
}

void 
ListView::
thread_function(void* arg)
{
    ListView* view = (ListView*)arg;

    view->ThreadFunction();
}




