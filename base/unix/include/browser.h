/* (PD) 2001 The Bitzi Corporation
 * Please see file COPYING or http://bitzi.com/publicdomain 
 * for more info.
 *
 * $Id$
 */
#ifndef BROWSER_H
#define BROWSER_H

typedef enum
{
   eBrowserNetscape = 0,
   eBrowserMozilla,
   eBrowserKonqueror,
   eBrowserOpera,
   eBrowserLynx
} BrowserEnum;

void launch_browser(const char* url, BrowserEnum browser);

#endif
