#ifndef APP_INPUT_APP_INPUT_H
#define APP_INPUT_APP_INPUT_H

#include "common/types.h"


void app_input_init(void);

void app_input_task(void);

bool app_input_get(uint8_t index /* 0 or 1 */);
#endif // APP_INPUT_APP_INPUT_H