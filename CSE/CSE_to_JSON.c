#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

char* CSE_to_json(CSE* cse_object) {
	char *json = NULL;

	cJSON *root = NULL;
	cJSON *cse = NULL;

	/* Our "cse" item: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "m2m:cb", cse = cJSON_CreateObject());
	cJSON_AddStringToObject(cse, "pi", cse_object->pi);
	cJSON_AddStringToObject(cse, "ri", cse_object->ri);
	cJSON_AddNumberToObject(cse, "ty", cse_object->ty);
	cJSON_AddStringToObject(cse, "ct", cse_object->ct);
	cJSON_AddStringToObject(cse, "rn", cse_object->rn);
	cJSON_AddStringToObject(cse, "lt", cse_object->lt);
	cJSON_AddStringToObject(cse, "csi", cse_object->csi);

	json = cJSON_Print(root);

	cJSON_Delete(root);

	return json;
}