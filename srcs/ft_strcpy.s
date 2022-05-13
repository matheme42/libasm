section .text
	global ft_strcpy
	global _ft_strcpy

_ft_strcpy:
ft_strcpy:	
	mov rcx, -1
	loop:
	inc rcx
	cmp BYTE [rsi + rcx], 0
	jne loop
	inc rcx
	rep movsb
	ret
