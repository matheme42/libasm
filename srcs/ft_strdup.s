;------------------------------------------------------------------------------;
; char  *ft_strdup(const char *s)                                              ;
;                                                                              ;
; 1st arg:  rdi  str to duplicate                                              ;
; return :  rax  addr of the dup str                                           ;
;------------------------------------------------------------------------------;

section .text
	global ft_strdup	; export ft_strdup (LINUX)
	global _ft_strdup	; export ft_strdup (MACOSX)
	extern MALLOC		; import malloc(Linux) or _malloc(Macosx)
	extern ft_strlen	; import ft_strlen
	extern ft_strcpy	; import ft_strcpy

_ft_strdup:				; ft_strdup (MACOSX)
ft_strdup:				; ft_strdup (LINUX)
	call	ft_strlen	; len = ft_strlen(src)
	inc		rax			; len = len + 1
	push	rdi			; push len
	mov		rdi, rax	; void *dest = malloc(len)
	call	MALLOC		;	
	pop		rsi			; pop len
	cmp		rax, 0		; if (dest == NULL)
	jz		exit		;	return (dest);
	mov		rdi, rax	; void *dest = ft_strcpy(dest, src)
	call	ft_strcpy	;
	exit:
	ret					; return (dest)
