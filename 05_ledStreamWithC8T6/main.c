
#define RCC_AP2ENR	*((unsigned volatile int*)0x40021018)

#define GPIOA_CRL	*((unsigned volatile int*)0x40010800)
#define	GPIOA_ORD	*((unsigned volatile int*)0x4001080C)

#define GPIOB_CRH	*((unsigned volatile int*)0x40010C04)
#define	GPIOB_ORD	*((unsigned volatile int*)0x40010C0C)

#define GPIOC_CRH	*((unsigned volatile int*)0x40011004)
#define	GPIOC_ORD	*((unsigned volatile int*)0x4001100C)

void  Delay_wxc( volatile  unsigned  int  t)
{
     unsigned  int  i;
     while(t--)
         for (i=0;i<800;i++);
}

int main()
{
	int j=100;
	RCC_AP2ENR|=1<<2;			
	RCC_AP2ENR|=1<<3;			
	RCC_AP2ENR|=1<<4;			
	
	GPIOA_CRL = 0;		
	GPIOA_CRL |= (0x01<<4*5);		
	GPIOA_ORD=0xffff;			
	
	GPIOB_CRH=0;		
	GPIOB_CRH |= (0x01<<4*1);	
	GPIOB_ORD=0xffff;			
	
	GPIOC_CRH=0;		
	GPIOC_CRH |= (0x01<<4*6);   	
	GPIOC_ORD=0xffff;		
	while(j)
	{	
		GPIOA_ORD=0x0<<5;		
		Delay_wxc(10000);
		GPIOA_ORD=0x1<<5;		
		
		GPIOB_ORD=0x0<<9;		
		Delay_wxc(10000);
		GPIOB_ORD=0x1<<9;		
		
		GPIOC_ORD=0x0<<14;		
		Delay_wxc(10000);
		GPIOC_ORD=0x1<<14;		
	}
}

