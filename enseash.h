#define COMMAND_SIZE 50
#define PRINT_SIZE 80
#define PATH_SIZE 150
#define StartMsg "Welcome to ENSEA Tiny Shell \nPour quitter, tapez 'exit'. \nenseash%"


int main(int argc, char **argv);

void ChildState(int state,long exectime);

char** FormatArgs(char *args);

int DetectChar(char **args,char * find);

int SpaceCount(char *);

void Display(char *);

char** ResToFile(char **args);




