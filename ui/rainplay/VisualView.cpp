#include "stdafx.h"
#include "VisualView.h"

/////////////////////////////////////////////////////////////////////////////
// CVisualView class
CVisualView::CVisualView(CWnd *pWnd, UINT x, UINT y, UINT width, UINT height)
		:	m_pWnd(pWnd),
			_x(x),
			_y(y),
			_width(width),
			_height(height),
			m_bDisplay(TRUE)
{
#if 0
	m_ringbuf = new RINGBUF();
	m_ringbuf->len = 13432;			//hack code
	m_ringbuf->mutex = FALSE;
	m_ringbuf->pbuf = new unsigned char[m_ringbuf->len];
#endif
	ASSERT(m_pWnd!=NULL);
	waveview = new CWaveformView(m_pWnd,_x,_y,_width,_height);
	_thread.SetPriority(THREAD_PRIORITY_BELOW_NORMAL);
	_thread.Resume ();
}

CVisualView::~CVisualView()
{
#if 0
	delete [] m_ringbuf->pbuf;
	delete m_ringbuf;
#endif
}

void CVisualView::Run ()
{
#if 0
	CSine a(m_pWnd,_x,_y,_width,_height);
    for (;;) {
		//_event.Wait ();
		Sleep(25);
		if (_isDying)
			return;
		CLock lock (_mutex);
		if (m_bDisplay)
			a.Render();
    }
#else
	for(;;) {
		_event.Wait();
		if (_isDying)
			return;

		CLock lock(_mutex);
		if (m_bDisplay) {
			waveview->SetBuffer(m_sample, _length);
			waveview->Render();
			_render.Release();
		} else {
			_render.Release();
		}
	}
#endif

}

void CVisualView::FlushThread ()
{
    _event.Release ();
}


BOOL CVisualView::Start ()
{
	CLock lock (_mutex);
	m_bDisplay = TRUE;
	return TRUE;
}


BOOL CVisualView::Stop()
{
    CLock lock (_mutex);
	m_bDisplay = FALSE;
	return TRUE;
}


BOOL CVisualView::VisDataReady(unsigned int length, unsigned char *buf) {
	_length = length;
	m_sample = (LPSAMPLE)buf;

	//Begin render
	_event.Release();
	//Wait for render end
	_render.Wait();
	return TRUE;
}


BOOL CWaveformView::Render()
{
	Clear();
	register int y;
	if (m_scopemode == line || m_scopemode == solid)
		memDC->MoveTo(0,m_voffset);
	for(register int i=0; i<m_width; i++) {
		if (abs(m_sample[i*4].l)<=8192)
			y = m_sample[i*4].l/(m_hratio*0.33);
		else if (abs(m_sample[i*4].l)<=16384)
			y = m_sample[i*4].l/(m_hratio*0.66);
		else
			y = m_sample[i*4].l/m_hratio;
		if (m_scopemode == dot )
			memDC->SetPixel(i, y+m_voffset, RGB(0,0,0));
		else {
			if (m_scopemode == solid)
				memDC->MoveTo(i, m_voffset);
			memDC->LineTo(i, y+m_voffset);
		}
	}
	Flip();
	return TRUE;
}