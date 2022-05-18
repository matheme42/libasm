;------------------------------------------------------------------------------;
; char *ft_strcpy(char *dest, const char *src)                                 ;
;                                                                              ;
; 1st arg:  rdi  dest                                                          ;
; 2nd arg:  rsi  src                                                           ;
; return :  rax  src address                                                   ;
;------------------------------------------------------------------------------;

section .text
	global ft_strcpy	; export ft_strcpy (LINUX)
	global _ft_strcpy	; export ft_strcpy (MACOSX)

_ft_strcpy:				; ft_strcpy (MACOSX)
ft_strcpy:				; ft_strcpy (LINUX)
	mov rax, rdi		; set return (dest)
	loop :
	cmp BYTE [rsi], 0	; r = cmp((char)*str, 0)
	je exit				; (r == 0) ? jmp exit
	movsb				; *dest++ = *src++
	jmp loop			; jmp ft_strcpy
	exit:
	movsb				; *dest++ = *src++
	ret					; return (dest);