
/**
 * @brief
 *
 * @param filename
 */
void tail(char *filename, int isGrepMode, char *find)
{
    ssize_t read;
    size_t len = 0;
    char *line = NULL;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Failed To open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sig_handler);

    while (keep_running)
    {
        if (fseek(fp, 0, SEEK_END))
        {
            perror("fseek() failed");
        }

        sleep(1);

        while ((read = getline(&line, &len, fp)) != -1)
        {
            if (isGrepMode == false || grep(line, find) == true)
            {
                char *str = setTColor(line, len);
                printf("%s", str);
            }
        }

        fflush(fp);
    }
}
