#ifndef	__VISUALVIEW_H__
#define	__VISUALVIEW_H__

#include "stdafx.h"
#include "active.h"
#include "cthread.h"

typedef enum _tagScopeMode {
	dot,
	line,
	solid
} SCOPEMODE;


typedef struct _tagSample {
	short l;
	short r;
} SAMPLE, *LPSAMPLE;

class MemCanvas
{
public:
	MemCanvas(CWnd *pWnd,  int x, int y, int width, int height, COLORREF brushRGB)
	:	m_x(x),
		m_y(y),
		m_height(height),
		m_width(width),
		m_pWnd(pWnd),
		m_brushRGB(brushRGB)
	{
		dc = new CClientDC(m_pWnd);
		memDC = new CDC();
		memBM = new CBitmap();
		solidBrush = new CBrush(m_brushRGB);

		memDC->CreateCompatibleDC(dc);
		memBM->CreateCompatibleBitmap(dc, m_width, m_height);
		oldBM = memDC->SelectObject(memBM);
		Clear();
	}
	~MemCanvas()
	{
		memDC->SelectObject(oldBM);
		delete solidBrush;
		delete memBM;
		delete memDC;
		delete dc;
	}
protected:
	int m_x,m_y,m_height,m_width;
	CDC *memDC;

	//pure virtual function Draw() in inherited class should call Flip() routie.
	virtual BOOL Render() = 0;
	BOOL Clear()
	{
		//Fill background with solib brush
		CRect rect(0, 0, m_width, m_height);
		memDC->FillRect(LPRECT(rect), solidBrush);
		return TRUE;
	}
	BOOL Flip(void)
	{
		//Flip Surface:Backsurface -> Frontsurface
		return(
		dc->BitBlt(	m_x, m_y, m_width, m_height,
					memDC,
					0,0,
					SRCCOPY)
		);
	}
	BOOL Flip(int off)
	{
		//Flip Surface:Backsurface -> Frontsurface
		return(
		dc->BitBlt(	m_x, m_y, m_width+off, m_height,
					memDC,
					0,0,
					SRCCOPY)
		);
	}
private:
	CClientDC *dc;
	CBitmap *memBM, *oldBM;
	CBrush *solidBrush;
	CWnd *m_pWnd;
	COLORREF m_brushRGB;
};

class CWaveformView: public MemCanvas
{
public:
	CWaveformView(CWnd *pWnd, int x, int y, int width, int height)
	:	m_voffset(height/2),
		m_scopemode(dot),
		m_hratio(0xFFFF/height),
		MemCanvas(pWnd, x, y, width, height, RGB(13,106,134)) {}
	virtual ~CWaveformView() {}
	void SetBuffer(LPSAMPLE sample, int length) { m_sample = sample; m_length = length; }
	void SetScopemode(SCOPEMODE mode) { m_scopemode = mode; }
	virtual BOOL Render();
private:
	SCOPEMODE m_scopemode;
	int m_voffset, m_hratio;
	LPSAMPLE m_sample;
	int m_length;
};

#include "math.h"

class CSine : public MemCanvas
{
public:
	CSine(CWnd* pWnd, int x, int y, int width, int height)
		:MemCanvas(pWnd, x, y, width+1, height, RGB(13,106,134)) { }
	virtual ~CSine() { }

	virtual BOOL Render()
	{
		static double counter=0;
		double delta;

		//Left shift curve 1 bit
		memDC->BitBlt(0,0,m_width-1,m_height,
					memDC,
					1,0,SRCCOPY);
#if 0
		delta = sin(((counter++)/((double)(m_width-1)/2.0))*6.28)*(double)m_height/2.0;
#else
		delta = sin((counter++)/((double)(m_width-1))*12.5664)*(double)m_height/2.0;
#endif
		memDC->SetPixel(m_width-2,(int)(m_height/2+delta), RGB(0,0,0));
		
		Flip(-1);
		return TRUE;
	}
};


/////////////////////////////////////////////////////////////////////////////
// CVisualView class

class CVisualView : public ActiveObject
{
// Construction
public:
	// standard constructor
	CVisualView(CWnd *pWnd, UINT x, UINT y, UINT width, UINT height);
	virtual ~CVisualView();

	BOOL Start();
	BOOL Stop();
	BOOL VisDataReady(unsigned int length, unsigned char *buf);
	BOOL IsStart() { return m_bDisplay; }

	void SetScopemode(SCOPEMODE mode) {
		if (waveview!=NULL)
			waveview->SetScopemode(mode);
	}

#if 0
	typedef struct _tagRingBuf {
		long len;
		bool mutex;
		unsigned char *pbuf;
	} RINGBUF, *LPRINGBUF;

	LPRINGBUF	m_ringbuf;
#endif

private:
    void InitThread () {}
    void Run ();
    void FlushThread ();

	LPSAMPLE m_sample;

	BOOL	m_bDisplay;
	unsigned int _length;
	CWaveformView *waveview;

    CMutex       _mutex;
    CEvent       _event;
	CEvent		 _render;

	CWnd*		m_pWnd;
	UINT		_x,_y,_width,_height;
};


#endif	//__VISUALVIEW_H__