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
	.align 16
.globl window_dual
	.type	 window_dual,@function
window_dual:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	subl $4,%esp

	movl 28(%esp),%esi
	movl $wincoef,%ecx	# coef = wincoef
	addl $16,%esi		# si = vb_ptr + 16
	movl $511,%edi		# edi = 511
	movl %esi,%ebx
	addl $32,%ebx
	movl 24(%esp),%ebp	# Load vbuf
	andl %edi,%ebx		# bx = (si + 32) & 511

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
	fmuls (%ebp,%esi,4)	# Multiply by vbuf[si]
	addl $64,%esi		# si += 64
	addl $4,%ecx		# Advance coef pointer
	andl %edi,%esi		# si &= 511
	faddp			# Add to sum
	
	flds (%ecx)		# Push *coef
	fmuls (%ebp,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	addl $4,%ecx		# Advance coef pointer
	andl %edi,%ebx		# bx &= 511
	fsubrp			# Subtract from sum
.endr

	decb %dl		# --j
	jg .FirstInner		# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	incl %esi		# si++
	movl (%esp),%eax
	decl %ebx		# bx--
	movl %eax,%edi
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .FirstInRange	# Jump if in range

	sarl $16,%eax		# Out of range
	movl $32767,%edi
	xorl %eax,%edi
.FirstInRange:
	movl 32(%esp),%eax
	movw %di,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%edi		# Reload edi with 511
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
	fmuls (%ebp,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	addl $4,%ecx		# Increment coef pointer
	andl %edi,%ebx		# bx &= 511
	faddp			# Add to sum
.endr
	
	decb %dl		# --j
	jg .SpecialInner	# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	decl %esi		# si--
	movl (%esp),%eax
	incl %ebx		# bx++
	movl %eax,%edi
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .SpecialInRange	# Jump if within range

	sarl $16,%eax		# Out of range
	movl $32767,%edi
	xorl %eax,%edi
.SpecialInRange:
	movl 32(%esp),%eax
	subl $36,%ecx		# Readjust coef pointer for last round
	movw %di,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%edi		# Reload edi with 511
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
	fmuls (%ebp,%esi,4)	# Multiply by vbuf[si]
	addl $64,%esi		# si += 64
	subl $4,%ecx		# Back up coef pointer
	andl %edi,%esi		# si &= 511
	faddp			# Add to sum
	
	flds (%ecx)		# Push *coef
	fmuls (%ebp,%ebx,4)	# Multiply by vbuf[bx]
	addl $64,%ebx		# bx += 64
	subl $4,%ecx		# Back up coef pointer
	andl %edi,%ebx		# bx &= 511
	faddp			# Add to sum
.endr

	decb %dl		# --j
	jg .LastInner		# Jump back if j > 0

	fistpl (%esp)		# tmp = (long) round (sum)
	decl %esi		# si--
	movl (%esp),%eax
	incl %ebx		# bx++
	movl %eax,%edi
	sarl $15,%eax
	incl %eax
	sarl $1,%eax
	jz .LastInRange		# Jump if in range

	sarl $16,%eax		# Out of range
	movl $32767,%edi
	xorl %eax,%edi
.LastInRange:
	movl 32(%esp),%eax
	movw %di,(%eax)		# Store sample in *pcm
	addl $4,%eax		# Increment pcm
	movl $511,%edi		# Reload edi with 511
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

.Lfe2:
	.size	 window_dual,.Lfe2-window_dual


	.align 16
.globl fdct32
	.type	 fdct32,@function
fdct32:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	subl $8,%esp
	movl $coef32-128,%ebp	# coef = coef32 - (32 * 4)
	movl $1,4(%esp)		# m = 1
	movl $16,%ecx		# n = 32 / 2

	.align 4
.ForwardOuterLoop:
	movl 4(%esp),%ebx	# ebx = m (temporarily)
	movl 32(%esp),%esi	# esi = f
	movl %ebx,0(%esp)	# mi = m
	movl 28(%esp),%edi	# edi = x
	sall $1,%ebx		# Double m for next iter
	leal (%ebp,%ecx,8),%ebp	# coef += n * 8
	movl %ebx,4(%esp)	# Store doubled m
	movl %esi,28(%esp)	# Exchange mem versions of f/x for next iter
	movl %edi,32(%esp)
	leal (%esi,%ecx,4),%ebx	# ebx = f2 = f + n * 4
	sall $3,%ecx		# n *= 8

	.align 4
.ForwardMiddleLoop:
	movl %ecx,%eax		# q = n
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
	faddp
	fstps (%esi,%edx)	# f[p] = x[p] + x[q]
	fsubp
	fmuls (%ebp,%edx)
	fstps (%ebx,%edx)	# f2[p] = coef[p] * (x[p] - x[q])
	addl $4,%edx		# p += 4

.ForwardInnerLoop1:
	subl $4,%eax		# q -= 4
	flds (%edi,%eax)	# push x[q]
	flds (%edi,%edx)	# push x[p]
	fld %st(1)		# Duplicate top two stack entries
	fld %st(1)
	faddp
	fstps (%esi,%edx)	# f[p] = x[p] + x[q]
	fsubp
	fmuls (%ebp,%edx)
	fstps (%ebx,%edx)	# f2[p] = coef[p] * (x[p] - x[q])
	addl $4,%edx		# p += 4

	cmpl %eax,%edx
	jb .ForwardInnerLoop	# Jump back if (p < q)

	addl %ecx,%esi		# f += n
	addl %ecx,%ebx		# f2 += n
	addl %ecx,%edi		# x += n
	decl 0(%esp)		# mi--
	jg .ForwardMiddleLoop	# Jump back if mi > 0

	sarl $4,%ecx		# n /= 16
	jg .ForwardOuterLoop	# Jump back if n > 0


# Setup back loop
	movl $8,%ebx		# ebx = m = 8 (temporarily)
	movl %ebx,%ecx		# n = 4 * 2

	.align 4
.BackOuterLoop:
	movl 32(%esp),%esi	# esi = f
	movl %ebx,0(%esp)	# mi = m
	movl 28(%esp),%edi	# edi = x
	movl %ebx,4(%esp)	# Store m
	movl %esi,28(%esp)	# Exchange mem versions of f/x for next iter
	movl %edi,%ebx
	movl %edi,32(%esp)
	subl %ecx,%ebx		# ebx = x2 = x - n
	sall $1,%ecx		# n *= 2

	.align 4
.BackMiddleLoop:
	movl -4(%ebx,%ecx),%ebp
	movl %ebp,-8(%esi,%ecx)	# f[n - 8] = x2[n - 4]
	flds -4(%edi,%ecx)	# push x[n - 4]
	fsts -4(%esi,%ecx)	# f[n - 4] = x[n - 4], without popping
	leal -8(%ecx),%eax	# q = n - 8
	leal -16(%ecx),%edx	# p = n - 16

	.align 4
.BackInnerLoop:
	movl (%ebx,%eax),%ebp
	movl %ebp,(%esi,%edx)	# f[p] = x2[q]
	flds (%edi,%eax)	# push x[q]
	fadd %st,%st(1)
	fxch
	fstps 4(%esi,%edx)	# f[p + 4] = x[q] + x[q + 4]
	subl $4,%eax		# q -= 4
	subl $8,%edx		# p -= 8
	jge .BackInnerLoop	# Jump back if p >= 0

	fstps -4(%esp)		# Pop (XXX is there a better way to do this?)
	addl %ecx,%esi		# f += n
	addl %ecx,%ebx		# x2 += n
	addl %ecx,%edi		# x += n
	decl 0(%esp)		# mi--
	jg .BackMiddleLoop	# Jump back if mi > 0

	movl 4(%esp),%ebx	# ebx = m (temporarily)
	sarl $1,%ebx		# Half m for next iter
	jg .BackOuterLoop	# Jump back if m > 0


	addl $8,%esp
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret
.Lfe3:
	.size	 fdct32,.Lfe3-fdct32
