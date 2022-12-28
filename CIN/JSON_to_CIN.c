#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cJSON.h"
#include "onem2m.h"

CIN* JSON_to_CIN(char *json_payload) {
	CIN *cin = (CIN *)malloc(sizeof(CIN));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *con = NULL;

	const char *error_ptr = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:cin");

	// con (mandatory)
	con = cJSON_GetObjectItem(root, "con");
	if (con == NULL || con->valuestring[0] == 0 || isspace(con->valuestring[0]))
	{
		fprintf(stderr, "Invalid con\n");
		goto end;
	}
	cin->con = cJSON_Print(con);
	cin->con = strtok(cin->con, "\"");


	// rn (optional)
	rn = cJSON_GetObjectItem(root, "rn");
	if (rn == NULL || isspace(rn->valuestring[0])) {
		cin->rn = NULL;
	}
	else {
		cin->rn = cJSON_Print(rn);
		cin->rn = strtok(cin->rn, "\"");
	}

end:
	cJSON_Delete(json);

	return cin;
}