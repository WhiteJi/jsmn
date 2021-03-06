#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
//#define DEBUG_MODE
int objectCount =0;
int arrayCount =0;
int arrayCount2=0;
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
	int num=0;

		if((t[0].type==JSMN_OBJECT && t[1].type ==JSMN_STRING)
		|| (t[0].type==JSMN_ARRAY && t[1].type ==JSMN_OBJECT))
		objectCount++; //첫시작이 오브젝트이고 다음이 스트링이면. 첫시작이 어레이 이나 다음이 오브젝트이면.
		if((t[0].type==JSMN_ARRAY && t[1].type ==JSMN_STRING)
		|| (t[0].type==JSMN_OBJECT && t[1].type ==JSMN_ARRAY))
			arrayCount++;//첫시작이 어레이이고 다음이 바로 스트링이면. 첫시작이 오브젝트이나 바로 다음이 어레이 이면.)

if(objectCount==1){ //첫 시작이 오브젝트일때
	 for(int i=0; i<tokcount;i++){
		 if(t[i].type ==JSMN_STRING && t[i+1].type !=JSMN_STRING)
		 continue; //다음에 문자열이 아닌 '['이 온 경우 -> real 네임을 찾기위함 (Ramen 거르 .)
	 	if(t[i].size>=1 && t[i].type ==JSMN_STRING){
	 	nameTokIndex[count]=i;
	 	count ++;
	 	break;
 		}
 	}
		int a= nameTokIndex[1]; //
		if(t[a-2].type==JSMN_ARRAY){ //만약 첫 네임이 객체에 속하나. 전체로는 배열에 포함되어있다면!
			arrayCount2=1;
			for(int i=a+1;i<tokcount;i++){ //모든 객체의 네임을 뽑아준다.
				if(t[i].size>=1 && t[i].type ==JSMN_STRING){
					nameTokIndex[count]=i;
					count++;
				}
			}
		}
		else{ //일반 객체인 경우
    	for(int i=a+1;i<tokcount;i++){ //같은패런트를 네임으로 저장한다.
				if((t[a].parent == t[i].parent &&t[i].size>=1&& t[i].type ==JSMN_STRING)){
					nameTokIndex[count]=i;
					count++;
				}
			}
		}
	}
else if(arrayCount==1){ //바로 배열에 속한경우
	 	for(int i=0; i<tokcount;i++){
		 	if(t[i].size>=1 && t[i].type ==JSMN_STRING){
		 	nameTokIndex[count]=i;
		 	count ++;
		 	break;
	 		}
	 	}
			int a= nameTokIndex[1];
	//printf("%d\n",a);
	    	for(int i=a+1;i<tokcount;i++){
					if(t[i].size>=1 && t[i].type ==JSMN_STRING){
						nameTokIndex[count]=i;
						count++;
					}
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
		//printf("%d\n\n",a);
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

void PrintByName(char *JSON_STR, jsmntok_t *t,int *nameTokIndex){
int number=0;
	int a,b;
	while(1){ //a Loop for print menu and print datas!
		printf("원하는 번호 입력 (exit :0) >> ");
		scanf("%d",&number);
		if(number == 0) break; //exit 0
		a=0;
		b=0;
		a = nameTokIndex[number];
	  b = nameTokIndex[number+1];
		if(b!=0){  //번호 입력시 그 다음 네임이 있을 때 ( nameTokIndex에 입력된 다음 변수가 0이 아닐때 )
			printf("%.*s  : ", t[a].end-t[a].start,	JSON_STR + t[a].start);
			printf("%.*s \n", t[a+1].end-t[a+1].start,JSON_STR + t[a+1].start);
			a+=2;
			for(int i=a;i<b/2;i+=2){
				printf("%.*s  : ", t[i].end-t[i].start,
				JSON_STR + t[i].start);
				printf("%.*s \n", t[i+1].end-t[i+1].start,
				JSON_STR + t[i+1].start);
			}
	}
	else {//b==0 (nameTokIndex에 입력된 다음 변수가 0일때 -> 마지막 네임일때)
			printf("%.*s  : ", t[a].end-t[a].start,	JSON_STR + t[a].start);
			printf("%.*s \n", t[a+1].end-t[a+1].start,JSON_STR + t[a+1].start);
			a+=2;
			for(int i=a;;i+=2){
				if(t[i].type != JSMN_STRING)break;
				printf("%.*s  : ", t[i].end-t[i].start,
				JSON_STR + t[i].start);
				printf("%.*s \n", t[i+1].end-t[i+1].start,
				JSON_STR + t[i+1].start);
			}
			}
		}
	}
void objectNameList(char *JSON_STR,jsmntok_t *t,int tokcount, int *objectCount ,int *nameTokIndex){
//object의 첫번쨰 네임이 무엇인지 알기위해서
//int firstString;
	int a = nameTokIndex[1];
if(arrayCount ==1 || arrayCount2==1){
	int count =1;
	for(int i=1; i<tokcount;i++){
		if(jsoneq(JSON_STR, &t[i], &t[a]) == 0){
		objectCount[count]=i;
		i++;
		count ++;
	}
}
}
else {
	int count =1;
 	for(int i=1; i<tokcount;i++){
	 	if(t[a].parent == t[i].parent && jsoneq(JSON_STR, &t[i], &t[a]) == 0){
	 	objectCount[count]=i;
	 	i++;
		count ++;
 	}
 }
}
//printf("%d\n ", objectCount[1]);
//printf("%d\n ", objectCount[2]);
//  printf("%d\n ", objectCount[3]);
}
void PrintObjectList(char *JSON_STR, jsmntok_t *t,int *objectCount){
	int count =1;
	int a;
	int i = 1;
printf("* * * * * OBJECT LIST * * * * * \n");
	for(;;i++){
		a = objectCount[i];
		if(a ==0) break;
		printf("[NAME %d]: %.*s \n", count, t[a+1].end-t[a+1].start,
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

//name의 패런트가 같은 얘들 == 깊이가 같은 ;
char *readJSONFile(char *fileName) {
	strcat(fileName,".json");
	FILE *fp = fopen(fileName,"r");
	if(fp ==NULL){
		printf("%s 파일이 존재하지 않습니다. \n",fileName);
		exit(0);
	}
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
	char fileName[15];
	printf("원하는 파일명 입력 : ");
	scanf("%s",fileName);
	char *JSON_STR;

	JSON_STR = readJSONFile(fileName);
	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STR, strlen(JSON_STR), t, sizeof(t)/sizeof(t[0]));

	jsonNameList(JSON_STR, t, r, nameTokIndexa);
	printNameList(JSON_STR,t, nameTokIndexa);
//	selectNameList(JSON_STR,t,nameTokIndexa);
	objectNameList(JSON_STR, t, r, objectCount,nameTokIndexa);
	PrintObjectList(JSON_STR,t, objectCount);
	PrintByName(JSON_STR,t,nameTokIndexa);//select
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
