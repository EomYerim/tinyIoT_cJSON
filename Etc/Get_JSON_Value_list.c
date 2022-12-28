#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

char *Get_JSON_Value_list(char *key, char *json) {
	char json_copy[300];
	char *resource = NULL;
	char *value = NULL;

	cJSON *root = NULL;
	cJSON *ckey = NULL;

	cJSON *cjson = cJSON_Parse(json);
	if (cjson == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	//Extracting resources from json
	strcpy(json_copy, json);
	resource = strstr(json_copy, "m2m:");
	resource = strtok(resource, "\"");

	root = cJSON_GetObjectItem(cjson, resource);

	if (strstr(resource, "acp") != NULL) {	// acp 처리  ex) key: pv-acr-acor, pv-acr-acop, pvs-acr-acor, pvs-acr-acop
		// pv / pvs
		char *pv_str = strtok(key, "-");
		cJSON *pv = cJSON_GetObjectItem(root, pv_str);

		// acr
		char *acr_str = strtok(NULL, "-");
		cJSON *acrs = cJSON_GetObjectItem(pv, acr_str);
		int acr_size = cJSON_GetArraySize(acrs);

		// acor / acop
		char *ckey_str = strtok(NULL, "-");
		cJSON *acr = NULL;
		cJSON *acor = NULL;
		cJSON *acop = NULL;
		char acorp_str[100] = { "\0" };
		int i = 0;
		cJSON_ArrayForEach(acr, acrs) {
			acor = cJSON_GetObjectItem(acr, "acor");
			int acor_size = cJSON_GetArraySize(acor);
			if (!strcmp(ckey_str, "acop")) {
				acop = cJSON_GetObjectItem(acr, "acop");
			}
			for (int j = 0; j < acor_size; j++) {
				if (strstr(ckey_str, "acor") != NULL) {	// acor
					strcat(acorp_str, cJSON_GetArrayItem(acor, j)->valuestring);
				}
				else {	// acop
					strcat(acorp_str, strtok(cJSON_Print(acop), "\""));
				}
				if (j < acor_size - 1) {
					strcat(acorp_str, ",");
				}
			}
			if (i < acr_size - 1)
				strcat(acorp_str, ",");
			i++;
		}

		value = (char *)malloc(sizeof(char) * strlen(acorp_str) + 1);
		strcpy(value, acorp_str);
	}
	else {	// acp 이외 처리
		// key 값 검사 : '-' 유무 확인
		if (strstr(key, "-") != NULL) {	// '-' 존재 ex) key: enc-net
			char *subRoot_str = strtok(key, "-");
			cJSON *subRoot = cJSON_GetObjectItem(root, subRoot_str);
			key = strtok(NULL, "-");
			ckey = cJSON_GetObjectItem(subRoot, key);
		}
		else {	// '-' 존재 X ex) key: acpi
			ckey = cJSON_GetObjectItem(root, key);
		}

		int ckey_size = cJSON_GetArraySize(ckey);
		char ckey_str[100] = { '\0' };
		char tmp[10] = { '\0' };
		for (int i = 0; i < ckey_size; i++) {
			if (cJSON_GetArrayItem(ckey, i)->valuestring != NULL) {	// 문자열 처리
				strcat(ckey_str, cJSON_GetArrayItem(ckey, i)->valuestring);
			}
			else {	// 정수 처리
				sprintf(tmp, "%d", cJSON_GetArrayItem(ckey, i)->valueint);
				strcat(ckey_str, tmp);
			}

			if (i < ckey_size - 1) {
				strcat(ckey_str, ",");
			}
		}
		value = (char *)malloc(sizeof(char) * strlen(ckey_str) + 1);
		strcpy(value, ckey_str);
	}


end:
	cJSON_Delete(cjson);

	return value;
}