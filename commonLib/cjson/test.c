
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

/**
 *  基础类型
 *      cJSON_NULL
 *      cJSON_False
 *      cJSON_True
 *      cJSON_Number
 *      cJSON_String
 *  构造类型
 *      cJSON_Array
 *      cJSON_Object
 */

/**
 *  字符串解析为JSON对象
 *  打印JSON对象
 *  创建各种类型JSON对象
 *  添加元素
 *  删除元素
 *  替换元素
 *  遍历元素
 *  获取元素
 *  获取元素至基础类型
 */


/**
 * @brief 打印对象，需要释放返回的字符串
 * @param json 
 */
void printCJson(cJSON* json){
    char* str = cJSON_PrintUnformatted(json);
    if(str != NULL){
        printf("%s\n", str);
        free(str);
    }
}

/**
 * 从字符串解析JSON对象
 */
void parseCjsonFromString(){
    char* string = "{\"helo\" : \"world\"}";
    cJSON* parseObject = cJSON_Parse(string);
    printCJson(parseObject);
    cJSON_Delete(parseObject);
}


/**
 * @brief 创建基础类型cJSON对象
 * 
 */
void create_cJSON(){
    cJSON* json;

    json = cJSON_CreateNull();
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateFalse();
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateTrue();
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateBool(0);
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateNumber(100);
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateString("hello");
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateObject();
    printCJson(json);
    cJSON_Delete(json);

    json = cJSON_CreateArray();
    printCJson(json);
    cJSON_Delete(json);
}


/**
 * @brief 对象
 * 
 */
void objType(){
    //创建对象
    cJSON* json = cJSON_CreateObject();
    //添加基础元素
    cJSON_AddTrueToObject(json, "true_key");
    cJSON_AddFalseToObject(json,"false_key");
    cJSON_AddNumberToObject(json, "number_key", 100);
    cJSON_AddStringToObject(json, "str_key", "hello");

    //构建对象元素
    cJSON* newJson = cJSON_CreateObject();
    cJSON_AddStringToObject(newJson, "string", "world");
    cJSON_AddNumberToObject(newJson, "number", 100);

    //更通用的添加元素的方式
    cJSON_AddItemToObject(json, "object", cJSON_CreateString("objectstring"));
    cJSON_AddItemToObject(json, "objectJson", newJson);

    //获取元素
    printCJson(cJSON_GetObjectItem(json, "false_key"));
    printf("%s\n", cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(json, "objectJson"), "string")));
    printf("%d\n", cJSON_GetObjectItem(cJSON_GetObjectItem(json, "objectJson"), "number")->valueint);
    
    
    cJSON_Delete(json);
}


/**
 * @brief 数组
 * 
 */
void ojbArray(){
    //创建数组对象
    cJSON* json = cJSON_CreateArray();

    //末尾添加
    cJSON_AddItemToArray(json, cJSON_CreateString("hello"));
    cJSON_AddItemToArray(json, cJSON_CreateNumber(100));
    //指定位置添加
    cJSON_InsertItemInArray(json, 0, cJSON_CreateFalse());

    //按位置索引元素
    cJSON_GetArrayItem(json, 0);
    cJSON_GetArrayItem(json, 1);

    //按位置删除元素
    cJSON_DeleteItemFromArray(json, 0);

    //遍历元素
    cJSON* elem;
    cJSON_ArrayForEach(elem, json){
        printCJson(elem);
        cJSON_Delete(cJSON_DetachItemViaPointer(json, elem));
        break;
    }

    printCJson(json);

    cJSON_Delete(json);
}


//元素删除、替换、索引
void object_delete(){
    cJSON* childJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(childJson, "key", 200);

    cJSON* object = cJSON_CreateObject();
    cJSON_AddNumberToObject(object, "number", 100);
    cJSON_AddItemToObject(object, "childJson", childJson);
    printCJson(object);

    cJSON_DeleteItemFromObject(object, "number");
    cJSON_Delete(cJSON_DetachItemViaPointer(object, childJson));
    printCJson(object);

    cJSON_Delete(object);
}


void array_delete(){
    cJSON* childJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(childJson, "key", 200);

    cJSON* json = cJSON_CreateArray();
    cJSON_AddItemToArray(json, childJson);
    cJSON_InsertItemInArray(json, 0, cJSON_CreateNumber(200));
    printCJson(json);

    cJSON_DeleteItemFromArray(json, 0);
    cJSON_Delete(cJSON_DetachItemViaPointer(json, childJson));
    printCJson(json);

    cJSON_Delete(json);
}


void del_replace_reference3(){
    cJSON* childJson = cJSON_CreateObject();
    cJSON_AddStringToObject(childJson, "string", "hello");

    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "childJson", childJson);
    printCJson(json);

    //被替换的元素会被cjson_delete掉
    cJSON_ReplaceItemInObject(json, "childJson", cJSON_CreateNumber(200));
    printCJson(json);

    cJSON_Delete(json);
}


void reference(){
    cJSON* smallJson = cJSON_CreateObject();
    cJSON_AddStringToObject(smallJson, "string", "hello");

    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(json, "smallJson", smallJson);
    cJSON_AddStringToObject(smallJson, "string1", "hello1");
    printCJson(json);

    cJSON_Delete(smallJson);
    cJSON_Delete(json);
}


void test(){
    cJSON* array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(3));
    printCJson(array);

    int arraySize = cJSON_GetArraySize(array);
    cJSON* deleteElementArray[arraySize];
    int i = 0;

    cJSON* element = NULL;
    cJSON_ArrayForEach(element, array){
        int number = element->valueint;
        if(number == 1 || number == 2){
            deleteElementArray[i] = element;
            i++;
        }
    }
    deleteElementArray[i] = NULL;
    for(int index = 0; index < i; ++index){
        cJSON_Delete(cJSON_DetachItemViaPointer(array, deleteElementArray[index]));
    }

    printCJson(array);
    cJSON_Delete(array);
}


int main(int argc, char* argv[])
{
    parseCjsonFromString();
    return 0;
}


