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

BITS 32
SECTION .text USE32

EXTERN _wincoef
EXTERN _coef32

GLOBAL _window_dual

	; ALIGN 16
_window_dual:
	push ebp
	push edi
	push esi
	push ebx
	sub esp,4

	mov esi,dword ptr [esp+28]
	mov ecx,_wincoef	; coef = wincoef
	add esi,16		; si = vb_ptr + 16
	mov edi,511		; edi = 511
	mov ebx,esi
	add ebx,32
	mov ebp,dword ptr [esp+24]	; Load vbuf
	and ebx,edi		; bx = (si + 32) & 511

; First 16
	mov dh,16		; i = 16
	; ALIGN 4
.FirstOuter:
	fldz			; sum = 0.0
	mov dl,2		; j = 2
	; ALIGN 4
.FirstInner:
; REPEAT 4		; Unrolled loop
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,edi		; bx &= 511
	fsubrp			; Subtract from sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,edi		; bx &= 511
	fsubrp			; Subtract from sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,edi		; bx &= 511
	fsubrp			; Subtract from sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	add ecx,4		; Advance coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Advance coef pointer
	and ebx,edi		; bx &= 511
	fsubrp			; Subtract from sum
;--
; END REPEAT

	dec dl		; --j
	jg .FirstInner		; Jump back if j > 0

	fistp dword ptr [esp]		; tmp = (long) round (sum)
	inc esi		; si++
	mov eax,dword ptr [esp]
	dec ebx		; bx--
	mov edi,eax
	sar eax,15
	inc eax
	sar eax,1
	jz .FirstInRange	; Jump if in range

	sar eax,16		; Out of range
	mov edi,32767
	xor edi,eax
.FirstInRange:
	mov eax,dword ptr [esp+32]
	mov word ptr [eax],di		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov edi,511		; Reload edi with 511
	mov dword ptr [esp+32],eax

	dec dh		; --i
	jg .FirstOuter		; Jump back if i > 0


; Special case
	fldz			; sum = 0.0
	mov dl,4		; j = 4
	; ALIGN 4
.SpecialInner:
; REPEAT 2		; Unrolled loop
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Increment coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	add ecx,4		; Increment coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
; END REPEAT

	dec dl		; --j
	jg .SpecialInner	; Jump back if j > 0

	fistp dword ptr [esp]		; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,dword ptr [esp]
	inc ebx		; bx++
	mov edi,eax
	sar eax,15
	inc eax
	sar eax,1
	jz .SpecialInRange	; Jump if within range

	sar eax,16		; Out of range
	mov edi,32767
	xor edi,eax
.SpecialInRange:
	mov eax,dword ptr [esp+32]
	sub ecx,36		; Readjust coef pointer for last round
	mov word ptr [eax],di		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov edi,511		; Reload edi with 511
	mov dword ptr [esp+32],eax


; Last 15
	mov dh,15		; i = 15
	; ALIGN 4
.LastOuter:
	fldz			; sum = 0.0
	mov dl,2		; j = 2
	; ALIGN 4
.LastInner:
; REPEAT 4		; Unrolled loop
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+esi*4]	; Multiply by vbuf[si]
	add esi,64		; si += 64
	sub ecx,4		; Back up coef pointer
	and esi,edi		; si &= 511
	faddp			; Add to sum

	fld dword ptr [ecx]		; Push *coef
	fmul dword ptr [ebp+ebx*4]	; Multiply by vbuf[bx]
	add ebx,64		; bx += 64
	sub ecx,4		; Back up coef pointer
	and ebx,edi		; bx &= 511
	faddp			; Add to sum
;--
; END REPEAT

	dec dl		; --j
	jg .LastInner		; Jump back if j > 0

	fistp dword ptr [esp]		; tmp = (long) round (sum)
	dec esi		; si--
	mov eax,dword ptr [esp]
	inc ebx		; bx++
	mov edi,eax
	sar eax,15
	inc eax
	sar eax,1
	jz .LastInRange		; Jump if in range

	sar eax,16		; Out of range
	mov edi,32767
	xor edi,eax
.LastInRange:
	mov eax,dword ptr [esp+32]
	mov word ptr [eax],di		; Store sample in *pcm
	add eax,4		; Increment pcm
	mov edi,511		; Reload edi with 511
	mov dword ptr [esp+32],eax

	dec dh		; --i
	jg .LastOuter		; Jump back if i > 0

; Restore regs and return
	add esp,4
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret


GLOBAL _fdct32

	; ALIGN 16
_fdct32:
	push ebp
	push edi
	push esi
	push ebx
	sub esp,8
	mov ebp,_coef32-128	; coef = coef32 - (32 * 4)
	mov dword ptr [esp+4],1		; m = 1
	mov ecx,16		; n = 32 / 2

	; ALIGN 4
