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
#include "preferences.h"
#include "facontext.h"

class CRainplayUI: public UserInterface
{
public:
    CRainplayUI(FAContext *context);
    ~CRainplayUI();

	virtual Error Init(int32 startup_type) { return kError_NoErr; }
    virtual void SetTarget(EventQueue*);
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int32,char **);
	virtual void SetPlayListManager(PlayListManager *);
	virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
    void CreateUI();

	Semaphore*      m_uiSemaphore;
	int32			m_state;
	PlayListManager *m_plm;
	EventQueue*     m_target;
protected:
      static void UIThreadFunc(void *);

private:
    FAContext*      m_context;
	Properties *    m_propManager;
    Preferences*     m_prefs;
	CRainplayDlg*   m_Dlg;
	Thread*         m_uiThread;
};

#endif	//__RAINPLAYUI_H__
