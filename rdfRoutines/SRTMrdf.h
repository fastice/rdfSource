

typedef struct RDFType {
  char *keyword;        /* RDF keyword */
  char *dimensions;            /* Dimension field */
  char *elements;          /* Element field */
  char *units;          /* Units field */
  char op;             /* Operator =,>, or < */
  char *value;          /* Data */
  char *comment;        /* ; preceeded comment */
  int visited;          /* This field is used to handle multiple 
                           identical keywords */
  struct RDFType *next; /* Next element in RDF list */

} RDF;


/*
   Write data in rdf format to a file
*/
   void rdfWrite(FILE *fp,char *keyword, char *units,
                 char *dimensions,char op,char *value, char *comment);
/*
   Parse rdf file putting data in an RDF linked list.
*/
    RDF *rdfParse(char *file, RDF *rdfParams);
/*
   Search for and return element that matches keyword.
*/
    RDF *rdfElement(RDF *rdfParams, char *keyword);
/*
   Search for and return element that matches keyword and has
   not been returned already. Sets value field to indicate
   it has now been visited.

*/
    RDF *rdfMultiElement(RDF *rdfParams, char *keyword);
/*
   Search for and return value that matches keyword.
*/
    char *rdfValue(RDF *rdfParams, char *keyword);
/*
   Search for and return value that matches keyword and has
   not been returned already. Sets value field to indicate
   it has now been visited.
*/
    char *rdfMultiValue(RDF *rdfParams, char *keyword);
