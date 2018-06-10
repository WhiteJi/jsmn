
typedef struct {
	int tokindex;
	int objectindex;
}NameTokenInfo ;

typedef enum{
	OTTUGI=0,
	SAMYANG=1,
}company_t; //ramen company type

typedef struct{
	company_t type;
	char name[20];
	char price[20];
	char gram[20];
}product_t;
