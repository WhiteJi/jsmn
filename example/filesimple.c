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

static int jsoneq(const char *json, jsmntok_t *tok, jsmntok_t *tok1) {
	if (tok->type == JSMN_STRING && (int) tok1->end - tok1->start == tok->end - tok->start &&
			strncmp(json + tok->start, json + tok1->start, tok->end - tok->start) == 0) {
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

void selectNameList(char *JSON_STR , jsmntok_t *t , int *nameTokIndex){
	int a;
	int num;

	while(1){
		printf("Select Name's no (exit :0) >> ");
		scanf("%d",&num);

		if(num == 0) break; //exit 0
		a = nameTokIndex[num];
		if(a!=0){

		printf("[NAME %d]: %.*s\n", num, t[a].end-t[a].start,
		JSON_STR + t[a].start);

		printf("%.*s \n", t[a+1].end-t[a+1].start,
		JSON_STR + t[a+1].start);
	}
		else{ printf("Out of bound the Value !! please Enter num again \n ");
		}
	}
}
void selectObjectList(char *JSON_STR, jsmntok_t *t,int *objectCount,int tokcount,int *nameTokIndex){
int num=0;
int a=0;
int b=0;
int z=0;
	while(1){
		printf("원하는 번호 입력 (exit :0) >> ");
		scanf("%d",&num);

		int i=1;

		if(num == 0) break; //exit 0

		a = objectCount[num];
		b= objectCount[num+1];

		if(a==0 ) printf("try again this num is out of bound! \n");

		if(b==0){
			printf("%.*s : ", t[a].end-t[a].start,
			JSON_STR + t[a].start);
			printf("%.*s \n", t[a+1].end-t[a+1].start,
			JSON_STR + t[a+1].start);
			a++;

			for(;;i++){
				z = nameTokIndex[a+i];
				if(z == 0) break;
				printf("      [%.*s]      ", t[z].end-t[z].start,
				JSON_STR + t[z].start);
				printf("%.*s \n", t[z+1].end-t[z+1].start,
				JSON_STR + t[z+1].start);
			}
		}
		else{
			printf("%.*s : ", t[a].end-t[a].start,
			JSON_STR + t[a].start);
			printf("%.*s \n", t[a+1].end-t[a+1].start,
			JSON_STR + t[a+1].start);
			a++;

			for(;;i++){
				z = nameTokIndex[a+i];
				if(z >= b) break;
				printf("      [%.*s]      ", t[z].end-t[z].start,
				JSON_STR + t[z].start);
				printf("%.*s \n", t[z+1].end-t[z+1].start,
				JSON_STR + t[z+1].start);
			}
		}
	}
}
void objectNameList(char *JSON_STR,jsmntok_t *t,int tokcount, int *objectCount ,int *nameTokIndex){
//object의 첫번쨰 네임이 무엇인지 알기위해서
//int firstString;

	int a = nameTokIndex[1];

	int count =1;
 	for(int i=1; i<tokcount;i++){
	 	if(jsoneq(JSON_STR, &t[i], &t[a]) == 0){
	 	objectCount[count]=i;
	 	i++;
		count ++;
 	}
 }
}
void PrintObjectList(char *JSON_STR, jsmntok_t *t,int *objectCount){
	int count =1;
	int a;
	int i = 1;
printf("* * * * * OBJECT LIST * * * * * \n");
	for(;;i++){
		a = objectCount[i];
		if(a ==0) break;
		printf("[NAME %d]: %.*s\n", count, t[a+1].end-t[a+1].start,
		JSON_STR + t[a+1].start);
		count++;
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
	int objectCount[100]={0};
	int nameTokIndexa[100]= {0};
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	char *JSON_STR;
	JSON_STR = readJSONFile();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STR, strlen(JSON_STR), t, sizeof(t)/sizeof(t[0]));

	jsonNameList(JSON_STR, t, r, nameTokIndexa);
//	printNameList(JSON_STR,t, nameTokIndexa);
//	selectNameList(JSON_STR,t,nameTokIndexa);
	objectNameList(JSON_STR, t, r, objectCount,nameTokIndexa);
	PrintObjectList(JSON_STR,t, objectCount);
	selectObjectList(JSON_STR,t,objectCount,r,nameTokIndexa);
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

	return EXIT_SUCCESS;
}
