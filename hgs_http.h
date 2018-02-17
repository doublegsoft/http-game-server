
#ifndef HGS_HTTP_H
#define HGS_HTTP_H

#include "mongoose.h"

void hgs_http_init(const char* http_port, const char* web_root);

void hgs_http_handle(struct mg_connection *conn, int evt, void* data);

#endif
