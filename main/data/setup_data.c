//
// Created by Sam Weffling on 02/01/2026.
//

#include "setup_data.h"


#include "src/misc/lv_log.h"
#include "stdio.h"
#include "cJSON.h"
#include "src/misc/lv_fs.h"
#include "string.h"

t_model_setup model_setup = {0};


t_channel_setup * get_channel_setup(const int channel) {

     if (channel < 0 || channel >= CHANNEL_COUNT) {
        LV_LOG_ERROR("INVALID CHANNEL !");
        return &model_setup.channel_setups[0];
     }

    return &model_setup.channel_setups[channel];
}


void init_model_setup() {
    snprintf(model_setup.model_name, sizeof(model_setup.model_name) , "Unnamed Model" );

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        t_channel_setup * channel_setup = get_channel_setup(i);


        switch (i) {
            case 0:
                channel_setup->source = SOURCE_ANALOG_A;
                break;
            case 1:
                channel_setup->source = SOURCE_ANALOG_B;
                break;
            case 2:
                channel_setup->source = SOURCE_ANALOG_C;
                break;
            case 3:
                channel_setup->source = SOURCE_ANALOG_D;
                break;
            default:
                channel_setup->source = SOURCE_NONE;
                break;
        }

        channel_setup->reversed = false;

        channel_setup->min_value = 100;
        channel_setup->max_value = 100;
        channel_setup->zero_value = 0;
        channel_setup->trim_value = 0;
        channel_setup->exp_value = 0;


    }
}

void list_existing_files() {

    lv_fs_res_t result;
    lv_fs_dir_t dir;

    lv_fs_dir_open(&dir , "S:/");
    char name[64];

    do {
        lv_fs_dir_read(&dir ,name , sizeof(name));
        LV_LOG_USER("FILE in dir : %s" , name);

    }while(strlen(name) > 0);

    lv_fs_dir_close(&dir);


}

void read_model_setup() {
    lv_fs_res_t result;
    lv_fs_file_t file;
    /*
    

    result = lv_fs_open(&file, "S:setup.json", LV_FS_MODE_RD);
    if (result != LV_FS_RES_OK) {
        LV_LOG_ERROR("Error opening file");
    }
    uint32_t file_size;

    lv_fs_get_size(&file , &file_size);
    LV_LOG_USER("File Size : %ld" , file_size);
    char buffer[file_size];

    lv_fs_read(&file , buffer , file_size , NULL);

    cJSON* setup_json = cJSON_Parse(buffer);

    cJSON* channel_setup = cJSON_GetObjectItem(setup_json, "channel_setup");
    cJSON* element;
    int index = 0;

    cJSON_ArrayForEach(element , channel_setup) {
        if (index < CHANNEL_COUNT) {

            cJSON* reversed = cJSON_GetObjectItem(element, "reversed");
            cJSON* min_value = cJSON_GetObjectItem(element , "min_value");
            cJSON* max_value = cJSON_GetObjectItem(element , "max_value" );
            cJSON* zero_value = cJSON_GetObjectItem(element , "zero_value");
            cJSON* trim_value = cJSON_GetObjectItem(element , "trim_value");
            cJSON* exp_value = cJSON_GetObjectItem(element , "exp_value");

            t_channel_setup * setup = get_channel_setup(index);

            if (cJSON_IsBool(reversed)) {
                setup->reversed = cJSON_IsTrue(reversed) ? true : false;
            }

            if (cJSON_IsNumber(min_value)) {
                setup->min_value = min_value->valueint;
            }

            if (cJSON_IsNumber(max_value)) {
                setup->max_value = max_value->valueint;
            }
            if (cJSON_IsNumber(zero_value)) {
                setup->zero_value = zero_value->valueint;
            }
            if (cJSON_IsNumber(trim_value)) {
                setup->trim_value = trim_value->valueint;
            }
            if (cJSON_IsNumber(exp_value)) {
                setup->exp_value = exp_value->valueint;
            }

        }
        index++;
    }


    cJSON_Delete(setup_json);
    lv_fs_close(&file);
    */
}

void write_model_setup() {


    lv_fs_res_t result;
    lv_fs_file_t file;
    /*
    list_existing_files();
    cJSON* setup_json = cJSON_CreateObject();
    if (setup_json == NULL) {
        LV_LOG_ERROR("cJSON_CreateObject failed");
        return;
    }

    cJSON* channel_setup_json = cJSON_CreateArray();

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        t_channel_setup * channel_setup = get_channel_setup(i);

        cJSON* channel_json = cJSON_CreateObject();

        cJSON* reversed = cJSON_CreateBool(channel_setup->reversed);
        cJSON* min_value = cJSON_CreateNumber(channel_setup->min_value);
        cJSON* max_value = cJSON_CreateNumber(channel_setup->max_value);
        cJSON* zero_value = cJSON_CreateNumber(channel_setup->zero_value);
        cJSON* trim_value = cJSON_CreateNumber(channel_setup->trim_value);
        cJSON* exp_value = cJSON_CreateNumber(channel_setup->exp_value);

        cJSON_AddItemToObject(channel_json , "reversed", reversed);
        cJSON_AddItemToObject(channel_json, "min_value", min_value);
        cJSON_AddItemToObject(channel_json, "max_value", max_value);
        cJSON_AddItemToObject(channel_json, "zero_value", zero_value);
        cJSON_AddItemToObject(channel_json, "trim_value", trim_value);
        cJSON_AddItemToObject(channel_json, "exp_value", exp_value);

        cJSON_AddItemToArray(channel_setup_json, channel_json);
    }

    cJSON_AddItemToObject(setup_json, "channel_setup", channel_setup_json);


    result = lv_fs_open(&file, "S:setup.json", LV_FS_MODE_RD | LV_FS_MODE_WR);
    if (result != LV_FS_RES_OK) {
        LV_LOG_ERROR("Error opening file");
    }
        char* string = cJSON_Print(setup_json);

        lv_fs_write(&file , string , strlen(string) , NULL);

    lv_fs_close(&file);

    cJSON_Delete(setup_json);
    */
}