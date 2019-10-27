// Medical_Information_Hiding_System.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include"SpreadSheet.h"
#include<typeinfo>
#include<stdlib.h>

//
static int r;

//医疗信息存储的数据结构
struct Patient
{
	char id[20];//标识符：数据的标号
	char age[20];//准标识符：年龄
	char region[20];//准标识符：地区
	char postcode[10];//准标识符：邮编
	char health[20];//敏感信息：健康状况
	char Sq_ID[10];//敏感组ID
	float weight;//权重
	int total;//总计，由于int是直接省去小数部分，符合判定需要。
	bool operator==(const Patient& rhs)//操作运算符重载：==表示两个结构体变量中的所有准标识符均相等
	{
		return (strcmp(age, rhs.age) == 0) && (strcmp(region, rhs.region) == 0) && (strcmp(postcode, rhs.postcode) == 0);
	}
}data[MAXDATANUM];//

//用户登陆模块
void login() 
{
	std::cout << "欢迎使用本医疗信息隐藏系统\n";
	printf("须知\n本系统目前所能处理的最大数据容量为100条数据");
	char user[17];
	char pwd[21];
	int count = 0;
	do
	{
		if (count > 0)
			puts("Not logged in\n");
		printf("请输入您的用户名和密码\n");
		printf("username = ");
		gets_s(user, 17);
		printf("password = ");
		gets_s(pwd, 21);
		count++;
	} while ((strcmp(user, USER)) || (strcmp(pwd, PWD)));
	puts("Successful user login!");
}

//用户参数输入
void parameter_input(int *k,int *p,int *a)
{	
	int select;
	printf("本系统采用(p+，a)-sensitive K-匿名化\n");
	printf("[1]标准匿名化（推荐）	[2]自定义匿名化参数		[3]HELP\n");
	scanf_s("%d", &select);

	if (select == 1)
	{
		*p = 2; *a = 2; *k = 4;
	}
	else if (select == 2)
	{
		printf("请输入p值(p=1,2,3,...,n,建议p取4)\np=");
		scanf_s("%d", p);
		printf("请输入a值(a=1,2,3,...,n,建议P取4)\na=");
		scanf_s("%d", a);
		printf("请输入K值(K=1,2,3,...,n,建议K取4)\nK=");
		scanf_s("%d", k);
	}
	else if (select == 3)
	{
		printf("【帮助】");
	}
	else
	{
		printf("输入有误，如需帮助输入3，查看帮助信息\n");
		parameter_input(k,p,a);
	}
}

//K匿名检测 满足K匿名检测则返回1
bool kcheck(int k)
{
	int flag = 1;//flag标志第i条数据是否满足k匿名检测，1表示真：满足，0表示假：不满足
	for (int i = 0; i < 12 && flag == 1; i++)
	{
		for (int t = 0, j = 1; t < 12; t++)
		{
			if (t == i)
			{
				continue;
			}
			if (data[i] == data[t])
			{
				j++;
			}
			if (j == k)
			{
				flag = 1;
				break;
			}
			flag = 0;
		}
	}
	return flag;
}

//输出excel表格
void printdata(int r,char p,char a,char k) 
{
	char Resulttablename[50] = "（PPLUS，A）sensitive—K—匿名化结果";
	//Resulttablename[3] = p;
	//Resulttablename[9] = a;
	//Resulttablename[23] = k;
	CSpreadSheet sheet2("result.xls", Resulttablename);
	CStringArray headers;
	headers.Add("ID"); headers.Add("年龄"); headers.Add("地区"); headers.Add("邮编");
	headers.Add("健康状况");	headers.Add("敏感组ID");	headers.Add("权重");	headers.Add("总计");
	sheet2.BeginTransaction();	
	sheet2.AddHeaders(headers);
	for (int i = 0; i < r; i++)
	{
		CStringArray row;
		char buf1[5];
		snprintf(buf1, sizeof(buf1), "%f", data[i].weight);
		char buf2[5];
		snprintf(buf2, sizeof(buf2), "%d", data[i].total);
		row.Add(data[i].id); row.Add(data[i].age); row.Add(data[i].region); row.Add(data[i].postcode); row.Add(data[i].health); row.Add(data[i].Sq_ID); row.Add(buf1); row.Add(buf2);
		sheet2.AddRow(row);
	}
	sheet2.Commit();
}

