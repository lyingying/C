#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef unsigned char boolean;

#define	TRUE				1
#define	FALSE				0
#define NOT_FOUND_INDEX			-1

typedef struct HUGE_NUM
{
	boolean sign;
	int *intNum;
	int *decNum;
	int intCount;//所申请的intNum数组的元素个数
	int decCount;//所申请的decNum数组的元素个数
}HUGE_NUM;

boolean initHuge(HUGE_NUM **head, int intCount, int decCount);
boolean destoryHuge(HUGE_NUM **head);
void getNum(HUGE_NUM **head, char *num);
void getIntNum(HUGE_NUM *hug, char *num, int intEndIndex);
void getDecNum(HUGE_NUM *hug, char *num);
void showHuge(HUGE_NUM hug);
void toAntiCode(HUGE_NUM hug);
void addHuge(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM **resHug);
void addDecNum(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM resHug);
void addIntNum(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM resHug, int *intCarry);
void subHuge(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM **resHug);

void subHuge(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM **resHug)
{
	anotherHug.sign = !anotherHug.sign;
	addHuge(oneHug, anotherHug, resHug);
}

void addIntNum(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM resHug, int *intCarry)
{
	int i;
	int c = 0;//进位标志位
	int number;

	if(oneHug.intCount == 0 && anotherHug.intCount == 0)//若没有整数位
		return;
	if(oneHug.intCount == 0)//若第一个数没有整数位
	{
		for(i = 0; i < anotherHug.intCount; i++)
			resHug.intNum[i] = anotherHug.intNum[i];
		return;
	}
	if(anotherHug.intCount == 0)//若第一个数没有整数位
	{
		for(i = 0; i < oneHug.intCount; i++)
			resHug.intNum[i] = oneHug.intNum[i];
		return;
	}

	for(i = 0 ; i < oneHug.intCount && i < anotherHug.intCount; i++)
	{
		number = (oneHug.intNum[i] + anotherHug.intNum[i] + c);
		resHug.intNum[i] = number%10000;
		c = number/10000;
	}
	//若第一个位数多
	while(i < oneHug.intCount)
	{
		number = (oneHug.intNum[i] + c);
		resHug.intNum[i] = number%10000;
		c = number/10000;
		i++;
	}
	//若第一个位数多
	while(i < anotherHug.intCount)
	{
		number = (anotherHug.intNum[i] + c);
		resHug.intNum[i] = number%10000;
		c = number/10000;
		i++;
	}

	*intCarry = c;//整数位加完的进位

	//加上整数位加完后的进位；
	i = 0;
	while(c && i < resHug.intCount)
	{
		number = (resHug.intNum[i] + c);
		resHug.intNum[i] = number%10000;
		c = number/1000;
		i++;
	}
	
}

void addDecNum(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM resHug)
{
	int i;//下标
	int c = 0;//进位标志位
	int number;
	
	if(oneHug.decCount == 0 && anotherHug.decCount == 0)//若没有小数位
		return;
	if(oneHug.decCount == 0)//若第一个数没有小数位
	{
		for(i = 0; i < anotherHug.decCount; i++)
			resHug.decNum[i] = anotherHug.decNum[i];
		return;
	}
	if(anotherHug.decCount == 0)//若第二个数没有小数位
	{
		for(i = 0; i < oneHug.decCount; i++)
			resHug.decNum[i] = oneHug.decNum[i];
		return;
	}

	i = resHug.decCount - 1;//由后往前加
	//若第一个位数多
	while(oneHug.decCount > anotherHug.decCount)
	{
		resHug.decNum[i] = oneHug.decNum[i];
		i--;
		oneHug.decCount--;
	}
	//若第二个位数多
	while(oneHug.decCount < anotherHug.decCount)
	{
		resHug.decNum[i] = anotherHug.decNum[i];
		i--;
		anotherHug.decCount--;
	}

	for(; i >=0; i--)
	{
		number = (oneHug.decNum[i] + anotherHug.decNum[i] + c);
		resHug.decNum[i] = number%10000;
		c = number/10000;
	}

	//加上小数位加完后的进位
	i = resHug.decCount - 1;
	while(c && i >= 0)
	{	
		number = (resHug.decNum[i] + c);
		resHug.decNum[i] = number%1000 ;
		c = number/10000;
		i--;
	}
}

void addHuge(HUGE_NUM oneHug, HUGE_NUM anotherHug, HUGE_NUM **resHug)
{
	int intCarry = FALSE;//表示整数位进位
	int resIntCount = oneHug.intCount > anotherHug.intCount ? oneHug.intCount - 1 : anotherHug.intCount - 1;//由于出始化得时候多申请了一组
	int resDecCount = oneHug.decCount > anotherHug.decCount ? oneHug.decCount - 1 : anotherHug.decCount - 1;
	int i = 0;
	int *tmp;
	int number;
	
	//初始化resHug
	initHuge(resHug, resIntCount, resDecCount);
	//将两个巨大数变为反码，以便计算
	toAntiCode(oneHug);
	toAntiCode(anotherHug);
	//小数位加法
	addDecNum(oneHug, anotherHug, **resHug);
	//整数位加法
	addIntNum(oneHug, anotherHug, **resHug, &intCarry);
	//处理符号问题
	(*resHug)->sign = oneHug.sign^anotherHug.sign^intCarry;
	//将所有的巨大数都置为原码
	toAntiCode(oneHug);
	toAntiCode(anotherHug);
	toAntiCode(**resHug);
	//处理小数有向整数进位问题
	if((*resHug)->decNum[0] == 1)
	{
		intCarry = TRUE;
		i = 0;
		while(intCarry)
		{
			number = (*resHug)->intNum[i] + intCarry;
			(*resHug)->intNum[i] = number%10000;
			intCarry = number/10000;
			i--;
		}
	}
	//处理整数最高位有进位问题，要再多申请一个数组空间。
	if((*resHug)->intNum[(*resHug)->intCount - 1])
	{
		tmp = (*resHug)->intNum;
		(*resHug)->intNum = (int *)calloc(sizeof(int), (*resHug)->intCount + 1);
		(*resHug)->intCount++;

		for(i = 0; i < (*resHug)->intCount -1; i++)
		{
			(*resHug)->intNum[i] = tmp[i];
		}
		free(tmp);
	}
}

