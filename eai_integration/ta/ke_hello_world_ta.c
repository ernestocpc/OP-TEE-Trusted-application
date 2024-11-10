#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <ke_hello_world_ta.h>

#define MAX_DECRYPTED_SIZE 256

TEE_Result TA_CreateEntryPoint(void) {
    DMSG("TA CreateEntryPoint has been called");
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void) {
    DMSG("TA DestroyEntryPoint has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4], void **sess_ctx) {
    (void)param_types;
    (void)params;
    (void)sess_ctx;

    DMSG("TA OpenSessionEntryPoint has been called");
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx) {
    (void)&sess_ctx;
    IMSG("Goodbye!\n");
}

/* Helper function to decrypt data, check price, and prepare response if needed */
static TEE_Result decrypt_and_check_price(const char *encrypted_data, char *response, size_t *response_size) {
    const char *prefix = "Encrypted: ";
    char decrypted_data[MAX_DECRYPTED_SIZE] = {0};

    // Mock decryption by removing prefix
    if (strncmp(encrypted_data, prefix, strlen(prefix)) != 0) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    snprintf(decrypted_data, sizeof(decrypted_data), "%s", encrypted_data + strlen(prefix));

    // Extract "price" and "id" fields from decrypted_data
    const char *price_key = "\"price\":";
    const char *id_key = "\"id\":";
    char *price_start = strstr(decrypted_data, price_key);
    char *id_start = strstr(decrypted_data, id_key);

    if (!price_start || !id_start) {
        return TEE_ERROR_BAD_FORMAT;
    }

    // Parse "price" value
    price_start += strlen(price_key);
    int price = 0;
    while (*price_start >= '0' && *price_start <= '9') {
        price = price * 10 + (*price_start - '0');
        price_start++;
    }

    // Parse "id" value
    id_start += strlen(id_key);
    int id = 0;
    while (*id_start >= '0' && *id_start <= '9') {
        id = id * 10 + (*id_start - '0');
        id_start++;
    }

    // Only respond if price > 300
    if (price > 300) {
        snprintf(response, *response_size, "Purchase with id %d validated, initiating integration process", id);
        *response_size = strlen(response) + 1;
    } else {
        *response_size = 0;  // No response if condition is not met
    }

    return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
                                      uint32_t param_types, TEE_Param params[4]) {
    (void)sess_ctx;

    if (cmd_id != TA_KE_HELLO_WORLD_CMD_DECRYPT) {
        return TEE_ERROR_NOT_SUPPORTED;
    }

    if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE)) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    // Get encrypted data from the normal world
    char *encrypted_data = params[0].memref.buffer;

    // Prepare buffer for response message
    char *response = params[1].memref.buffer;
    size_t response_size = params[1].memref.size;

    // Decrypt, check price, and prepare response if necessary
    TEE_Result res = decrypt_and_check_price(encrypted_data, response, &response_size);
    if (res != TEE_SUCCESS) {
        return res;
    }

    // Update response size in output parameter
    params[1].memref.size = response_size;
    return TEE_SUCCESS;
}
