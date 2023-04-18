
/*
   This file contains routines to partially implement the functionality
   of Scott Shaffers FORTRAN RDF reader tools. Specifically, the routines
   necessary to implement the SRTM mosaicking code are included. Additional
   functionality can be added as needed.
*/
#include "ctype.h"
#include "stdio.h"
#include "SRTMrdf.h"
#include "stdarg.h"
#include "string.h"
#include <stdlib.h>
#include "clib/standard.h"
#define MAXLINE 1024

static char *stripWS(char *string);
static char *getRDFLine(char *bufferSpace, FILE *fp);

/*
   Write data in rdf format to a file
*/
void rdfWrite(FILE *fp, char *keyword, char *units,
              char *dimensions, char op, char *value, char *comment)
{
    char *com;
    char unitString[MAXLINE], dimensionString[MAXLINE];
    /*
       Comment only line
    */
    if (keyword == NULL)
    {
        fprintf(fp, "! %-79s\n", comment);
        return;
    }
    if (units == NULL)
        sprintf(unitString, "(-)");
    else
        sprintf(unitString, "(%s)", units);

    if (dimensions == NULL)
        sprintf(dimensionString, " ");
    else
        sprintf(dimensionString, "[%s]", dimensions);

    if (value == NULL)
        value = " ";
    if (op != '=' || op != '<' || op != '>')
        op = '=';
    if (comment == NULL)
    {
        com = " ";
        comment = " ";
    }
    else
        com = "! ";
    fprintf(fp, "%-30s %-18s %-10s %c %-18s %-2s %s\n",
            keyword, unitString, dimensionString, op, value, com, comment);
}

/*
   Search for and return element that matches keyword.
*/
RDF *rdfElement(RDF *rdfParams, char *keyword)
{
    RDF *tmp;
    int32_t i;
    char lcKeyword[MAXLINE];

    for (i = 0; i <= strlen(keyword); i++)
        lcKeyword[i] = (char)tolower((int)keyword[i]);

    for (tmp = rdfParams; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(lcKeyword, tmp->keyword) == 0)
            return tmp;
    }
    return (NULL); /* No match so return NULL */
}

/*
   Search for and return element that matches keyword and has
   not been returned already. Sets value field to indicate
   it has now been visited.

*/
RDF *rdfMultiElement(RDF *rdfParams, char *keyword)
{
    RDF *tmp;
    int32_t i;
    char lcKeyword[MAXLINE];

    for (i = 0; i <= strlen(keyword); i++)
        lcKeyword[i] = (char)tolower((int)keyword[i]);

    for (tmp = rdfParams; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(lcKeyword, tmp->keyword) == 0 &&
            tmp->visited == FALSE)
        {
            tmp->visited = TRUE;
            return tmp;
        }
    }
    return (NULL); /* No match so return NULL */
}

/*
   Search for and return value that matches keyword.
*/
char *rdfValue(RDF *rdfParams, char *keyword)
{
    RDF *tmp;
    int32_t i;
    char lcKeyword[MAXLINE];

    for (i = 0; i <= strlen(keyword); i++)
        lcKeyword[i] = (char)tolower((int)keyword[i]);

    for (tmp = rdfParams; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(lcKeyword, tmp->keyword) == 0)
            return tmp->value;
    }
    return (NULL); /* No match so return NULL */
}

/*
   Search for and return value that matches keyword and has
   not been returned already. Sets value field to indicate
   it has now been visited.
*/
char *rdfMultiValue(RDF *rdfParams, char *keyword)
{
    RDF *tmp;
    int32_t i;
    char lcKeyword[MAXLINE];

    for (i = 0; i <= strlen(keyword); i++)
        lcKeyword[i] = (char)tolower((int)keyword[i]);

    for (tmp = rdfParams; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(lcKeyword, tmp->keyword) == 0 &&
            tmp->visited == FALSE)
        {
            tmp->visited = TRUE;
            return tmp->value;
        }
    }
    return (NULL); /* No match so return NULL */
}

static char *getRDFLine(char *bufferSpace, FILE *fp)
{
    char *buffer, *result;
    char tmp[MAXLINE];
    int32_t lastChar;
    int32_t i;

    result = NULL;
    while (TRUE)
    {
        buffer = fgets(tmp, MAXLINE, fp); /* Read line */
        if (buffer == NULL)
        {                  /* EOF so return */
            return result; /* EOF so return */
        }
        /*
          First line copy tmp to bufferSpace, ow cat strings
        */
        if (result == NULL)
            result = strcpy(bufferSpace, buffer);
        else
            result = strcat(result, buffer);
        /*
           Strip white space
        */
        result = stripWS(result);
        /*
           Done if not a line continuation character \
        */
        lastChar = strlen(result) - 1;
        if (result[lastChar] != '\\')
        {
            return (result);
        }
        result[lastChar] = '\0';
    }
}