//统计准标识符,返回其最大值所对应的准标识符
int count() 
{
	int a[MAXDATANUM];//age中不同的标识符
	a[0] = 0;//先存放一个
	int reg[MAXDATANUM];//regin中不同的标识符的下标
	reg[0] = 0;//先放一个
	int c[MAXDATANUM];//class_num中不同的标识符的下标
	c[0] = 0;//先放一个
	int n[MAXDATANUM];
	for (int ii = 0; ii < QINUM; ii++)
	{
		n[ii] = 1;
	}//表示五个数值各自的当前长度,即为统计值,初始为1，因为之前放进去了一个
	for (int i = 1; i < r; i++)//遍历数据
	{
		for (int j = 0; j < n[0]; j++)//将data[i].regin于r[12]里头所有的下标对应的data[r[j]]对比,若重复break。否则添加到r[n[1]],n[1]++;
		{
			if (strcmp(data[i].age, data[a[j]].age) == 0)
			{
				break;
			}
			else if ((j + 1) == n[0])
			{
				a[n[0]] = i;
				n[0]++;
				break;
			}
		}

	}
	for (int i = 1; i < r; i++)//遍历数据
	{
		//再统计regin的统计值
		for (int j = 0; j < n[1]; j++)//将data[i].regin于r[12]里头所有的下标对应的data[r[j]]对比,若重复break。否则添加到r[n[1]],n[1]++;
		{
			if (strcmp(data[i].region, data[reg[j]].region) == 0)//data[i].regin == data[r[j]].regin//)
			{
				break;
			}
			else if ((j + 1) == n[1])
			{
				reg[n[1]] = i;
				n[1]++;
				break;
			}
		}

	}
	for (int i = 1; i < r; i++)//遍历数据
	{
		//再统计class_num的统计值
		for (int j = 0; j < n[2]; j++)//将data[i].regin于r[12]里头所有的下标对应的data[r[j]]对比,若重复break。否则添加到r[n[1]],n[1]++;
		{
			if (strcmp(data[i].postcode, data[c[j]].postcode) == 0)
			{
				break;
			}
			else if ((j + 1) == n[2])
			{
				c[n[2]] = i;
				n[2]++;
				break;
			}
		}

	}
	int max = 0;
	int temp = 0;
	for (int i = 0; i < 3; i++)
	{
		if (n[i] > temp)
		{
			temp = n[i];
			max = i;
		}
	}
	return max;
}

//泛化
void generalization(int max)
{
	switch (max)
	{
	case 0:
		if ((strcmp(data[0].age, "<30") != 0) && (strcmp(data[0].age, ">40") != 0) && (strcmp(data[0].age, "3*") != 0)&& (strcmp(data[0].age, "<40") != 0)&& (strcmp(data[0].age, "*") != 0))//数据从未经过泛化
		{
			for (int i = 0; i < r; i++)
			{
				if (strcmp(data[i].age, "30") < 0)
				{
					strcpy_s(data[i].age, 10, "<30");
				}
				else if (strcmp(data[i].age, "40") > 0)
				{
					strcpy_s(data[i].age, 10, ">40");
				}
				else
				{
					strcpy_s(data[i].age, 10, "3*");
				}
			}
		}
		else if ((strcmp(data[0].age, "<30") == 0) || (strcmp(data[0].age, "3*") == 0)|| (strcmp(data[0].age, ">40") == 0))//结果是第一次泛化后结果
		{
			for (int i = 0; i < r; i++)
			{
				if((strcmp(data[i].age, "<30") == 0)|| (strcmp(data[i].age, "3*") == 0))
				{
					
					strcpy_s(data[i].age, 10, "< 40");
				}
				else if (strcmp(data[i].age, ">40") == 0)
				{
					strcpy_s(data[i].age, 10, "> 40");
				}
				else
				{
					strcpy_s(data[i].age, 10, "*");
				}
			}
		}
		else//本系统最多对同一“有效”准标识符泛化2次
		{
			for (int i = 0; i < 12; i++)
			{
				strcpy_s(data[i].age, 10, "*");
			}
		}
		break;
	case 1:
		if ((strcmp(data[0].region, "广东") != 0) && (strcmp(data[0].region, "广西") != 0) && (strcmp(data[0].region, "其他") != 0))//数据还未被泛化过
		{
			for (int i = 0; i < r; i++)
			{
				if ((strcmp(data[i].region, "中山") == 0) || (strcmp(data[i].region, "东莞") == 0))
				{
					strcpy_s(data[i].region, 20, "广东");
				}
				else if ((strcmp(data[i].region, "桂林") == 0) || (strcmp(data[i].region, "北海") == 0) || (strcmp(data[i].region, "南宁") == 0))
				{
					strcpy_s(data[i].region, 20, "广西");
				}
				else
				{
					strcpy_s(data[i].region, 20, "其他");
				}
			}
		}
		else
		{
			for (int i = 0; i < r; i++)
			{
				strcpy_s(data[i].region, 20, "*");
			}
		}
		break;
	case 2:
		if (data[0].postcode[5] != '*')//第一次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[5] = '*';
			}
		}
		else if (data[0].postcode[4] != '*')//第二次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[4] = '*';
			}
		}
		else if (data[0].postcode[3] != '*')//第二次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[3] = '*';
			}
		}
		else if (data[0].postcode[2] != '*')//第二次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[2] = '*';
			}
		}
		else if (data[0].postcode[1] != '*')//第二次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[1] = '*';
			}
		}
		else if (data[0].postcode[0] != '*')//第二次泛化class_num
		{
			for (int i = 0; i < r; i++)
			{
				data[i].postcode[0] = '*';
			}
		}
		else 
		{
			for (int i = 0; i < 12; i++)
				strcpy_s(data[i].age, 10, "*");
		}
		break;
	default:
		break;
	}
}

