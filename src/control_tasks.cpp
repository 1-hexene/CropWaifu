#include <control_tasks.h>

QueueHandle_t ctrlCmdQueue;

canwaifu_status control_init(void) {
    ctrlCmdQueue = xQueueCreate(10, sizeof(ControlCommand*));
    return CANWAIFU_OK;
}

void control_task(void *pvParameters) {
    ControlCommand* ctrlCmd;
    while (true) {
        if (xQueueReceive(ctrlCmdQueue, &ctrlCmd, portMAX_DELAY) == pdPASS) {
            Serial.print("[CTRL] ");
            ctrlCmd->print(); 
            delete ctrlCmd;  // 用完记得释放内存
        }
    }
}