void toAntiCode(HUGE_NUM hug)
{
	int i;

	if(0 == hug.sign)
		return;//正数的反码为它本身

	for(i = 0; i < hug.intCount; i++)
	{
		hug.intNum[i] = 9999 - hug.intNum[i];
	}
	for(i = 0; i < hug.decCount; i++)
	{
		hug.decNum[i] = 9999 - hug.decNum[i];
	}
}

void showHuge(HUGE_NUM hug)
{
	int i;
	int intNumIndex = hug.intCount - 2;
	//输出符号位
	if(hug.sign)
	{
		printf("-");
	}

	//输出整数位
	printf("%d", hug.intNum[intNumIndex--]);//处理第一位
	for(i = intNumIndex; i >= 0; i--)
	{
		printf("%04d",hug.intNum[i]);
	}
	//输出小数
	if(hug.decCount)
	{
		printf(".");//输出小数点
	
		//输出小数位
		for(i = 1; i < hug.decCount; i++)
		{
			printf("%04d",hug.decNum[i]);
		}
	}
	printf("\n");
}

void getDecNum(HUGE_NUM *hug, char *num)
{
	int i;
	int number = 0;//用于保存生成的数值
	int power = 1000;
	int j = 1;//控制decNum数组的下标,留出下标为0的空间一便以后补9999或0000
	int lenth;

	if(num == NULL)
	{
		return;
	}
	
	lenth = strlen(num);
	for(i = 0; i < lenth; i++)
	{
		number += (num[i] - '0') * power;

		if(power <= 1)
		{
			hug->decNum[j++] = number;
			number = 0;
			power = 1000;
		}
		else
			power /= 10;
	}
	//处理最后剩余问题
	if(number)
	{
		hug->decNum[j] = number;
	}
}

void getIntNum(HUGE_NUM *hug, char *num, int intEndIndex)
{	
	int index;
	int i;
	int number = 0;//用于保存生成的数值
	int power = 1;
	int j = 0;//控制intNum数组的下标，留出下标为intCount-1的空间一便以后补9999或0000
	int numStart;//表示num中是否带符号

	if(num == NULL)
	{
		return;
	}

	hug->sign = num[0] == '-' ? 1 : 0;
	numStart = (num[0] == '+' || num[0] =='-') ? 1 : 0;
	index = intEndIndex == NOT_FOUND_INDEX ? strlen(num) - 1 : intEndIndex; 

	for(i = index; i >= numStart; i--)
	{
		number += (num[i] - '0') * power;

		if(power >= 1000)
		{
			hug->intNum[j++] = number;
			number = 0;
			power = 1;
		}
		else
			power *= 10;
	}
	//处理最后剩余问题
	if(number) 
	{
		hug->intNum[j] = number;
	}
}

void getNum(HUGE_NUM **head, char *num)
{
	int index = 0;
	char *p;
	int intLen = 0;
	int decLen = 0;
	int intEndIndex = NOT_FOUND_INDEX;//整数位的最后一位的下标

	if(num == NULL)
	{
		return;
	}

	index = (num[0] == '+' || num[0] =='-') ? 1 : 0;
	p = strchr(num , '.');
	if(p)
	{
		*p = 0;					 //num	 p
		intEndIndex = p - num -1;//+123456.1478
								 //012345678901
		p++;
		decLen = strlen(p);
	}
	
	intLen = strlen(num + index);

	//初始化巨大数控制头
	if(FALSE == initHuge(head, (intLen+3)/4, (decLen+3)/4))
	{
		return;
	}
	//处理整数部分
	getIntNum(*head, num, intEndIndex);
	//处理小数部分
	getDecNum(*head,p);
}

boolean destoryHuge(HUGE_NUM **head)
{
	if(*head != NULL)
	{
		return FALSE;
	}

	free((*head)->intNum);
	free((*head)->decNum);
	free(*head);
	*head = NULL;

	return TRUE;
}

boolean initHuge(HUGE_NUM **head, int intCount, int decCount)
{
	if(*head)
	{
		return FALSE;
	}

	*head = (HUGE_NUM *)calloc(sizeof(HUGE_NUM),1);
	if(intCount++)//多申请一组，以便补9999或0000
	{
		(*head)->intNum = (int *)calloc(sizeof(int), intCount);
		(*head)->intCount = intCount;
	}
	if(decCount++)//多申请一组
	{
		(*head)->decNum = (int *)calloc(sizeof(int), decCount);
		(*head)->decCount = decCount;
	}

	return TRUE;
}

void main(void)
{
	char num[80];

	HUGE_NUM *huge1 = NULL;
	HUGE_NUM *huge2 = NULL;
	HUGE_NUM *resHuge = NULL;
	printf("请输入数字:\n");

	gets(num);

	getNum(&huge1, num);
	showHuge(*huge1);

	printf("请输入数字:\n");

	gets(num);

	getNum(&huge2, num);
	showHuge(*huge2);

	addHuge(*huge1, *huge2, &resHuge);
	showHuge(*resHuge);

	subHuge(*huge1, *huge2, &resHuge);
	showHuge(*resHuge);

	//toAntiCode(*huge1);
	showHuge(*huge1);
	showHuge(*huge2);

	destoryHuge(&huge1);
}
