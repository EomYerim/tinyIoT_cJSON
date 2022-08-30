#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "onem2m.h"

Sub* JSON_to_Sub(char *json_payload) {
	Sub *sub = (Sub *)malloc(sizeof(Sub));

	cJSON *root = NULL;
	cJSON *rn = NULL;
	cJSON *enc = NULL;
	cJSON *net = NULL;
	cJSON *nu = NULL;

	cJSON* json = cJSON_Parse(json_payload);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		goto end;
	}

	root = cJSON_GetObjectItem(json, "m2m:sub");

	// rn
	rn = cJSON_GetObjectItem(root, "rn");
	if (!cJSON_IsString(rn) && rn->valuestring == NULL)
	{
		goto end;
	}
	sub->rn = cJSON_Print(rn);
	sub->rn = strtok(sub->rn, "\"");

	// enc
	enc = cJSON_GetObjectItem(root, "enc");

	// net
	net = cJSON_GetObjectItem(enc, "net");
	int net_size = cJSON_GetArraySize(net);
	char net_str[10] = { '\0' };
	char tmp[10] = { '\0' };
	for (int i = 0; i < net_size; i++) {
		sprintf(tmp, "%d", cJSON_GetArrayItem(net, i)->valueint);
		strcat(net_str, tmp);
		if (i < net_size - 1) {
			strcat(net_str, ",");
		}
	}
	sub->net = (char *)malloc(sizeof(char) * strlen(net_str) + 1);
	strcpy(sub->net, net_str);

	// nu
	nu = cJSON_GetObjectItem(root, "nu");
	int nu_size = cJSON_GetArraySize(nu);
	char nu_str[100] = { '\0' };
	for (int i = 0; i < nu_size; i++) {
		strcat(nu_str, cJSON_GetArrayItem(nu, i)->valuestring);
		if (i < nu_size - 1) {
			strcat(nu_str, ",");
		}
	}
	sub->nu = (char *)malloc(sizeof(char) * strlen(nu_str) + 1);
	strcpy(sub->nu, nu_str);


end:
	cJSON_Delete(json);

	return sub;
}
