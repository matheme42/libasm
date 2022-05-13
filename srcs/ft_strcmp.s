section .text
	global ft_strcmp
	global _ft_strcmp
_ft_strcmp:
ft_strcmp:
	xor rax, rax
	xor rcx, rcx
	xor rdx, rdx
loop:
	mov cl, [rdi]
	mov al, [rsi]
	inc rdi
	inc rsi
	cmp cl, 0
	je exit
	cmp al, 0
	je exit
	cmp al, cl
	je loop
	exit:
	sub al, cl
	jc overflow
	neg rax
	ret
	overflow:
	mov dl, 255
	sub dl, al
	mov al, dl
	inc rax
	ret