NAME				=	ft_traceroute
AUTHOR				:=	gboucett

PATH_INC			:=	-I./include
PATH_SRCS			:=	src
PATH_OBJS			:=	objs

CC					:=	gcc
RM					:=	@rm -f
MKDIR				:=	@mkdir -p

DEBUG_MODE			:=	1

SYS_ENDIAN			:=	1
NO_SUDO				:=	1		# 1 if sudo is not needed to run the program, 0 otherwise
CAP_FLAGS			:=	cap_net_raw=+ep

CFLAGS				:=	-Wall -Werror -Wextra -g3 -ggdb -fdiagnostics-color=always
LDFLAGS				:=	-lm

BASENAME			:=	main.c							\
						endian.c						\
						init_options.c					\
						throw.c							\
						ft_trace.c						\
						options/get_option.c			\
						options/parse_args.c			\
						options/print_flags.c			\
						utils/string.c					\
						utils/random.c					\
						core/setup.c					\
						core/ft_cksum.c					\
						core/trace.c					\
						core/create_packet.c			\
						core/time.c						\

SRCS				:=	$(addprefix $(PATH_SRCS)/, $(BASENAME))
OBJS				:=	$(addprefix $(PATH_OBJS)/, $(BASENAME:%.c=%.o))
DEPS				:=	$(addprefix $(PATH_OBJS)/, $(BASENAME:%.c=%.d))

#######################################################################################################################################################
#######################################################################################################################################################


PROGRESS			:=	0
NO_OF_FILES			:=	$(words $(SRCS))

