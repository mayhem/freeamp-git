;; ***************************************************
;; ************ DO NOT EDIT THIS FILE!!!! ************
;; ***************************************************
;; This file was automatically generated by gas2intel.
;; Edit the original gas version instead.

;
;	FreeAmp - The Free MP3 Player
;
;	Based on MP3 decoder originally Copyright (C) 1995-1997
;	Xing Technology Corp.  http://www.xingtech.com
;
;	Copyright (C) 1999 Mark H. Weaver <mhw@netris.org>
;
;	This program is free software; you can redistribute it and/or modify
;	it under the terms of the GNU General Public License as published by
;	the Free Software Foundation; either version 2 of the License, or
;	(at your option) any later version.
;
;	This program is distributed in the hope that it will be useful,
;	but WITHOUT ANY WARRANTY; without even the implied warranty of
;	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;	GNU General Public License for more details.
;
;	You should have received a copy of the GNU General Public License
;	along with this program; if not, write to the Free Software
;	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;
;	$Id$
;

extern _wincoef:dword
extern _coef32:dword

.386
;FLAT	group _TEXT
;	assume cs:FLAT, ds:FLAT, ss:FLAT
_TEXT	segment para public use32 'CODE'

public _window_dual
	align 16
_window_dual proc near
	push ebp
	push edi
	push esi
	push ebx
	sub esp,4

	mov esi,DWORD PTR [esp+28]
	mov ecx,_wincoef	; coef = wincoef
	add esi,16		; si = vb_ptr + 16
	mov ebp,511		; ebp = 511
	mov ebx,esi
	add ebx,32
	mov edi,DWORD PTR [esp+24]	; Load vbuf
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
	fsubrp st(1),st	; Subtract from sum
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
	fsubrp st(1),st	; Subtract from sum
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
	fsubrp st(1),st	; Subtract from sum
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
	fsubrp st(1),st	; Subtract from sum
;--
; END REPEAT

	dec dl		; --j
	jg FirstInner		; Jump back if j > 0

	fistp DWORD PTR [esp]		; tmp = (long) round (sum)
	inc esi		; si++
	mov eax,DWORD PTR [esp]
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
	mov eax,DWORD PTR [esp+32]
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+32],eax

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

	fistp DWORD PTR [esp]		; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,DWORD PTR [esp]
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
	mov eax,DWORD PTR [esp+32]
	sub ecx,36		; Readjust coef pointer for last round
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+32],eax


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

	fistp DWORD PTR [esp]		; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,DWORD PTR [esp]
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
	mov eax,DWORD PTR [esp+32]
	mov WORD PTR [eax],bp		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov ebp,511		; Reload ebp with 511
	mov DWORD PTR [esp+32],eax

	dec dh		; --i
	jg LastOuter		; Jump back if i > 0

; Restore regs and return
	add esp,4
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret 
_window_dual endp

public _fdct32
	align 16
_fdct32 proc near
	push ebp
	push edi
	push esi
	push ebx
	sub esp,140
	mov ecx,_coef32-128	; coef = coef32 - (32 * 4)
	mov DWORD PTR [esp+4],1		; m = 1
	mov ebp,16		; n = 32 / 2

	mov edi,DWORD PTR [esp+160]	; edi = x
	mov esi,DWORD PTR [esp+164]	; esi = f
	lea ebx,DWORD PTR [esp+12]
	mov DWORD PTR [esp+164],ebx	; From now on, use temp buf instead of orig x
	jmp ForwardLoopStart

	align 4
ForwardOuterLoop:
	mov edi,DWORD PTR [esp+160]	; edi = x
	mov esi,DWORD PTR [esp+164]	; esi = f
	mov DWORD PTR [esp+164],edi	; Exchange mem versions of f/x for next iter
ForwardLoopStart:
	mov DWORD PTR [esp+160],esi
	mov ebx,DWORD PTR [esp+4]	; ebx = m (temporarily)
	mov DWORD PTR [esp+0],ebx	; mi = m
	sal ebx,1		; Double m for next iter
	lea ecx,DWORD PTR [ecx+ebp*8]	; coef += n * 8
	mov DWORD PTR [esp+4],ebx	; Store doubled m
	lea ebx,DWORD PTR [esi+ebp*4]	; ebx = f2 = f + n * 4
	sal ebp,3		; n *= 8

	align 4
