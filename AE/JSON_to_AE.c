#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cJSON.h"
#include "onem2m.h"

AE* JSON_to_AE(char *json_payload) {
	AE *ae = (AE *)malloc(sizeof(AE));

	cJSON *root = NULL;
	cJSON *api = NULL;
	cJSON *rr = NULL;
	cJSON *rn = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:ae");

	// api (mandatory)
	api = cJSON_GetObjectItem(root, "api");
	rr = cJSON_GetObjectItemCaseSensitive(root, "rr");	// + rr
	if (api == NULL || api->valuestring[0] == 0 || isspace(api->valuestring[0])) {
		fprintf(stderr, "Invalid api\n");
		if (!cJSON_IsTrue(rr) && !cJSON_IsFalse(rr)) {
			fprintf(stderr, "Invalid rr\n");
		}
		goto end;
	}
	else {
		ae->api = cJSON_Print(api);
		ae->api = strtok(ae->api, "\"");
	}


	// rr (mandatory)
	//rr = cJSON_GetObjectItemCaseSensitive(root, "rr");
	if (!cJSON_IsTrue(rr) && !cJSON_IsFalse(rr))
	{
		fprintf(stderr, "Invalid rr\n");
		goto end;
	}
	else if (cJSON_IsTrue(rr))
	{
		ae->rr = true;
	}
	else if (cJSON_IsFalse(rr))
	{
		ae->rr = false;
	}


	// rn (optional)
	rn = cJSON_GetObjectItem(root, "rn");
	if (rn == NULL || isspace(rn->valuestring[0])) {
		ae->rn = NULL;
	}
	else {
		ae->rn = cJSON_Print(rn);
		ae->rn = strtok(ae->rn, "\"");
	}


end:
	cJSON_Delete(json);

	return ae;
}