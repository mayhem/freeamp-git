/* *************************************************** */
/* ************ DO NOT EDIT THIS FILE!!!! ************ */
/* *************************************************** */
/* This file was automatically generated by gas2intel. */
/* Edit the original gas version instead. */


/*	FreeAmp - The Free MP3 Player */

/*	Based on MP3 decoder originally Copyright (C) 1995-1997 */
/*	Xing Technology Corp.  http://www.xingtech.com */

/*	Copyright (C) 1999 Mark H. Weaver <mhw@netris.org> */

/*	This program is free software; you can redistribute it and/or modify */
/*	it under the terms of the GNU General Public License as published by */
/*	the Free Software Foundation; either version 2 of the License, or */
/*	(at your option) any later version. */

/*	This program is distributed in the hope that it will be useful, */
/*	but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*	GNU General Public License for more details. */

/*	You should have received a copy of the GNU General Public License */
/*	along with this program; if not, write to the Free Software */
/*	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/*	$Id$ */
/*	Generated from Id: x86gas.s,v 1.7 1999/03/04 07:28:16 mhw Exp $ */


extern float wincoef[264];
extern float coef32[31];

#define L_tmp 0
#define L_pcm 4
void window_dual(float *vbuf, int vb_ptr, short *pcm)
{
__asm {

	mov esi,vb_ptr
	mov edi,vbuf
	mov ecx,pcm
	push ebp
	sub esp,8
	mov DWORD PTR [esp+L_pcm],ecx

	mov ebp,511		; ebp = 511
	lea ecx,wincoef	; coef = wincoef
	add esi,16		; si = vb_ptr + 16
	mov ebx,esi
	add ebx,32
	and ebx,ebp		; bx = (si + 32) & 511

; First 16
	mov dh,16		; i = 16
	align 4
FirstOuter:
	fldz 			; sum = 0.0
	mov dl,2		; j = 2
	align 4
FirstInner:
; REPEAT 4		; Unrolled loop
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,ebp		; bx &= 511
	fsubp st(1),st	; Subtract from sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,ebp		; bx &= 511
	fsubp st(1),st	; Subtract from sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,ebp		; bx &= 511
	fsubp st(1),st	; Subtract from sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,ebp		; bx &= 511
	fsubp st(1),st	; Subtract from sum
;--
; END REPEAT

	dec dl		; --j
	jg FirstInner		; Jump back if j > 0

	fistp DWORD PTR [esp+L_tmp]	; tmp = (long) round (sum)
	inc esi		; si++
	mov eax,DWORD PTR [esp+L_tmp]
	dec ebx		; bx--
	mov ebp,eax
	sar eax,15
	inc eax
	sar eax,1
	jz FirstInRange	; Jump if in range

	sar eax,16		; Out of range
	mov ebp,32767
	xor ebp,eax
FirstInRange:
	mov eax,DWORD PTR [esp+L_pcm]
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+L_pcm],eax

	dec dh		; --i
	jg FirstOuter		; Jump back if i > 0


; Special case
	fldz 			; sum = 0.0
	mov dl,4		; j = 4
	align 4
SpecialInner:
; REPEAT 2		; Unrolled loop
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Increment coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Increment coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
; END REPEAT

	dec dl		; --j
	jg SpecialInner	; Jump back if j > 0

	fistp DWORD PTR [esp+L_tmp]	; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,DWORD PTR [esp+L_tmp]
	inc ebx		; bx++
	mov ebp,eax
	sar eax,15
	inc eax
	sar eax,1
	jz SpecialInRange	; Jump if within range

	sar eax,16		; Out of range
	mov ebp,32767
	xor ebp,eax
SpecialInRange:
	mov eax,DWORD PTR [esp+L_pcm]
	sub ecx,36		; Readjust coef pointer for last round
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+L_pcm],eax


; Last 15
	mov dh,15		; i = 15
	align 4
LastOuter:
	fldz 			; sum = 0.0
	mov dl,2		; j = 2
	align 4
LastInner:
; REPEAT 4		; Unrolled loop
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,ebp		; si &= 511
	faddp st(1),st	; Add to sum

	fld DWORD PTR [ecx]		; Push *coef
	fmul DWORD PTR [edi+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,ebp		; bx &= 511
	faddp st(1),st	; Add to sum
;--
; END REPEAT

	dec dl		; --j
	jg LastInner		; Jump back if j > 0

	fistp DWORD PTR [esp+L_tmp]	; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,DWORD PTR [esp+L_tmp]
	inc ebx		; bx++
	mov ebp,eax
	sar eax,15
	inc eax
	sar eax,1
	jz LastInRange		; Jump if in range

	sar eax,16		; Out of range
	mov ebp,32767
	xor ebp,eax
LastInRange:
	mov eax,DWORD PTR [esp+L_pcm]
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+L_pcm],eax

	dec dh		; --i
	jg LastOuter		; Jump back if i > 0

	add esp,8
	pop ebp

  }
}

