/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015  Henrique Silva <henrique.silva@lnls.br>
 *   Copyright (C) 2015  Piotr Miedzik  <P.Miedzik@gsi.de>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

/* Kernel includes. */
#include "FreeRTOS.h"

/* Project includes */
#include "port.h"
#include "led.h"
#include "ipmi.h"
#include "payload.h"
#include "i2c.h"
#include "fru.h"
#include "scansta1101.h"
#include "fpga_spi.h"
#include "watchdog.h"
#include "uart_debug.h"
#ifdef MODULE_RTM
#include "rtm.h"
#endif

/*-----------------------------------------------------------*/
int main( void )
{
    pin_init();

#ifdef MODULE_UART_DEBUG
    uart_debug_init( 19200 );
#endif

    DEBUG_MSG("openMMC Starting!\n");

#ifdef MODULE_WATCHDOG
    watchdog_init();
#endif

    LED_init();
    i2c_init();

#ifdef MODULE_FRU
    fru_init(FRU_AMC);
#endif

#ifdef MODULE_SDR
    sdr_init();
#endif
#ifdef MODULE_SENSORS
    sensor_init();
#endif
#ifdef MODULE_PAYLOAD
    payload_init();
#endif
#ifdef MODULE_SCANSTA1101
    scansta1101_init();
#endif
#ifdef MODULE_FPGA_SPI
    fpga_spi_init();
#endif
#ifdef MODULE_RTM
    rtm_manage_init();
#endif
    /*  Init IPMI interface */
    /* NOTE: ipmb_init() is called inside this function */
    ipmi_init();

    /* Start the tasks running. */
    vTaskStartScheduler();

    /* If all is well we will never reach here as the scheduler will now be
       running.  If we do reach here then it is likely that there was insufficient
       heap available for the idle task to be created. */
    for( ;; );

}

/*-----------------------------------------------------------*/
/* Put the MCU in sleep state when no task is running */
void vApplicationIdleHook (void) {
    pm_sleep();
}