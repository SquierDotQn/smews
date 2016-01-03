#include <rflpc17xx/rflpc17xx.h>
#include <rflpc17xx/drivers/spi.h>

#define SPI_PORT RFLPC_SPI1
#define SIZE 8
#define NLED SIZE*SIZE

#define SPI_WRITE(val) rflpc_spi_write(SPI_PORT, (val))

#define SPI_INIT do { \
   int spi_peripheral_clock = rflpc_clock_get_system_clock() / 8; \
   int needed_divider = spi_peripheral_clock / 125000; \
   int serial_clock_rate_divider = 1; \
   while (needed_divider / serial_clock_rate_divider > 254) \
      serial_clock_rate_divider++; \
   needed_divider /= serial_clock_rate_divider; \
   int polarity = RFLPC_SPI_CPOL_RISING_EDGE | RFLPC_SPI_CPHA_PHASE_FIRST_EDGE; \
   rflpc_spi_init(SPI_PORT, RFLPC_SPI_MASTER, RFLPC_CCLK_8, 8, needed_divider, serial_clock_rate_divider, polarity); \
   }while(0);

#define INIT_WAIT do { \
   rflpc_timer_enable(RFLPC_TIMER3); \
   rflpc_timer_set_clock(RFLPC_TIMER3,RFLPC_CCLK/8);\
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER3, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */ \
   rflpc_timer_start(RFLPC_TIMER3); \
  } while (0)

#define BLEU  0xFF
#define ROUGE  0xF0    
#define VERT  0x0F
#define CLEAR  0x00

void clear_led();
void unie ();

