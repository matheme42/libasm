section .text
	global ft_read
	global _ft_read
	extern ERRNO

; ft_read(int fd, char *buff, int size)
_ft_read:
	pushf
	push rbp
	mov rbp, rsp
	mov rax, SYS_READ         	;   put the code of read in rax register
	syscall
	jnc exit_mac
	read_errno_mac:
	push rax
	call ERRNO
	mov rbx, rax
	pop rax
	mov [rbx], rax
	mov rax, -1
	exit_mac:
	pop rbp
	popf
	ret

ft_read:
	pushf
	mov rax, SYS_READ
	syscall
	cmp rax, 0
	jns exit
	read_errno:
	cmp rax, 0
	jns ft_write_absolute
    neg rax    			; get absolute value of syscall return
	ft_write_absolute:
    mov rdi, rax
    call ERRNO			; get the location of errno
    mov [rax], rdi  	; set the value of errno
	mov rax, -1
	exit :
	popf
	ret