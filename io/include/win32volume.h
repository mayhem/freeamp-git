#include <windows.h>
#include <stdlib.h>

#include "config.h"
#include "utility.h"

class Win32Volume
{

	public:
        enum eDeviceType { eWaveOut = 0, eCDOut };
		static Win32Volume* GetInstance( eDeviceType, HWND  );

		void GetVolume(int32 &left, int32 &right);
		void SetVolume(int32 left, int32 right);


		static Win32Volume* m_Instance;

        

	protected:

	private:
		Win32Volume( eDeviceType, HWND );
		
		bool InitVolumeControl( eDeviceType, HWND );

		UINT m_nNumMixers;
		HMIXER m_hMixer;
		MIXERCAPS m_mxcaps;

        MIXERLINE mxl;
        MIXERCONTROL mxc;
        MIXERLINECONTROLS mxlc;

		string m_oDstLineName, m_oVolumeControlName;
		DWORD m_dwMinimum, m_dwMaximum;
		DWORD m_dwVolumeControlID;

};