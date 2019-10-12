#include "http_info.h"

/***************************************************
 *  Literals
 * ************************************************/
#define HASH_TABLE_SIZE 30

/***************************************************
 *  Types
 * ************************************************/
typedef struct CookieElement {
   char     data[ MAX_COOKIE_VAL_LEN ];   
   char     key[ MAX_COOKIE_NAME_LEN ];
}tCookieElement;

typedef int boolean;
enum{
   FALSE = 0,
   TRUE  = 1
};

/***************************************************
 *  Function Declarations
 * ************************************************/
tCookieElement *hashSearch(char* key);

void hashInsert(char* key, char* data);

tCookieElement* hashDelete(tCookieElement* item);

void hashDisplay( boolean verbose );
