#include <stdlib.h>

#include "hgs_http.h"

static struct mg_serve_http_opts http_server_opts;

static int count_of_player = 0;

static int hgs_util_ishex(int x)
{
    return	(x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F');
}

static int hgs_http_decode(const char *s, char *dec)
{
  char *o;
  const char *end = s + strlen(s);
  int c;

  for (o = dec; s <= end; o++) {
    c = *s++;
    if (c == '+') c = ' ';
    else if (c == '%' && 
        (!hgs_util_ishex(*s++) ||
        !hgs_util_ishex(*s++) ||
        !sscanf(s - 2, "%2x", &c)))
        return -1;

    if (dec) *o = c;
  }

  return o - dec;
}

static void hgs_http_broadcast(struct mg_connection *nc, const struct mg_str msg) {
  struct mg_connection *c;
  char buf[500];
  char addr[32];
  mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                      MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

//  snprintf(buf, sizeof(buf), "%s %.*s", addr, (int) msg.len, msg.p);
  snprintf(buf, sizeof(buf), "%.*s", (int) msg.len, msg.p);
  // printf("%s\n", buf); /* Local echo. */
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    //if (c == nc) continue; /* Don't send to the sender. */
    mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, buf, strlen(buf));
  }
}

void hgs_http_handle(struct mg_connection *conn, int evt, void* data) {
  switch (evt) {
  case MG_EV_HTTP_REQUEST: 
  {  
    struct http_message *hm = (struct http_message *) data;
    mg_serve_http(conn, hm, http_server_opts);
    break;
  }
  case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
  {
    /* New websocket connection. Tell everybody. */
    // broadcast(nc, mg_mk_str("++ joined"));
    char buf[500];
    int x = rand();
    int y = rand();
    sprintf(buf, "{\"id\": %d, \"x\": %d, \"y\": %d}", ++count_of_player, x % 600, y % 600);
    mg_send_websocket_frame(conn, WEBSOCKET_OP_TEXT, buf, strlen(buf));
    break;
  }
  case MG_EV_WEBSOCKET_FRAME: 
  {
    struct websocket_message *wm = (struct websocket_message *) data;
    /* New websocket message. Tell everybody. */
    struct mg_str d = {(char *) wm->data, wm->size};
    hgs_http_broadcast(conn, d);
    break;
  }
  case MG_EV_CLOSE: 
  {
    break;
  }
  default:
    break;
  }
}

void hgs_http_init(const char* http_port, const char* web_root) 
{
  // start mongoose http server
  struct mg_mgr mgr;
  struct mg_connection *conn;

  mg_mgr_init(&mgr, NULL);
  conn = mg_bind(&mgr, http_port, hgs_http_handle);
  mg_set_protocol_http_websocket(conn);

  http_server_opts.document_root = web_root;  // Serve current directory
  http_server_opts.enable_directory_listing = "false";

  for (;;)
    mg_mgr_poll(&mgr, 100);
  mg_mgr_free(&mgr);
}
