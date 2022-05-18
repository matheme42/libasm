;------------------------------------------------------------------------------;
; int  ft_list_size(t_list *begin_list);                                       ;
;                                                                              ;
; 1st arg:  rdi  (const tlist *begin_list)                                     ;
; return :  rax  len                                                           ;
;------------------------------------------------------------------------------;

%include "tlist.s"
section .text
	global ft_list_size		; export ft_list_size (LINUX)
	global _ft_list_size	; export ft_list_size (MACOSX)

_ft_list_size:				; ft_list_size (MACOSX)
ft_list_size:				; ft_list_size (LINUX)
    xor rax, rax			; int i = 0
	jmp loop				; jmp loop
	loop_body:
	inc rax					; i = i + 1
	mov rdi, [rdi + next]	; data = data->next
	loop:
	cmp rdi, 0				; r = cmp(tlist *data, NULL)
	jne loop_body			; (r != 0) ? jmp loop_body
    ret						; return (i)