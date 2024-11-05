#include "controller_memory.h"

#define PORTS_NUMBER_DEFAULT 1

void
_controller_memory_init
(
    Controller_Memory *mem,
    I2C_HandleTypeDef *hi2c
)
{
    fram_init(&(mem->fram), hi2c, 0xA0, GPIOB, GPIO_PIN_1, MB85RC16, 10);
    mem->address_intrans   = 0xA;
    mem->address_status    = 0xB;
    mem->address_available = 0xC;
    mem->address_transid   = 0xD; // NEXT 0x11
    mem->address_fatal_err = 0x11; // 2 bytes
    mem->address_ports_num = 0x15;

    // _controller_memory_reset(mem);

    fram_read_byte (&(mem->fram), mem->address_intrans,   (uint8_t  *)&(mem->in_transaction));
    fram_read_byte (&(mem->fram), mem->address_status,    (uint8_t  *)&(mem->status        ));
    fram_read_byte (&(mem->fram), mem->address_available, (uint8_t  *)&(mem->available     ));
    fram_read_dword(&(mem->fram), mem->address_transid,   (uint32_t *)&(mem->transaction_id));
    fram_read_byte (&(mem->fram), mem->address_ports_num, (uint8_t  *)&(mem->ports_number  )); 

    fram_read_byte(&(mem->fram), mem->address_fatal_err,   (uint8_t *)&(mem->fatal_err.type));
    fram_read_byte(&(mem->fram), mem->address_fatal_err+1, (uint8_t *)&(mem->fatal_err.errors));
}


void
_controller_memory_reset(Controller_Memory *mem)
{
    mem->available      = false;
    mem->in_transaction = false;
    mem->status         = CPS_Unavailable;
    mem->transaction_id = 0;
    mem->ports_number   = PORTS_NUMBER_DEFAULT;
    
    _controller_memory_store(mem);
}

void
_controller_memory_store(Controller_Memory *mem)
{
    fram_write_byte (&(mem->fram), mem->address_intrans,   (uint8_t )(mem->in_transaction));
    fram_write_byte (&(mem->fram), mem->address_status,    (uint8_t )(mem->status        ));
    fram_write_byte (&(mem->fram), mem->address_available, (uint8_t )(mem->available     ));
    fram_write_dword(&(mem->fram), mem->address_transid,   (uint32_t)(mem->transaction_id));
    fram_write_byte (&(mem->fram), mem->address_ports_num, (uint8_t )(mem->ports_number  )); 

    fram_write_byte(&(mem->fram), mem->address_fatal_err,   (uint8_t)(mem->fatal_err.type));
    fram_write_byte(&(mem->fram), mem->address_fatal_err+1, (uint8_t)(mem->fatal_err.errors.ocpp_err));
}

void
_controller_memory_load(Controller_Memory *mem)
{
    fram_read_byte (&(mem->fram), mem->address_intrans,   (uint8_t  *)&(mem->in_transaction));
    fram_read_byte (&(mem->fram), mem->address_status,    (uint8_t  *)&(mem->status        ));
    fram_read_byte (&(mem->fram), mem->address_available, (uint8_t  *)&(mem->available     ));
    fram_read_dword(&(mem->fram), mem->address_transid,   (uint32_t *)&(mem->transaction_id));
    fram_read_byte (&(mem->fram), mem->address_ports_num, (uint8_t  *)&(mem->ports_number  )); 

    fram_read_byte(&(mem->fram), mem->address_fatal_err,   (uint8_t *)&(mem->fatal_err.type));
    fram_read_byte(&(mem->fram), mem->address_fatal_err+1, (uint8_t *)&(mem->fatal_err.errors));
}