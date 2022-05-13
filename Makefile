# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: matheme <matheme@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/03 11:06:26 by yalabidi          #+#    #+#              #
#    Updated: 2022/05/13 15:39:16 by matheme          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DISTRIB_NAME = $(shell uname -s)

BLUE=\033[0;38;5;123m
LIGHT_PINK = \033[0;38;5;200m
PINK = \033[0;38;5;198m
DARK_BLUE = \033[1;38;5;110m
GREEN = \033[1;32;111m
LIGHT_GREEN = \033[1;38;5;121m
LIGHT_RED = \033[0;38;5;110m
FLASH_GREEN = \033[33;32m
WHITE_BOLD = \033[37m

# nom de l'executable
NAME= 	libasm.a
TESTER=	tester

#sources path
SRC_PATH= srcs

#objects path
OBJ_PATH= objs

NAME_SRC= ft_strlen.s ft_strcmp.s ft_strcpy.s ft_read.s ft_write.s ft_strdup.s\


NAME_SRC_LEN	= $(shell echo -n $(NAME_SRC) | wc -w)
I				= 0


OBJ_NAME		= $(NAME_SRC:.s=.o)

OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

CC			= clang
NASM_ELF	= nasm -f elf64 -DSYS_WRITE=0x01 -DSYS_READ=0x00 -DERRNO=__errno_location -DMALLOC=malloc
NASM_MACHO	= nasm -f macho64 -DSYS_WRITE=0x2000004  -DSYS_READ=0x2000003 -DERRNO=___error -DMALLOC=_malloc

ARCH		= ar rcs
CFLAGS		= -Wall -Werror -Wextra


all: $(NAME)

$(NAME) : $(OBJS)
	@$(ARCH) $@  $^
	@echo "	\033[2K\r$(DARK_BLUE)$(NAME):\t\t$(GREEN)loaded\033[0m"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.s
	@mkdir $(OBJ_PATH) 2> /dev/null || true
ifeq ("$(DISTRIB_NAME)", "Linux")
	@$(NASM_ELF) $< -o $@
else
	@$(NASM_MACHO) $< -o $@
endif
	@$(eval I=$(shell echo $$(($(I)+1))))
	@printf "\033[2K\r${G}$(DARK_BLUE)>>\t\t\t\t$(I)/$(shell echo $(NAME_SRC_LEN)) ${N}$(BLUE)$<\033[36m \033[0m"


clean:
ifeq ("$(wildcard $(OBJ_PATH))", "")
else
	@rm -f $(OBJS)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@printf "\033[2K\r$(DARK_BLUE)$(NAME) objects:\t$(LIGHT_PINK)removing\033[36m \033[0m\n"
endif


fclean: clean
	@rm -f $(NAME)
ifeq ("$(wildcard $(NAME))", "")
else
	@rm -f $(NAME)
	@printf "\033[2K\r$(DARK_BLUE)$(NAME):\t\t$(PINK)removing\033[36m \033[0m\n"
endif

re: fclean all

test: all $(TESTER)

$(TESTER) :
	@$(CC) $(SRC_PATH)/main.c $(NAME) -o $(TESTER)
	@./$(TESTER)
	@rm -f $(TESTER)
	@echo "	\033[2K\r$(DARK_BLUE)$(NAME):\t\t$(WHITE_BOLD)complete\033[0m"

.PHONY: all re clean fclean lib test silent
