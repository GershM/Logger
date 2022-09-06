#include <curses.h>
#include <ncurses.h>
#include <strings.h>
const int STEPSIZE = 50;

/**
 * @brief Window object
 *
 */
typedef struct Window
{
    int x, y, max_x, max_y;
    int lineNumber, columnNumber, maxColumn, length;
    char **lines;
    WINDOW *win;

} Window;

/**
 * @brief
 * Create colors:
 * Error Level , fore, background
 * -------------------------------
 * EMERGENCY, COLOR_RED, COLOR_YELLOW
 * ALERT, COLOR_RED, COLOR_BLACK
 * FATAL, COLOR_RED, COLOR_BLACK
 * ERROR, COLOR_RED, COLOR_BLACK
 * WARNING, COLOR_YELLOW, COLOR_BLACK
 * NOTICE, COLOR_CYAN, COLOR_BLACK
 * INFO, COLOR_BLUE, COLOR_BLACK
 * DEBUG, COLOR_BLACK, COLOR_GREEN
 */
void pairVColors()
{
    init_pair(EMERGENCY, COLOR_RED, COLOR_YELLOW);
    init_pair(ALERT, COLOR_RED, COLOR_BLACK);
    init_pair(FATAL, COLOR_RED, COLOR_BLACK);
    init_pair(ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(WARNING, COLOR_YELLOW, COLOR_BLACK);
    init_pair(NOTICE, COLOR_CYAN, COLOR_BLACK);
    init_pair(INFO, COLOR_BLUE, COLOR_BLACK);
    init_pair(DEBUG, COLOR_BLACK, COLOR_GREEN);
}

/**
 * @brief Load file to memmory
 *
 * @param filename
 * @param length
 * @return char**
 */
char **loadfile(char *filename, int *length)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Can't open file %s.\n", filename);
        return NULL;
    }
    ssize_t read;
    size_t len = 0;
    int arrLen = STEPSIZE;
    char *line = NULL;
    char **lines = (char **)malloc(arrLen * sizeof(char *));
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i == arrLen)
        {
            arrLen += STEPSIZE;
            char **newlines = realloc(lines, arrLen * sizeof(char *));
            if (!newlines)
            {
                printf("The memory is full, cannot read lines.\n");
                return NULL;
            }
            lines = newlines;
        }

        lines[i] = (char *)malloc(read - 1);
        strcpy(lines[i], line);
        i++;
    }
    *length = i;
    return lines;
}

Window initWindow(char *filename)
{
    /* initialize curses */
    initscr();
    cbreak();
    noecho();
    clear();
    keypad(stdscr, TRUE);

    /* Initialize colors */
    start_color();
    pairVColors();

    /* initialize window parameters */
    int x = 0, y = 0, max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    /* initialize fileLines*/
    int length = 0;
    int maxColumn = 0;
    char **lines = loadfile(filename, &length);

    /* initialize main window Struct*/
    Window mwin = {
        max_x : max_x - 1,
        max_y : max_y - 1,
        x : x,
        y : y,
        lineNumber : 0,
        columnNumber : 0,
        maxColumn : 0,
        length : length,
        lines : lines,
        win : stdscr
    };

    return mwin;
}

void command(Window *mwin)
{
    char command = getch();
    switch ((unsigned int)command)
    {
    case 'q':
        keep_running = 0;
        break;
    case KEY_DOWN:
    case 2:
        if (mwin->lineNumber < mwin->max_y)
        {
            mwin->lineNumber++;
            break;
        }
        mwin->lineNumber = mwin->max_y;
        break;
    case KEY_UP:
    case 3:
        if (mwin->lineNumber > 0)
        {
            mwin->lineNumber--;
            break;
        }
        mwin->lineNumber = 0;
        break;
    case KEY_LEFT:
    case 4:
        if (mwin->columnNumber > 0)
        {
            mwin->columnNumber--;
            break;
        }
        mwin->columnNumber = 0;
        break;
    case KEY_RIGHT:
    case 5:
        if (mwin->maxColumn > mwin->columnNumber)
        {
            mwin->columnNumber++;
            break;
        }
        mwin->columnNumber = mwin->maxColumn;
        break;
    default:
        break;
    }
}

/**
 * @brief split the string to chunks
 *
 * @param str
 * @param size
 * @param chunckCount
 * @return char**
 */
char **stringToChunks(char *str, size_t size, int *chunckCount)
{
    int length = strlen(str);
    size_t chunksSize = (length / size);

    char **spleated = (char **)malloc(chunksSize + 1);

    for (size_t i = 0, j = 0; i < chunksSize; i++, j += size)
    {
        char *subbuff = (char *)malloc(size);
        memcpy(subbuff, &str[j], size);
        spleated[i] = subbuff;
    }

    *chunckCount = chunksSize;
    return spleated;
}

/**
 * @brief
 *
 * @param filename
 * @param isGrepMode
 * @param find
 */
void view(char *filename, int isGrepMode, char *find)
{
    Window mwin = initWindow(filename);
    if (has_colors() == false)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    do
    {
        clear();
        for (int i = mwin.lineNumber, written = 0; i < mwin.length && written < mwin.max_y; i++, written++)
        {
            size_t line_length = strlen(mwin.lines[i]);
            char line[line_length];
            memcpy(line, mwin.lines[i], line_length);

            if (isGrepMode == false || grep(line, find) == true)
            {
                mwin.maxColumn = line_length / mwin.max_x;
                if (mwin.maxColumn > mwin.columnNumber)
                {
                    char *subbuff = (char *)malloc(mwin.max_x);
                    memcpy(subbuff, &line[mwin.columnNumber * mwin.max_x], mwin.max_x);
                    int color = setVColor(line);
                    attron(COLOR_PAIR(color));
                    printw("%s\n", subbuff);
                    attroff(COLOR_PAIR(color));
                    free(subbuff);
                }
                else
                {
                    printw("\n");
                }
            }
        }
        wrefresh(mwin.win);
        command(&mwin);
    } while (keep_running);

    endwin();
}
