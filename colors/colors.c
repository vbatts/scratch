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

const int WRAP = 20;

const char* FG_ESCAPE = "\x1b[38;5;";
const char* BG_ESCAPE = "\x1b[48;5;";
const char* CLEAR = "\x1b[0m";

// Color Tone:
// We set it up here so others can use it.
// 0 is black, so we'll steer from that.
int tone = 0;

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
	signal(sig, SIG_DFL);
}

int main()
{
	int count = 0;
	int i;

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

	for (i = 0; i < 256; i++)
	{
		set_escape_color(tone);

		printf("%4.3d", tone);

		tone++;
		count++;
		if (count == WRAP)
		{
			printf("\n");
			count = 0 ;
		}

	}

	set_escape_clear();
	printf("\n");

	return 0;
}
