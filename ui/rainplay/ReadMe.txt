Jan/17/1999
elrod@gold.liquidmetal.com,jdw@freeamp.org
freeamp-dev@freeamp.org
Dear Elrod,Jason:
Here I attached the lasted version of rainplay update.
Please update this to freeamp-snap.*.

Rainplay current supporting:
[new]:
1. oscilliscope display
about 40 fps on my PII 300 compaq notebook
which work well with fusion skin.
2. scope mode dot/line/solid
3. fix some bugs.
4. seek bar
5. full rclick menu support
6. to find out the change other than rainplay ui files, just search key work.
'#define _VISUAL_ENABLE_'

base\include\event.h
base\include\eventdata.h
base\src\player.cpp
lmc\xingmp3\src\xinglmc.cpp
[TODO]
1.add volume change function
2.spectrum
3.CVisualView should has it's own buffer. That means
once the PCM being copy into the CVisualView, the SendVisBufEvent
should be delete at once(reduce the load of Event queue), then
another thread render the waveform display sync with the xinglmc's
work.
4.user can change fps.


Jan/13/1999
elrod@gold.liquidmetal.com,jdw@freeamp.org
freeamp-dev@freeamp.org

Dear Elrod,Jason:
Here I attached the lasted version of rainplay update.
Please update this to freeamp-snap.*.

Rainplay current supporting:
[new]:
1. Sine wave display when haven't buffing data.

2.playlist/Equalizer modueless dialog.You must start playing
songs 1st, then open the Equalizer dialog(P:-))

3.fix some bugs

4.simple EQ function works(with some modify on files below)
to find out this modify, just search key work.
'#define _EQUALIZER_ENABLE_'
base\include\event.h
base\include\eventdata.h
base\src\player.cpp

lmc\include\lmc.h
lmc\xingmp3\include\xinglmc.h
lmc\xingmp3\src\cdct.c
lmc\xingmp3\src\xinglmc.cpp

5.the EQ band's value is learn from XAUDIO 3.07 sdk doc, I simply
modify the 32 sub-bands's factor.It current only support layer3.
