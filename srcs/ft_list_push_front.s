;------------------------------------------------------------------------------;
; void  ft_list_push_front(t_list **begin_list, void *data);                   ;
;                                                                              ;
; 1st arg:  rdi  (const tlist **begin_list)                                    ;
; 2st arg:  rdi  (const void *data)                                            ;
;------------------------------------------------------------------------------;

%include "tlist.s"
section .text
	global ft_list_push_front   ; export ft_list_size (LINUX)
	global _ft_list_push_front  ; export ft_list_size (MACOSX)
    extern MALLOC               ; import malloc(Linux) or _malloc(Macosx)

_ft_list_push_front:            ; ft_list_size (MACOSX)
ft_list_push_front:             ; ft_list_size (LINUX)
    cmp rdi, 0                  ; if (begin_list == NULL)
    je exit                     ;   jump exit
    push rdi
    push rsi
    mov rdi, tlist_size         ; tlist *ptr = malloc(sizeof(tlist))
    call MALLOC
    pop rsi
    pop rdi
    cmp rax, 0                  ; if(ptr == NULL)    
    jz exit                     ;   jmp exit
    mov rbx, [rdi]              ; tlist *maillon = *begin_list
    mov [rax + data], rsi       ; ptr->data = data
    mov [rax + next], rbx       ; ptr->next = maillon
    mov [rdi], rax              ; *begin_list = ptr
    exit:
    ret                         ; return