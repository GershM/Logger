#include "./lib/global.h"
#include "./lib/tail.h"
#include "./lib/view.h"


int isFileExists(const char *path)
{
    // Try to open file
    FILE *fptr = fopen(path, "r");

    // If file does not exists
    if (fptr == NULL)
        return false;

    // File exists hence close file and return true.
    fclose(fptr);

    return true;
}

static unsigned hasOption(char *name, int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], name) == false)
        {
            return true;
        }
    }
    return false;
}

static char *argsFind(char *flag, int argc, char *argv[])
{
    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], flag) == 0)
        {
            size_t size = strlen(argv[i + 1]);
            char *buffer = (char *)malloc(size);
            sprintf(buffer, "%s", argv[i + 1]);

            return buffer;
        }
    }
    return "";
}

int main(int argc, char *argv[])
{
    if (argc < 2 || hasOption("-h", argc, argv) == true || hasOption("--help", argc, argv) == true)
    {
        printf("Usage: omcLogger [Options]... [File]\n");
        printf("Example: omcLogger -t log.log\n");
        printf("\n");
        printf("Options:\n");
        printf("\t-h, --help: Shows this help message.\n");
        printf("\t-t, --tail: Acts like \"tail -f\" command\n");
        printf("\t-g, --grep [VALUE] : Greping content from the reading content\n");
        printf("\n");

        exit(EXIT_FAILURE);
    }

    char *filename = argv[argc - 1];
    char *find = "";
    int isGrepMode = false;

    if (isFileExists(filename) == false)
    {
        printf("The file '%s' does not exist.\n", filename);
        exit(EXIT_FAILURE);
    }

    if (hasOption("-g", argc, argv) == true)
    {
        find = argsFind("-g", argc, argv);
        isGrepMode = true;
    }

    if (hasOption("--grep", argc, argv) == true)
    {
        find = argsFind("--grep", argc, argv);
        isGrepMode = true;
    }

    if (hasOption("-t", argc, argv) == true || hasOption("--tail", argc, argv) == true)
    {
        tail(filename, isGrepMode, find);
    }
    else
    {
        // printf("\n\t\tThe view option is not working yet use -t or --tail options\n\t\t\t Work in Progress\n");
        view(filename, isGrepMode, find);
    }

    printf("\033[0m\n");
    exit(EXIT_SUCCESS);
}
