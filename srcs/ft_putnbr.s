;------------------------------------------------------------------------------;
; void   ft_putnbr(const long nb)                                              ;
;                                                                              ;
; 1st arg:  rdi  nb                                                            ;
;------------------------------------------------------------------------------;

section .text
	global ft_putnbr		; export ft_putnbr (LINUX)
	global _ft_putnbr		; export ft_putnbr (MAXOSX)

; rbx, nb
_ft_putnbr:					; ft_putnbr (MAXOSX)
ft_putnbr:					; ft_putnbr (LINUX)
    enter 21, 0             ; char buff[21];  
    mov rax, rdi            ; long tmp = nb
    mov rbx, 21             ; int idx = 21;
    cmp rdi, 0              ; if (nb < 0)
    jz print_zero
    jns loop                ;   nb = abs(nb)
    neg rax
    loop:
    cmp rax, 0              ; while (nb)
    jz endloop              ;
    xor rdx, rdx            ;
    mov rcx, 10             ; nb = nb / 10
    div rcx                 ;
    add dl, '0'             ; c = nb % 10 + '0'
    dec rbx                 ; idx = idx - 1
    mov [rsp + rbx], dl     ; buff[idx] = c
    jmp loop                ;
    endloop:
    cmp rdi, 0              ; if (nb < 0)
    jns exit                ; idx = idx - 1
    dec rbx                 ; buff[idx] = '-'
    mov byte [rsp + rbx], '-'
    exit:
    mov rax, 0x01
    mov rdi, 1
    mov rsi, rsp
    add rsi, rbx
    mov rdx, 21
    sub rdx, rbx
    syscall                 ; write(1, buff, 21 - buff)
    leave
    ret
    print_zero:
    dec rbx
    mov byte [rsp + rbx], '0'
    jmp exit