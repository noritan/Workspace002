/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "project.h"

struct Bme280Device {
    uint32 (*writeBuf)(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode);
    uint32 (*readBuf)(uint32 slaveAddress, uint8 * rdData, uint32 cnt, uint32 mode);
    uint32 (*masterStatus)(void);
    uint32 (*clearStatus)(void);
    uint32 (*getWriteBufSize)(void);
    uint32 (*getReadBufSize)(void);    
} bme280device = {
    .writeBuf = I2CM_I2CMasterWriteBuf,
    .readBuf = I2CM_I2CMasterReadBuf,
    .masterStatus = I2CM_I2CMasterStatus,
    .clearStatus = I2CM_I2CMasterClearStatus,
    .getWriteBufSize = I2CM_I2CMasterGetWriteBufSize,
    .getReadBufSize = I2CM_I2CMasterGetReadBufSize,
};

struct Bme280Context {
    struct  Bme280Device *dev;
    uint8   address;
    uint16  t1;
    int16   t2;
    int16   t3;
    uint16  p1;
    int16   p2;
    int16   p3;
    int16   p4;
    int16   p5;
    int16   p6;
    int16   p7;
    int16   p8;
    int16   p9;
};

struct Bme280Uncomp {
    uint32  p;
    uint32  t;
    uint32  h;
    int32   t_fine;
};

#define BME280_ADDRESS      (0x76)  // I2C device address
#define BME280_DIG_T        (0x88)  // address of T Parameters
#define BME280_DIG_T_LEN    (6)     // Length of T parameters field
#define BME280_DIG_P        (0x8e)  // address of T Parameters
#define BME280_DIG_P_LEN    (18)     // Length of T parameters field
#define BME280_CTRL_HUM     (0xf2)  // ctrl_hum register address
#define BME280_CTRL_MEAS    (0xf4)  // ctrl_meas register address
#define BME280_PTH          (0xf7)  // Press, Temp, Hum register address
#define BME280_PTH_LEN      (8)     // Length of PTH measurement

struct Bme280Context context;
void bme280_init(struct Bme280Context *context) {
    context->dev = &bme280device;
    context->address = BME280_ADDRESS;
}

void bme280_writeRegister(struct Bme280Context *context, uint8 *wbuf, uint32 length) {
    uint32 status;
    
    // Write register address and content
    context->dev->clearStatus();
    status = context->dev->writeBuf(context->address, wbuf, length, I2CM_I2C_MODE_COMPLETE_XFER);
    CYASSERT(status == I2CM_I2C_MSTR_NO_ERROR);
    for (;;) {
        status = context->dev->masterStatus();
        if (status & I2CM_I2C_MSTAT_WR_CMPLT) break;
    }
    CYASSERT(!(status & I2CM_I2C_MSTAT_ERR_XFER));
    CYASSERT(context->dev->getWriteBufSize() == length);
}

void bme280_writeAddress(struct Bme280Context *context, uint8 address) {
    uint8 wbuf[1];
    
    // Write register address
    wbuf[0] = address;
    bme280_writeRegister(context, wbuf, sizeof wbuf);
}

void bme280_readRegister(struct Bme280Context *context, uint32 length, uint8 *rbuf) {
    uint32 status;

    // Read registers at the specified address
    context->dev->clearStatus();
    status = context->dev->readBuf(context->address, rbuf, length, I2CM_I2C_MODE_COMPLETE_XFER);
    CYASSERT(status == I2CM_I2C_MSTR_NO_ERROR);
    for (;;) {
        status = context->dev->masterStatus();
        if (status & I2CM_I2C_MSTAT_RD_CMPLT) break;
    }
    CYASSERT(!(status & I2CM_I2C_MSTAT_ERR_XFER));
    CYASSERT(context->dev->getReadBufSize() == length);    
}

void bme280_getParameters(struct Bme280Context *context) {
    uint8 rbuf[32];

    // Read T parameters
    bme280_writeAddress(context, BME280_DIG_T);
    bme280_readRegister(context, BME280_DIG_T_LEN, rbuf);
    context->t1 = (rbuf[1] << 8) | rbuf[0];
    context->t2 = (int16)((rbuf[3] << 8) | rbuf[2]);
    context->t3 = (int16)((rbuf[5] << 8) | rbuf[4]);
    // Read P parameters
    bme280_writeAddress(context, BME280_DIG_P);
    bme280_readRegister(context, BME280_DIG_P_LEN, rbuf);
    context->p1 = (rbuf[1] << 8) | rbuf[0];
    context->p2 = (int16)((rbuf[3] << 8) | rbuf[2]);
    context->p3 = (int16)((rbuf[5] << 8) | rbuf[4]);
    context->p4 = (int16)((rbuf[7] << 8) | rbuf[6]);
    context->p5 = (int16)((rbuf[9] << 8) | rbuf[8]);
    context->p6 = (int16)((rbuf[11] << 8) | rbuf[10]);
    context->p7 = (int16)((rbuf[13] << 8) | rbuf[12]);
    context->p8 = (int16)((rbuf[15] << 8) | rbuf[14]);
    context->p9 = (int16)((rbuf[17] << 8) | rbuf[16]);
}

