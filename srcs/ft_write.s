section .text
	global ft_write
	global _ft_write
	extern ERRNO
_ft_write:
	pushf
	mov rax, SYS_WRITE
	syscall
	jnc exit_mac
	read_errno_mac:
	cmp rax, 0
	jns ft_write_absolute2
    neg rax    			; get absolute value of syscall return
	ft_write_absolute2:
    mov rdi, rax
    call ERRNO			; get the location of errno
    mov [rax], rdi  	; set the value of errno
	mov rax, -1
	exit_mac :
	popf
	ret
ft_write:
	pushf
	mov rax, SYS_WRITE
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