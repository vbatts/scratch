/*

 Copyright 2011  Vincent Batts, Vienna, VA, USA
 All rights reserved.

 Redistribution and use of this source, with or without modification, is
 permitted provided that the following conditions are met:

 1. Redistributions of this source must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HAVE_STATE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

const int IN = 0;
const int OUT = 1;

const int MAX_SPACER = 10;

const char* FG_ESCAPE = "\x1b[38;5;";
const char* BG_ESCAPE = "\x1b[48;5;";
const char* CLEAR = "\x1b[0m";

// Color Tone:
// We set it up here so others can use it.
// 0 is black, so we'll steer from that.
int tone = 1;

#ifdef HAVE_STATE
const char* file = "/tmp/lolcat.state";

int get_start_tone(int default_ret) {
	struct stat s;

	if (stat(file, &s) == -1)
	{
		return default_ret;
	}

	if (S_ISREG(s.st_mode) && s.st_mode & S_IRUSR)
	{
		FILE *in_state;
		char buf[4];
		int i;

		in_state = fopen(file, "r");
		read(fileno(in_state), buf, 3);
		i = atoi(buf);
		close(fileno(in_state));
		if (i > 0 && i <= 256)
			return i;
	}
	
	return default_ret;
}

void set_start_tone() {
	struct stat s;

	if (stat(file, &s) == -1)
	{
		printf("foo\n");
		return ;
	}

	if (S_ISREG(s.st_mode) && s.st_mode & S_IWUSR)
	{
		FILE *out_state;
		char buf[4];

		sprintf(buf, "%d", tone);
		out_state = fopen(file, "w");
		write(fileno(out_state), buf, 3);
		close(fileno(out_state));
	}
}
#endif

void set_escape_clear()
{
	printf("%s", CLEAR);
}

void set_escape_color(int color)
{
	// "\x1b[38;5;123m"
	printf("%s%dm", FG_ESCAPE, color);
}

void cleanup(int sig)
{
	set_escape_clear();
#ifdef HAVE_STATE
	set_start_tone();
#endif
	signal(sig, SIG_DFL);
}

int main()
{
	char buf[MAX_SPACER];
	char esc[255];
	int r;

	int increasing = 1;
	int spacer = 0;
	int i;

#ifdef HAVE_STATE
	tone = get_start_tone(tone);
#endif

	// This way we won't leave a painted mess behind ourselves :)
	// FIXME: SIGPIPE is not doing as it should :( 
	signal( SIGINT, cleanup);
	signal( SIGPIPE, cleanup);
	signal( SIGQUIT, cleanup);
	signal( SIGHUP, cleanup);
	signal( SIGABRT, cleanup);
	signal( SIGCHLD, cleanup);
	signal( SIGUSR1, cleanup);
	signal( SIGUSR2, cleanup);

	while (1)
	{
		r = read(IN, buf, MAX_SPACER);
		if (r == 0)
			break;

		set_escape_color(tone);

		// 'r' is the number of items in the buffer 'buf'
		for (i = 0; i < r; i++)
			putchar(buf[i]);

		if (increasing)
			tone++;
		else
			tone--;

		if (tone == 255)
			increasing = 0;
		else if (tone == 1)
			increasing = 1;
	}

	set_escape_clear();
#ifdef HAVE_STATE
	set_start_tone();
#endif

	return 0;
}
