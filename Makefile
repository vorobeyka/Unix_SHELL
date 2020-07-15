NAME = ush
INC = inc/header.h
SRC := $(wildcard src/*.c)
OBJ = $(addprefix obj/, $(notdir $(SRC:%.c=%.o)))
LIBMX = libmx/libmx.a
FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
SANFLAG = -g -fsanitize=address

all: $(NAME)

install: $(NAME)

$(NAME): $(LIBMX) $(OBJ)
	@make clean
	@clang $(FLAGS) $(OBJ) $(LIBMX) -o $(NAME)
	@printf "\x1b[32;1m$(NAME) created\x1b[0m\n"
	@sudo cp $(NAME) /bin/

$(LIBMX):
	@make -C libmx

$(OBJ): | obj

obj:
	@mkdir obj

obj/%.o: src/%.c $(INC)
	@clang $(FLAGS) -c $< -o $@
	@printf "\x1b[32mcompiled: \x1b[0m[$<]\n"

clean:
	@rm -rf *.o ush.dSYM
	@make clean -C libmx

uninstall: clean
	@make uninstall -C libmx
	@make clean
	@rm -rf ./obj
	@rm -rf $(NAME)

reinstall: uninstall install

d:
	@clang $(FLAGS) $(SANFLAG) src/*.c libmx/libmx.a -o ush --debug
