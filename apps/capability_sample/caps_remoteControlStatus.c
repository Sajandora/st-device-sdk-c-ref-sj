/* ***************************************************************************
 *
 * Copyright 2019-2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "st_dev.h"
#include "caps_remoteControlStatus.h"

static int caps_remoteControlStatus_attr_remoteControlEnabled_str2idx(const char *value)
{
    int index;

    for (index = 0; index < CAP_ENUM_REMOTECONTROLSTATUS_REMOTECONTROLENABLED_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_remoteControlStatus.attr_remoteControlEnabled.values[index])) {
            return index;
        }
    }
    return -1;
}

static const char *caps_remoteControlStatus_get_remoteControlEnabled_value(caps_remoteControlStatus_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->remoteControlEnabled_value;
}

static void caps_remoteControlStatus_set_remoteControlEnabled_value(caps_remoteControlStatus_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->remoteControlEnabled_value) {
        free(caps_data->remoteControlEnabled_value);
    }
    caps_data->remoteControlEnabled_value = strdup(value);
}

static void caps_remoteControlStatus_attr_remoteControlEnabled_send(caps_remoteControlStatus_data_t *caps_data)
{
    int sequence_no = -1;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->remoteControlEnabled_value) {
        printf("value is NULL\n");
        return;
    }

    ST_CAP_SEND_ATTR_STRING(caps_data->handle,
            (char *)caps_helper_remoteControlStatus.attr_remoteControlEnabled.name,
            caps_data->remoteControlEnabled_value,
            NULL,
            NULL,
            sequence_no);

    if (sequence_no < 0)
        printf("fail to send remoteControlEnabled value\n");
    else
        printf("Sequence number return : %d\n", sequence_no);

}

static void caps_remoteControlStatus_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_remoteControlStatus_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_remoteControlStatus_attr_remoteControlEnabled_send(caps_data);
}

caps_remoteControlStatus_data_t *caps_remoteControlStatus_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_remoteControlStatus_data_t *caps_data = NULL;

    caps_data = malloc(sizeof(caps_remoteControlStatus_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_remoteControlStatus_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_remoteControlStatus_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_remoteControlEnabled_value = caps_remoteControlStatus_get_remoteControlEnabled_value;
    caps_data->set_remoteControlEnabled_value = caps_remoteControlStatus_set_remoteControlEnabled_value;
    caps_data->attr_remoteControlEnabled_str2idx = caps_remoteControlStatus_attr_remoteControlEnabled_str2idx;
    caps_data->attr_remoteControlEnabled_send = caps_remoteControlStatus_attr_remoteControlEnabled_send;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_remoteControlStatus.id, caps_remoteControlStatus_init_cb, caps_data);
    }

    return caps_data;
}
