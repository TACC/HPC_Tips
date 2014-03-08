#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/ioctl.h>


void printwrap(const char *s, int lineSize, const char *prefix) {
    const char *head = s;
    int pos, lastSpace;

    pos = lastSpace = 0;

    while(head[pos]!=0) {

        int isLf = (head[pos]=='\n');

        if (isLf || pos==lineSize) {

            if (isLf || lastSpace == 0) { lastSpace = pos; } // just cut it

            if (prefix!=NULL) { printf("%s", prefix); }

            while(*head!=0 && lastSpace-- > 0) { printf("%c", *head++); }

            printf("\n");

            if (isLf) { head++; } // jump the line feed

            while (*head!=0 && *head==' ') { head++; } // clear the leading space

            lastSpace = pos = 0;

        } else {
            if (head[pos]==' ') { lastSpace = pos; }
            pos++;
        }
    }
    printf("%s\n", head);
}
 
int main(){
    struct winsize w;
    int i, twidth;
    char *text = "0        1         2         3         4         5\n12345678901234567890123456789012345678901234567890123456789012345678901234567890\nHi there.  I'm working on a                          little project that I'm going to release for several systems. I need help on how to write a simple word wrap function. The console can be everything from 18 characters wide and upwards.\n\nI'd like to make a function for this myself, but after an hour of searching for a C word wrap function (that I can understand and is usable), I decided to come for help. Obviously, the function needs to be able to work with different terminal widths, as I'm releasing it for several platforms.\n";

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    twidth = w.ws_col - 2;

    printwrap(text, twidth, "  ");
    return 0;
}
