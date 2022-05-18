;------------------------------------------------------------------------------;
; ssize_t  ft_read(int fd, void *buf, size_t count);                           ;
;                                                                              ;
; 1st arg:  rdi  file descriptor, output location STDIN (0)                    ;
; 2nd arg:  rsi  buffer (address of where to place data)                       ;
; 3th arg:  rdx  count (max numb of char to read)                              ;
; return :  rax  sys_read(0)                                                   ;
;------------------------------------------------------------------------------;

section .text
	global ft_read		; export ft_read (LINUX)
	global _ft_read		; export ft_read (MACOSX)
	extern ERRNO		; include ___errno(Macosx) or  _get_errno_location(Linux)

_ft_read:				; ft_read (MACOSX)
	pushf				; push flag
	mov rax, SYS_READ   ; prepare syscall 0x2000003
	syscall				; write(rdi, rsi, rdx)
	jnc exit			; if (!error) jmp exit
	jmp exit_errno		; jmp errno

ft_read:				; ft_read (LINUX)
	pushf				; push flag
	mov rax, SYS_READ	; prepare syscall 0x01
	syscall				; ret = write(rdi, rsi, rdx)
	cmp rax, 0			; cmp (ret, 0)
	jns exit			; (ret < 0) jmp exit
    neg rax    			; ret = -ret

exit_errno:
    mov rdi, rax
    call ERRNO			; void *errno = ___errno()  errno = _get_errno_location()
    mov [rax], rdi  	; *errno = ret
	mov rax, -1			; ret = -1
exit :
	popf				; pop flags
	ret					; return (ret)