/*
   Parse rdf file putting data in an RDF linked list.
   This routine does not support an {} field as does Scott S
   FORTRAN reader.
*/
RDF *rdfParse(char *rdfFile, RDF *rdfParams)
{
    RDF *rdfTmp, *rdfFirst;
    FILE *fp;
    char *tmp1, *tmp2;
    char bufferSpace[MAXLINE], *buffer;              /* Input buffer, pointer to buffer */
    char tmp[MAXLINE], tmpa[MAXLINE], tmpb[MAXLINE]; /* Tmp space */
    char *comment, *value, oper, *units, *dimensions, *keyword;
    int32_t i;
    /*    fprintf(stderr,"___");
    fprintf(stderr,"RDF PARSE %s\n",rdfFile);
    */
    /*
       Open input file
    */
    fp = openInputFile(rdfFile);
    /*
       Loop through lines
    */
    for (buffer = getRDFLine(bufferSpace, fp); buffer != NULL;
         buffer = getRDFLine(bufferSpace, fp))
    {
        /*
           Strip off comment.
        */
        comment = strpbrk(buffer, "!");
        if (comment != NULL)
        {
            *comment = '\0';
            comment++;
        }
        /*
           Strip off value and only continue if there is value
        */
        value = strpbrk(buffer, "=><");
        if (value != NULL)
        {
            oper = *value;
            *value = '\0';
            value++;
            value = stripWS(value);
            /*fprintf(stderr,"%s\n",value);*/
            /*
               Strip off dimensions
            */
            dimensions = strpbrk(buffer, "[");
            if (dimensions != NULL)
            {
                *dimensions = '\0';
                dimensions++;
                tmp1 = strpbrk(dimensions, "]");
                if (tmp1 != NULL)
                    *tmp1 = '\0';
                else
                    error("rdfParse: Unmatched [");
                dimensions = stripWS(dimensions);
                for (i = 0; i < strlen(dimensions); i++)
                    dimensions[i] = (char)tolower((int)dimensions[i]);
            }
            else if (strpbrk(buffer, "]") != NULL)
                error("rdfParse: Unmatched ]");
            /*
               Strip off units
            */
            units = strpbrk(buffer, "(");
            if (units != NULL)
            {
                *units = '\0';
                units++;
                tmp1 = strpbrk(units, ")");
                if (tmp1 != NULL)
                    *tmp1 = '\0';
                else
                    error("rdfParse: Unmatched (");
                units = stripWS(units);
                for (i = 0; i < strlen(units); i++)
                    units[i] = (char)tolower((int)units[i]);
            }
            else if (strpbrk(buffer, ")") != NULL)
                error("rdfParse: Unmatched )");
            /*
               Process keyword
            */
            keyword = stripWS(buffer);
            for (i = 0; i < strlen(keyword); i++)
                keyword[i] = (char)tolower((int)keyword[i]);

            /*
               Initialize list element with parsed values
            */
            rdfTmp = (RDF *)malloc(sizeof(RDF));

            if (keyword != NULL)
            {
                rdfTmp->keyword = (char *)malloc(strlen(keyword) + 1);
                rdfTmp->keyword = strcpy(rdfTmp->keyword, keyword);
            }
            else
                error("rdfParse: NULL keyword");

            rdfTmp->op = oper;

            if (dimensions != NULL)
            {
                rdfTmp->dimensions = (char *)malloc(strlen(dimensions) + 1);
                rdfTmp->dimensions = strcpy(rdfTmp->dimensions, dimensions);
            }
            else
                rdfTmp->dimensions = NULL;

            if (units != NULL)
            {
                rdfTmp->units = (char *)malloc(strlen(units) + 1);
                rdfTmp->units = strcpy(rdfTmp->units, units);
            }
            else
                rdfTmp->units = NULL;

            if (value != NULL)
            {
                rdfTmp->value = (char *)malloc(strlen(value) + 1);
                rdfTmp->value = strcpy(rdfTmp->value, value);
            }
            else
                rdfTmp->value = NULL;
            rdfTmp->visited = FALSE;
            rdfTmp->next = NULL;

            /*
                Add element to list
            */
            if (rdfParams != NULL)
            {
                rdfParams->next = rdfTmp;
                rdfParams = rdfTmp;
            }
            else
            {
                rdfFirst = rdfTmp;
                rdfParams = rdfTmp;
            }

        } /* End if value... */
    }     /* End for buffer... */
    return (rdfFirst);
}

/*
   Strip leading and trailing spaces and tabs from a string
*/

static char *stripWS(char *string)
{
    char *tmp1;
    while (*string == ' ' || *string == '\t' || *string == '\n')
        string++;
    tmp1 = string + strlen(string) - 1;
    while (*tmp1 == ' ' || *tmp1 == '\t' || *tmp1 == '\n')
    {
        *tmp1 = '\0';
        tmp1--;
    }
    return (string);
}
