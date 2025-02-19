# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 16:32:21 by halnuma           #+#    #+#              #
#    Updated: 2025/02/19 17:01:39 by halnuma          ###   ########.fr        #
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

# ----- FILES ----- #

MAIN 			= main			philo_utils

HDR_SRC			= philo

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
HEADERS			= $(addprefix $(P_INC), $(addsuffix .h, $(HDR_SRC)))

SRC_ALL			= $(SRC_MAIN)

# ----- RULES ----- #

all: 			$(NAME) 

$(NAME): 		$(SRC_ALL) Makefile $(HEADERS)
				@$(CC) $(CFLAGS) -I $(P_INC) $(SRC_ALL) -o $@
				@echo "$(_YELLOW)Compiling $(SRC_ALL)$(_END)"
				@echo "$(_GREEN)$(_BOLD)=> $(NAME) compiled!$(_END)"

# ----- BASIC RULES ----- #

clean:			
				@rm -rf $(NAME)
				@echo "$(_CYAN)$(NAME) cleaned!$(_END)"

fclean:
				@$(MAKE) clean --no-print-directory

re:
				@$(MAKE) fclean --no-print-directory
				@$(MAKE) all --no-print-directory
