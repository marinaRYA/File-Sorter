# define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include < stdio.h >
#include<io.h>
#include <fstream>
#include <stdarg.h>
#define N 25
#define L 200
using namespace std;
struct strings {
	char name[N];
	int len;
	int pos;
	int flag;
	int operator > (strings& right)
	{
		if (len > right.len) return 1;
		return 0;
	}
	int operator < (strings& right)
	{
		if (len < right.len) return 1;
		return 0;
	}
	strings& operator = (strings& c)
	{
		len = c.len;
		strcpy(name, c.name);
		pos = c.pos;
		flag = c.flag;
		return *this;
	}
};
void Sort_Min(strings* str, int& n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = (n - 1); j > i; j--) // для всех элементов после i-ого
		{
			if (str[j - 1] > str[j]) // если текущий элемент меньше предыдущего
			{
				strings t = str[j - 1]; // меняем их местами
				str[j - 1] = str[j];
				str[j] = t;
			}
		}
	}
}

void Sort_Max(strings* str, int& n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = (n - 1); j > i; j--) // для всех элементов после i-ого
		{
			if (str[j - 1] < str[j]) // если текущий элемент меньше предыдущего
			{
				strings t = str[j - 1]; // меняем их местами
				str[j - 1] = str[j];
				str[j] = t;
			}
		}
	}
}
void Result(strings* str, int& n, int v)
{
	void (*Sort) (strings * str, int& n);
	if (v == 1) Sort = Sort_Min;
	else Sort = Sort_Max;
	Sort(str, n);
	FILE* F = fopen("result.txt", "w+");
	FILE* f;
	char* s;
	for (int i = 0; i < n; i++)
	{
		if (str[i].flag == 0)
		{
			f = fopen(str[i].name, "rt");
			fseek(f, str[i].pos, SEEK_SET);
			s = (char*)malloc((str[i].len + 1) * sizeof(char));
			fgets(s, str[i].len, f);
			fprintf(F, "%s\n", s);
			fclose(f);
		}
		else
		{
			f = fopen(str[i].name, "rb");
			fseek(f, str[i].pos, SEEK_SET);
			s = (char*)malloc((str[i].len + 1) * sizeof(char));
			fread(s, sizeof(char), str[i].len - 1, f);
			fprintf(F, "%s\n", s);
			fclose(f);
		}
	}
	fclose(F);
}


void Init_str(strings* str, int& n, char filename[], char txt)
{
	int len, pos = 0;
	char s[200];
	FILE* f;
	if ((f = fopen(filename, "rt")) == NULL) printf("error");
	else
	{
		f = fopen(filename, "rt");
		while (!feof(f))
		{
			fgets(s, 200, f);
			len = strlen(s);
			str[n].pos = pos;
			str[n].len = len;
			strcpy(str[n].name, filename);
			str[n].flag = 0;
			pos += (len + 1) * sizeof(char);
			if (n + 1 > 200)  str = (strings*)realloc(str, (n++) * sizeof(strings));
			n++;
		}
	}
	fclose(f);
}
int Init_str(strings* str, int& n, char f_name[N], int bin)
{
	FILE* f;
	char* s;
	int pos = 0;
	if ((f = fopen(f_name, "rb")) == NULL) puts("Ошибка загрузки"); // Открываем файл для чтения
	else
	{
		int d;
		int pos = 0;
		while (!feof(f))
		{
			fread(&d, sizeof(d), 1, f);
			s = (char*)malloc((d + 1) * sizeof(char));
			fread(s, sizeof(char), d, f);
			str[n].len = d;
			pos += sizeof(d);
			str[n].pos = pos;
			pos += d * sizeof(char);
			str[n].flag = 1;
			strcpy(str[n].name, f_name);
			if (n + 1 > 200)  str = (strings*)realloc(str, n + 1 * sizeof(strings));
			n++;
		}
		fclose(f);
	}
	return 0;
}
void Add_File(strings* str, int& n, int count, ...)
{
	char txt = '0';
	int bin = 0;
	char f_name[N];
	va_list ap;
	va_start(ap, count);
	for (int i = 1; i <= count; i++) {
		strcpy(f_name, va_arg(ap, const char*));
		int len = strlen(f_name);
		int j = 0;
		while ((f_name[j] != '.') && (j <= len)) j++;
		j++;
		if ((f_name[j] == 't') && (f_name[j + 1] == 'x') && (f_name[j + 2] == 't') && (j + 2 < len))
		{
			txt = '1';
			Init_str(str, n, f_name, txt);
		}
		if ((f_name[j] == 'b') && (f_name[j + 1] == 'i') && (f_name[j + 2] == 'n') && (j + 2 < len))
		{
			bin = 1;
			Init_str(str, n, f_name, bin);
		}
		if ((bin == 0) && (txt == '0')) printf("%s- неверный формат файла", f_name);
	}
	va_end(ap);
}

void Menu()
{
	strings* str = (strings*)malloc(200 * sizeof(strings));
	int n = 0;
	char c;
	const char* ss[] = { "0 - добавить новые файлы", "1 - Запустить сортировку ", "2 - ВЫХОД" };
	int k = sizeof(ss) / sizeof(ss[0]);
	int n_files = 0;
	int count = 0;
	for (;;)
	{
		for (int i = 0; i < k; i++) puts(ss[i]);
		c = _getch(); // выбор действия
		if (c == '0') {
			char file_name[4][N];
			printf("введите количество файлов (до 4ёх) ");
			scanf_s("%d", &count);
			if (count > 4)
			{
				printf("error");
				return;
			}
			else
			{
				for (int j = 0; j < count; j++)
				{
					printf("введите название файла:");
					scanf("%s", file_name[j]);
				}
				switch (count)
				{
				case 1: Add_File(str, n, count, file_name[0]); break;
				case 2: Add_File(str, n, count, file_name[0], file_name[1]); break;
				case 3: Add_File(str, n, count, file_name[0], file_name[1], file_name[2]); break;
				case 4: Add_File(str, n, count, file_name[0], file_name[1], file_name[2], file_name[3]); break;
				}
			}
		}
		if (c == '1')
		{
			printf("Выберете тип сортировки\n1-по возрастанию\n2-по убыванию\n");
			int v;
			scanf_s("%d", &v);
			if ((v == 1) || (v == 2)) Result(str, n, v);
			else printf("error");
		}

		if (c == '2') return;
	}
}


int main()
{
	//setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F3");
	Menu();
}

