# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 10:55:00 by lsoto-do          #+#    #+#              #
#    Updated: 2024/04/07 16:21:10 by dacortes         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv
CFLAGS      = -std=c++98 -pedantic -g  #-fsanitize=address #-Wall -Wextra  -Werror 
CC			= c++
RM          = rm -f
SRCS_PATH	= src/
AUX_PATH	= Aux/
SERVER_PATH	=	Server/
OVERSEER_PATH = Overseer/
CLIENT_PATH =	Client/
PARSING_PATH =	Parsing/
BASE_PATH	= BaseHandler/
CGI_PATH 	= CGI/
OBJS_PATH	= obj/
MAKE_OBJ_DIR	=	$(OBJS_PATH) $(addprefix $(OBJS_PATH), $(AUX_PATH) $(SERVER_PATH) $(OVERSEER_PATH) $(CLIENT_PATH) $(PARSING_PATH) $(CGI_PATH) $(BASE_PATH)) 
DEPS_PATH	= deps/
INCS        = -I./includes


#Colors


DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
LIGHT_GREEN = \033[1;92m

###

SRC        		=	main.cpp 

AUX			=	aux.cpp

AUX_FILES		=	$(addprefix $(AUX_PATH), $(AUX))

CGI			=	CGI.cpp getters.cpp

CGI_FILES		=	$(addprefix $(CGI_PATH), $(CGI))

BASE			=	BaseHandler.cpp

BASE_FILES		=	$(addprefix $(BASE_PATH), $(BASE))

PARSING			=	Parsing.cpp

PARSING_FILES		=	$(addprefix $(PARSING_PATH), $(PARSING))

SERVER			=	Server.cpp getters.cpp

SERVER_FILES		=	$(addprefix $(SERVER_PATH), $(SERVER))

OVERSEER			=	Overseer.cpp mainLoop.cpp

OVERSEER_FILES		=	$(addprefix $(OVERSEER_PATH), $(OVERSEER))

CLIENT			=	Client.cpp getters.cpp setters.cpp

CLIENT_FILES		=	$(addprefix $(CLIENT_PATH), $(CLIENT))

DEPS			= 	$(addprefix $(DEPS_PATH), $(SRC:.cpp=.d) $(SERVER:.cpp=.d) $(AUX:.cpp=.d) $(OVERSEER:.cpp=.d) $(CLIENT:.cpp=.d) $(PARSING:.cpp=.d) $(CGI:.cpp=.d) $(BASE:.cpp=.d))

SRC				+=	$(AUX_FILES) $(SERVER_FILES) $(OVERSEER_FILES) $(CLIENT_FILES) $(PARSING_FILES) $(CGI_FILES) $(BASE_FILES)

OBJS 			=	$(addprefix $(OBJS_PATH), $(SRC:.cpp=.o))
				
all: $(NAME)


$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(LIGHT_GREEN)Created $(NAME) executable$(DEF_COLOR)"
	
$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp | $(MAKE_OBJ_DIR) $(DEPS_PATH)
			@echo "$(CYAN)Compiling $< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) $(INCS) -MMD -MP -c $< -o $@
			@mv $(basename $@).d $(DEPS_PATH)

$(MAKE_OBJ_DIR):
	@echo "$(GREEN)Creating $(NAME) Obj Dir $(DEF_COLOR)"
	@mkdir -p $(MAKE_OBJ_DIR)


$(DEPS_PATH):
	@echo "$(GREEN)Creating $(NAME) Deps Dir $(DEF_COLOR)"
	@mkdir -p $(DEPS_PATH)

-include $(DEPS)


clean: clean_objects

fclean:  clean_objects
	@$(RM) $(NAME)
	@echo "$(GREEN)$(NAME) executable cleaned!$(DEF_COLOR)"

clean_objects:
	@echo "$(GREEN)$(NAME) Objects and Dependencies cleaned!$(DEF_COLOR)"
	@$(RM) -r $(OBJS_PATH) $(DEPS_PATH)


re: fclean all 

.PHONY: all fclean clean re 
