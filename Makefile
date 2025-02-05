##
## eriizu
## cpp_template 20210818
## File description:
## Makefile for a c++ project
##

CXX		=	clang++

NAME		=	gfx.out

RM		=	rm -vf

MAIN		=	src/main.cpp \

SRCS		=	src/MorpionGame.cpp \
                src/gfx/TextPieces.cpp \
                src/gfx/TextStatus.cpp \
                src/gfx/GridLines.cpp \
                src/GfxPlayer.cpp \
                src/TermPlayer.cpp \
                src/StandaloneNetPlayer.cpp \
                src/GameArena.cpp \
                src/NetPlayerFactory.cpp \
                src/client.cpp

OBJ_MAIN	=	$(MAIN:.cpp=.o)

OBJS		=	$(SRCS:.cpp=.o)

TEST		=	unit_tests.out

SRCS_TEST	=	test/example.cpp

SRCS_TEST	+=	$(SRCS)

OBJS_TEST	=	$(SRCS_TEST:.cpp=.o)

CPPFLAGS	=	-W -Wextra -Wall -Iinclude/ -std=c++20 -gdwarf-4

LDFLAGS		=	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

%.o: %.cpp
	@printf "[\033[0;36mcompiling\033[0m]% 39s\r" $< | tr " " "."
	@$(CXX) -c -o $@ $< $(CPPFLAGS)
	@printf "[\033[0;32mcompiled\033[0m]% 40s\n" $< | tr " " "."

all: $(NAME)

debug: CPPFLAGS += -ggdb
# debug: fclean
debug: $(NAME)

tests: CXX=g++
tests: CPPFLAGS += -ggdb
tests: CPPFLAGS += -Itest/
tests: $(TEST)

tests_run: CXX=g++
tests_run: CPPFLAGS += --coverage -I../src/
tests_run: LDFLAGS += -lgcov
tests_run: $(TEST)
	@./$(TEST) --verbose -j 1 --always-succeed
	@cd src/ && rm -f main.gcda main.gcno

$(NAME): $(OBJ_MAIN) $(OBJS)
	@printf "[\033[0;36mlinking\033[0m]% 41s\r" $(NAME) | tr " " "."
	@$(CXX) $(OBJ_MAIN) $(OBJS) -o $(NAME) $(LDFLAGS)
	@printf "[\033[0;36mlinked\033[0m]% 42s\n" $(NAME) | tr " " "."

$(TEST): $(OBJS_TEST)
	@printf "[\033[0;36mlinking\033[0m]% 41s\r" $(TEST) | tr " " "."
	@$(CXX) $(OBJS_TEST) -o $(TEST) $(LDFLAGS)
	@printf "[\033[0;36mlinked\033[0m]% 42s\n" $(TEST) | tr " " "."

clean: artifacts_clean
	@printf "[\033[0;31mdeletion\033[0m][objects]% 31s\n" `$(RM) $(OBJ_MAIN) $(OBJS) $(OBJS_TEST) | wc -l | tr -d '\n'` | tr " " "."

fclean: clean
	@$(RM) $(NAME) $(TEST) > /dev/null
	@printf "[\033[0;31mdeletion\033[0m][binary]% 32s\n" $(NAME) | tr " " "."

artifacts_clean:
	@printf "[\033[0;31mdeletion\033[0m][artifacts]% 29s\n" `find -type f \( -name "*.gcno" -o -name "*.gc*" \) -delete -print | wc -l | tr -d '\n'` | tr " " "."

re: fclean all

.PHONY: all clean fclean re debug tests artifacts_clean
