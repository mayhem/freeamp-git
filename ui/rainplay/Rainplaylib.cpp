/* project headers */
#include "stdafx.h"
#include "Rainplay.h"
#include "RainplayUI.h"

void Initialize(UIRef ref)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here

    if(ref )
    {
        UserInterface* ui = new CRainplayUI;
        ref->ref = ui;

        ref->SetTarget = SetTarget;
        ref->SetArgs = SetArgs;
        ref->AcceptEvent = AcceptEvent;
        ref->Cleanup = Cleanup;  
    }	
}

void SetTarget(UIRef ref, EventQueueRef eq)
{
    CRainplayUI* ui = (CRainplayUI*)ref->ref;

    ui->SetTarget(eq);
}

void SetArgs(UIRef ref, int32 argc, char **argv)
{
    CRainplayUI* ui = (CRainplayUI*)ref->ref;

    ui->SetArgs(argc, argv);
}

int32 AcceptEvent(UIRef ref, Event* event)
{
    CRainplayUI* ui = (CRainplayUI*)ref->ref;

    return ui->AcceptEvent(event);
}

void Cleanup(UIRef ref)
{
	CRainplayUI* ui = (CRainplayUI*)ref->ref;

    delete ui;
}