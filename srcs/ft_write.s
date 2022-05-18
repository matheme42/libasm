;------------------------------------------------------------------------------;
; ssize_t    ft_write(int fd, const void *buf, size_t count)                   ;
;                                                                              ;
; 1st arg:  rdi file descriptor, output location STDOUT (1)                    ;
; 2nd arg:  rsi buffer (address of chars to write)                             ;
; 3th arg:  rdx count (numb of chars to output)                                ;
; return :  rax sys_write (1)                                                  ;
;------------------------------------------------------------------------------;

section .text
	global ft_write		; export ft_write (LINUX)
	global _ft_write	; export ft_write (MACOSX)
	extern ERRNO		; include ___errno(Macosx) or  _get_errno_location(Linux)
_ft_write:				; ft_write (MACOSX)
	pushf				; push flag
	mov rax, SYS_WRITE	; prepare syscall 0x2000004
	syscall				; write(rdi, rsi, rdx)
	jnc exit			; if (!error) jmp exit
	jmp exit_errno		; jmp errno
ft_write:				; ft_write (LINUX)
	pushf				; push flag
	mov rax, SYS_WRITE	; prepare syscall 0x01
	syscall				; ret = write(rdi, rsi, rdx)
	cmp rax, 0			; cmp (ret, 0)
	jns exit			; (ret < 0) jmp exit
    neg rax 			; ret = -ret
	jmp exit_errno
exit_errno:
    mov rdi, rax
    call ERRNO			; void *errno = ___errno()  errno = _get_errno_location()
    mov [rax], rdi  	; *errno = ret
	mov rax, -1			; ret = -1
exit :
	popf				; pop flags
	ret					; return (ret)