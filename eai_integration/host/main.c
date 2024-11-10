#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tee_client_api.h>
#include <ke_hello_world_ta.h>
#include <stdint.h>

void encrypt_data(const char *input, char *encrypted_data, size_t *encrypted_size);

int main(void) {
    TEEC_Result res;
    TEEC_Context ctx;
    TEEC_Session sess;
    TEEC_Operation op;
    TEEC_UUID uuid = TA_KE_HELLO_WORLD_UUID;
    uint32_t err_origin;

    // Initialize TEE context
    res = TEEC_InitializeContext(NULL, &ctx);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

    // Open a session with the TA
    res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_OpenSession failed with code 0x%x origin 0x%x", res, err_origin);

    while (1) {
        // Fetch data from the URL
        char response[256] = {0};
        FILE *fp = popen("wget -qO- http://34.68.51.137:3000", "r");
        if (!fp) {
            errx(1, "Failed to execute wget");
        }
        if (!fgets(response, sizeof(response), fp)) {
            pclose(fp);
            errx(1, "Failed to read from wget response");
        }
        pclose(fp);

        // Encrypt data in the normal world
        char encrypted_data[512] = {0};
        size_t encrypted_size = sizeof(encrypted_data);
        encrypt_data(response, encrypted_data, &encrypted_size);

        // Prepare the TEE operation to send encrypted data and receive a message if needed
        char ta_response[128] = {0};
        size_t ta_response_size = sizeof(ta_response);

        memset(&op, 0, sizeof(op));
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE, TEEC_NONE);
        op.params[0].tmpref.buffer = encrypted_data;
        op.params[0].tmpref.size = encrypted_size;
        op.params[1].tmpref.buffer = ta_response;
        op.params[1].tmpref.size = ta_response_size;

        // Invoke the TA command for decryption and validation
        res = TEEC_InvokeCommand(&sess, TA_KE_HELLO_WORLD_CMD_DECRYPT, &op, &err_origin);
        if (res != TEEC_SUCCESS)
            errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);

        // Print response if the TA sends a message
        if (op.params[1].tmpref.size > 0) {
            printf("%s\n", ta_response);
        }

        // Sleep for 1 second before the next request
        sleep(1);
    }

    // Clean up
    TEEC_CloseSession(&sess);
    TEEC_FinalizeContext(&ctx);

    return 0;
}

void encrypt_data(const char *input, char *encrypted_data, size_t *encrypted_size) {
    snprintf(encrypted_data, *encrypted_size, "Encrypted: %s", input);
    *encrypted_size = strlen(encrypted_data) + 1;
}
