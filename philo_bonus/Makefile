# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 16:32:21 by halnuma           #+#    #+#              #
#    Updated: 2025/05/05 11:18:35 by halnuma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

# ----- COLORS ----- #

_END			= \033[0m
_GREY			= \033[0;30m
_RED			= \033[0;31m
_GREEN			= \033[0;32m
_YELLOW			= \033[0;33m
_BLUE			= \033[0;34m
_PURPLE			= \033[0;35m
_CYAN			= \033[0;36m
_BOLD			= \e[1m

# ----- VARIABLES ----- #

NAME			= philo_bonus
CC 				= cc
CFLAGS 			= -Wall -Wextra -Werror -pthread

# ----- PATHS ----- #

P_INC			= includes/
P_SRC			= src/
P_OBJ			= obj/

# ----- FILES ----- #

MAIN 			= main			philo_utils		\
				process			actions			\
				libft			init			\
				time			exit			\
				sems

HDR_SRC			= philo_bonus

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
OBJ_MAIN		= $(addprefix $(P_OBJ), $(addsuffix .o, $(MAIN)))
HEADERS			= $(addprefix $(P_INC), $(addsuffix .h, $(HDR_SRC)))

SRC_ALL			= $(SRC_MAIN)
OBJ_ALL			= $(OBJ_MAIN)

# ----- RULES ----- #

all: 			$(NAME)

$(NAME):		$(OBJ_ALL) Makefile $(HEADERS)
				@$(CC) $(CFLAGS) -I $(P_INC) $(OBJ_ALL) -o $@
				@echo "$(_GREEN)$(_BOLD)=> $(NAME) compiled!$(_END)"

$(P_OBJ):
				@mkdir -p $(P_OBJ)

$(P_OBJ)%.o:	$(P_SRC)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

# ----- BASIC RULES ----- #

clean:			
				@rm -rf $(P_OBJ)
				@echo "$(_CYAN)$(NAME) cleaned!$(_END)"

fclean:
				@rm -rf $(NAME)
				@$(MAKE) clean --no-print-directory
				@echo "$(_CYAN)$(NAME) full cleaned!$(_END)"

re:
				@$(MAKE) fclean --no-print-directory
				@$(MAKE) all --no-print-directory
