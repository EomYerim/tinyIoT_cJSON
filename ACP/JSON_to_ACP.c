#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

ACP* JSON_to_ACP(char *json_payload) {
	ACP *acp = (ACP *)malloc(sizeof(ACP));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *pv = NULL;
	cJSON *pvs = NULL;
	cJSON *acrs = NULL;
	cJSON *acr = NULL;
	cJSON *acor = NULL;
	cJSON *acop = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:acp");

	// rn
	rn = cJSON_GetObjectItem(root, "rn");
	if (!cJSON_IsString(rn) && rn->valuestring == NULL)
	{
		goto end;
	}
	acp->rn = cJSON_Print(rn);
	acp->rn = strtok(acp->rn, "\"");

	// pv
	pv = cJSON_GetObjectItem(root, "pv");

	// acr (접근 제어 규칙)
	acrs = cJSON_GetObjectItem(pv, "acr");
	int acr_size = cJSON_GetArraySize(acrs);
	char acor_str[100] = { '\0' };
	char acop_str[100] = { '\0' };
	int i = 0;
	cJSON_ArrayForEach(acr, acrs) {
		acor = cJSON_GetObjectItem(acr, "acor");
		int acor_size = cJSON_GetArraySize(acor);
		for (int j = 0; j < acor_size; j++) {
			strcat(acor_str, cJSON_GetArrayItem(acor, j)->valuestring);
			if (j < acor_size - 1) {
				strcat(acor_str, ",");
			}
		}
		if (i < acr_size - 1)
			strcat(acor_str, ",");

		acop = cJSON_GetObjectItem(acr, "acop");
		for (int j = 0; j < acor_size; j++) {
			strcat(acop_str, strtok(cJSON_Print(acop), "\""));
			if (j < acor_size - 1) {
				strcat(acop_str, ",");
			}
		}
		if (i < acr_size - 1)
			strcat(acop_str, ",");
		i++;
	}
	acp->pv_acor = (char *)malloc(sizeof(char) * strlen(acor_str) + 1);
	strcpy(acp->pv_acor, acor_str);

	acp->pv_acop = (char *)malloc(sizeof(char) * strlen(acop_str) + 1);
	strcpy(acp->pv_acop, acop_str);


	// pvs
	pvs = cJSON_GetObjectItem(root, "pvs");

	// acr
	acrs = cJSON_GetObjectItem(pvs, "acr");
	acr_size = cJSON_GetArraySize(acrs);
	memset(acor_str, 0, 100);	// 배열 재 초기화
	memset(acop_str, 0, 100);
	i = 0;
	cJSON_ArrayForEach(acr, acrs) {
		acor = cJSON_GetObjectItem(acr, "acor");
		int acor_size = cJSON_GetArraySize(acor);
		for (int j = 0; j < acor_size; j++) {
			strcat(acor_str, cJSON_GetArrayItem(acor, j)->valuestring);
			if (j < acor_size - 1) {
				strcat(acor_str, ",");
			}
		}
		if (i < acr_size - 1)
			strcat(acor_str, ",");

		acop = cJSON_GetObjectItem(acr, "acop");
		for (int j = 0; j < acor_size; j++) {
			strcat(acop_str, strtok(cJSON_Print(acop), "\""));
			if (j < acor_size - 1) {
				strcat(acop_str, ",");
			}
		}
		if (i < acr_size - 1)
			strcat(acop_str, ",");
		i++;
	}
	acp->pvs_acor = (char *)malloc(sizeof(char) * strlen(acor_str) + 1);
	strcpy(acp->pvs_acor, acor_str);

	acp->pvs_acop = (char *)malloc(sizeof(char) * strlen(acop_str) + 1);
	strcpy(acp->pvs_acop, acop_str);

end:
	cJSON_Delete(json);

	return acp;
}