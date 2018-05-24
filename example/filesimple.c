#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
//#define DEBUG_MODE

static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void jsonNameList(char *JSON_STR,jsmntok_t *t, int tokcount,int *nameTokIndex){

	int count =1;
 for(int i=1; i<tokcount;i++){
	 if(t[i].size >=1 && t[i].type ==3){
	 nameTokIndex[count]=i;
	 i++;
	 count ++;
 	}
 }
}

void printNameList(char *JSON_STR, jsmntok_t *t,int *nameTokIndex){
	int a;
	int i = 1;  //0부터 시작하면 0을 제외시키고 nameTokIndex에 넣었기때문에 시작에 0으로 초기화해준값이 되므로 바로 중지됨
printf("* * * * * NAME LIST * * * * * \n");
	for(;;i++){
		a = nameTokIndex[i];
		if(a ==0) break;
		printf("[NAME %d]: %.*s\n", i, t[a].end-t[a].start,
		JSON_STR + t[a].start);
	}
}
//name의 특징!
// 1번특징 문자열로 되어있다.
// 2번특징 parent 이다!
// 제일쉬운것 name다음 무조건 value
/*void jsonNameList(char *JSON_STR, jsmntok_t *t, int tokcount){
	printf("* * * * * NAME LIST * * * * * \n");
 	int count =1;
	for (int i = 1; i < tokcount; i++) {
		if(t[i].size >=1)//value가 있는경우. 즉 name인 경우
		{
			printf("[NAME %d ]: %.*s\n", count, t[i].end-t[i].start,
					JSON_STR + t[i].start);
					count++;
					i++; //자식 skip.
		}
	}
}*/
char *readJSONFile() {
	FILE *fp = fopen("data.json","r");
	int count=0;
	char *JSON_STRING;
	char joneLine[255];
	JSON_STRING=(char *)malloc(sizeof(char)*50);

	while(1){
		fgets(joneLine,sizeof(joneLine),fp);
		if(feof(fp)) break;

		count+=strlen(joneLine);
		JSON_STRING=realloc(JSON_STRING,count+1);
		strcat(JSON_STRING,joneLine);
	}
	fclose(fp);
	return JSON_STRING;
}

int main() {
	int i;
	int r;
	int nameTokIndexa[100]= {0};
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	char *JSON_STR;
	JSON_STR = readJSONFile();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STR, strlen(JSON_STR), t, sizeof(t)/sizeof(t[0]));

	jsonNameList(JSON_STR, t, r, nameTokIndexa);
	printNameList(JSON_STR,t, nameTokIndexa);

	return 0;

	#ifdef DEBUG_MODE

 printf("Token's count : %d \n",r);
	#endif

//디버그에요~

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STR, &t[i], "name") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STR + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STR, &t[i], "keywords") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STR + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STR, &t[i], "description") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STR + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STR, &t[i], "example") == 0) {
			int j;
			printf("- example:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STR + g->start);
			}
			i += t[i+1].size + 1;
		}

		/*else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STR + t[i].start);
		}*/
	}
	return EXIT_SUCCESS;
}
