typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     /* startup routine */
extern void UART_TX_IRQHandler(void);
extern void UART_RX_IRQHandler(void);

#define NoneHandled (interrupt_handler_t)NonHandledInterrupt

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NoneHandled}, /* trap  */
	{0x82, NoneHandled}, /* irq0  */
	{0x82, NoneHandled}, /* irq1  */
	{0x82, NoneHandled}, /* irq2  */
	{0x82, NoneHandled}, /* irq3  */
	{0x82, NoneHandled}, /* irq4  */
	{0x82, NoneHandled}, /* irq5  */
	{0x82, NoneHandled}, /* irq6  */
	{0x82, NoneHandled}, /* irq7  */
	{0x82, NoneHandled}, /* irq8  */
	{0x82, NoneHandled}, /* irq9  */
	{0x82, NoneHandled}, /* irq10 */
	{0x82, NoneHandled}, /* irq11 */
	{0x82, NoneHandled}, /* irq12 */
	{0x82, NoneHandled}, /* irq13 */
	{0x82, NoneHandled}, /* irq14 */
	{0x82, NoneHandled}, /* irq15 */
	{0x82, NoneHandled}, /* irq16 */
	{0x82, (interrupt_handler_t)UART_TX_IRQHandler}, /* irq17 */
	{0x82, (interrupt_handler_t)UART_RX_IRQHandler}, /* irq18 */
	{0x82, NoneHandled}, /* irq19 I2C */
	{0x82, (interrupt_handler_t)UART_TX_IRQHandler}, /* irq20 */
	{0x82, (interrupt_handler_t)UART_RX_IRQHandler}, /* irq21 */
	{0x82, NoneHandled}, /* irq21 */
	{0x82, NoneHandled}, /* irq22 */
	{0x82, NoneHandled}, /* irq23 */
	{0x82, NoneHandled}, /* irq24 */
	{0x82, NoneHandled}, /* irq25 */
	{0x82, NoneHandled}, /* irq26 */
	{0x82, NoneHandled}, /* irq27 */
	{0x82, NoneHandled}, /* irq28 */
	{0x82, NoneHandled}, /* irq29 */
};