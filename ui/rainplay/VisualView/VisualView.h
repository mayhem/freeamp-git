#ifndef	__VISUALVIEW_H__
#define	__VISUALVIEW_H__

#include "stdafx.h"
#include "active.h"
#include "cthread.h"
#include "BmpSize.h"

typedef enum _tagScopeMode {
	dot,
	line,
	solid
} SCOPEMODE;


typedef enum _tagAnalyzerMode {
	lines,
	bars
} ANALYZERMODE;


typedef struct _tagSample {
	short l;
	short r;
} SAMPLE, *LPSAMPLE;



typedef enum _tagVisMode {
	flip,
	oscilliscope,
	spectrum,
	disable
} VISMODE;


class MemCanvas
{
private:
	CBmpSize *m_bkgBmp;
	CClientDC *dc;
	CDC *memBMDC, *memBMDC1;
	CBitmap *memBM, *memBKGBM, *oldBM, *oldBM1, *oldBM2;
	CWnd *m_pWnd;
	CPen *m_pen, *oldpen;
public:
	MemCanvas(CWnd *pWnd,
				int x, int y, int width, int height,
				CBmpSize *background, COLORREF pencolor);
	~MemCanvas();
protected:
	int m_x,m_y,m_height,m_width;
	CDC *memDC;

	//pure virtual function Draw() in inherited class should call Flip() routie.
	virtual BOOL Render() = 0;
	BOOL Clear(void);
	BOOL Flip(void);
	BOOL Flip(int off);
};


extern COLORREF rgbDisplayColur;
class CWaveformView: public MemCanvas
{
public:
	CWaveformView(CWnd *pWnd, int fps, SCOPEMODE mode,
		int x, int y, int width, int height, CBmpSize *bkgBmp)
	:	m_voffset(height/2),
		m_fps(fps),
		m_scopemode(mode),
		m_hratio(0xFFFF/height),
		MemCanvas(pWnd, x, y, width, height, bkgBmp, rgbDisplayColur) {}
	virtual ~CWaveformView() {}
	void SetFps(int fps) {
		CLock lock(_mutex);
		m_fps = fps;
	}
	void SetBuffer(LPSAMPLE sample, int length) { 
		CLock lock (_mutex);
		m_sample = sample; m_length = length;
	}
	void SetScopemode(SCOPEMODE mode) {
		CLock lock (_mutex);
		m_scopemode = mode;
	}
	virtual BOOL Render();
private:
	CMutex	_mutex;
	SCOPEMODE m_scopemode;
	int m_voffset, m_hratio;
	LPSAMPLE m_sample;
	int m_length;
	int m_fps;
};


#include "Fft.h"
class CSpectrumView: public MemCanvas
{
#define	FFT_POINTS		512
private:
	CMutex	_mutex;
	ANALYZERMODE m_analyzer;
	long	m_samplerate;
	int m_height, m_length, m_points;
	Fft * m_fftTransformer;
	LPSAMPLE m_sample;
	int m_fps;
	int m_specdata[FFT_POINTS];
public:
	CSpectrumView(CWnd *pWnd, int fps, ANALYZERMODE mode, int x, int y, int width, int height, CBmpSize *bkgBmp)
	:	m_height(height),
		m_fps(fps),
		m_points(FFT_POINTS),
		m_samplerate(44100),
		m_analyzer(mode),
		MemCanvas(pWnd, x, y, width, height, bkgBmp, rgbDisplayColur) { 
		m_fftTransformer = new Fft(m_points, m_samplerate);
	}
	virtual ~CSpectrumView() {
		delete m_fftTransformer;
	}
	void SetFps(int fps) {
		CLock lock(_mutex);
		m_fps = fps;
	}
	void SetSampleRate(long samplerate) { 
		if (samplerate==0)
			return;
		CLock lock (_mutex);
		m_samplerate = samplerate;
		delete m_fftTransformer;
		m_fftTransformer = new Fft(m_points, m_samplerate);
	}
	void SetAnalyzerMode(ANALYZERMODE mode) {
		CLock lock (_mutex);
		m_analyzer = mode;
	}
	void SetBuffer(LPSAMPLE sample, int length) {
		CLock lock (_mutex);
		m_sample = sample; m_length = length;
	}
	virtual BOOL Render();
};


#include "math.h"
class CSine : public MemCanvas
{
public:
	CSine(CWnd* pWnd, int x, int y, int width, int height, CBmpSize *bkgBmp)
		:MemCanvas(pWnd, x, y, width+1, height, bkgBmp, rgbDisplayColur) { }
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
	CVisualView(CWnd *pWnd, int fps,
				 VISMODE mode, ANALYZERMODE analyzermode, SCOPEMODE scopemode,
				 UINT x, UINT y, UINT width, UINT height, CBmpSize *bkgBmp);
	virtual ~CVisualView();

	BOOL Start();
	BOOL Stop();
	BOOL SetVisualMode(VISMODE mode) {
		CLock lock(_mutex);
		m_visualmode = mode;
		return TRUE;
	}
	BOOL VisDataReady(unsigned int length, unsigned char *buf);
	BOOL IsStart() { return m_bDisplay; }

	void SetFps(int fps) {
		if (waveview!=NULL)
			waveview->SetFps(fps);
		if (specview!=NULL)
			specview->SetFps(fps);
	}
	void SetScopemode(SCOPEMODE mode) {
		if (waveview!=NULL)
			waveview->SetScopemode(mode);
	}
	void SetAnalyzerMode(ANALYZERMODE mode) {
		if (specview!=NULL)
			specview->SetAnalyzerMode(mode);
	}
	void SetSampleRate(long samplerate) {
		if (specview!=NULL)
			specview->SetSampleRate(samplerate);
	}

private:
    void InitThread () {}
    void Run ();
    void FlushThread ();

	LPSAMPLE m_sample;

	VISMODE m_visualmode;

	BOOL	m_bDisplay;
	unsigned int _length;
	CWaveformView *waveview;
	CSpectrumView *specview;

    CMutex       _mutex;
    CEvent       _event;
	CEvent		 _render;

	CWnd*		m_pWnd;
	UINT		_x,_y,_width,_height;
};


#endif	//__VISUALVIEW_H__