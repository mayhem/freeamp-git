#include "stdafx.h"
#include "VisualView.h"
#include "BmpSize.h"

/////////////////////////////////////////////////////////////////////////////
// MemCanvas class
MemCanvas::MemCanvas(CWnd *pWnd,
	int x, int y, int width, int height,
	CBmpSize *background, COLORREF pencolor)
:	m_x(x),
	m_y(y),
	m_height(height),
	m_width(width),
	m_bkgBmp(background),
	m_pWnd(pWnd)
{
	dc = new CClientDC(m_pWnd);
	memDC = new CDC();
	memBM = new CBitmap();
	memDC->CreateCompatibleDC(dc);
	memBM->CreateCompatibleBitmap(dc, m_width, m_height);
	oldBM = memDC->SelectObject(memBM);


	memBMDC1 = new CDC();
	memBKGBM = new CBitmap();
	//准备背景
	memBMDC1->CreateCompatibleDC(dc);		//用来放background
	memBKGBM->CreateCompatibleBitmap(dc, m_width, m_height);	//用来放background
	oldBM2 = memBMDC1->SelectObject(memBKGBM);

	memBMDC = new CDC();
	memBMDC->CreateCompatibleDC(dc);		//用来放mainup
	oldBM1 = memBMDC->SelectObject(&(m_bkgBmp->m_cBmp));	//选mainup
	memBMDC1->BitBlt(0, 0, m_width, m_height, memBMDC, m_x, m_y, SRCCOPY);
	memBMDC->SelectObject(oldBM1);

	m_pen = new CPen(PS_SOLID, 1, pencolor);
	oldpen = memDC->SelectObject(m_pen);
	Clear();
}


MemCanvas::~MemCanvas()
{
	memDC->SelectObject(oldpen);
	delete m_pen;
	memBMDC1->SelectObject(oldBM2);
	delete memBKGBM;
	delete memBMDC1;

	delete memBMDC;

	memDC->SelectObject(oldBM);
	delete memBM;
	delete memDC;
	delete dc;
}


BOOL MemCanvas::Clear(void)
{
	//向memDC复制背景
	memDC->BitBlt(0, 0, m_width, m_height, memBMDC1, 0, 0, SRCCOPY);
	return TRUE;
}
BOOL MemCanvas::Flip(void)
{
	//Flip Surface:Backsurface -> Frontsurface
	return(
	dc->BitBlt(	m_x, m_y, m_width, m_height,
				memDC,
				0,0,
				SRCCOPY)
	);
}
BOOL MemCanvas::Flip(int off)
{
	//Flip Surface:Backsurface -> Frontsurface
	return(
	dc->BitBlt(	m_x, m_y, m_width+off, m_height,
				memDC,
				0,0,
				SRCCOPY)
	);
}

/////////////////////////////////////////////////////////////////////////////
// CVisualView class
CVisualView::CVisualView(CWnd *pWnd, int fps,
						 VISMODE mode, ANALYZERMODE analyzermode, SCOPEMODE scopemode,
						 UINT x, UINT y, UINT width, UINT height, CBmpSize *bkgBmp)
:	m_pWnd(pWnd),
	m_visualmode(mode),
	_x(x),
	_y(y),
	_width(width),
	_height(height),
	m_bDisplay(TRUE)
{
	ASSERT(m_pWnd!=NULL);

	waveview = new CWaveformView(m_pWnd, fps, scopemode, _x,_y,_width,_height, bkgBmp);
	specview = new CSpectrumView(m_pWnd, fps, analyzermode, _x,_y,_width,_height, bkgBmp);

	_thread.SetPriority(THREAD_PRIORITY_BELOW_NORMAL);
	_thread.Resume ();
}

CVisualView::~CVisualView()
{
	delete waveview;
	delete specview;
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
			if (m_visualmode==oscilliscope) {
				waveview->SetBuffer(m_sample, _length);
				waveview->Render();
			} else if (m_visualmode==spectrum) {
				specview->SetBuffer(m_sample, _length);
				specview->Render();
			}
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


BOOL CVisualView::VisDataReady(unsigned int length, unsigned char *buf)
{
	_length = length;
	m_sample = (LPSAMPLE)buf;

	//Begin render
	_event.Release();
	//Wait for render end
	_render.Wait();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveformView class
BOOL CWaveformView::Render()
{
	CLock lock (_mutex);
	Clear();
	register int y;
	if (m_scopemode == line || m_scopemode == solid)
		memDC->MoveTo(0,m_voffset);
	for(register int i=0; i<m_width; i++) {
		if (abs(m_sample[i*4].l)<=6000)
			y = (int)(m_sample[i*4].l/(m_hratio*0.67));
		else
			y = m_sample[i*4].l/m_hratio;
		if (m_scopemode == dot )
			memDC->SetPixel(i, y+m_voffset, rgbDisplayColur);
		else {
			if (m_scopemode == solid)
				memDC->MoveTo(i, m_voffset);
			memDC->LineTo(i, y+m_voffset);
		}
	}
	Flip();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSpectrumView class
BOOL CSpectrumView::Render()
{
	CLock lock (_mutex);
	Clear();
	CRect rect;
	CBrush brush(rgbDisplayColur);
	register int y;
	m_fftTransformer->CopyIn(m_sample, m_points);
	m_fftTransformer->Transform();

	if (this->m_analyzer==lines) {
		memDC->MoveTo(0,m_height/2);
		for(register int i=0; i<m_width; i++) {
			y = 0 - int (m_fftTransformer->GetIntensity(i*2) / 1024);
			memDC->MoveTo(i,m_height);
			memDC->LineTo(i,y+m_height);
		}
	} else if (this->m_analyzer==bars) {
		int step = m_width/20;
		for(register int i=0; i<m_width; i+=step) {
			y = int (m_fftTransformer->GetIntensity(i*2) / 1024);
			if (y<m_specdata[i])
				y = m_specdata[i] - 3;		//fall off
			m_specdata[i] = y;
			rect.left = i;
			rect.top = m_height - y;
			rect.right = i+step-1;
			rect.bottom = m_height;
			memDC->FillRect(&rect, &brush);
		}
	}
	Flip();
	return TRUE;
}