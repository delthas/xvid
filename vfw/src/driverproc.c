/**************************************************************************
 *
 *	XVID VFW FRONTEND
 *	driverproc main
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *************************************************************************/

/**************************************************************************
 *
 *	History:
 *
 *	... ???
 *	01.12.2001	inital version; (c)2001 peter ross <suxen_drol@hotmail.com>
 *
 *************************************************************************/

#include <windows.h>
#include <vfw.h>

#include "codec.h"
#include "config.h"
#include "resource.h"


BOOL APIENTRY DllMain(
	HANDLE hModule, 
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved)
{
	hInst = (HINSTANCE) hModule;
    return TRUE;
}



__declspec(dllexport) LRESULT WINAPI DriverProc(
	DWORD dwDriverId, 
	HDRVR hDriver, 
	UINT uMsg, 
	LPARAM lParam1, 
	LPARAM lParam2) 
{
	CODEC * codec = (CODEC *)dwDriverId;
	CONFIG temp;

	switch(uMsg)
	{

	/* driver primitives */

	case DRV_LOAD :
	case DRV_FREE :
		DRV_OK;

	case DRV_OPEN :
		DEBUG("DRV_OPEN");
		{
			ICOPEN * icopen = (ICOPEN *)lParam2;
			
			if (icopen != NULL && icopen->fccType != ICTYPE_VIDEO) {
				return DRV_CANCEL;
			}
			codec = malloc(sizeof(CODEC));
			if (codec == NULL)
			{
				if (icopen != NULL) {
					icopen->dwError = ICERR_MEMORY;
				}
				return 0;
			}
			codec->ehandle = codec->dhandle = NULL;
			config_reg_get(&codec->config);

			if (icopen != NULL) {
				icopen->dwError = ICERR_OK;
			}
			return (LRESULT)codec;
		}

	case DRV_CLOSE :
		DEBUG("DRV_CLOSE");
		// compress_end/decompress_end don't always get called
		compress_end(codec);
		decompress_end(codec);
		free(codec);
		return DRV_OK;

	case DRV_DISABLE :
	case DRV_ENABLE :
		return DRV_OK;

	case DRV_INSTALL :
	case DRV_REMOVE :
		return DRV_OK;

	case DRV_QUERYCONFIGURE :
	case DRV_CONFIGURE :
		return DRV_CANCEL;


	/* info */

	case ICM_GETINFO :
		DEBUG("ICM_GETINFO");
		{
			ICINFO *icinfo = (ICINFO *)lParam1;

			icinfo->fccType = ICTYPE_VIDEO;
			icinfo->fccHandler = FOURCC_XVID;
			icinfo->dwFlags =
				VIDCF_FASTTEMPORALC |
				VIDCF_FASTTEMPORALD |
				VIDCF_COMPRESSFRAMES;

			icinfo->dwVersion = 0;
			icinfo->dwVersionICM = ICVERSION;
			
			wcscpy(icinfo->szName, XVID_NAME_L); 
			wcscpy(icinfo->szDescription, XVID_DESC_L);
						
			return lParam2; /* size of struct */
		}

		
		/* state control */

	case ICM_ABOUT :
	case ICM_CONFIGURE :

		DEBUG("ICM_CONFIGURE");

		codec->config.save = FALSE;
		memcpy(&temp, &codec->config, sizeof(CONFIG));

		if (lParam1 != -1) {
			PROPSHEETINFO psi[DLG_COUNT];
			PROPSHEETPAGE psp[DLG_COUNT];
			PROPSHEETHEADER psh;
	
			psp[DLG_MAIN].dwSize = sizeof(PROPSHEETPAGE);
			psp[DLG_MAIN].dwFlags = 0;
			psp[DLG_MAIN].hInstance = hInst;
			psp[DLG_MAIN].pszTemplate = MAKEINTRESOURCE(IDD_MAIN);
			psp[DLG_MAIN].pfnDlgProc = config_proc;
				psi[DLG_MAIN].page = DLG_MAIN;
				psi[DLG_MAIN].config = &temp;
			psp[DLG_MAIN].lParam = (LPARAM)&psi[DLG_MAIN];
			psp[DLG_MAIN].pfnCallback = NULL;

			psp[DLG_ADV].dwSize = sizeof(PROPSHEETPAGE);
			psp[DLG_ADV].dwFlags = 0;
			psp[DLG_ADV].hInstance = hInst;
			psp[DLG_ADV].pszTemplate = MAKEINTRESOURCE(IDD_ADV);
			psp[DLG_ADV].pfnDlgProc = config_proc;
				psi[DLG_ADV].page = DLG_ADV;
				psi[DLG_ADV].config = &temp;
			psp[DLG_ADV].lParam = (LPARAM)&psi[DLG_ADV];
			psp[DLG_ADV].pfnCallback = NULL;

			psp[DLG_DEBUG].dwSize = sizeof(PROPSHEETPAGE);
			psp[DLG_DEBUG].dwFlags = 0;
			psp[DLG_DEBUG].hInstance = hInst;
			psp[DLG_DEBUG].pszTemplate = MAKEINTRESOURCE(IDD_DEBUG);
			psp[DLG_DEBUG].pfnDlgProc = config_proc;
				psi[DLG_DEBUG].page = DLG_DEBUG;
				psi[DLG_DEBUG].config = &temp;
			psp[DLG_DEBUG].lParam = (LPARAM)&psi[DLG_DEBUG];
			psp[DLG_DEBUG].pfnCallback = NULL;

			psp[DLG_CPU].dwSize = sizeof(PROPSHEETPAGE);
			psp[DLG_CPU].dwFlags = 0;
			psp[DLG_CPU].hInstance = hInst;
			psp[DLG_CPU].pszTemplate = MAKEINTRESOURCE(IDD_CPU);
			psp[DLG_CPU].pfnDlgProc = config_proc;
				psi[DLG_CPU].page = DLG_CPU;
				psi[DLG_CPU].config = &temp;
			psp[DLG_CPU].lParam = (LPARAM)&psi[DLG_CPU];
			psp[DLG_CPU].pfnCallback = NULL;

			psp[DLG_ABOUT].dwSize = sizeof(PROPSHEETPAGE);
			psp[DLG_ABOUT].dwFlags = 0;
			psp[DLG_ABOUT].hInstance = hInst;
			psp[DLG_ABOUT].pszTemplate = MAKEINTRESOURCE(IDD_ABOUT);
			psp[DLG_ABOUT].pfnDlgProc = config_proc;
				psi[DLG_ABOUT].page = DLG_ABOUT;
				psi[DLG_ABOUT].config = &temp;
			psp[DLG_ABOUT].lParam = (LPARAM)&psi[DLG_ABOUT];
			psp[DLG_ABOUT].pfnCallback = NULL;

			psh.dwSize = sizeof(PROPSHEETHEADER);
			psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
			psh.hwndParent = (HWND)lParam1;
			psh.hInstance = hInst;
			psh.pszCaption = (LPSTR) "XviD Configuration";
			psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
			psh.nStartPage = (uMsg == ICM_CONFIGURE ? DLG_MAIN : DLG_ABOUT);
			psh.ppsp = (LPCPROPSHEETPAGE)&psp;
			psh.pfnCallback = NULL;

			PropertySheet(&psh);

			if(temp.save) {
				memcpy(&codec->config, &temp, sizeof(CONFIG));
			}
		}
		return ICERR_OK;
			
	case ICM_GETSTATE :
		DEBUG("ICM_GETSTATE");
		if ((void*)lParam1 == NULL) {
			return sizeof(CONFIG);
		}
		memcpy((void*)lParam1, &codec->config, sizeof(CONFIG));
		return ICERR_OK;

	case ICM_SETSTATE :
		DEBUG("ICM_SETSTATE");
		if ((void*)lParam1 == NULL) {
			DEBUG("ICM_SETSTATE : DEFAULT");
			config_reg_get(&codec->config);
			return 0;
		}
		memcpy(&codec->config,(void*)lParam1, sizeof(CONFIG));
		return 0; // sizeof(CONFIG);

	/* not sure the difference, private/public data? */

	case ICM_GET :
	case ICM_SET :
		return ICERR_OK;


	/* older-stype config */

	case ICM_GETDEFAULTQUALITY :
	case ICM_GETQUALITY :
	case ICM_SETQUALITY :
	case ICM_GETBUFFERSWANTED :
	case ICM_GETDEFAULTKEYFRAMERATE :
		return ICERR_UNSUPPORTED;


	/* compressor */

	case ICM_COMPRESS_QUERY :
		DEBUG("ICM_COMPRESS_QUERY");
		return compress_query(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_COMPRESS_GET_FORMAT :
		DEBUG("ICM_COMPRESS_GET_FORMAT");
		return compress_get_format(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_COMPRESS_GET_SIZE :
		DEBUG("ICM_COMPRESS_GET_SIZE");
		return compress_get_size(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_COMPRESS_FRAMES_INFO :
		DEBUG("ICM_COMPRESS_FRAMES_INFO");
		return compress_frames_info(codec, (ICCOMPRESSFRAMES *)lParam1);

	case ICM_COMPRESS_BEGIN :
		DEBUG("ICM_COMPRESS_BEGIN");
		return compress_begin(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_COMPRESS_END :
		DEBUG("ICM_COMPRESS_END");
		return compress_end(codec);

	case ICM_COMPRESS :
		DEBUG("ICM_COMPRESS");
		return compress(codec, (ICCOMPRESS *)lParam1);

	/* decompressor */
	
	case ICM_DECOMPRESS_QUERY :
		DEBUG("ICM_DECOMPRESS_QUERY");
		return decompress_query(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_DECOMPRESS_GET_FORMAT :
		DEBUG("ICM_DECOMPRESS_GET_FORMAT");
		return decompress_get_format(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);
	
	case ICM_DECOMPRESS_BEGIN :
		DEBUG("ICM_DECOMPRESS_BEGIN");
		return decompress_begin(codec, (BITMAPINFO *)lParam1, (BITMAPINFO *)lParam2);

	case ICM_DECOMPRESS_END :
		DEBUG("ICM_DECOMPRESS_END");
		return decompress_end(codec);

	case ICM_DECOMPRESS :
		DEBUG("ICM_DECOMPRESS");
		return decompress(codec, (ICDECOMPRESS *)lParam1);

	case ICM_DECOMPRESS_GET_PALETTE :
	case ICM_DECOMPRESS_SET_PALETTE :
	case ICM_DECOMPRESSEX_QUERY:
	case ICM_DECOMPRESSEX_BEGIN:
	case ICM_DECOMPRESSEX_END:
	case ICM_DECOMPRESSEX:
		return ICERR_UNSUPPORTED;

	default:
		return DefDriverProc(dwDriverId, hDriver, uMsg, lParam1, lParam2);
	}
}
