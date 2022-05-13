section .text
	global ft_strlen
	global _ft_strlen

_ft_strlen:
ft_strlen:
	mov rax, -1
	loop:
	inc rax
	cmp BYTE [rdi + rax], 0
	jne loop
	ret