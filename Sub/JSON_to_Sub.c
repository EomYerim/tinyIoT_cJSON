#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

	// nu (mandatory)
	nu = cJSON_GetObjectItem(root, "nu");
	int nu_size = cJSON_GetArraySize(nu);
	if (nu == NULL || nu_size == 0) {
		fprintf(stderr, "Invalid nu\n");
		goto end;
	}
	else {
		char nu_str[100] = { '\0' };
		int is_NULL = 0;
		for (int i = 0; i < nu_size; i++) {
			if (isspace(cJSON_GetArrayItem(nu, i)->valuestring[0]) || (cJSON_GetArrayItem(nu, i)->valuestring[0] == 0)) {
				fprintf(stderr, "Invalid nu\n");
				is_NULL = 1;
				goto end;
			}
			strcat(nu_str, cJSON_GetArrayItem(nu, i)->valuestring);
			if (i < nu_size - 1) {
				strcat(nu_str, ",");
			}
		}
		sub->nu = (char *)malloc(sizeof(char) * strlen(nu_str) + 1);
		strcpy(sub->nu, nu_str);
	}


	// rn (optional)
	rn = cJSON_GetObjectItem(root, "rn");
	if (rn == NULL || isspace(rn->valuestring[0])) {
		sub->rn = NULL;
	}
	else {
		sub->rn = cJSON_Print(rn);
		sub->rn = strtok(sub->rn, "\"");
	}

	// enc (optional)
	enc = cJSON_GetObjectItem(root, "enc");
	if (enc == NULL) {
		sub->net = NULL;
	}
	else {
		// net (optional)
		net = cJSON_GetObjectItem(enc, "net");
		int net_size = cJSON_GetArraySize(net);
		if (net == NULL || net_size == 0) {
			sub->net = NULL;
		}
		else {
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
		}
	}


end:
	cJSON_Delete(json);

	return sub;
}