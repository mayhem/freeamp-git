/***************************************************************************
                          slcient.h  -  description
                             -------------------
    begin                : Wed Sep 6 2000
    copyright            : (C) 2000 by Relatable 
    programmed by        : Isaac Richards
    email                : ijr@relatable.com
 ***************************************************************************/

#ifndef SOUNDSLIKECLIENT
#define SOUNDSLIKECLIENT

#include <iostream>
#include <string>
#include <vector>
// Any other includes go here

using namespace std;

class COMSocket;
class Mutex;
// Any other class declares for function params go here

/** Client Stub to manipulate collection sets */
class SoundsLikeClient
{
public:
	SoundsLikeClient();
       ~SoundsLikeClient();
	void SetAddress(string strIP, int nPort) { m_strIP = strIP; m_nPort = nPort; }
	vector<pair<string, float> > *SoundsLike(vector<string> *pSeedGUIDs, 
			                         vector<string> *pCollGUIDs,
						 int nNumClosest, 
						 float fMaxDistance = -1.0);

protected:
	int Connect(string& strIP, int nPort);
	int Disconnect();

private:
	COMSocket* m_pSocket;
	Mutex*	m_pMutex;
	string	m_strIP;
	int	m_nPort;
};

#endif
