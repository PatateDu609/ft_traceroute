#include "ft_getopt.h"
#include "defines.h"

static void init_help(t_option *option)
{
	option->name = "help";
	option->short_name = 0; // disables short option
	option->description = "Display this help message and exit.";
	option->need_value = 0;
	option->check = NULL;
	option->value = NULL;
	option->flag = OPT_H;
}

static void init_t(t_option *option)
{
	option->name = "tcp";
	option->short_name = 'T';
	option->description = "Use TCP SYN for probes.";
	option->need_value = 0;
	option->check = NULL;
	option->value = NULL;
	option->flag = OPT_T;
}

static void init_i(t_option *option)
{
	option->name = "icmp";
	option->short_name = 'I';
	option->description = "Use ICMP ECHO for probes.";
	option->need_value = 0;
	option->check = NULL;
	option->value = NULL;
	option->flag = OPT_I;
}

static void init_n(t_option *option)
{
	option->name = NULL;
	option->short_name = 'n';
	option->description = "Do not try to map IP addresses to host names when displaying them.";
	option->need_value = 0;
	option->check = NULL;
	option->value = NULL;
	option->flag = OPT_N;
}

t_option *init_options(int *nb)
{
	*nb = 4;
	if (!*nb)
		return NULL;
	t_option *options = malloc(sizeof(t_option) * *nb);
	if (!options)
		return NULL;
	init_help(&options[0]);
	init_t(&options[1]);
	init_i(&options[2]);
	init_n(&options[3]);

	return options;
}
