#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"
#include "onem2m.h"

CNT* Json_to_CNT(char *json_payload) {
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

	// rn
	rn = cJSON_GetObjectItem(root, "rn");
	if (!cJSON_IsString(rn) && (rn->valuestring == NULL))
	{
		goto end;
	}
	cnt->rn = cJSON_Print(rn);
	cnt->rn = strtok(cnt->rn, "\"");

	// acpi
	acpi = cJSON_GetObjectItem(root, "acpi");
	if (!cJSON_IsString(rn) && (rn->valuestring == NULL))
	{
		goto end;
	}
	cnt->acpi = cJSON_Print(acpi);
	cnt->acpi = strtok(cnt->acpi, "\"");

end:
	cJSON_Delete(json);

	return cnt;
}