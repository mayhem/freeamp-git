#ifndef	__RAINPLAYUI_H__
#define	__RAINPLAYUI_H__

#include "stdafx.h"

#include "config.h"
#include "ui.h"
#include "thread.h"
#include "event.h"
#include "semaphore.h"
#include "queue.h"
#include "RainplayDlg.h"

class CRainplayUI: public UserInterface
{
public:
    CRainplayUI();
    ~CRainplayUI();

	virtual void Init() {}
    virtual void SetTarget(EventQueue*);
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int32,char **);

    void CreateUI();
    
	Semaphore*      m_uiSemaphore;

protected:
      static void UIThreadFunc(void *);

private:
	CRainplayDlg*	m_Dlg;
	Thread*         m_uiThread;
    EventQueue*     m_target;
};

#endif	//__RAINPLAYUI_H__