;------------------------------------------------------------------------------;
; void  t_list_sort(t_list **begin_list, int (*cmp)());                        ;
;                                                                              ;
; 1st arg:  rdi  (const tlist *begin_list)                                     ;
; 2nd arg:  rsi  (const int (*cmp()))	                                       ;
;------------------------------------------------------------------------------;

%include "tlist.s"
section .text
	global ft_list_sort			; export ft_list_sort (LINUX)
	global _ft_list_sort		; export ft_list_sort (MACOSX)


_ft_list_sort:					; ft_list_sort (MACOSX)
ft_list_sort:					; ft_list_sort (LINUX)
	mov		rbx, rsi
	mov		rdx, rdi
	cmp	    rdx, 0				; if (begin_list == NULL)
	je		exit				;	return ;
	mov		r8, [rdx] 			; tmp = *begin_list
	cmp		r8, 0				; if (*begin_list == NULL)
	je		exit				;	return ;

    loop:						; do
	mov		r9, [r8 + next]		; {
	cmp		r9, 0				; if ((*begin_list)->next == NULL)
	je		exit				;	return ;

	mov		rdi, [r8 + data]	; a = (*begin_list)->data
	mov		rsi, [r9 + data]	; b = ((*begin_list)->next)->data
	push	rbx
	call	rbx					; if (cmp(a , b) <= 0)
	pop		rbx					; {

	jg		sort				;  *begin_list = (*begin_list)->next
	mov		r8, [r8 + next]		; 	continue ;
	jmp		loop				; }
    sort:
	mov		rdi, [r8]			; a = *begin_list
	mov		rsi, [r9]			; b = *next
	mov		[r8], rsi			; *begin_list = b
	mov		[r9], rdi			; *next = a
    restart:
	mov		r8, [rdx]			; *begin_list = tmp
	jmp		loop				; while (1)
    exit:
	ret							; return ;