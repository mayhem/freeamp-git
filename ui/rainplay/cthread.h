#if !defined CTHREAD_H
#define CTHREAD_H
//------------------------------------
//  thread.h
//  (c) Bartosz Milewski, 1996
//------------------------------------

#include <windows.h>

class CThread
{
public:
    CThread (DWORD (WINAPI * pFun) (void* arg), void* pArg)
    {
        _handle = CreateThread (
            0, // Security attributes
            0, // Stack size
            pFun, 
            pArg, 
            CREATE_SUSPENDED, 
            &_tid);
    }
    ~CThread () { CloseHandle (_handle); }
    void Resume () { ResumeThread (_handle); }
    void WaitForDeath ()
    {
        WaitForSingleObject (_handle, 2000);
    }
	int GetPriority() const	{
		return GetThreadPriority(_handle);
	}

	int SetPriority(int priority) {
		int old = GetThreadPriority(_handle);
		SetThreadPriority(_handle, priority);
		return old;
	}

private:
    HANDLE _handle;
    DWORD  _tid;     // thread id
};

class CMutex
{
    friend class CLock;
public:
    CMutex () { InitializeCriticalSection (&_critSection); }
    ~CMutex () { DeleteCriticalSection (&_critSection); }
private:
    void Acquire () 
    { 
        EnterCriticalSection (&_critSection);
    }
    void Release () 
    { 
        LeaveCriticalSection (&_critSection);
    }

    CRITICAL_SECTION _critSection;
};

class CLock 
{
public:
	// Acquire the state of the semaphore
	CLock ( CMutex& mutex ) 
		: _mutex(mutex) 
	{
		_mutex.Acquire();
	}
	// Release the state of the semaphore
	~CLock ()
	{
		_mutex.Release();
	}
private:
	CMutex& _mutex;
};

class CEvent
{
public:
    CEvent ()
    {
        // start in non-signaled state (red light)
        // auto reset after every Wait
        _handle = CreateEvent (0, FALSE, FALSE, 0);
    }

    ~CEvent ()
    {
        CloseHandle (_handle);
    }

    // put into signaled state
    void Release () { SetEvent (_handle); }
    void Wait ()
    {
        // Wait until event is in signaled (green) state
        WaitForSingleObject(_handle, INFINITE);
    }
    operator HANDLE () { return _handle; }
private:
    HANDLE _handle;
};

class CTrafficLight
{
public:
    CTrafficLight ()
    {
        // Start in non-signaled state (red light)
        // Manual reset
        _handle = CreateEvent (0, TRUE, FALSE, 0);
    }

    ~CTrafficLight ()
    {
        CloseHandle (_handle);
    }

    // put into signaled state
    void GreenLight () { SetEvent (_handle); }

    // put into non-signaled state
    void RedLight () { ResetEvent (_handle); }

    void Wait ()
    {
        // Wait until event is in signaled (green) state
        WaitForSingleObject(_handle, INFINITE);
    }

private:
    HANDLE _handle;
};

#endif