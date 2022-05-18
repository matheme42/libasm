;------------------------------------------------------------------------------;
; size_t  ft_strlen(const char *s)                                             ;
;                                                                              ;
; 1st arg:  rdi  *s                                                            ;
; return :  rax  len                                                           ;
;------------------------------------------------------------------------------;

section .text
	global ft_strlen		; export ft_strlen (LINUX)
	global _ft_strlen		; export ft_strlen (MAXOSX)

_ft_strlen:					; ft_strlen (MAXOSX)
ft_strlen:					; ft_strlen (LINUX)
	mov rax, -1				; int i = -1
	loop:					; loop
	inc rax					; i = i + 1
	cmp BYTE [rdi + rax], 0	; cmp (s[i], 0)
	jne loop				; (s[i] != 0) ? jmp loop
	ret						; return (i);