#	
#	FreeAmp - The Free MP3 Player
#
#	Based on MP3 decoder originally Copyright (C) 1995-1997
#	Xing Technology Corp.  http://www.xingtech.com
#
#	Copyright (C) 1999 Mark H. Weaver <mhw@netris.org>
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program; if not, write to the Free Software
#	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#	
#	$Id$
#

#%% extern wincoef,dword
#%% extern coef32,dword

.globl window_dual

	.align 16
window_dual:	#%% proc
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	subl $4,%esp

	movl 28(%esp),%esi
	movl $wincoef,%ecx	# coef = wincoef
	addl $16,%esi		# si = vb_ptr + 16
	movl $511,%ebp		# ebp = 511
	movl %esi,%ebx
	addl $32,%ebx
	movl 24(%esp),%edi	# Load vbuf
	andl %ebp,%ebx		# bx = (si + 32) & 511

# First 16
	movb $16,%dh		# i = 16
	.align 4
.FirstOuter:
	fldz			# sum = 0.0
	movb $2,%dl		# j = 2
	.align 4
.FirstInner:
.rept 4		# Unrolled loop
	flds (%ecx)		# Push *coef
	fmuls (%edi,%esi,4)	# Multiply by vbuf[si]
	addl $64,%esi		# si += 64
	addl $4,%ecx		# Advance coef pointer
	andl %ebp,%esi		# si &= 511
	faddp %st,%st(1)	# Add to sum
	
	flds (%ecx)		# Push *coef
	fmuls (%edi,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	addl $4,%ecx		# Advance coef pointer
	andl %ebp,%ebx		# bx &= 511
	fsubrp %st,%st(1)	# Subtract from sum
.endr

	decb %dl		# --j
	jg .FirstInner		# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	incl %esi		# si++
	movl (%esp),%eax
	decl %ebx		# bx--
	movl %eax,%ebp
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .FirstInRange	# Jump if in range

	sarl $16,%eax		# Out of range
	movl $32767,%ebp
	xorl %eax,%ebp
.FirstInRange:
	movl 32(%esp),%eax
	movw %bp,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%ebp		# Reload ebp with 511
	movl %eax,32(%esp)

	decb %dh		# --i
	jg .FirstOuter		# Jump back if i > 0


# Special case
	fldz			# sum = 0.0
	movb $4,%dl		# j = 4
	.align 4
.SpecialInner:
.rept 2		# Unrolled loop
	flds (%ecx)		# Push *coef
	fmuls (%edi,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	addl $4,%ecx		# Increment coef pointer
	andl %ebp,%ebx		# bx &= 511
	faddp %st,%st(1)	# Add to sum
.endr
	
	decb %dl		# --j
	jg .SpecialInner	# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	decl %esi		# si--
	movl (%esp),%eax
	incl %ebx		# bx++
	movl %eax,%ebp
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .SpecialInRange	# Jump if within range

	sarl $16,%eax		# Out of range
	movl $32767,%ebp
	xorl %eax,%ebp
.SpecialInRange:
	movl 32(%esp),%eax
	subl $36,%ecx		# Readjust coef pointer for last round
	movw %bp,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%ebp		# Reload ebp with 511
	movl %eax,32(%esp)


# Last 15
	movb $15,%dh		# i = 15
	.align 4
.LastOuter:
	fldz			# sum = 0.0
	movb $2,%dl		# j = 2
	.align 4
.LastInner:
.rept 4		# Unrolled loop
	flds (%ecx)		# Push *coef
	fmuls (%edi,%esi,4)	# Multiply by vbuf[si]
	addl $64,%esi		# si += 64
	subl $4,%ecx		# Back up coef pointer
	andl %ebp,%esi		# si &= 511
	faddp %st,%st(1)	# Add to sum
	
	flds (%ecx)		# Push *coef
	fmuls (%edi,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	subl $4,%ecx		# Back up coef pointer
	andl %ebp,%ebx		# bx &= 511
	faddp %st,%st(1)	# Add to sum
.endr

	decb %dl		# --j
	jg .LastInner		# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	decl %esi		# si--
	movl (%esp),%eax
	incl %ebx		# bx++
	movl %eax,%ebp
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .LastInRange		# Jump if in range

	sarl $16,%eax		# Out of range
	movl $32767,%ebp
	xorl %eax,%ebp
.LastInRange:
	movl 32(%esp),%eax
	movw %bp,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%ebp		# Reload ebp with 511
	movl %eax,32(%esp)

	decb %dh		# --i
	jg .LastOuter		# Jump back if i > 0

# Restore regs and return
	addl $4,%esp	
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret
#%% endp

.globl fdct32

	.align 16
fdct32:		#%% proc
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	subl $8,%esp
	movl $coef32-128,%ecx	# coef = coef32 - (32 * 4)
	movl $1,4(%esp)		# m = 1
	movl $16,%ebp		# n = 32 / 2

	.align 4
.ForwardOuterLoop:
	movl 4(%esp),%ebx	# ebx = m (temporarily)
	movl 32(%esp),%esi	# esi = f
	movl %ebx,0(%esp)	# mi = m
	movl 28(%esp),%edi	# edi = x
	sall $1,%ebx		# Double m for next iter
	leal (%ecx,%ebp,8),%ecx	# coef += n * 8
	movl %ebx,4(%esp)	# Store doubled m
	movl %esi,28(%esp)	# Exchange mem versions of f/x for next iter
	movl %edi,32(%esp)
	leal (%esi,%ebp,4),%ebx	# ebx = f2 = f + n * 4
	sall $3,%ebp		# n *= 8

	.align 4
.ForwardMiddleLoop:
	movl %ebp,%eax		# q = n
	xorl %edx,%edx		# p = 0
	test $8,%eax
	jnz .ForwardInnerLoop1

	.align 4
.ForwardInnerLoop:
	subl $4,%eax		# q -= 4
	flds (%edi,%eax)	# push x[q]
	flds (%edi,%edx)	# push x[p]
	fld %st(1)		# Duplicate top two stack entries
	fld %st(1)
	faddp %st,%st(1)
	fstps (%esi,%edx)	# f[p] = x[p] + x[q]
	fsubp %st,%st(1)
	fmuls (%ecx,%edx)
	fstps (%ebx,%edx)	# f2[p] = coef[p] * (x[p] - x[q])
	addl $4,%edx		# p += 4

.ForwardInnerLoop1:
	subl $4,%eax		# q -= 4
	flds (%edi,%eax)	# push x[q]
	flds (%edi,%edx)	# push x[p]
	fld %st(1)		# Duplicate top two stack entries
	fld %st(1)
	faddp %st,%st(1)
	fstps (%esi,%edx)	# f[p] = x[p] + x[q]
	fsubp %st,%st(1)
	fmuls (%ecx,%edx)
	fstps (%ebx,%edx)	# f2[p] = coef[p] * (x[p] - x[q])
	addl $4,%edx		# p += 4

	cmpl %eax,%edx
	jb .ForwardInnerLoop	# Jump back if (p < q)

	addl %ebp,%esi		# f += n
	addl %ebp,%ebx		# f2 += n
	addl %ebp,%edi		# x += n
	decl 0(%esp)		# mi--
	jg .ForwardMiddleLoop	# Jump back if mi > 0

	sarl $4,%ebp		# n /= 16
	jg .ForwardOuterLoop	# Jump back if n > 0


# Setup back loop
	movl $8,%ebx		# ebx = m = 8 (temporarily)
	movl %ebx,%ebp		# n = 4 * 2

	.align 4
.BackOuterLoop:
	movl 32(%esp),%esi	# esi = f
	movl %ebx,0(%esp)	# mi = m
	movl 28(%esp),%edi	# edi = x
	movl %ebx,4(%esp)	# Store m
	movl %esi,28(%esp)	# Exchange mem versions of f/x for next iter
	movl %edi,%ebx
	movl %edi,32(%esp)
	subl %ebp,%ebx		# ebx = x2 = x - n
	sall $1,%ebp		# n *= 2

	.align 4
.BackMiddleLoop:
	movl -4(%ebx,%ebp),%ecx
	movl %ecx,-8(%esi,%ebp)	# f[n - 8] = x2[n - 4]
	flds -4(%edi,%ebp)	# push x[n - 4]
	fsts -4(%esi,%ebp)	# f[n - 4] = x[n - 4], without popping
	leal -8(%ebp),%eax	# q = n - 8
	leal -16(%ebp),%edx	# p = n - 16

	.align 4
.BackInnerLoop:
	movl (%ebx,%eax),%ecx
	movl %ecx,(%esi,%edx)	# f[p] = x2[q]
	flds (%edi,%eax)	# push x[q]
	fadd
	fxch
	fstps 4(%esi,%edx)	# f[p + 4] = x[q] + x[q + 4]
	subl $4,%eax		# q -= 4
	subl $8,%edx		# p -= 8
	jge .BackInnerLoop	# Jump back if p >= 0

	fstps -4(%esp)		# Pop (XXX is there a better way to do this?)
	addl %ebp,%esi		# f += n
	addl %ebp,%ebx		# x2 += n
	addl %ebp,%edi		# x += n
	decl 0(%esp)		# mi--
	jg .BackMiddleLoop	# Jump back if mi > 0

	movl 4(%esp),%ebx	# ebx = m (temporarily)
	sarl $1,%ebx		# Halve m for next iter
	jg .BackOuterLoop	# Jump back if m > 0


	addl $8,%esp
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret
#%% endp

