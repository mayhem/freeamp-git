#if !defined ACTIVE_H
#define ACTIVE_H
//------------------------------------
//  active.h
//  Active object framework
//  (c) Bartosz Milewski, 1996
//------------------------------------

#include "cthread.h"
#include <windows.h>

class ActiveObject
{
public:
    ActiveObject ();
    virtual ~ActiveObject () {}
    void Kill ();

protected:
    virtual void InitThread () = 0;
    virtual void Run () = 0;
    virtual void FlushThread () = 0;
	int GetPriority() const	{
		return _thread.GetPriority();
	}

	int SetPriority(int priority) {
		int old = _thread.GetPriority();
		_thread.SetPriority(priority);
		return old;
	}
    int             _isDying;

    static DWORD WINAPI ThreadEntry (void *pArg);
    CThread          _thread;
};

// Last thing in the constructor of a class derived from 
// ActiveObject you must call
//    _thread.Resume ();
// Inside the loop the Run method you must keep checking _isDying
//    if (_isDying)
//         return;
#endif