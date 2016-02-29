%{
/* mousse: CFD toolbox
 * Copyright (C) 2011-2014 OpenFOAM Foundation
 * Copyright (C) 2016 mousse project
 * Application
 *     wmkdep
 * Description
 *     A fast dependency list generator that emulates the behaviour and the
 *     output of cpp -M. However, the output contains no duplicates and
 *     is approx. 40% faster than cpp.
 *
 *     The algorithm uses flex to scan for includes and searches the files
 *     found.  Each file is entered into a hash table so that files are scanned
 *     only once.  This is why this program is faster than cpp.
 *
 * Usage
 *     wmkdep [-v] [ -Idir ... -Idir ] [ -iheader .. -iheader ] filename
 */

#define FILE_STACK_SIZE 300
#define HASH_TABLE_SIZE 500

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void next_file(const char* fileName);
void import_file(const char* fileName);
void import_dir(const char* dirName);

#   undef yywrap        /* sometimes a macro by default */

%}

%x CMNT CFNAME SCFNAME JFNAME FFNAME
%%

"//".*\n                  ;            /* remove c++ style one line comments */

"/*"                      BEGIN(CMNT);    /* start removing c style comment */
<CMNT>.|\n                ;
<CMNT>"*/"                BEGIN(INITIAL); /* end removing c style comment */

^[ \t]*#[ \t]*include[ \t]+\"   BEGIN(CFNAME);  /* c-file name */
<CFNAME>[^"\n ]*        { BEGIN(INITIAL); next_file(yytext); } /*"*/

"      "include[ \t]+\'   BEGIN(FFNAME);  /* FORTRAN-file name */
<FFNAME>[^']*           { BEGIN(INITIAL); next_file(yytext); } /*'*/

.|\t|\n                   ;

%%


/* char* entry in hash table */
struct hash_entry
{
    char* name;
    struct hash_entry* next;
};


/*
 * lookup name in hash table.
 * if found - return 1
 * if not found - insert in table and return 0
 */
int lookup(struct hash_entry** hash_table, const char* p)
{
    int ii = 0;
    struct hash_entry* n;
    struct hash_entry* nn;

    /* hash */
    const char* pp = p;
    while (*pp) ii = ii<<1 ^ *pp++;
    if (ii < 0) ii = -ii;
    ii %= HASH_TABLE_SIZE;

    /* search */
    for (n=hash_table[ii]; n; n=n->next)
    {
        if (strcmp(p, n->name) == 0)
        {
            /* entry found so return true */
            return 1;
        }
    }

    /* insert */
    nn = (struct hash_entry*)malloc(sizeof(struct hash_entry));
    nn->name = strdup(p);
    nn->next = hash_table[ii];
    hash_table[ii] = nn;

    /* entry not found, and therefore added. return false */
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int ndirectories = 0;
char** directories;
char* source_file = NULL;
char* dep_file = NULL;

/* Set of files already visited */
struct hash_entry* visited_files[HASH_TABLE_SIZE];

/* Buffer pointer stack counter */
int current_buffer = 0;

/* Buffer pointer stack */
YY_BUFFER_STATE buffers[FILE_STACK_SIZE];

/* Directory paths for the loaded files */
const char* buffer_paths[FILE_STACK_SIZE];


int main(int argc, char* argv[])
{
    char *base_pos, *dot_pos;
    int i;
    static char verbose;

    if (argc == 1)
    {
        fprintf(stderr, "input file not supplied\n");
        exit(1);
    }

    source_file = strdup(argv[argc-1]);
    verbose = (strncmp(argv[1], "-v", 2) == 0);

    if (verbose)
    {
        fprintf(stderr, "Making dependency list for source file %s\n",
                source_file);
    }
    else
    {
        fprintf(stderr, " [DEP] %s\n", source_file);
    }


    if ((base_pos = strrchr(source_file, '/')) == NULL)
    {
        base_pos = source_file;
    }
    else
    {
        base_pos++;
    }

    if
    (
        (dot_pos = strrchr(source_file, '.')) == NULL
     || (dot_pos < base_pos)
    )
    {
        fprintf
        (
            stderr,
            "cannot find extension in source file name %s\n",
            source_file
        );
        exit(1);
    }

    /* count number of -I directories */
    ndirectories = 0;
    for (i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "-I", 2) == 0)
        {
            if (strlen(argv[i]) > 2)
            {
                ndirectories++;
            }
        }
    }

    directories = (char**)malloc(sizeof(char*)*ndirectories);

    /* build list of -I directories and add -i ignores */
    ndirectories = 0;
    for (i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "-I", 2) == 0)
        {
            if (strlen(argv[i]) > 2)
            {
                directories[ndirectories++] = strdup(argv[i] + 2);
            }
        }
        else if (strncmp(argv[i], "-i", 2) == 0)
        {
            if (strlen(argv[i]) > 2)
            {
                lookup(visited_files, (argv[i] + 2));
            }
        }
    }

    /*
     * initialise depFile to zero and use strncat rather than strncpy
     * because there is a bug in the SGI strncat that if 0 preceeds the '.'
     * it inserts a space
     */
    dep_file = (char*)malloc(strlen(source_file) + 20);
    dep_file[0] = 0;
    strcat(dep_file, "$(OBJECTS_DIR)/");
    strcat(dep_file, source_file);
    strcat(dep_file, ".dep");

    char *object_file = strdup(base_pos);
    object_file[(dot_pos - base_pos)/sizeof(char)] = 0;

    /* printf("$(OBJECTS_DIR)/%s.o: %s\n", objectFile, depFile); */
    printf("%s: \\\n", dep_file);
    free(object_file);

    /* Initialize buffer path for current_buffer */
    buffer_paths[current_buffer] = NULL;

    next_file(source_file);
    yylex();

    puts("\n");

    for (i = 0; i < ndirectories; i++)
    {
        free(directories[i]);
    }
    free(directories);

    free(source_file);
    free(dep_file);

    return 0;
}


