#include <control/control_tasks.h>

QueueHandle_t ctrlCmdQueue;

canwaifu_status control_init(void) {
    pinMode(PIN_FAN_CON, OUTPUT); // 设置风扇控制引脚为输出
    pinMode(PIN_LIGHT_CON, OUTPUT); // 设置LED控制引脚为输出
    ctrlCmdQueue = xQueueCreate(10, sizeof(ControlCommand*));
    Serial.println("[CTRL] Control command queue initialized.");
    return CANWAIFU_OK;
}

void control_task(void *pvParameters) {
    ControlCommand* ctrlCmd;

    while (true) {
        if (xQueueReceive(ctrlCmdQueue, &ctrlCmd, portMAX_DELAY) == pdPASS) {
            Serial.print("[CTRL] ");
            ctrlCmd->print();
            switch (ctrlCmd->_mode)
            {
            case CTRL_MODE_ABS:
                analogWrite(PIN_FAN_CON, ctrlCmd->_fan); // 控制风扇
                analogWrite(PIN_LIGHT_CON, ctrlCmd->_led); // 控制LED红色
                break;
            
            case CTRL_MODE_PID:
                /* code */
                break;
            
            default:
                break;
            }
            delete ctrlCmd;  // 用完记得释放内存
        }
        
    }
}
