# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 10:55:00 by lsoto-do          #+#    #+#              #
#    Updated: 2024/06/11 09:57:05 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv
CFLAGS      = -std=c++98 -pedantic -g -Wall -Wextra -Werror #-fsanitize=address
CC			= c++
RM          = rm -f
SRCS_PATH	= src/
CONF_PATH	= Config/
AUX_PATH	= Aux/
SERVER_PATH	=	Server/
OVERSEER_PATH = Overseer/
PARSING_PATH =	Parsing/
BASE_PATH	= BaseHandler/
CLIENT_PATH =	Client/
RESPONSE_PATH = Responses/
CGI_PATH 	= CGI/
FILE_READ_PATH = FileHandler/
COOKIES_PATH = Cookies/
DELETED_PATH = DeletedFiles/

OBJS_PATH	= obj/
DEPS_PATH	= deps/

MAKE_OBJ_DIR	=	$(OBJS_PATH) $(addprefix $(OBJS_PATH), $(AUX_PATH) $(SERVER_PATH)\
								$(OVERSEER_PATH) $(FILE_READ_PATH) $(CLIENT_PATH)\
								$(PARSING_PATH) $(CGI_PATH) $(BASE_PATH) $(CONF_PATH)\
								$(RESPONSE_PATH) $(COOKIES_PATH) $(DELETED_PATH) )  \
								\
					$(addprefix $(DEPS_PATH), $(AUX_PATH) $(SERVER_PATH)\
								$(OVERSEER_PATH) $(FILE_READ_PATH) $(CLIENT_PATH)\
								$(PARSING_PATH) $(CGI_PATH) $(BASE_PATH) $(CONF_PATH)\
								$(RESPONSE_PATH) $(COOKIES_PATH) $(DELETED_PATH))  
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

FILE_READ			=	FileHandler.cpp

FILE_READ_FILES		=	$(addprefix $(FILE_READ_PATH), $(FILE_READ))

BASE			=	BaseHandler.cpp

BASE_FILES		=	$(addprefix $(BASE_PATH), $(BASE))

PARSING			=	ParsingCgi.cpp      ParsingGlobal.cpp   ParsingLocation.cpp ParsingElement.cpp    ParsingServer.cpp    Parsing.cpp 	Path.cpp

PARSING_FILES		=	$(addprefix $(PARSING_PATH), $(PARSING))

SERVER			=	Server.cpp getters.cpp

SERVER_FILES		=	$(addprefix $(SERVER_PATH), $(SERVER))

OVERSEER			=	Overseer.cpp mainLoop.cpp

OVERSEER_FILES		=	$(addprefix $(OVERSEER_PATH), $(OVERSEER))

CLIENT			=	Client.cpp getters.cpp setters.cpp RequestHandler.cpp DirectResponse.cpp 

CLIENT_FILES		=	$(addprefix $(CLIENT_PATH), $(CLIENT))

CONF			=	ConfigVirtualServer.cpp ConfigElement.cpp ConfigLocation.cpp ConfigCgi.cpp

CONF_FILES		=	$(addprefix $(CONF_PATH), $(CONF))

RESPONSE			=	Response.cpp

RESPONSE_FILES		=	$(addprefix $(RESPONSE_PATH), $(RESPONSE))

COOKIES			=	Cookies.cpp

COOKIES_FILES		=	$(addprefix $(COOKIES_PATH), $(COOKIES))


DELETED			=	DeletedFiles.cpp

DELETED_FILES		=	$(addprefix $(DELETED_PATH), $(DELETED))


SRC				+=	$(AUX_FILES) $(SERVER_FILES) $(OVERSEER_FILES) $(CLIENT_FILES) \
					$(PARSING_FILES) $(CGI_FILES) $(BASE_FILES) $(FILE_READ_FILES) \
					$(CONF_FILES) $(RESPONSE_FILES) $(COOKIES_FILES) $(DELETED_FILES)
				
DEPS			= 	$(addprefix $(DEPS_PATH), $(SRC:.cpp=.d))

OBJS 			=	$(addprefix $(OBJS_PATH), $(SRC:.cpp=.o))

all: $(NAME)


$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(LIGHT_GREEN)Created $(NAME) executable$(DEF_COLOR)"
	
$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp | $(MAKE_OBJ_DIR) $(DEPS_PATH)
			@echo "$(CYAN)Compiling $< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) $(INCS) -MMD -MP -c $< -o $@
			@mv $(basename $@).d $(patsubst $(SRCS_PATH)%,$(DEPS_PATH)%,$(dir $<))


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
