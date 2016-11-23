// JsonShader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>

//#define MUL 1
#define TOFILE 1

struct ShaderProgram
{
    struct Path
    {
        char *vertex_shader_path;
        char *fragment_shader_path;
    } path;

    struct FragmentShaderUniformLocation
    {
        char *channel_0;
        char *channel_1;
        char *global_time;
        char *resolution;
    } fsul;

};

void printJSON(cJSON *json)
{
    char *out = cJSON_Print(json);
    printf("%s\n", out);
    free(out);
}

/* Parse text to JSON, then render back to text, and print! */
void easyParse(char *text)
{

    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
		printJSON(json);
        cJSON_Delete(json);
    }
}

void itemParse(char *text)
{

    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        int size_0 = cJSON_GetArraySize(json);
        printf("Shader Program Nums = %d\n", size_0);

        for (int i = 0; i < size_0; i++)
        {
            cJSON *pItem = cJSON_GetArrayItem(json, i);
            if (NULL == pItem)
            {
                continue;
            }
            int size_1 = cJSON_GetArraySize(pItem);
            //printf("Shader Program Item Nums = %d\n", size_1);
            printf("===========================\n");
            for (int j = 0; j < size_1; j++)
            {
                cJSON *ppItem = cJSON_GetArrayItem(pItem, j);

                //printJSON(ppItem);

                int size_2 = cJSON_GetArraySize(ppItem);
                printf("Shader Program Item Size  = %d\n", size_2);
                //printf("---------------------------\n");
                for (int k = 0; k < size_2; k++)
                {
                    cJSON *pppItem = cJSON_GetArrayItem(ppItem, k);

                    printJSON(pppItem);
                }
                printf("===========================\n");
            }
        }
    }

    cJSON_Delete(json);
}

/* Read a file, parse, render back, etc. */
void parseFile(char *filename)
{
    FILE *f;
    long len;
    char *data;

    f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);

    //easyParse(data);
    itemParse(data);

    free(data);
}

void writeToFile(const char *file_name, const char *out)
{
    // 获得待写入的字符长度
    int len = strlen(out);
    // 测试
    //printf("len = %d\n", len);

    FILE *fd = fopen(file_name, "wb");	// w
    fwrite(out, 1, len, fd);

    fclose(fd);
}

void createObjects()
{
#if MUL
	ShaderProgram sp[2] =
	{
		{
			{ "../Shader/vs_0.glsl", "../Shader/fs_0.glsl" },
			{ "iChannel0", "iChannel1", "iGlobalTime", "iResolution" }
		},	// sp[0]

		{
			{ "../Shader/vs_1.glsl", "../Shader/fs_1.glsl" },
			{ "iChannel0", "iChannel1", "iGlobalTime", "iResolution" }
		}	// sp[1]
	};
#else
	ShaderProgram sp =
	{
		{ "../Shader/vs_0.glsl", "../Shader/fs_0.glsl" },
		{ "iChannel0", "iChannel1", "iGlobalTime", "iResolution" }
	};

#endif

	cJSON *root = cJSON_CreateArray();
	cJSON *fld = NULL, *fldd_0 = NULL, *fldd_1;

#if MUL
	for (int i = 0; i < 2; i++)
	{
		cJSON_AddItemToArray(root, fld = cJSON_CreateArray());

		cJSON_AddItemToArray(fld, fldd_0 = cJSON_CreateObject());
		cJSON_AddStringToObject(fldd_0, "Vertex Shader Path", sp[i].path.vertex_shader_path);
		cJSON_AddStringToObject(fldd_0, "Fragment Shader Path", sp[i].path.fragment_shader_path);

		cJSON_AddItemToArray(fld, fldd_1 = cJSON_CreateObject());
		cJSON_AddStringToObject(fldd_1, "Channel 0", sp[i].fsul.channel_0);
		cJSON_AddStringToObject(fldd_1, "Channel 1", sp[i].fsul.channel_1);
		cJSON_AddStringToObject(fldd_1, "Global Time", sp[i].fsul.global_time);
		cJSON_AddStringToObject(fldd_1, "Resolution", sp[i].fsul.resolution);
	}
#else

	cJSON_AddItemToArray(root, fld = cJSON_CreateArray());

	cJSON_AddItemToArray(fld, fldd_0 = cJSON_CreateObject());
	cJSON_AddStringToObject(fldd_0, "Vertex Shader Path", sp.path.vertex_shader_path);
	cJSON_AddStringToObject(fldd_0, "Fragment Shader Path", sp.path.fragment_shader_path);

	cJSON_AddItemToArray(fld, fldd_1 = cJSON_CreateObject());
	cJSON_AddStringToObject(fldd_1, "Channel 0", sp.fsul.channel_0);
	cJSON_AddStringToObject(fldd_1, "Channel 1", sp.fsul.channel_1);
	cJSON_AddStringToObject(fldd_1, "Global Time", sp.fsul.global_time);
	cJSON_AddStringToObject(fldd_1, "Resolution", sp.fsul.resolution);

#endif

	char *out = cJSON_Print(root);
	// 保存信息到文件
	writeToFile("result.txt", out);
	printf("Debug Info: %s", out);
	free(out);

	cJSON_Delete(root);
}

int main()
{
#ifdef TOFILE
	createObjects();
#else
    parseFile("result.txt");
#endif
    return 0;
}

