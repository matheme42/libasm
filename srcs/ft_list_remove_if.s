;------------------------------------------------------------------------------;
; void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(),    ;
;       void (*free_fct)(void *))                                              ;
;                                                                              ;
; 1st arg:  rdi const tlist **begin_list                                       ;
; 2nd arg:  rsi const void *data_ref                                           ;
; 3th arg:  rdx const int (*cmp)()                                             ;
; 4th arg : rcx const void (*free_fct)(void *)                                 ;
;------------------------------------------------------------------------------;

%include "tlist.s"
section .text
	global ft_list_remove_if    ; export ft_list_remove_if (LINUX)
	global _ft_list_remove_if   ; export ft_list_remove_if (MACOSX)
    extern MALLOC               ; import malloc(Linux) or _malloc(Macosx)
    extern FREE                 ; import free(Linux) or _free(Macosx)

_ft_list_remove_if:             ; ft_list_remove_if (MACOSX)
ft_list_remove_if:              ; ft_list_remove_if (LINUX)
    ; compare si le pointeur passé est nul ou sont contenu
    cmp rdi, 0                  ; if (begin_list == NULL)
    jz exit                     ;   return ;
    cmp dword [rdi], 0          ; if (*begin_list == NULL)
    jz exit                     ;   return ;
    ; appel de la fonction de comparaison
    push rdi                    ; preserve arg1
    push rsi                    ; preserve arg2
    push rdx                    ; preserve arg3
    push rcx                    ; preserve arg4
    mov rdi, [rdi]              ; tmp = *begin_list
    mov rdi, [rdi + data]       ; data = tmp->data
    call rdx                    ; if (cmp(data, data_ref) != 0)
    pop rcx                     ; { unpreserve arg4
    pop rdx                     ;   unpreserve arg3
    pop rsi                     ;   unpreserve arg2
    pop rdi                     ;   unpreserve arg1
    cmp rax, 0                  
    jz remove
    ; si la comparaison donne 0
    push rdi                    ;   preserve arg1
    push rsi                    ;   preserve arg2
    push rdx                    ;   preserve arg3
    push rcx                    ;   preserve arg4
    mov rdi, [rdi]              ;   tmp = *begin_list
    add rdi, next               ;   tmp = tmp->next
    call ft_list_remove_if      ;   ft_list_remove_if(tmp, data_ref, cmp, free_fct)
    pop rcx                     ;   unpreserve arg4
    pop rdx                     ;   unpreserve arg3
    pop rsi                     ;   unpreserve arg2
    pop rdi                     ;   unpreserve arg1
    jmp exit                    ;   return ;
    remove:                     ; }
    push rdi                    ; preserve arg1
    push rsi                    ; preserve arg2
    push rdx                    ; preserve arg3
    push rcx                    ; preserve arg4
    mov rdi, [rdi]              ; tmp = *begin_list
    mov rbx, [rdi + next]       ; next = tmp->next
    push rbx
    push rdi
    mov rdi, [rdi + data]       ; a = tmp->data
    call rcx                    ; free_fct(a)
    pop rdi
    call FREE                   ; free(tmp)
    pop rbx
    pop rcx                     ; unpreserve arg4
    pop rdx                     ; unpreserve arg3
    pop rsi                     ; unpreserve arg2
    pop rdi                     ; unpreserve arg1
    mov [rdi], rbx              ; *begin_list = next
    call ft_list_remove_if      ; ft_list_remove_if(begin_list, data_ref, cmp, free_fct)   
    exit:
    ret                         ; return ;