ForwardMiddleLoop:
	mov eax,ebp		; q = n
	xor edx,edx		; p = 0
	test eax,8
	jnz ForwardInnerLoop1

	align 4
ForwardInnerLoop:
	sub eax,4		; q -= 4
	fld DWORD PTR [edi+eax]	; push x[q]
	fld DWORD PTR [edi+edx]	; push x[p]
	fld st(1)		; Duplicate top two stack entries
	fld st(1)
	faddp st(1),st
	fstp DWORD PTR [esi+edx]	; f[p] = x[p] + x[q]
	fsubp st(1),st
	fmul DWORD PTR [ecx+edx]
	fstp DWORD PTR [ebx+edx]	; f2[p] = coef[p] * (x[p] - x[q])
	add edx,4		; p += 4

ForwardInnerLoop1:
	sub eax,4		; q -= 4
	fld DWORD PTR [edi+eax]	; push x[q]
	fld DWORD PTR [edi+edx]	; push x[p]
	fld st(1)		; Duplicate top two stack entries
	fld st(1)
	faddp st(1),st
	fstp DWORD PTR [esi+edx]	; f[p] = x[p] + x[q]
	fsubp st(1),st
	fmul DWORD PTR [ecx+edx]
	fstp DWORD PTR [ebx+edx]	; f2[p] = coef[p] * (x[p] - x[q])
	add edx,4		; p += 4

	cmp edx,eax
	jb ForwardInnerLoop	; Jump back if (p < q)

	add esi,ebp		; f += n
	add ebx,ebp		; f2 += n
	add edi,ebp		; x += n
	dec DWORD PTR [esp+0]		; mi--
	jg ForwardMiddleLoop	; Jump back if mi > 0

	sar ebp,4		; n /= 16
	jg ForwardOuterLoop	; Jump back if n > 0


; Setup back loop
	mov ebx,8		; ebx = m = 8 (temporarily)
	mov ebp,ebx		; n = 4 * 2

	align 4
BackOuterLoop:
	mov esi,DWORD PTR [esp+164]	; esi = f
	mov DWORD PTR [esp+0],ebx	; mi = m
	mov edi,DWORD PTR [esp+160]	; edi = x
	mov DWORD PTR [esp+4],ebx	; Store m
	mov DWORD PTR [esp+160],esi	; Exchange mem versions of f/x for next iter
	mov ebx,edi
	mov DWORD PTR [esp+164],edi
	sub ebx,ebp		; ebx = x2 = x - n
	sal ebp,1		; n *= 2

	align 4
BackMiddleLoop:
	mov ecx,DWORD PTR [ebx+ebp-4]
	mov DWORD PTR [esi+ebp-8],ecx	; f[n - 8] = x2[n - 4]
	fld DWORD PTR [edi+ebp-4]	; push x[n - 4]
	fst DWORD PTR [esi+ebp-4]	; f[n - 4] = x[n - 4], without popping
	lea eax,DWORD PTR [ebp-8]	; q = n - 8
	lea edx,DWORD PTR [ebp-16]	; p = n - 16

	align 4
BackInnerLoop:
	mov ecx,DWORD PTR [ebx+eax]
	mov DWORD PTR [esi+edx],ecx	; f[p] = x2[q]
	fld DWORD PTR [edi+eax]	; push x[q]
	fadd 
	fxch 
	fstp DWORD PTR [esi+edx+4]	; f[p + 4] = x[q] + x[q + 4]
	sub eax,4		; q -= 4
	sub edx,8		; p -= 8
	jge BackInnerLoop	; Jump back if p >= 0

	fstp DWORD PTR [esp+8]		; Pop (XXX is there a better way to do this?)
	add esi,ebp		; f += n
	add ebx,ebp		; x2 += n
	add edi,ebp		; x += n
	dec DWORD PTR [esp+0]		; mi--
	jg BackMiddleLoop	; Jump back if mi > 0

	mov ebx,DWORD PTR [esp+4]	; ebx = m (temporarily)
	sar ebx,1		; Halve m for next iter
	jg BackOuterLoop	; Jump back if m > 0


	add esp,140
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret 
_fdct32 endp

_TEXT	ends
	end
