#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-13.h"
#include "er-coap-13-engine.h"

#include <string.h>

#define DEBUG DEBUG_NONE
#include "uip-debug.h"

static struct uip_udp_conn *udp_conn = NULL;
/*-----------------------------------------------------------------------------------*/
void
coap_init_communication_layer(uint16_t port)
{
  /* new connection with remote host */
  udp_conn = udp_new(NULL, 0, NULL);
  udp_bind(udp_conn, port);
  PRINTF("Listening on port %u\n", uip_ntohs(udp_conn->lport));
}
/*-----------------------------------------------------------------------------------*/
void
coap_send_message(context_t * ctx, uip_ipaddr_t *addr, uint16_t port, uint8_t *data, uint16_t length)
{
  /* Configure connection to reply to client */
  uip_ipaddr_copy(&ctx->ripaddr, addr);
  ctx->rport = port;

  uip_udp_packet_send(ctx, data, length);
  PRINTF("-sent UDP datagram (%u)-\n", length);

  /* Restore server connection to allow data from any node */
  memset(&ctx->ripaddr, 0, sizeof(ctx->ripaddr));
  udp_conn->rport = 0;
}
/*-----------------------------------------------------------------------------------*/
void
coap_handle_receive()
{
  coap_receive(udp_conn);
}
