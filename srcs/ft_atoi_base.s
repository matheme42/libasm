;------------------------------------------------------------------------------;
; int  ft_atoi_base(char *str, char *base);                                    ;
;                                                                              ;
; 1st arg:  rdi  (const char *str)       		                               ;
; 2nd arg:  rsi  (const char *base)       		                               ;
; return :  rax  number                                                        ;
;------------------------------------------------------------------------------;

section .text
	global ft_atoi_base			; export ft_atoi_base (LINUX)
	global _ft_atoi_base		; export ft_atoi_base (MACOSX)
	extern ft_strlen			; import ft_strlen


_ft_atoi_base:					; ft_atoi_base (MACOSX)
ft_atoi_base:					; ft_atoi_base (LINUX)
	push r8						; preserve r8
	push r9						; preserve r9

	get_base_radix:
	mov rbx, rdi
	mov rdi, rsi
	call ft_strlen				; int radix = ft_strlen(base)
	cmp rax, 2					; if (radix < 0)
	jl occurd					;	return (-1)
	mov r8, rax					; radix (r8)
    mov rdi, rbx

	check_base_correct:
	mov rbx, -1					; int i = -1
	check_base_loop:			; do
								; {
	inc rbx						;	i = i + 1
	mov rcx, rbx				;	int j = i + 1
	check_base_loop2:			;	do
								;	{
	inc rcx						;		j = j + 1
	mov dl, byte [rsi + rcx]
	cmp byte [rsi + rbx], dl	;		if(base[i] == base[j])
	je occurd					;			return (-1)
	cmp byte [rsi + rbx], ' '	;		if (base[i] == ' ')
	je occurd					;			return (-1)
	cmp byte [rsi + rbx], 9		;		if (base[i] >= 9)
	jl check_base_cmp			;		{
	cmp byte [rsi + rbx], 13	; 			if (base[i] < 13)
	jl occurd					; 				return (-1)
	check_base_cmp:				;			if (base[i] == 9 || base[i] == 13)
	je occurd					;				return (-1)
	cmp rcx, r8					;		}
	jl check_base_loop2			;	} while (j < radix)
	cmp rbx, r8					; 
	jl check_base_loop			; } while (i < radix) 
	;end check base correct

	;start skip \t \n \r \v \f ' '
	skip_char:					; do
								; {
	cmp byte [rdi], 9			;	if (*str < 9)
	jl check_sign_neg			;		break ;
	cmp byte [rdi], ' '			;	if (*str != ' ')
	je skip_char_jump			;	{
	cmp byte [rdi], 13			;		if (*str > 13)
	jg check_sign_neg			;			break ;
	skip_char_jump:				;	}
	inc rdi						;	str++
	jmp skip_char				;	while(1)
	;end skip \t \n \r \v \f ' '

	; start check sign
	check_sign_neg:
	mov r9, 1					; sign = 1
	cmp r8, 10					; if (radix == 10)
	jne check_sign_plus			; {
	cmp byte [rdi], '-'			;	if (*str == '-')
	jne check_sign_plus			;	{
	inc rdi						;		str++
	mov r9, -1					;		sign = -1
	jmp resolve					;	}
	check_sign_plus:
	cmp byte [rdi], '+'			; else if (*str == '+')
	jne resolve					; {
	inc rdi						;		str++
	; end check sign			; }
	; start resolving the number
	resolve:
	xor rcx, rcx				; nb = 0
	jmp resolve_cmp				; while (*str)
	resolve_loop:				; {
	mov rbx, -1					; 	int i = -1
	get_index:					;	do {
	inc rbx						;		i = i + 1
	mov dl, byte [rsi + rbx]	;	char c = base[i]
	cmp rbx, r8					;	if (i == radix)
	je occurd					;		return (-1)
	cmp dl, byte [rdi]		;	} while (c != *str)
	jne get_index 				;

	mov rax, r8					; 
	mul rcx						; 
	mov rcx, rax				;	nb = nb * radix

	xor rdx, rdx				;
	mov rax, rbx				;
	div r8						;
	add rcx, rdx				;	nb = nb + (i % radix)


	cmp rcx, 2147483647			; if (nb > 2147483647)
	jg occurd					;	return (-1)
	cmp rcx, -2147483648		; if (nb < -2147483648)
	jl occurd					; return (-1)

	inc rdi						; str++
	resolve_cmp:				;
	cmp byte [rdi], 0			;
	jne resolve_loop			; } while (*str)

	mov rax, r9					;
	mul rcx						; nb = nb * sign
	jmp exit					; // skip next line
	occurd:
	mov rax, -1					; nb = -1
	exit:
	pop r9						; unpreserve r19
	pop r8						; unpreserve r8
	ret							; return (nb)