#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include "../myproduct.h"
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

static int jsoneq2(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int jsonNameList(char *JSON_STR,jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo,product_t *ramenList ){
int count =0;
	for(int i=0; i<tokcount;i++){
		if(t[i].type ==JSMN_STRING && t[i+1].type !=JSMN_STRING)
		continue; //다음에 문자열이 아닌 '['이 온 경우 -> real 네임을 찾기위함 (Ramen 거르 .)
	 if(t[i].size>=1 && t[i].type ==JSMN_STRING){
	 nameTokenInfo[count].tokindex=i;
	 count ++;
	 break;
	 }
 }
	 int a= nameTokenInfo[0].tokindex;
		 for(int i=a+1;i<tokcount;i++){ //모든 객체의 네임을 뽑아준다.
			 if(t[i].size>=1 && t[i].type ==JSMN_STRING){
				 nameTokenInfo[count].tokindex=i;
				 count++;
			 }
		 }
		 //sprintf("%d \n",count);
		 return count;
	 }

void objectNameList(char *JSON_STR,jsmntok_t *t, int nameCount,NameTokenInfo *nameTokenInfo,product_t *ramenList){
	 //object의 첫번쨰 네임이 무엇인지 알기위해서
	 //int firstString;
	 int count =0;
	 	int a = nameTokenInfo[0].tokindex;
	 	for(int i=0; i<nameCount;i++){
			int j=nameTokenInfo[i].tokindex;
	 		if(jsoneq(JSON_STR, &t[j], &t[a]) == 0){
				count++;
	 			nameTokenInfo[i].objectindex=count;
		//	printf("%d\n",nameTokenInfo[i].objectindex);
	 		}
			else{
				nameTokenInfo[i].objectindex=count;
			//	printf("%d\n",nameTokenInfo[i].objectindex);
			}
	 }

 }
void printProduct1(char *JSON_STR,jsmntok_t *t, int nameCount, NameTokenInfo *nameTokenInfo, product_t *ramenList){
int nameindex;
int companyindex;
int priceindex;
int countindex;
int totalprice;
char company[]="company";
char name[]="name";
char count[]="count";
char price []="price";
char s[100]; //for Total Price
char s2[30];
printf("\t*********************************************\n");
printf("\t번호\t제품명\t제조사\t가격\t개수\t총가격\n");
printf("\t*********************************************\n");

	for(int i=1;i<5;i++){
		nameindex= returnTokindex(JSON_STR,t,nameCount,nameTokenInfo,name,i);
		companyindex=returnTokindex(JSON_STR,t,nameCount,nameTokenInfo,company,i);
		priceindex=returnTokindex(JSON_STR,t,nameCount,nameTokenInfo,price,i);
		countindex=returnTokindex(JSON_STR,t,nameCount,nameTokenInfo,count,i);

		 // atoi(t[countindex+1].end-t[countindex+1].start,JSON_STR + t[countindex+1].start);
		 ///TOTAL PRICE 구하기 ->>

	  strncpy(s, JSON_STR + t[priceindex+1].start,t[nameindex+1].end-t[nameindex+1].start);
		int j= strlen(s);
		s[j]='\0';
		strncpy(s2,JSON_STR + t[countindex+1].start ,t[countindex+1].end-t[countindex+1].start );
		int k= strlen(s2);
		s2[k]='\0';
 		totalprice=atoi(s)*atoi(s2);

		printf("%10d \t%.*s\t%.*s\t%.*s\t%.*s\t%d\n",i,
		t[nameindex+1].end-t[nameindex+1].start,	JSON_STR + t[nameindex+1].start,
		t[companyindex+1].end-t[companyindex+1].start,	JSON_STR + t[companyindex+1].start,
		t[priceindex+1].end-t[priceindex+1].start,	JSON_STR + t[priceindex+1].start,
		t[countindex+1].end-t[countindex+1].start,	JSON_STR + t[countindex+1].start, totalprice);
	}
}

int returnTokindex(char *JSON_STR,jsmntok_t *t,int nameCount, NameTokenInfo *nameTokenInfo, char *s, int objectNum){

	for(int i=0; i<nameCount;i++){
		int tokenI=nameTokenInfo[i].tokindex;
		if(nameTokenInfo[i].objectindex==objectNum &&  jsoneq2(JSON_STR, &t[tokenI], s) == 0)
		return nameTokenInfo[i].tokindex;
	}
	return 0;
}
void inputString(const char *json, jsmntok_t *t,char *target){
	int len=t->end -t->start;
	strncpy(target,json+t->start,len);
	target[len]='\0';

}
int makeProduct(const char *json, jsmntok_t *t, int tokcount, product_t* p[]){
	int i;
	int j;
	int count =0;

	for(i=2; t[i].type ==JSMN_OBJECT || t[i].type==JSMN_ARRAY;i++){
		int parent = t[i].parent;
		p[0]= (product_t*)malloc(sizeof(product_t));
		for(;i<tokcount;i++){
			if(t[i].parent ==parent){
				if(jsoneq2(json,&t[i], "name")==0){
					inputString(json,&t[i+1], p[count]->name);
				}
			else if(jsoneq2(json,&t[i], "price")==0){
						inputString(json,&t[i+1], p[count]->price);
					}
			else if(jsoneq2(json,&t[i], "gram")==0){
							inputString(json,&t[i+1], p[count]->gram);
							}
		 else if(jsoneq2(json,&t[i], "company")==0){
			 char rcompany[30];
							inputString(json,&t[i+1], rcompany);
						if(strcmp("오뚜기",rcompany)==0)p[count]->type=OTTUGI;
						else if(strcmp("삼양",rcompany)==0) p[count]->type=SAMYANG;
					}
			}
			else if(t[i].parent <parent){
				count++;
				p[count]=(product_t*)malloc(sizeof(product_t));
				parent=i;
			}
		}
	}
	return count+1;
}
void getTypeString(int type, char *t){
	if(type==OTTUGI) strcpy(t,"오뚜기\0");
	else if(type ==SAMYANG) strcpy(t,"삼양\0");
}
void printProduct(product_t * p[], int pcount){
	printf("*********************************************\n");
	printf("번호\t제품명\t제조사\t가격\t그램수\n");
	printf("*********************************************\n");
	char type[20];
	for(int i=0; i<pcount; i++){
		getTypeString(p[i]->type, type);
		printf("%5d %s %s %s %s\n",i+1,p[i]->name, type ,p[i]->price,p[i]->gram);
	}
}

//name의 패런트가 같은 얘들 == 깊이가 같은 ;
char *readJSONFile() {

	FILE *fp = fopen("myproduct.json","r");
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
	product_t *ramenList[20];
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	char *JSON_STR;
	NameTokenInfo nameTokenInfo[16];
	JSON_STR = readJSONFile();
	jsmn_init(&p);

	r = jsmn_parse(&p, JSON_STR, strlen(JSON_STR), t, sizeof(t)/sizeof(t[0]));

	//int nameCount =jsonNameList(JSON_STR, t, r,nameTokenInfo, ramenList);
//	objectNameList(JSON_STR, t, nameCount, nameTokenInfo,ramenList );
//	printProduct1(JSON_STR,t,nameCount,nameTokenInfo,*ramenList);

int pcount =makeProduct(JSON_STR,t,r,ramenList);
printProduct(ramenList,pcount);
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