//datafly算法
void datafly(int k) 
{
	while (kcheck(k) == 0)//泛化后的表格进行k匿名检测。
	{
		generalization(count());//如果泛化后的数据表符合k匿名检测，则输出，如果不符合，goto1。
	}
}

//读取excel表格,返回数据数量
int readdata(char filename[20],char tablename[20]) 
{
	CSpreadSheet sheet1(filename, tablename);
	CStringArray Rows;
	sheet1.BeginTransaction();
	for (int i = 1; i < sheet1.GetTotalRows(); i++)
	{
		sheet1.ReadRow(Rows, i+1);
		strncpy_s(data[i-1].id, Rows.GetAt(0), sizeof(data[i-1].id));//CString转char*
		strncpy_s(data[i - 1].age, Rows.GetAt(1), sizeof(data[i - 1].age));
		strncpy_s(data[i - 1].region, Rows.GetAt(2), sizeof(data[i - 1].region));
		strncpy_s(data[i - 1].postcode, Rows.GetAt(3), sizeof(data[i - 1].postcode));
		strncpy_s(data[i - 1].health, Rows.GetAt(4), sizeof(data[i - 1].health));
		if(strcmp(data[i-1].health,TopSecret[0])==0|| (strcmp(data[i - 1].health, TopSecret[1]) == 0))
		{
			strncpy_s(data[i - 1].Sq_ID, "One", sizeof(data[i - 1].Sq_ID));
		}else if (strcmp(data[i - 1].health, Secret[0]) == 0 || (strcmp(data[i - 1].health, Secret[1]) == 0))
		{
			strncpy_s(data[i - 1].Sq_ID, "Two", sizeof(data[i - 1].Sq_ID));
		}else if (strcmp(data[i - 1].health, LessSecret[0]) == 0 || (strcmp(data[i - 1].health, LessSecret[1]) == 0))
		{
			strncpy_s(data[i - 1].Sq_ID, "Three", sizeof(data[i - 1].Sq_ID));
		}else if (strcmp(data[i - 1].health, NonSecret[0]) == 0 || (strcmp(data[i - 1].health, NonSecret[1]) == 0))
		{
			strncpy_s(data[i - 1].Sq_ID, "Four", sizeof(data[i - 1].Sq_ID));
		}
		else
		{
			strncpy_s(data[i - 1].Sq_ID, "Unknow", sizeof(data[i - 1].Sq_ID));
		}
	}
	return sheet1.GetTotalRows()-1;
}


int main()
{
	login();//用户登陆
	char filename[20];
	char tablename[20];
	printf("请确保将原数据文件以.xls格式放于/Medical_Information_Hiding_System/目录下\n");
	system("pause");
	printf("请输入文件名与工作表名\n");
	printf("filename=");
	gets_s(filename, 20);
	printf("tablename=");
	gets_s(tablename, 20);
	int k, p, a;
	parameter_input(&k,&p,&a);//用户输入
	r=readdata(filename, tablename);//读数据
	datafly(k);
	printdata(r, p, a, k);//输出数据
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
