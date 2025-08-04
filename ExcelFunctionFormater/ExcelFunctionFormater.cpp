// ExcelFunctionFormater.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <cstdio>
#include <fstream>
#include <cstring>

using namespace std;

char* global_expression_vector[1024] = { nullptr };
unsigned short global_expression_sum = 0;

void Formater(char* buffer, size_t buf_size, char* str, size_t str_size);
bool SplitExpression(char* str, size_t str_size, char split_flag = '=');
void InsertControlChar(void);
void RemoveControlChar(void);

int main(int argc,char* argv[])
{
	ifstream in;
	ofstream out;
	char* data = nullptr;
	size_t data_size = 0;

	if (argc != 3) return 0;

	in.open(argv[1]);

	if (!in.is_open()) return -1;

	size_t offset = 0;
	do {
		char* temp;
		data_size += 1024 * sizeof(char);
		temp = (char*)realloc(data, data_size);
		if (temp == nullptr) return -2;
		else data = temp;
		memset(data + offset, '\0', data_size - offset);
		in.read(data + offset, 1024 * sizeof(char));
		offset += 1024;
	} while (!in.eof());

	in.close();

	out.open(argv[2]);
	if (!out.is_open()) return -1;

	size_t buffer_size = data_size * 2;
	char* buffer = (char*)malloc(buffer_size);
	memset(buffer, '\0', buffer_size);
	Formater(buffer, buffer_size, data, data_size);
	out.write(buffer, buffer_size);
	out.close();

	free(buffer);
	free(data);

    return 0;
}

void Formater(char* buffer, size_t buf_size, char* str, size_t str_size = -1) {

	if (!SplitExpression(str, str_size)) { 
		printf("Illegal syntax.\n"); 
		return; 
	}

	RemoveControlChar();

	InsertControlChar();

	for (size_t i = 0, offset=0; i < global_expression_sum; i++) 
		strcat_s(buffer, buf_size, global_expression_vector[i]);

	// 析构global_expression_sum
	for (size_t i = 0; i < global_expression_sum; i++)
		free(global_expression_vector[i]);

	return;
}

// 以split_flag为标志分割每个表达式，默认为“=”，结果保存在global_expression_vector中
bool SplitExpression(char* str, size_t str_size, char split_flag) {
	int index_flag = 0;

	int bracket_count = 0;

	// 构造global_expression_sum
	for (size_t i = 0; i < str_size; i++) {

		if (str[i] == '(') bracket_count++;
		if (str[i] == ')') bracket_count--;

		if (((str[i] == split_flag && bracket_count == 0) || str[i] == '\0') && (i != index_flag)) {
			//printf("%c %d %d\n", str[i], str[i],i);
			size_t temp_size = (i - index_flag) * sizeof(char);
			char* temp = (char*)malloc(temp_size + 1);
			if (temp == nullptr) exit(-2);
			memset(temp, '\0', temp_size + 1);
			memcpy(temp, str + index_flag, temp_size);
			global_expression_vector[global_expression_sum++] = temp;
			index_flag = i;
		}

		if (str[i] == '\0') break;
	}
	return bracket_count == 0 ? true : false;
}

// 移除控制字符（\t,\n,\r,空格），对global_expression_vector进行处理，无传参/返回
void RemoveControlChar(void) {
	for (int i = 0; i < global_expression_sum; i++) {
		char* expression = global_expression_vector[i];
		char* temp_expression = (char*)malloc(strlen(expression) * sizeof(char));
		if (temp_expression == nullptr) exit(-2);
		memset(temp_expression, '\0', strlen(expression) * sizeof(char));

		size_t index_tmp_expression = 0;
		for (size_t j = 0; j < strlen(expression); j++) {
			if (expression[j] != ' ' && expression[j] != '\n' && expression[j] != '\r' && expression[j] != '\t')
				temp_expression[index_tmp_expression++] = expression[j];
		}
		//printf("%d\n", strlen(expression) * sizeof(char));
		strcpy_s(expression, strlen(expression) * sizeof(char), temp_expression);
		free(temp_expression);
	}
}

// 插入控制字符（\t,\n），根据“(”、“)”、“,”重新整理global_expression_vector中的数据，插入换行符和水平制表符
void InsertControlChar(void) {
	for (int i = 0; i < global_expression_sum; i++) {
		const size_t temp_length = strlen(global_expression_vector[i]) * 2;
		char* temp = (char*)calloc(temp_length, sizeof(char));
		if (temp == nullptr) exit(-2);
		const char* expression = global_expression_vector[i];

		int j = 0, k = 0;
		int tab_count = 0;
		for (j = 0; expression[j] != '\0'; j++) {
			if (k == temp_length * sizeof(char)) {
				free(temp);
				return;
			}
			
			switch (expression[j])
			{
			case '(':
				temp[k++] = expression[j];
				tab_count++;
				temp[k++] = '\n';
				for (int _ = 0; _ < tab_count; _++) temp[k++] = '\t';
				break;
			case ')':
				tab_count--;
				temp[k++] = '\n';
				for (int _ = 0; _ < tab_count; _++) temp[k++] = '\t';
				temp[k++] = expression[j];
				//if (expression[j + 1] != ',') temp[k++] = '\n';
				break;
			case ',':
				temp[k++] = expression[j];
				temp[k++] = '\n';
				for (int _ = 0; _ < tab_count; _++) temp[k++] = '\t';
				break;
			default:
				temp[k++] = expression[j];
				break;
			}
		}
		temp[k++] = '\n';
		
		if (strlen(global_expression_vector[i]) * sizeof(char) < temp_length * sizeof(char)) {
			char* tmp_pointer = (char*)realloc(global_expression_vector[i], temp_length * sizeof(char));
			if (tmp_pointer == nullptr) exit(-2);
			global_expression_vector[i] = tmp_pointer;
		}
		strcpy_s(global_expression_vector[i], temp_length * sizeof(char), temp);

		free(temp);
	}
}