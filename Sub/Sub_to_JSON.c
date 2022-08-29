#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

char* Sub_to_json(Sub *sub_object) {
	char *json = NULL;

	cJSON *root = NULL;
	cJSON *sub = NULL;
	cJSON *nu = NULL;
	cJSON *enc = NULL;
	cJSON *net = NULL;

	/* Our "sub" item: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "m2m:sub", sub = cJSON_CreateObject());
	cJSON_AddStringToObject(sub, "rn", sub_object->rn);
	cJSON_AddNumberToObject(sub, "ty", sub_object->ty);
	cJSON_AddStringToObject(sub, "pi", sub_object->pi);
	cJSON_AddStringToObject(sub, "ri", sub_object->ri);
	cJSON_AddStringToObject(sub, "ct", sub_object->ct);
	cJSON_AddStringToObject(sub, "lt", sub_object->lt);
	cJSON_AddStringToObject(sub, "et", sub_object->et);

	// nu
	nu = cJSON_CreateArray();
	char *nu_str = strtok(sub_object->nu, ",");
	do {
		cJSON_AddItemToArray(nu, cJSON_CreateString(nu_str));
		nu_str = strtok(NULL, ",");
	} while (nu_str != NULL);
	cJSON_AddItemToObject(sub, "nu", nu);

	// net
	cJSON_AddItemToObject(sub, "enc", enc = cJSON_CreateObject());

	net = cJSON_CreateArray();
	char *net_str = strtok(sub_object->net, ",");
	do {
		cJSON_AddItemToArray(net, cJSON_CreateNumber(atof(net_str)));
		net_str = strtok(NULL, ",");
	} while (net_str != NULL);
	cJSON_AddItemToObject(enc, "net", net);

	// nct
	cJSON_AddNumberToObject(sub, "nct", sub_object->nct);

	json = cJSON_Print(root);

	cJSON_Delete(root);

	return json;
}
