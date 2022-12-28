#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

char* CNT_to_json(CNT* cnt_object) {
	char *json = NULL;

	cJSON *root = NULL;
	cJSON *cnt = NULL;

	/* Our "cnt" item: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "m2m:cnt", cnt = cJSON_CreateObject());
	cJSON_AddStringToObject(cnt, "rn", cnt_object->rn);
	cJSON_AddNumberToObject(cnt, "ty", cnt_object->ty);
	cJSON_AddStringToObject(cnt, "pi", cnt_object->pi);
	cJSON_AddStringToObject(cnt, "ri", cnt_object->ri);
	cJSON_AddStringToObject(cnt, "ct", cnt_object->ct);
	cJSON_AddStringToObject(cnt, "lt", cnt_object->lt);
	cJSON_AddNumberToObject(cnt, "st", cnt_object->st);
	cJSON_AddStringToObject(cnt, "et", cnt_object->et);
	cJSON_AddNumberToObject(cnt, "cni", cnt_object->cni);
	cJSON_AddNumberToObject(cnt, "cbs", cnt_object->cbs);
	cJSON_AddStringToObject(cnt, "lbl", cnt_object->lbl);

	json = cJSON_Print(root);

	cJSON_Delete(root);

	return json;
}