/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

#include <iostream.h>
#include <stdio.h>

#include "player.h"
#include "event.h"
#include "ui.h"
#include "vector.h"
#include "hashtable.h"
#include "semaphore.h"
#include "registrar.h"


void testVector();
void testHashTable();

#if 0
bool CompareName(const char *p1, const char *p2) {
    //cout << "Comparing " << p1 << " to " << p2 << endl;
    int32 i=0; int32 j=0;
    if(!p1 ||!p2) return false;
    while ((p1[i] == p2[j]) && p1[i] && p2[j]) {
	i++; j++;
    }
    if (!p1[i] && (p2[j]=='-')) {
	return true;
    } else {
	return false;
    }
}
#endif
int main(int argc, char **argv) {
    //testVector();
    //testBuffer();
    //testHashTable();

    Preferences *prefs = new Preferences();
    Registrar *registrar= new Registrar();
    LMCRegistry *lmc;
    PMIRegistry *pmi;
    PMORegistry *pmo;
    UIRegistry* ui;
    
//    registrar->SetSubDir("lmc");
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    lmc = new LMCRegistry();
    registrar->InitializeRegistry(lmc,prefs);

//    registrar->SetSubDir("io");
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.pmi");
    pmi = new PMIRegistry;
    registrar->InitializeRegistry(pmi,prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new PMORegistry;
    registrar->InitializeRegistry(pmo,prefs);


//    registrar->SetSubDir("ui");
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.ui");
    ui = new UIRegistry;
    registrar->InitializeRegistry(ui,prefs);

    delete registrar;


    Semaphore *termSemaphore;
    termSemaphore = new Semaphore();
    Player *pP = Player::GetPlayer();

    pP->RegisterLMCs(lmc);
    pP->RegisterPMIs(pmi);
    pP->RegisterPMOs(pmo);
    pP->RegisterUIs(ui);




    if (pP->SetArgs(argc,argv)) {
	pP->SetPreferences(prefs);
	pP->SetTerminationSemaphore(termSemaphore);
	pP->Run();
	
	termSemaphore->Wait();
    }
    delete pP;
    return 0;
}

#if 0
void testVector() {
    cout << "Beginning testVector..." << endl;
    Vector<char *> *pVect = new Vector<char *>(2);
    char *p1 = "Hey man!";
    char *p2 = "Yo dude!";
    char *p3 = "And, the third.";
    pVect->Insert(p1);
    pVect->Insert(p2);
    pVect->Insert(p3);
    
    cout << pVect->ElementAt(0) << endl;
    cout << pVect->ElementAt(1) << endl;
    cout << pVect->ElementAt(2) << endl;

    pVect->RemoveElementAt(1);

    cout << pVect->ElementAt(0) << endl;
    cout << pVect->ElementAt(1) << endl;

    pVect->RemoveAll();

    if (pVect->ElementAt(0) != NULL) {
	cout << "Final Test failed!!" << endl;
    }

    Vector<DummyUI *> *pVect2 = new Vector<DummyUI *>(2);
    DummyUI *pd = new DummyUI(NULL);
    pVect2->Insert(pd);
    pd = new DummyUI(NULL);
    pVect2->Insert(pd);
    pd = new DummyUI(NULL);
    pVect2->Insert(pd);

    pVect2->DeleteAll();
    if (pVect2->ElementAt(0) != NULL) {
	cout << "Final Test failed!! (2nd one)" << endl;
   }
    cout << "Ending testVector..." << endl;
}
#endif

void testHashTable() {
    HashTable<int32 *> *pHT = new HashTable<int32 *>();
    cout << "Adding 'foo'->1" << endl;
    char *foo = "foo";
    char *bar = "bar";
    int32 *bb = new int32;
    *bb = 1;
    pHT->Insert(foo,bb);
    cout << "Adding 'bar'->2" << endl;
    int32 *bc = new int32;
    *bc = 2;
    pHT->Insert(bar,bc);

    int32 *out = pHT->Value(foo);
    cout << "out 1:  " << *out << endl;
    out = pHT->Value(bar);
    cout << "out 2:  " << *out << endl;
    exit(1);
}


