#include <stdio.h>
#include <stdlib.h>

#define V void
#define P ()
#define BEGIN {
#define END }

#define MAX(x,y) (x>y) ? x : y

V cool P
BEGIN

END

#define A(name, type, N) type name[N]

#define M(type, N) (type*)malloc(sizeof(type)*N)


//
//https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html#Concatenation
void quit_command()
{

}

void help_command()
{

}

struct command
{
	const char *name;
	void(*function) (void);
};

#define COMMAND(NAME)  { #NAME, NAME ## _command }

command commands[] =
{
  COMMAND(quit), // "quit", quit_command
  COMMAND(help)
};

int main()
{
	int a = 2;
	int b = 7;
	printf("%d\n", MAX(a,b));

	A(mark, int, 10);
	A(chinh, float, 10);
	A(hollie, int, 10);
	A(wai, char*, 10);

	int *p = M(int, 100);

	getchar();
	return 0;
}