#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "http_info.h"

/***************************************************
 *  Literals
 * ************************************************/
/* Un-comment this for more comments in output */
//#define VERBOSE

/***************************************************
 *  Types
 * ************************************************/
typedef struct HttpResponse{
  char                 requestStr[MAX_HTTP_RESPONSE_LEN];
  struct HttpResponse* nextResponse;
}tHttpResponse;

/***************************************************
 *  Function Declarations
 * ************************************************/
tHttpResponse* parseHttpResponses( char* responses );

void parseSetCookies( tHttpResponse * httpResponseCurr );

/***************************************************
 *  Procedure: parseHttpResponses()
 *    
 *  Description:
 *    This function recieves an input string which
 *    contains HTTP/1.1 response messages. This
 *    function will separate each response into 
 *    it's own structure and link them together
 *    in a HttpResponseStructure list.
 * 
 *  Assumptions: 
 *    Caller of this function MUST ENSURE that 
 *    responses is a null terminated string.
 * 
 * ************************************************/
tHttpResponse* parseHttpResponses( char* responses ){
/*Local literals       */
#define HTTP_VER "HTTP/1.1"

/*Local variables       */
tHttpResponse*      currentHttpResponse;
tHttpResponse*      firstHttpResponse;
int                 i;
char*               nextRespPtr;
int                 respLen;
char*               s;

s = responses;
i = 0;

/* Find all instances of HTTP version string (indicating new response)*/
while( s = strstr(s, HTTP_VER) ){

  /* Setup and/or adjust response list        */
  if(i == 0){
    firstHttpResponse = malloc(sizeof(tHttpResponse));
    firstHttpResponse->nextResponse = NULL;
    currentHttpResponse = firstHttpResponse;
  }
  else{
    currentHttpResponse->nextResponse =  malloc( sizeof( tHttpResponse ) );
    currentHttpResponse = currentHttpResponse->nextResponse;
  }
  
  nextRespPtr = strstr( s+strlen(HTTP_VER), HTTP_VER);

  if( nextRespPtr ){
    /* get length to next http response       */
    respLen = nextRespPtr - s;

    /* copy the request to the request buffer */
    strncpy(currentHttpResponse->requestStr, s, respLen );

    /* append null terminator                 */
    currentHttpResponse->requestStr[respLen+1]='\0';
  }
  else{
    /* we have reached the end of the string, strcpy to null term*/
    respLen = strcspn( s, "\0");
    strcpy(currentHttpResponse->requestStr, s );
  }

  /* Increment s pointer by length of request*/
  s += respLen;
  i++;

}

/* return the first response in the list */
return( firstHttpResponse );
}


/***************************************************
 *  Procedure: parseSetCookies()
 *    
 *  Description:
 *    This function recieves an httpResponse structure
 *    list. It will parse through this list to search
 *    for Set-Cookier headers only. This function only
 *    cares about the name and value of the cookie.
 *    Any other directives will be ignored.
 * 
 * ************************************************/
void parseSetCookies( tHttpResponse * httpResponseCurr ){
  /*Local variables       */
  int               cookieCount;
  tCookieElement*   hashItem;
  int               len;
  char*             s;
  char              tempNameBuff[MAX_COOKIE_NAME_LEN];
  char              tempValBuff[MAX_COOKIE_VAL_LEN];

  s = httpResponseCurr->requestStr;

  /* Search for all Set-Cookie headers in HTTP request (ignore everything else)*/
  while( s = strstr(s, "Set-Cookie:") ){
    
    /* incrememnt the string iterator pointer past 'Set-Cookie'*/
    s += strlen("Set-Cookie:");
    
    /*Get the Name of the Cookie*/
    len = strcspn( s, "=");
    strncpy( tempNameBuff, s, len );
    tempNameBuff[len]='\0';
    
    /*Move the string iterator pointer past the cookie name*/
    s+= strlen(tempNameBuff) + 1;
  
    /*Get the value of the Cookie*/
    len = strcspn( s, "\r\n;");
    strncpy( tempValBuff, s, len );
    tempValBuff[len]='\0';

    /*Check if the cookie name already exists in the hash table */
    if( hashItem = hashSearch(tempNameBuff) ){
      /*Replace value of existing cookie*/
      strcpy( hashItem->data, tempValBuff );
    }
    else{
      /*Insert new cookie into hash table */
      hashInsert( tempNameBuff, tempValBuff );
    } 
  }
}


/***************************************************
 *  Procedure: main()
 *    
 *  Description:
 *    Entry point into program
 * 
 * ************************************************/
int main()
{
/*Local literals        */
#define MAX_STD_IN_BUFF 100000

/*Local variables       */  
int                 c;
tHttpResponse*      currResponse;
int                 i;
char                std_in_http[ MAX_STD_IN_BUFF ];
tHttpResponse*      responses;

printf("Enter all HTTP\\1.1 responses below. When done, force the end of file (EOF) in stdin by pressing ctrl+d two times.\n");

/* Read standard input until EOF (if in console EOF can be invoked via pressing ctrl+D twice */
i = 0; 
while( ( c = getchar() ) != EOF && ( i < MAX_STD_IN_BUFF - 1 ) ){
  std_in_http[i++] = c;
}

/*ensure string buffer is null terminated*/
if( i < MAX_STD_IN_BUFF - 1 ){
  std_in_http[++i] = '\0';
}
else{
  std_in_http[MAX_STD_IN_BUFF - 1] = '\0';
}

/* Parse the standard input into a list of responses */
responses = parseHttpResponses( std_in_http );

#if VERBOSE
i = 1;
currResponse = responses;
while(currResponse != NULL ){
  printf("------------\nResponse #%d\n------------\n\n", i);
  printf( "%s\n\n", currResponse->requestStr );
 i++;
  currResponse = currResponse->nextResponse;
}
#endif
  

currResponse = responses;
while( currResponse != NULL){
  parseSetCookies( currResponse );
  currResponse = currResponse->nextResponse;
}

#if VERBOSE
/* Print the contents of the hash table */
hashDisplay( TRUE );
#else
hashDisplay( FALSE );
#endif

return 0;

#undef MAX_STD_IN_BUFF
}