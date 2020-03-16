#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum VERSION_RESULT
{
    //FAIL = -1,
    LEFT = 1,
    RIGHT = 2,
    EQUAL = 3
};

typedef struct _version
{
    unsigned int major;
    unsigned int minor;
    unsigned int build;
    unsigned int revision;
} VERSION;

VERSION *version_new(char *str);
int version_parse(VERSION *pVer, char *str);
bool validate_version_string(char *str);
void version_print(VERSION *pVer);
void version_compare_print(VERSION *pL, VERSION *pR);
bool consecutive_chars(char *const str, char c);

int main(void) {
    char ver_string[] = {'5','.','5','.','5','.','0','\0'}; // CANNOT use read-only memory due to strtok
    char ver_string2[] = {'5','.','1','.','5','.','1','\0'};
    VERSION *ver = version_new(ver_string);
    VERSION *ver2 = version_new(ver_string2);
    version_compare_print(ver, ver2);
    free(ver);
    free(ver2);
    ver = NULL;
    ver2 = NULL;
    return EXIT_SUCCESS;
}

VERSION *version_new(char *str)
{
    assert(str);

    VERSION *pVer = malloc(sizeof(VERSION));
    assert(pVer);
    int result = version_parse(pVer, str);
    if(result == -1)
    {
        fprintf(stderr, "Failed to parse version from string.\n");
        free(pVer);
        return NULL;
    }
    return pVer;
}

int version_parse(VERSION *pVer, char * str)
{
    assert(pVer && str);
    if(!validate_version_string(str))
    {
        fprintf(stderr, "Invalid version #. Format xxx.xxx.xxx.xxx\n");
        return -1;
    }
    char *token;
    char *delim = ".";

    token = strtok(str, delim);
    pVer->major = (unsigned int) atoi(token);
    token = strtok(NULL, delim);
    pVer->minor = (unsigned int) atoi(token);
    token = strtok(NULL, delim);
    pVer->build = (unsigned int) atoi(token);
    token = strtok(NULL, delim);
    pVer->revision = (unsigned int) atoi(token);

    return 0;

}

bool validate_version_string(char *str)
{
    assert(str);
    unsigned int count = 0;
    char *copy;
    char tmp;
    for(copy = str; *copy != '\0'; ++copy)
    {
        tmp = *copy;
        if(tmp == '.') ++count;
        if((tmp > '9' || tmp < '0') && tmp != '.') return false;
    }
    if(consecutive_chars(str, '.')) return false;
    if(count == 3) return true;
    return false;
}

void version_print(VERSION *pVer)
{
    assert(pVer);
    printf("%u.%u.%u.%u", pVer->major,pVer->minor,pVer->build,pVer->revision);
    return;
}

/*
 * Compare 2 versions. Return -1 if error, 1 if left is largest, 2 if right is largest, 3 if equal
 */
enum VERSION_RESULT version_compare(VERSION *pL, VERSION *pR)
{
    assert(pL && pR);

    //Major checks
    if(pL->major > pR->major) return LEFT;
    if(pR->major > pL->major) return RIGHT;
    //Majors are equal
    if(pL->minor > pR->minor) return LEFT;
    if(pR->minor > pL->minor) return RIGHT;
    //Minors are equal
    if(pL->build > pR->build) return LEFT;
    if(pR->build > pL->build) return RIGHT;
    //Builds are equal
    if(pL->revision > pR->revision) return LEFT;
    if(pR->revision > pL->revision) return RIGHT;
    //Revisions are equal
    return EQUAL;
}
void version_compare_print(VERSION *pL, VERSION *pR)
{
    assert(pL && pR);

    switch(version_compare(pL, pR))
    {
        case LEFT:
            version_print(pL);
            printf(" > ");
            version_print(pR);
            putchar('\n');
            break;
        case RIGHT:
            version_print(pL);
            printf(" < ");
            version_print(pR);
            putchar('\n');
            break;
        case EQUAL:
            version_print(pL);
            printf(" == ");
            version_print(pR);
            putchar('\n');
            break;
        default:
            fprintf(stderr, "An error occurred in the version compare function.\n");
    }
    return;
}

/*
 * Does str have consecutive c?
 */
bool consecutive_chars(char *const str, char c)
{
    assert(str);
    unsigned char consecutive;
    size_t i, len = strlen(str);

    for(i = 0; i < len; ++i)
    {
        if(i > 0 && str[i] == c)
        {
            if(str[i-1] == str[i]) return true;
        }
        if((i + 1) < len && str[i] == c)
        {
            if(str[i+1] == str[i]) return true;
        }
    }
    return false;
}