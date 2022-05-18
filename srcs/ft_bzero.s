;------------------------------------------------------------------------------;
; void ft_bzero(char *src, const int size)                                     ;
;                                                                              ;
; 1st arg:  rdi  src                                                           ;
; 2nd arg:  rsi  size                                                          ;
;------------------------------------------------------------------------------;

section .text
	global ft_bzero	    ; export ft_bzero (LINUX)
	global _ft_bzero	; export ft_bzero (MACOSX)

_ft_bzero:				; ft_bzero (MACOSX)
ft_bzero:				; ft_bzero (LINUX)
	mov rcx, rsi        ; int i = len
    xor rax, rax        ; char c = '\0'
    rep stosb           ; while (i--) s[i] = c
	ret					; return ;