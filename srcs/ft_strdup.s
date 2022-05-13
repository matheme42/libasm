section .text
	global _ft_strdup
	global ft_strdup
	extern MALLOC
	extern ft_strlen
	extern ft_strcpy

_ft_strdup:
ft_strdup:
	call	ft_strlen
	inc		rax
	push	rdi
	mov		rdi, rax
	call	MALLOC
	pop		rsi	
	mov		rdi, rax
	call	ft_strcpy
	ret
