###############################################################################
#********************** Makefile for Webserv  ********************************#
###############################################################################

#******************************  COMPILER  ***********************************#

NAME			= webserv
CC 				= c++
CFLAGS 			= -Wall -Werror -Wextra -Wshadow -Wno-shadow -std=c++98

#******************************  SOURCE  *************************************#

INCS = ./include
PATHS = main.cpp \
		[...]
SRCS = $(addprefix $(SRCDIRS)/, $(PATHS))
SRCDIRS = srcs

#******************************  OBJ  ****************************************#

OBJDIRS = objs
OBJS = $(addprefix $(OBJDIRS)/, $(patsubst %.cpp, %.o, $(PATHS)))

#******************************  COLORS  *************************************#

GREEN			=	\033[0;32m
END				=	\033[0m

#******************************  RULES  **************************************#

all: $(NAME)

$(NAME): 	$(OBJDIRS) $(OBJS)
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -I$(INCS)
			@echo "${GREEN}> ${NAME} was successfuly compiled 🎉${END}"

$(OBJDIRS):
	@mkdir -p $(OBJDIRS)

$(OBJDIRS)/%.o: $(SRCDIRS)/%.cpp
			@$(CC) $(CFLAGS) -I$(INCS) -c $< -o $@

clean:
			@rm -rf $(OBJDIRS)
			@echo "${GREEN}> Objects deleted${END}"

fclean : clean
			@rm -f ${NAME}
			@echo "${GREEN}> Cleaning of ${NAME} has been done${END}"

re: fclean all