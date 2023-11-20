#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define LIB_NAME "freebind.so"

#ifndef LIB_DIR
#define LIB_DIR "/usr/local/lib"
#endif

static int help(char *name)
{
	fprintf(stderr, "Usage: %s [options] program [arguments]\n"
			"  -r  --random  IPv4 or IPv6 network in CIDR notation.\n"
			"  -t  --type    Only apply to the specified socket type. Either STREAM or DGRAM.\n", name);
	return EXIT_FAILURE;
}

int main(int argc, char **argv)
{
	char buf[8192];
	char *orig_env = getenv("LD_PRELOAD");
	int opt = 0;
	int option_index = 0;
	size_t buflen = 0;

	static struct option long_options[] =
	{
		{"random", required_argument, 0, 'r'},
		{"type", required_argument, 0, 't'},
		{0, 0, 0, 0}
	};

	if(argc < 2)
	{
		return help(argv[0]);
	}

	buf[0] = 0;
	opterr = 0;
	while((opt = getopt_long(argc, argv, "r:t:", long_options, &option_index)) != -1)
	{
		switch(opt)
		{
			case 'r':
				buflen = strlen(buf);
				snprintf(buf + buflen, sizeof(buf) - buflen, "%s ", optarg);
				break;
			case 't':
				if(strcasecmp(optarg, "stream") == 0)
				{
					putenv("FREEBIND_TYPE_FILTER=STREAM");
				}
				else if(strcasecmp(optarg, "dgram") == 0)
				{
					putenv("FREEBIND_TYPE_FILTER=DGRAM");
				}
				else
				{
					fprintf(stderr, "Invalid filter type.\n");
					exit(1);
				}
				break;
			default:
				optind -= 1;
				goto endwhile;
		}
	}
endwhile:

	if(optind >= argc)
	{
		fprintf(stderr, "No program specified.\n");
		return EXIT_FAILURE;
	}

	buf[sizeof(buf) - 1] = 0;
	setenv("FREEBIND_RANDOM", buf, 1);

	snprintf(buf, sizeof(buf), LIB_DIR "/" LIB_NAME "%s%s",
			orig_env ? " " : "",
			orig_env ? orig_env : "");
	buf[sizeof(buf) - 1] = 0;
	setenv("LD_PRELOAD", buf, 1);

	execvp(argv[optind], &argv[optind]);
	snprintf(buf, sizeof(buf), "Failed to open program \"%s\"", argv[optind]);
	buf[sizeof(buf) - 1] = 0;
	perror(buf);
	return help(argc > 0 ? argv[0] : "freebind");
}