.ForwardOuterLoop:
	mov ebx,dword ptr [esp+4]	; ebx = m (temporarily)
	mov esi,dword ptr [esp+32]	; esi = f
	mov dword ptr [esp+0],ebx	; mi = m
	mov edi,dword ptr [esp+28]	; edi = x
	sal ebx,1		; Double m for next iter
	lea ebp,dword ptr [ebp+ecx*8]	; coef += n * 8
	mov dword ptr [esp+4],ebx	; Store doubled m
	mov dword ptr [esp+28],esi	; Exchange mem versions of f/x for next iter
	mov dword ptr [esp+32],edi
	lea ebx,dword ptr [esi+ecx*4]	; ebx = f2 = f + n * 4
	sal ecx,3		; n *= 8

	; ALIGN 4
.ForwardMiddleLoop:
	mov eax,ecx		; q = n
	xor edx,edx		; p = 0
	test eax,8
	jnz .ForwardInnerLoop1

	; ALIGN 4
.ForwardInnerLoop:
	sub eax,4		; q -= 4
	fld dword ptr [edi+eax]	; push x[q]
	fld dword ptr [edi+edx]	; push x[p]
	fld st1		; Duplicate top two stack entries
	fld st1
	faddp
	fstp dword ptr [esi+edx]	; f[p] = x[p] + x[q]
	fsubp
	fmul dword ptr [ebp+edx]
	fstp dword ptr [ebx+edx]	; f2[p] = coef[p] * (x[p] - x[q])
	add edx,4		; p += 4

.ForwardInnerLoop1:
	sub eax,4		; q -= 4
	fld dword ptr [edi+eax]	; push x[q]
	fld dword ptr [edi+edx]	; push x[p]
	fld st1		; Duplicate top two stack entries
	fld st1
	faddp
	fstp dword ptr [esi+edx]	; f[p] = x[p] + x[q]
	fsubp
	fmul dword ptr [ebp+edx]
	fstp dword ptr [ebx+edx]	; f2[p] = coef[p] * (x[p] - x[q])
	add edx,4		; p += 4

	cmp edx,eax
	jb .ForwardInnerLoop	; Jump back if (p < q)

	add esi,ecx		; f += n
	add ebx,ecx		; f2 += n
	add edi,ecx		; x += n
	dec dword ptr [esp+0]		; mi--
	jg .ForwardMiddleLoop	; Jump back if mi > 0

	sar ecx,4		; n /= 16
	jg .ForwardOuterLoop	; Jump back if n > 0


; Setup back loop
	mov ebx,8		; ebx = m = 8 (temporarily)
	mov ecx,ebx		; n = 4 * 2

	; ALIGN 4
.BackOuterLoop:
	mov esi,dword ptr [esp+32]	; esi = f
	mov dword ptr [esp+0],ebx	; mi = m
	mov edi,dword ptr [esp+28]	; edi = x
	mov dword ptr [esp+4],ebx	; Store m
	mov dword ptr [esp+28],esi	; Exchange mem versions of f/x for next iter
	mov ebx,edi
	mov dword ptr [esp+32],edi
	sub ebx,ecx		; ebx = x2 = x - n
	sal ecx,1		; n *= 2

	; ALIGN 4
.BackMiddleLoop:
	mov ebp,dword ptr [ebx-4+ecx]
	mov dword ptr [esi-8+ecx],ebp	; f[n - 8] = x2[n - 4]
	fld dword ptr [edi-4+ecx]	; push x[n - 4]
	fst dword ptr [esi-4+ecx]	; f[n - 4] = x[n - 4], without popping
	lea eax,dword ptr [ecx-8]	; q = n - 8
	lea edx,dword ptr [ecx-16]	; p = n - 16

	; ALIGN 4
.BackInnerLoop:
	mov ebp,dword ptr [ebx+eax]
	mov dword ptr [esi+edx],ebp	; f[p] = x2[q]
	fld dword ptr [edi+eax]	; push x[q]
	fadd st1,st
	fxch
	fstp dword ptr [esi+4+edx]	; f[p + 4] = x[q] + x[q + 4]
	sub eax,4		; q -= 4
	sub edx,8		; p -= 8
	jge .BackInnerLoop	; Jump back if p >= 0

	fstp dword ptr [esp-4]		; Pop (XXX is there a better way to do this?)
	add esi,ecx		; f += n
	add ebx,ecx		; x2 += n
	add edi,ecx		; x += n
	dec dword ptr [esp+0]		; mi--
	jg .BackMiddleLoop	; Jump back if mi > 0

	mov ebx,dword ptr [esp+4]	; ebx = m (temporarily)
	sar ebx,1		; Halve m for next iter
	jg .BackOuterLoop	; Jump back if m > 0


	add esp,8
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
end
