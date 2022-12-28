#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

char *Get_JSON_Value_char(char *key, char *json) {
	char json_copy[100];
	char *resource = NULL;
	char *value = NULL;


	cJSON *root = NULL;
	cJSON *ckey = NULL;

	cJSON *cjson = cJSON_Parse(json);
	if (cjson == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	//Extracting resources from json
	strcpy(json_copy, json);
	resource = strstr(json_copy, "m2m:");
	resource = strtok(resource, "\"");

	root = cJSON_GetObjectItem(cjson, resource);

	ckey = cJSON_GetObjectItem(root, key);
	if (ckey != NULL) {
		value = cJSON_Print(ckey);
		value = strtok(value, "\"");
	}

end:
	cJSON_Delete(cjson);

	return value;
}