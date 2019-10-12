#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hash.h"

/***************************************************
 *  Function Declarations
 * ************************************************/
static int hashCode(char* key);

/***************************************************
 *  Variables
 * ************************************************/
tCookieElement* hashArray[ HASH_TABLE_SIZE ]; 
tCookieElement* dummyItem;
tCookieElement* item;

/***************************************************
 *  Procedure: hashCode()
 *    
 *  Description:
 *    This function returns a hash code with the 
 *    provided string. 
 * 
 *  Assumptions:
 *    This function assumes a NULL terminated string
 *    is passed in. 
 * 
 * ************************************************/
static int hashCode(char * key) {
   /* Local Variables   */
   int i = 0;
   int hash = 7;

   /*create a hash code for the provided string*/
   for( i = 0; i < strlen(key); i++ ){
      hash = abs(hash*31 + key[i]);
   }

   /* ensure the hash index fits within the 
      hash table size                         */
   hash = hash % HASH_TABLE_SIZE;

   return( hash );
}

/***************************************************
 *  Procedure: hashSearch()
 *    
 *  Description:
 *    This function searches the hash table for 
 *    a given key and will return a pointer to 
 *    cookie element.
 * 
 * ************************************************/
tCookieElement *hashSearch(char* key) {
   /*find the hash index   */
   int hashIndex = hashCode(key);  
	
   /* Find the correct cookie element */
   while(hashArray[hashIndex] != NULL) {
	
      if(strcmp( hashArray[hashIndex]->key, key ) == 0)
         return hashArray[hashIndex]; 
			
      /* move to next cookie */
      ++hashIndex;
		
      /* wrap around the hash table */
      hashIndex %= HASH_TABLE_SIZE;
   }        
	
   return NULL;        
}

/***************************************************
 *  Procedure: hashInsert()
 *    
 *  Description:
 *    This functions takes a key (cookie name) and
 *    data (cookie value) and inserts it into the
 *    hash table.
 * 
 * ************************************************/
void hashInsert(char* key, char* data) {
   /* Local Variables */
   tCookieElement *item;
   
   item = (tCookieElement*) malloc(sizeof(tCookieElement));
   strncpy( item->data, data, MAX_COOKIE_VAL_LEN );
   item->data[MAX_COOKIE_VAL_LEN - 1] = '\0'; 
   
   strncpy( item->key, key, MAX_COOKIE_NAME_LEN );
   item->key[MAX_COOKIE_NAME_LEN - 1] = '\0'; 

   /* Get a hash index for this key */
   int hashIndex = hashCode(key);

   /* If a collision occurs, find the next open slot */
   while( hashArray[hashIndex] != NULL 
       && hashArray[hashIndex]->key[0] != '\0' ) {
      
      /*go to next slot */
      ++hashIndex;
		
      /* wrap around the hash table */
      hashIndex %= HASH_TABLE_SIZE;
   }
   hashArray[hashIndex] = item;
}

/***************************************************
 *  Procedure: hashDelete()
 *    
 *  Description:
 *    This function takes a pointer to an element
 *    in the hash table and removes the element.
 * 
 * ************************************************/
tCookieElement* hashDelete(tCookieElement* item) {
   char *key = item->key;

   /* get tha hash code associated with 
      the key (cookie string)          */
   int hashIndex = hashCode(key);

   /* Find the correct cookie element */
   while(hashArray[hashIndex] != NULL) {
	
      if( strcmp(hashArray[hashIndex]->key, key) == 0 ) {
         tCookieElement* temp = hashArray[hashIndex]; 
			
         /*assign a dummy data item to the hash index */
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      /*go to next slot */
      ++hashIndex;
		
      /* wrap around the hash table */
      hashIndex %= HASH_TABLE_SIZE;
   }      
	
   return NULL;        
}

/***************************************************
 *  Procedure: hashDisplay()
 *    
 *  Description:
 *    This function prints the elements in the hash 
 *    table. Default behavior is to print only the 
 *    cookie name and value. If verbose is set to 
 *    TRUE, more information about the hash table 
 *    will be printed. 
 * 
 * ************************************************/
void hashDisplay( boolean verbose ) {
   int i = 0;
	
   /* Print the cookie names and values */
   printf( "\n\n<Cookie Name> : <Cookie Value>\n" );

   for(i = 0; i < HASH_TABLE_SIZE; i++) {

      if(verbose){
         printf("HASH[%d] = ", i);
      }
      
      if(hashArray[i] != NULL){
         printf("%s : %s\n",hashArray[i]->key,hashArray[i]->data);
      }       
      else if( verbose ){
         printf("--:--\n");
      }
   }
	
   printf("\n");
}