/*
 * Add a directory name to the file name
 */
char* add_directory_name(const char* dir_name, const char* file_name)
{
    char* path_name = (char*)malloc(strlen(dir_name) + strlen(file_name) + 2);
    strcpy(path_name, dir_name);

    if (dir_name[strlen(dir_name)-1] != '/')
    {
        strcat(path_name, "/");
    }

    strcat(path_name, file_name);

    return path_name;
}


/*
 * open a file and create buffer and put on stack
 */
void next_file(const char* file_name)
{
    if (lookup(visited_files, file_name))
    {
        return;   /* already existed (did not insert) */
    }

    if (current_buffer >= FILE_STACK_SIZE)
    {
        fprintf
        (
            stderr,
            "depth of file search exceeds stack size %d "
            "while opening %s for file %s\n",
            FILE_STACK_SIZE, file_name, source_file
        );
        exit(1);
    }

    /* Pointer to new file which is set if the file is successfully opened */
    FILE* newyyin = NULL;

    /* Check if the file has same path as the file in the current buffer */
    if (buffer_paths[current_buffer] != NULL)
    {
        char* path_name = add_directory_name(buffer_paths[current_buffer],
					     file_name);

        if ((newyyin = fopen(path_name, "r")))
        {
            printf("%s \\\n", path_name);

            buffers[current_buffer++] = YY_CURRENT_BUFFER;
            buffer_paths[current_buffer] = buffer_paths[current_buffer - 1];

            yy_switch_to_buffer(yy_create_buffer(newyyin, YY_BUF_SIZE));

            free(path_name);

            return;
        }

        free(path_name);
    }

    if (!(newyyin = fopen(file_name, "r")))
    {
        int d;
        for (d=0; d<ndirectories; d++)
        {
            char* path_name = add_directory_name(directories[d], file_name);

            if ((newyyin = fopen(path_name, "r")))
            {
                printf("%s \\\n", path_name);

                buffers[current_buffer++] = YY_CURRENT_BUFFER;
                buffer_paths[current_buffer] = directories[d];

                yy_switch_to_buffer(yy_create_buffer(newyyin, YY_BUF_SIZE));

                free(path_name);

                return;
            }

            free(path_name);
        }

        if (ndirectories == 0)
        {
            fprintf
            (
                stderr,
                "could not open file %s for source file %s\n",
                file_name, source_file
            );
        }
        else
        {
            fprintf
            (
                stderr,
                "could not open file %s for source file %s due to %s\n",
                file_name, source_file, strerror(errno)
            );
        }

        fflush(stdout);
        fflush(stderr);

        /* only report the first occurance */
        lookup(visited_files, file_name);
    }
    else
    {
        printf("%s \\\n", file_name);
        fflush(stdout);

        buffers[current_buffer++] = YY_CURRENT_BUFFER;
        buffer_paths[current_buffer] = NULL;

        yy_switch_to_buffer(yy_create_buffer(newyyin, YY_BUF_SIZE));
    }
}


/*
 * Replace all '.' with '/'
 */
void dot_to_slash(char* file_name)
{
    int i, len;
    len = strlen(file_name);

    for (i=0; i<len; i++)
    {
        if (file_name[i] == '.') file_name[i] = '/';
    }
}


/*
 * The lexer calls yywrap to handle EOF conditions
 */
int yywrap()
{
    /* Close the file for the buffer which has just reached EOF */
    /* This causes strange problems on some systems
    fclose(yyin);
    yyin = 0;
    */

    /* Delete the buffer */
    yy_delete_buffer(YY_CURRENT_BUFFER);

    /* Set buffer counter to previous buffer */
    current_buffer--;

    if (current_buffer >= 0) /* if buffer counter refers to a valid file */
    {
        /* reset input buffer to the previous buffer on the stack */
        yy_switch_to_buffer(buffers[current_buffer]);

        /* Return to the normal state for the previous buffer on the stack */
        BEGIN(INITIAL);

        /* return 0 to inform lex to continue reading */
        return 0;
    }
    else                 /* else there are no more buffers on the stack */
    {
        /* return 1 to inform lex finish now that all buffers have been read */
        return 1;
    }
}
