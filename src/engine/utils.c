/*
* Wire
* Copyright (C) 2016 Wire Swiss GmbH
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/* libavs -- The Engine
 *
 * Utility functions.
 */

#include <re.h>
#include "avs_uuid.h"
#include "avs_zapi.h"
#include "avs_media.h"
#include "avs_flowmgr.h"
#include "avs_jzon.h"
#include "avs_rest.h"
#include "avs_store.h"
#include "avs_engine.h"
#include "utils.h"


int engine_alloc_status(struct engine_status_data **datap,
			engine_status_h *h, void *arg)
{
	struct engine_status_data *data;

	if (!datap)
		return EINVAL;

	data = mem_alloc(sizeof(*data), NULL);
	if (!data)
		return ENOMEM;

	data->h = h;
	data->arg = arg;

	*datap = data;

	return 0;
}


/* XXX Deprecated. Use rest_err directly.
 */
int engine_rest_err(int err, const struct http_msg *msg)
{
	return rest_err(err, msg);
}


void engine_status_handler(int err, const struct http_msg *msg,
			   struct mbuf *mb, struct json_object *jobj,
			   void *arg)
{
	struct engine_status_data *data = arg;

	err = engine_rest_err(err, msg);
	if (data->h)
		data->h(err, data->arg);

 	mem_deref(data);
}


int engine_str_repl(char **dstp, const char *src)
{
	char *dst;
	int err;

	err = str_dup(&dst, src);
	if (err)
		return err;
	mem_deref(*dstp);
	*dstp = dst;
	return 0;
}
