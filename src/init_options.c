#include "ft_getopt.h"
#include "defines.h"

int ft_is_number(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

int ft_chk_method(char *str)
{
	for (int i = 0, max = ft_strlen(str); i < max; i++)
		str[i] = ft_tolower(str[i]);

	if (!ft_strcmp(str, "icmp"))
		return (1);
	else if (!ft_strcmp(str, "udp"))
		return (1);
	else
		return (0);
}

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

static void init_I(t_option *option)
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
	option->flag = OPT_n;
}

static void init_f(t_option *option)
{
	option->name = "first-hop";
	option->short_name = 'f';
	option->description = "Set the first hop TTL.";
	option->need_value = 1;
	option->check = ft_is_number;
	option->value = NULL;
	option->arg_help = "TTL";
	option->small_print = "start";
	option->flag = OPT_f;
}

static void init_m(t_option *option)
{
	option->name = "max-hops";
	option->short_name = 'm';
	option->description = "Set the maximum number of hops.";
	option->need_value = 1;
	option->check = ft_is_number;
	option->arg_help = "Number of hops";
	option->small_print = "num";
	option->value = NULL;
	option->flag = OPT_m;
}

static void init_M(t_option *option)
{
	option->name = "type";
	option->short_name = 'M';
	option->description = "Set the type of the probe. Can be 'icmp' or 'udp'.";
	option->need_value = 1;
	option->check = ft_chk_method;
	option->arg_help = "Type of probe can be either 'icmp' or 'udp'.";
	option->small_print = "method";
	option->value = NULL;
	option->flag = OPT_M;
}

static void init_p(t_option *option)
{
	option->name = "port";
	option->short_name = 'p';
	option->description = "Set the port number of the probe. (default is 33434)";
	option->need_value = 1;
	option->check = ft_is_number;
	option->arg_help = "Port number";
	option->small_print = "num";
	option->value = NULL;
	option->flag = OPT_p;
}

static void init_w(t_option *option)
{
	option->name = "wait";
	option->short_name = 'w';
	option->description = "Set the time to wait between probes. (default is 3)";
	option->need_value = 1;
	option->check = ft_is_number;
	option->arg_help = "Time in seconds";
	option->small_print = "time";
	option->value = NULL;
	option->flag = OPT_w;
}

static void init_q(t_option *option)
{
	option->name = "tries";
	option->short_name = 'q';
	option->description = "Set the number of probes per hop. (default is 3)";
	option->need_value = 1;
	option->check = ft_is_number;
	option->arg_help = "Number of probes";
	option->small_print = "num";
	option->value = NULL;
	option->flag = OPT_q;
}

t_option *init_options(int *nb)
{
	*nb = 9;
	if (!*nb)
		return NULL;
	t_option *options = malloc(sizeof(t_option) * *nb);
	if (!options)
		return NULL;
	init_help(&options[0]);
	init_I(&options[1]);
	init_n(&options[2]);
	init_f(&options[3]);
	init_m(&options[4]);
	init_M(&options[5]);
	init_p(&options[6]);
	init_w(&options[7]);
	init_q(&options[8]);

	return options;
}
