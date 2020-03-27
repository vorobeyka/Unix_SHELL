NAME = ush

INC = ./inc/header.h

HEADTMP = ./header.h

SRC = ./main.c ./mx_new_terminal.c ./mx_history.c ./mx_help.c ./mx_builtin.c ./mx_char.c ./mx_read.c ./mx_ush_loop.c ./mx_ush_execute.c ./mx_signal.c ./mx_support.c ./mx_jobs.c ./mx_parse_all.c ./mx_cmnd_export.c ./mx_cmnd_unset.c ./mx_cmndcd.c ./mx_cmndpwd.c ./mx_parse_pipe_and_tz.c ./mx_cmnd_echo.c

SRCS = ./src/main.c ./src/mx_new_terminal.c ./src/mx_history.c ./src/mx_help.c ./src/mx_builtin.c ./src/mx_char.c ./src/mx_read.c ./src/mx_ush_loop.c ./src/mx_ush_execute.c ./src/mx_signal.c ./src/mx_support.c ./src/mx_jobs.c ./src/parser/mx_parse_all.c ./src/built-in/mx_cmnd_export.c ./src/built-in/mx_cmnd_unset.c ./src/built-in/mx_cmndcd.c ./src/built-in/mx_cmndpwd.c ./src/parser/mx_parse_pipe_and_tz.c ./src/built-in/mx_cmnd_echo.c

OBJ = ./main.o ./mx_new_terminal.o ./mx_history.o ./mx_help.o ./mx_builtin.o ./mx_char.o ./mx_read.o ./mx_ush_loop.o ./mx_ush_execute.o ./mx_signal.o ./mx_support.o ./mx_jobs.o ./mx_parse_all.o ./mx_cmnd_export.o ./mx_cmnd_unset.o ./mx_cmndcd.o ./mx_cmndpwd.o ./mx_parse_pipe_and_tz.o ./mx_cmnd_echo.o

OBJO = ./obj/main.o ./obj/mx_new_terminal.o ./obj/mx_history.o ./obj/mx_help.o ./obj/mx_builtin.o ./obj/mx_char.o ./obj/mx_read.o ./obj/mx_ush_loop.o ./obj/mx_ush_execute.o ./obj/mx_signal.o ./obj/mx_support.o ./obj/mx_jobs.o ./obj/mx_parse_all.o ./obj/mx_cmnd_export.o ./obj/mx_cmnd_unset.o ./obj/mx_cmndcd.o ./obj/mx_cmndpwd.o ./obj/mx_parse_pipe_and_tz.o ./obj/mx_cmnd_echo.o

CFLGS = -std=c99 -g -fsanitize=address#-Wall -Wextra -Wpedantic -Werror -g -fsanitize=address

all: install clean

install:
	@make -C libmx -f Makefile install
	@cp $(INC) $(SRCS) .
	@clang $(CFLGS) -c $(SRC)
	@mkdir -pv obj
	@mv $(OBJ) ./obj
	@clang $(CFLGS) $(OBJO) libmx/libmx.a -o $(NAME)
	@sudo cp $(NAME) /bin/

uninstall: clean
	@make -C libmx -f Makefile uninstall
	@rm -rf $(NAME)

clean:
	@make clean -C libmx
	@rm -rf $(OBJO)
	@rm -rf $(SRC)
	@rm -rf $(OBJ)
	@rm -rf $(HEADTMP)
	@rm -rf ./obj

reinstall: uninstall install