void bme280_normalMode(struct Bme280Context *context) {
    uint8 wbuf[2];
    
    // Set ctrl_hum
    wbuf[0] = BME280_CTRL_HUM;
    wbuf[1] = 0x01;
    bme280_writeRegister(context, wbuf, 2);
    // Set ctrl_meas
    wbuf[0] = BME280_CTRL_MEAS;
    wbuf[1] = 0x27;
    bme280_writeRegister(context, wbuf, 2);
}

void bme280_storePTH(struct Bme280Uncomp *uncomp, uint8 *rbuf) {
    uncomp->p = ((uint32)rbuf[0]<<12)|((uint32)rbuf[1]<<4)|((uint32)rbuf[2]>>4);
    uncomp->t = ((uint32)rbuf[3]<<12)|((uint32)rbuf[4]<<4)|((uint32)rbuf[5]>>4);
    uncomp->h = ((uint32)rbuf[6]<<8)|((uint32)rbuf[7]);
}

int32 bme280_compensate_T(struct Bme280Context *context, struct Bme280Uncomp *uncomp) {
    int32 var1;
    int32 var2;
    int32 temperature;
    int32 temperature_min = -4000;
    int32 temperature_max = 8500;

    var1 = (int32)((uncomp->t >> 3) - ((int32)context->t1 << 1));
    var1 = (var1 * ((int32)context->t2)) >> 11;
    var2 = (int32)((uncomp->t >> 4) - ((int32)context->t1));
    var2 = (((var2 * var2) >> 12) * ((int32)context->t3)) >> 14;
    uncomp->t_fine = var1 + var2;
    temperature = (uncomp->t_fine * 5 + 128) >> 8;
    if (temperature < temperature_min)
    {
        temperature = temperature_min;
    }
    else if (temperature > temperature_max)
    {
        temperature = temperature_max;
    }

    return temperature;
}

uint32 bme280_compensate_P(struct Bme280Context *context, struct Bme280Uncomp *uncomp) {
    int64 var1;
    int64 var2;
    int64 var3;
    int64 var4;
    uint32 pressure;
    uint32 pressure_min = 3000000;
    uint32 pressure_max = 11000000;

    var1 = ((int64)uncomp->t_fine) - 128000;
    var2 = var1 * var1 * (int64)context->p6;
    var2 = var2 + ((var1 * (int64)context->p5) << 17);
    var2 = var2 + (((int64)context->p4) << 35);
    var1 = ((var1 * var1 * (int64)context->p3) >> 8) + ((var1 * ((int64)context->p2) << 12));
    var3 = ((int64)1) << 47;
    var1 = (var3 + var1) * ((int64)context->p1) >> 33;

    /* To avoid divide by zero exception */
    if (var1 != 0)
    {
        var4 = (((uint64)1) << 20) - uncomp->p;
        var4 = (((var4 << 31) - var2) * 3125) / var1;
        var1 = (((int64)context->p9) * (var4 >> 13) * (var4 >> 13)) >> 25;
        var2 = (((int64)context->p8) * var4) >> 19;
        var4 = ((var4 + var1 + var2) >> 8) + (((int64)context->p7) << 4);
        pressure = (uint32)(((var4 >> 1) * 100) >> 7);
        if (pressure < pressure_min)
        {
            pressure = pressure_min;
        }
        else if (pressure > pressure_max)
        {
            pressure = pressure_max;
        }
    }
    else
    {
        pressure = pressure_min;
    }

    return pressure;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    UART_UartPutString("HELLO WORLD\r\n");
    I2CM_Start();
    
    // Initialize BME280 context
    bme280_init(&context);
    bme280_getParameters(&context);
    {
        char sbuf[128];
        sprintf(sbuf, "T1=%d T2=%d T3=%d\r\n", context.t1, context.t2, context.t3);
        UART_UartPutString(sbuf);
    }
    
    // Start measurement
    bme280_normalMode(&context);
    

    for(;;)
    {
        /* Place your application code here. */
        {
            uint8 rbuf[BME280_PTH_LEN];
            int32 temp;
            uint32 press;
            char sbuf[128];
            struct Bme280Uncomp uncomp;
            
            bme280_writeAddress(&context, BME280_PTH);
            bme280_readRegister(&context, BME280_PTH_LEN, rbuf);
            bme280_storePTH(&uncomp, rbuf);
            temp = bme280_compensate_T(&context, &uncomp);
            press = bme280_compensate_P(&context, &uncomp);
            sprintf(sbuf, "TEMP=%ld PRESS=%ld\r\n", temp, press); 
            UART_UartPutString(sbuf);
        }
        CyDelay(1000);
    }
}


/* [] END OF FILE */
