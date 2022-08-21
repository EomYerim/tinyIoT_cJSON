#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"
#include "onem2m.h"

CSE* Json_to_CSE(char *json_payload) {
	CSE *cse = (CSE *)malloc(sizeof(CSE));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *csi = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:cse");

	// rn
	rn = cJSON_GetObjectItem(root, "rn");
	if (!cJSON_IsString(rn) && (rn->valuestring == NULL))
	{
		goto end;
	}
	cse->rn = cJSON_Print(rn);
	cse->rn = strtok(cse->rn, "\"");

	// csi
	csi = cJSON_GetObjectItem(root, "csi");
	if (!cJSON_IsString(csi) && (csi->valuestring == NULL))
	{
		goto end;
	}
	cse->csi = cJSON_Print(csi);
	cse->csi = strtok(cse->csi, "\"");

end:
	cJSON_Delete(json);

	return cse;
}