#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define true 1
#define false 0

#define EMERGENCY 1
#define ALERT 2
#define FATAL 3
#define ERROR 4
#define WARNING 5
#define NOTICE 6
#define INFO 7
#define DEBUG 8

static volatile sig_atomic_t keep_running = true;

/**
 * @brief Catch CTL-C char
 *
 * @param _
 */
static void sig_handler(int _)
{
    (void)_;
    keep_running = false;
}

/**
 * @brief Set the Color object
 *
 * @param line
 * @param size
 * @return char*
 */
char *setTColor(char *line, size_t size)
{
    char *buffer = (char *)malloc(size + 50);
    if (strstr(line, "level=\"emergency\"") != NULL)
    {
        sprintf(buffer, "\033[1;101m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"alert\"") != NULL)
    {
        sprintf(buffer, "\033[31m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"fatal\"") != NULL)
    {
        sprintf(buffer, "\033[1;31m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"error\"") != NULL)
    {
        sprintf(buffer, "\033[31m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"notice\"") != NULL)
    {
        sprintf(buffer, "\033[1;33m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"info\"") != NULL)
    {
        sprintf(buffer, "\033[36m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"warning\"") != NULL)
    {
        sprintf(buffer, "\033[33m%s\033[0m", line);
    }
    else if (strstr(line, "level=\"debug\"") != NULL)
    {
        sprintf(buffer, "\033[0;32m%s\033[0m", line);
    }
    else
    {
        sprintf(buffer, "%s", line);
    }
    return buffer;
}

/**
 * @brief Set the Color object
 *
 * @param line
 * @return int
 */
int setVColor(char *line)
{
    if (strstr(line, "level=\"emergency\"") != NULL)
    {
        return EMERGENCY;
    }
    else if (strstr(line, "level=\"alert\"") != NULL)
    {
        return ALERT;
    }
    else if (strstr(line, "level=\"fatal\"") != NULL)
    {
        return FATAL;
    }
    else if (strstr(line, "level=\"error\"") != NULL)
    {
        return ERROR;
    }
    else if (strstr(line, "level=\"notice\"") != NULL)
    {
        return NOTICE;
    }
    else if (strstr(line, "level=\"info\"") != NULL)
    {
        return INFO;
    }
    else if (strstr(line, "level=\"warning\"") != NULL)
    {
        return WARNING;
    }
    else if (strstr(line, "level=\"debug\"") != NULL)
    {
        return DEBUG;
    }
    return 0;
}

/**
 * @brief
 *
 * @param char * line
 * @param char * find
 * @return int
 */
int grep(char *line, char *find)
{
    if (strstr(line, find) != NULL)
    {
        return true;
    }
    return false;
}