COLOR_HEADER		= 	\033[1;31m
COLOR_DONE			= 	\033[1;32m
COLOR_HEADER_CONT	= 	\033[38;5;247m
COLOR_FILE			= 	\033[34m
COLOR_PERCENT		= 	\033[1;94m
COLOR_INFO			=	\033[1;38;5;33m
COLOR_ERROR			=	\033[1;38;5;196m
CLEAR_COLOR			=	\033[m

NAME_PRINT			=	$(COLOR_HEADER)Name$(CLEAR_COLOR)
PROGRESS_BAR_CHAR	=	$(COLOR_PERCENT)▰$(CLEAR_COLOR)
REMAINING_BAR_CHAR 	=	$(COLOR_PERCENT)╍$(CLEAR_COLOR)

ifeq ($(DEBUG_MODE), 1)
	CFLAGS			+=	-DDEBUG
endif

ifeq ($(SYS_ENDIAN), 1)
	CFLAGS			+=	-DSYS_ENDIAN
endif

ifeq ($(NO_SUDO), 1)
	CFLAGS			+=	-DNO_SUDO
endif

define show_git_infos =
	$(eval TMP := $(shell [ -d .git ] && echo "true"))
	$(eval __REPOSITORY_URL__ := $(if $(TMP),$(shell git remote get-url origin 2> /dev/null),Not a git repository))
	$(eval __BRANCH__ := $(if $(TMP), $(shell git branch --show-current 2> /dev/null), $(echo "Not a git repository")))
	$(eval __REPOSITORY__ := $(if $(__REPOSITORY_URL__), $(__REPOSITORY_URL__),Unknown repository))
	$(eval TMP_REPOS := $(COLOR_HEADER)Git repository$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(subst :,!,$(__REPOSITORY__))$(CLEAR_COLOR))
	$(eval TMP_BRANCH := $(if $(TMP),$(COLOR_HEADER)Git branch$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(__BRANCH__)$(CLEAR_COLOR)))
endef

define init_makefile =
	$(eval TMP_NAME = $(NAME_PRINT):$(COLOR_HEADER_CONT)$(NAME)$(CLEAR_COLOR))
	$(eval TMP_AUTHOR = $(COLOR_HEADER)Author$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(AUTHOR)$(CLEAR_COLOR))
	$(eval TMP_CC = $(COLOR_HEADER)Compiler$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(subst @,,$(CC))$(CLEAR_COLOR))
	$(eval TMP_FLAGS = $(COLOR_HEADER)Flags$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(CFLAGS)$(CLEAR_COLOR))
	$(eval TMP_LINKER = $(COLOR_HEADER)Linker flags$(CLEAR_COLOR):$(COLOR_HEADER_CONT)$(LDFLAGS)$(CLEAR_COLOR))
	$(show_git_infos)

	@if [ $(PROGRESS) -eq 0 ]; then \
		clear; \
		tput civis;\
		/bin/echo -e '$(TMP_NAME)\n$(TMP_AUTHOR)\n$(TMP_CC)\n$(TMP_FLAGS)\n$(TMP_LINKER)\n$(TMP_REPOS)$(if $(TMP_BRANCH),\n$(TMP_BRANCH))' \
		| sed -e 's/^ //' | column -t -s':' | tr '!' ':'; \
		echo ""; \
	fi
endef

define draw_bar =
	$(eval MAX := $(shell echo $(shell tput cols) - 1 | bc))
	$(eval CUR := $(shell echo "scale = 3; $(1) / 100 * $(MAX)" | bc | sed -e 's/^\.//'))

	@for i in $(shell seq 1 $(CUR)); do \
		/bin/echo -en '$(PROGRESS_BAR_CHAR)'; \
	done
	@for i in $(shell seq $(CUR) $(MAX)); do \
		/bin/echo -en '$(REMAINING_BAR_CHAR)'; \
	done
	@echo
endef

define clear_line =
	if [ -d .git ]; then \
		tput cup 8 0; \
	else \
		tput cup 7 0; \
	fi; \
	tput el
endef

define clear_progress =
	if [ -d .git ]; then \
		tput cup 9 0; \
	else \
		tput cup 8 0; \
	fi
	tput el
endef

define show_progress =
	$(eval PROGRESS = $(shell expr $(PROGRESS) + 1))
	$(eval TMP := $(shell echo "scale=3;$(PROGRESS) / $(NO_OF_FILES) * 100" | bc))
	$(eval TMP := $(shell echo "$(TMP)" | sed 's/\..*//'))

	@$(clear_line)
	@printf "[$(COLOR_PERCENT)%4d %-2s$(CLEAR_COLOR)]%13s $(COLOR_FILE)$(subst objs/,,$@)$(CLEAR_COLOR)\n" $(TMP) % Compiling
	$(call draw_bar,$(TMP))
endef

define tear_down =
	@if [ $(PROGRESS) -eq 0 ]; then \
		/bin/echo -e "$(COLOR_INFO)No recompilation needed$(CLEAR_COLOR)"; \
	else \
		/bin/echo -e "$(COLOR_DONE)Compilation done \xE2\x9C\x94$(CLEAR_COLOR)"; \
	fi
endef


#######################################################################################################################################################
#######################################################################################################################################################


$(PATH_OBJS)/%.o:	$(PATH_SRCS)/%.c
					$(init_makefile)
					$(show_progress)

					$(shell mkdir -p $(dir $@))
					$(eval TMP_COMP := $(shell $(CC) $(CFLAGS) $(PATH_INC) -MMD -c $< -o $@ 2> /tmp/compilation_error_$(subst /,_,$@) && echo 'true'))
					@if [ '$(TMP_COMP)' != 'true' ]; then \
						tput cnorm; \
						echo ""; \
						cat /tmp/compilation_error_$(subst /,_,$@); \
						exit 2; \
					fi
					@if [ $(PROGRESS) -eq $(NO_OF_FILES) ]; then \
						$(clear_line); \
						tput cnorm; \
					fi

all:				$(NAME)
ifeq ($(NO_SUDO),0)
					@make -s cap
endif
					$(tear_down)

-include $(DEPS)

$(NAME):			check_sudo $(OBJS)
					$(init_makefile)
					@tput cnorm
					@$(clear_progress)
					@echo "Linking $(NAME)..."
					@$(CC) $(OBJS) -o $@ $(LDFLAGS)
					@$(clear_progress)
					@$(clear_line)

cap:
					@[ -f $(NAME) ] && sudo setcap $(CAP_FLAGS) $(NAME)

clean:
					$(RM) -r $(PATH_OBJS)


fclean:				clean
					$(RM) $(NAME) woody

re:					fclean all

check_sudo:
					$(eval TMP := $(shell sudo -nv 2>&1))
					$(eval START := $(firstword $(TMP:,= )))

					@if [ $(NO_SUDO) -eq 0 ]; then \
						if [ "$(START)" = "Sorry" ]; then \
							/bin/echo -e "$(COLOR_ERROR)Sudo is needed to run this makefile$(CLEAR_COLOR)\n"; \
							exit 1; \
						fi; \
					fi

no_sudo:
					@make -s NO_SUDO=0

.PHONY:				all clean fclean re cap check_sudo no_sudo
