/*
   Test programs for rdf reader
*/
#include "stdio.h"
#include "SRTMrdf.h"

  int main(int argc, char *argv[])
{
   char *infile;
   RDF *rdfParams,*tmp;
   char *s;
   float d1,d2,d3,d4;
   int32_t i1,i2,i3,i4;
   

   infile = "rdftest.in";
   rdfParams = NULL;
   rdfParams = rdfParse(infile,rdfParams); 

   for(tmp=rdfParams; tmp != NULL; tmp = tmp->next) {
          fprintf(stderr,"\nKeyword    = |%s|\n", tmp->keyword);
          if(tmp->dimensions != NULL) 
             fprintf(stderr,"Dimensions   = |%s|\n",tmp->dimensions);
          if(tmp->units != NULL) 
             fprintf(stderr,"Units   = |%s|\n", tmp->units);

          if(tmp->value != NULL) 
              fprintf(stderr,"Data    = |%s|\n", tmp->value);
          if(tmp->comment != NULL) 
              fprintf(stderr,"Comment = %s\n", tmp->comment);
   }
   fprintf(stderr,"\n\n-------------------------------------------------\n\n");

   tmp = rdfElement(rdfParams,"Peg Longitude Path 1");
   if(tmp != NULL) {
        fprintf(stderr,"\nKeyword    = |%s|\n", tmp->keyword);
       if(tmp->dimensions != NULL) 
          fprintf(stderr,"Dimensions   = |%s|\n",tmp->dimensions);
       if(tmp->units != NULL) 
          fprintf(stderr,"Units   = |%s|\n", tmp->units);

       if(tmp->value != NULL)
          fprintf(stderr,"Data    = |%s|\n", tmp->value);
       if(tmp->comment != NULL) 
          fprintf(stderr,"Comment = %s\n", tmp->comment);
   }
   fprintf(stderr,"\n\n********************************************\n\n");
   s = "%f";
   d1 = 10;
   sscanf(rdfValue(rdfParams,"Peg Longitude Path 1"),s,&d1);

   rdfWrite(stdout,NULL,NULL,NULL,' ',NULL,"fjfkjakgjaskdg");
   rdfWrite(stdout,"Keyword 1",NULL,NULL,' ',NULL,"fjfkjakgjaskdg");
   rdfWrite(stdout,"Keyword 2","m,m",NULL,'=',"1 2 3","fjfkjakgjaskdg");
   rdfWrite(stdout,"Keyword 3","m,m","1:3",'=',"23425213",NULL);

   fprintf(stderr,s,d1);
   fprintf(stderr,"\n");


}
