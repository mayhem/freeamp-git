#ifndef	__RAINPLAYUI_H__
#define	__RAINPLAYUI_H__

#include "stdafx.h"

#include "config.h"
#include "ui.h"
#include "thread.h"
#include "event.h"
#include "semaphore.h"
#include "queue.h"
#include "playlist.h"
#include "RainplayDlg.h"
#include "player.h"

class CRainplayUI: public UserInterface
{
public:
    CRainplayUI();
    ~CRainplayUI();

	virtual Error Init() { return kError_NoErr; }
    virtual void SetTarget(EventQueue*);
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int32,char **);
	virtual void SetPlayListManager(PlayListManager *);

    void CreateUI();

	Semaphore*      m_uiSemaphore;
	int32			m_state;
	PlayListManager *m_plm;
	EventQueue*     m_target;
protected:
      static void UIThreadFunc(void *);

private:
	CRainplayDlg*	m_Dlg;
	Thread*         m_uiThread;
};

#endif	//__RAINPLAYUI_H__