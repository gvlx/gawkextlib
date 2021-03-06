#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "gawkextlib.h"

#undef DBGMSG


awk_bool_t
gawk_api_varinit_scalar(const gawk_api_t *api, awk_ext_id_t ext_id,
			const char *name, awk_value_t *initial_value,
			awk_bool_t override_existing_value,
			awk_scalar_t *cookie_result)
{
  awk_value_t val;
  awk_bool_t rc = 1;
  awk_bool_t dofree = 1;

  if (override_existing_value || !sym_lookup(name, AWK_UNDEFINED, &val) ||
      (val.val_type == AWK_UNDEFINED)) {
    /* either does not exist already, or we want to override it anyway */
    if (sym_update(name, initial_value))
      dofree = 0;	/* memory now belongs to gawk */
    else {
      rc = 0;
      goto freeit;
    }
  }
  if (sym_lookup(name, AWK_SCALAR, &val))
    *cookie_result = val.scalar_cookie;
  else
    rc = 0;

#ifdef DBGMSG
  if (rc) {
    sym_lookup_scalar(*cookie_result, AWK_STRING, &val);
    fprintf(stderr,
	    "DEBUG: initialized scalar %s with cookie %p and value [%s]\n",
	    name, *cookie_result, val.str_value.str);
  }
  else
    fprintf(stderr, "DEBUG: failed to initialize scalar %s\n", name);
#endif

freeit:
  if (dofree && (initial_value->val_type == AWK_STRING))
    free(initial_value->str_value.str);
  return rc;
}

awk_bool_t
gawk_api_varinit_constant(const gawk_api_t *api, awk_ext_id_t ext_id,
			  const char *name, awk_value_t *initial_value,
			  awk_scalar_t *cookie_result)
{
  awk_value_t val;

  if (sym_lookup(name, AWK_UNDEFINED, &val) ||
      !sym_constant(name, initial_value)) {
    if (initial_value->val_type == AWK_STRING)
      free(initial_value->str_value.str);
    return 0;
  }

  if (sym_lookup(name, AWK_SCALAR, &val)) {
    *cookie_result = val.scalar_cookie;
    return 1;
  }
  return 0;
}

awk_bool_t
gawk_api_varinit_array(const gawk_api_t *api, awk_ext_id_t ext_id,
		       const char *name, awk_bool_t clear_it,
		       awk_array_t *cookie_result)
{
  awk_value_t val;

  if (sym_lookup(name, AWK_UNDEFINED, &val)) {
    if (val.val_type != AWK_ARRAY)
      return 0;
    if (clear_it)
      clear_array(val.array_cookie);
  }
  else {
    val.val_type = AWK_ARRAY;
    val.array_cookie = create_array();
    /* Note: the sym_update call updates the array_cookie */
    if (!sym_update(name, &val))
      return 0;
  }
  *cookie_result = val.array_cookie;
  return 1;
}
