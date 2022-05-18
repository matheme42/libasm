;------------------------------------------------------------------------------;
; int  ft_strcmp(const char *s1, const char *s2)                               ;
;                                                                              ;
; 1st arg:  rdi  (const char *s1)                                              ;
; 2nd arg:  rsi  (const char *s2)                                              ;
; return :  rax  s1 - s2                                                       ;
;------------------------------------------------------------------------------;
	
section .text
	global ft_strcmp			; export ft_strcmp (LINUX)
	global _ft_strcmp			; export ft_strcmp (MACOSX)
	extern ft_strlen
_ft_strcmp:						; ft_strcmp (MACOSX)
ft_strcmp:						; ft_strcmp (LINUX)
	call ft_strlen
	mov rcx, rax
	inc rcx
	repz cmpsb
	movzx rax, byte [rdi - 1]	; long ret = (long)*s1
	movzx rbx, BYTE [rsi - 1]	; long tmp = (long)*s2
	sub	rax, rbx				; ret -= tmp
	ret
