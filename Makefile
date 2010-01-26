PATH_BIN	=	./bin
PATH_SRC	=	./src
PATH_INCLUDE	=	./include
PATH_LIBRARY	=	.

NAME		=	$(PATH_BIN)/es

SRCS		=	\
			$(PATH_SRC)/main.cpp			\
			$(PATH_SRC)/operators.cpp		\
			$(PATH_SRC)/operations.cpp		\
			$(PATH_SRC)/node.cpp			\
			$(PATH_SRC)/facts_parsing.cpp		\
			$(PATH_SRC)/files_parsing.cpp		\
			$(PATH_SRC)/prepare_fact.cpp		\
			$(PATH_SRC)/prepare_rule.cpp		\
			$(PATH_SRC)/print_out_facts_table.cpp	\
			$(PATH_SRC)/print_out_rules_table.cpp	\
			$(PATH_SRC)/fill_out_line.cpp		\
			$(PATH_SRC)/options_parsing.cpp		\
			$(PATH_SRC)/bool_conclusion.cpp		\
			$(PATH_SRC)/bool_expression.cpp		\
			$(PATH_SRC)/truth_value.cpp		\
			$(PATH_SRC)/bool_to_string.cpp		\
			$(PATH_SRC)/print_out_fired_rules.cpp	\
			$(PATH_SRC)/print_out_binary_tree.cpp

OBJS		=	$(SRCS:.cpp=.o)

CC		=	g++
RM		=	rm -f
MKDIR		=	mkdir -p
ECHO		=	/usr/bin/env echo
ECHOE		=	$(ECHO) -e
ECHON		=	$(ECHO) -n

SUCCESS		=	$(ECHOE) "[\033[32mSUCCESS\033[0m] "
FAILED		=	$(ECHOE) "[\033[31mFAILED\033[0m] "

INCLUDES	+=	-I$(PATH_INCLUDE)
LIBRARIES	+=	-L$(PATH_LIBRARY) -lboost_program_options-mt

DEBUG_TRUE	=	-g -DDEBUG
COMPAT		+=	-D__BSD_VISIBLE
WARNINGS	+=	-Wall -W -Werror

CFLAGS		+=	$(INCLUDES) $(DEBUG_$(DEBUG)) $(COMPAT) $(WARNINGS)
LDFLAGS		+=	$(LIBRARIES)

.SUFFIXES	:	.cpp.o

$(NAME)		:	$(OBJS)
			-@$(ECHON) "*** Making directory $(PATH_BIN) "
			-@$(MKDIR) $(PATH_BIN) && $(SUCCESS) || $(FAILED)
			-@$(ECHON) "*** Compiling $(OBJS) to $@ "
			-@$(CC) -o $@ $(OBJS) $(LDFLAGS) && $(SUCCESS) || $(FAILED)

all		:	$(NAME)

clean		:
			-@$(ECHON) "*** Removing $(PATH_SRC)/*~ "
			-@$(RM) $(PATH_SRC)/*~ && $(SUCCESS) || $(FAILED)
			-@$(ECHON) "*** Removing $(PATH_SRC)/*.o "
			-@$(RM) $(PATH_SRC)/*.o && $(SUCCESS) || $(FAILED)
			-@$(ECHON) "*** Removing $(PATH_BIN)/*.core "
			-@$(RM) $(PATH_BIN)/*.core && $(SUCCESS) || $(FAILED)

fclean		:	clean
			-@$(ECHON) "*** Removing $(NAME) "
			-@$(RM) $(NAME) && $(SUCCESS) || $(FAILED)

re		:	fclean all

.PHONY		:	all clean fclean re

.cpp.o		:
			-@$(ECHON) "*** Compiling $< to $@ "
			-@$(CC) $(CFLAGS) -c $< -o $@ && $(SUCCESS) || $(FAILED)
