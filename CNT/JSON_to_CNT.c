#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cJSON.h"
#include "onem2m.h"

CNT* JSON_to_CNT(char *json_payload) {
	CNT *cnt = (CNT *)malloc(sizeof(CNT));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *acpi = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:cnt");

	// rn (optional)
	rn = cJSON_GetObjectItem(root, "rn");
	if (rn == NULL || isspace(rn->valuestring[0])) {
		cnt->rn = NULL;
	}
	else {
		cnt->rn = cJSON_Print(rn);
		cnt->rn = strtok(cnt->rn, "\"");
	}


	// acpi (optional)
	acpi = cJSON_GetObjectItem(root, "acpi");
	if (acpi == NULL) {
		cnt->acpi = NULL;
	}
	else {
		int acpi_size = cJSON_GetArraySize(acpi);
		int is_NULL_flag = 0;
		if (acpi_size == 0) {
			cnt->acpi = NULL;
		}
		else {
			char acpi_str[100] = { '\0' };
			for (int i = 0; i < acpi_size; i++) {
				if (isspace(cJSON_GetArrayItem(acpi, i)->valuestring[0]) || (cJSON_GetArrayItem(acpi, i)->valuestring[0] == 0)) {
					cnt->acpi = NULL;
					is_NULL_flag = 1;
					break;
				}
				else {
					strcat(acpi_str, cJSON_GetArrayItem(acpi, i)->valuestring);
					if (i < acpi_size - 1) {
						strcat(acpi_str, ",");
					}
				}
			}
			if (is_NULL_flag != 1) {
				cnt->acpi = (char *)malloc(sizeof(char *) * strlen(acpi_str) + 1);
				strcpy(cnt->acpi, acpi_str);
			}
		}
	}


end:
	cJSON_Delete(json);

	return cnt;
}