#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"
#include "onem2m.h"

CIN* Json_to_CIN(char *json_payload) {
	CIN *cin = (CIN *)malloc(sizeof(CIN));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *con = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:cin");

	// rn
	rn = cJSON_GetObjectItem(root, "rn");
	if (!cJSON_IsString(rn) && (rn->valuestring == NULL))
	{
		goto end;
	}
	cin->rn = cJSON_Print(rn);
	cin->rn = strtok(cin->rn, "\"");

	// con
	con = cJSON_GetObjectItem(root, "con");
	if (!cJSON_IsString(con) && (con->valuestring == NULL))
	{
		goto end;
	}
	cin->con = cJSON_Print(con);
	cin->con = strtok(cin->con, "\"");

	// cs
	cin->cs = strlen(cin->con);

end:
	cJSON_Delete(json);

	return cin;
}
