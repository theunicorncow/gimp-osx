/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* NOTE: This file is auto-generated by pdbgen.pl. */

#include "config.h"

#include <string.h>

#include <glib-object.h>

#include "pdb-types.h"

#include "base/temp-buf.h"
#include "core/gimp.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimpparamspecs.h"
#include "core/gimppattern.h"

#include "gimppdb.h"
#include "gimppdb-utils.h"
#include "gimpprocedure.h"
#include "internal-procs.h"


static GValueArray *
patterns_refresh_invoker (GimpProcedure      *procedure,
                          Gimp               *gimp,
                          GimpContext        *context,
                          GimpProgress       *progress,
                          const GValueArray  *args,
                          GError            **error)
{
  gimp_data_factory_data_refresh (gimp->pattern_factory);

  return gimp_procedure_get_return_values (procedure, TRUE, NULL);
}

static GValueArray *
patterns_get_list_invoker (GimpProcedure      *procedure,
                           Gimp               *gimp,
                           GimpContext        *context,
                           GimpProgress       *progress,
                           const GValueArray  *args,
                           GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *filter;
  gint32 num_patterns = 0;
  gchar **pattern_list = NULL;

  filter = g_value_get_string (&args->values[0]);

  if (success)
    {
      pattern_list = gimp_container_get_filtered_name_array (gimp->pattern_factory->container,
                                                             filter, &num_patterns);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], num_patterns);
      gimp_value_take_stringarray (&return_vals->values[2], pattern_list, num_patterns);
    }

  return return_vals;
}

static GValueArray *
patterns_get_pattern_invoker (GimpProcedure      *procedure,
                              Gimp               *gimp,
                              GimpContext        *context,
                              GimpProgress       *progress,
                              const GValueArray  *args,
                              GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  gchar *name = NULL;
  gint32 width = 0;
  gint32 height = 0;

  GimpPattern *pattern = gimp_context_get_pattern (context);

  if (pattern)
    {
      name   = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
      width  = pattern->mask->width;
      height = pattern->mask->height;
    }
  else
    success = FALSE;

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    {
      g_value_take_string (&return_vals->values[1], name);
      g_value_set_int (&return_vals->values[2], width);
      g_value_set_int (&return_vals->values[3], height);
    }

  return return_vals;
}

static GValueArray *
patterns_get_pattern_data_invoker (GimpProcedure      *procedure,
                                   Gimp               *gimp,
                                   GimpContext        *context,
                                   GimpProgress       *progress,
                                   const GValueArray  *args,
                                   GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *name;
  gchar *actual_name = NULL;
  gint32 width = 0;
  gint32 height = 0;
  gint32 mask_bpp = 0;
  gint32 length = 0;
  guint8 *mask_data = NULL;

  name = g_value_get_string (&args->values[0]);

  if (success)
    {
      GimpPattern *pattern;

      if (name && strlen (name))
        pattern = gimp_pdb_get_pattern (gimp, name, error);
      else
        pattern = gimp_context_get_pattern (context);

      if (pattern)
        {
          actual_name = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
          width       = pattern->mask->width;
          height      = pattern->mask->height;
          mask_bpp    = pattern->mask->bytes;
          length      = pattern->mask->height * pattern->mask->width *
                        pattern->mask->bytes;
          mask_data   = g_memdup (temp_buf_data (pattern->mask), length);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    {
      g_value_take_string (&return_vals->values[1], actual_name);
      g_value_set_int (&return_vals->values[2], width);
      g_value_set_int (&return_vals->values[3], height);
      g_value_set_int (&return_vals->values[4], mask_bpp);
      g_value_set_int (&return_vals->values[5], length);
      gimp_value_take_int8array (&return_vals->values[6], mask_data, length);
    }

  return return_vals;
}

void
register_patterns_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-patterns-refresh
   */
  procedure = gimp_procedure_new (patterns_refresh_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-patterns-refresh");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-patterns-refresh",
                                     "Refresh current patterns. This function always succeeds.",
                                     "This procedure retrieves all patterns currently in the user's pattern path and updates all pattern dialogs accordingly.",
                                     "Michael Natterer <mitch@gimp.org>",
                                     "Michael Natterer",
                                     "2002",
                                     NULL);
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-patterns-get-list
   */
  procedure = gimp_procedure_new (patterns_get_list_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-patterns-get-list");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-patterns-get-list",
                                     "Retrieve a complete listing of the available patterns.",
                                     "This procedure returns a complete listing of available GIMP patterns. Each name returned can be used as input to the 'gimp-context-set-pattern'.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("filter",
                                                       "filter",
                                                       "An optional regular expression used to filter the list",
                                                       FALSE, TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-patterns",
                                                          "num patterns",
                                                          "The number of patterns in the pattern list",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("pattern-list",
                                                                 "pattern list",
                                                                 "The list of pattern names",
                                                                 GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-patterns-get-pattern
   */
  procedure = gimp_procedure_new (patterns_get_pattern_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-patterns-get-pattern");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-patterns-get-pattern",
                                     "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
                                     "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
                                     "",
                                     "",
                                     "",
                                     "gimp-context-get-pattern");
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("name",
                                                           "name",
                                                           "The pattern name",
                                                           FALSE, FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The pattern width",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The pattern height",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-patterns-get-pattern-data
   */
  procedure = gimp_procedure_new (patterns_get_pattern_data_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-patterns-get-pattern-data");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-patterns-get-pattern-data",
                                     "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
                                     "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
                                     "",
                                     "",
                                     "",
                                     "gimp-pattern-get-pixels");
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The pattern name (\"\" means currently active pattern)",
                                                       FALSE, TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("actual-name",
                                                           "actual name",
                                                           "The pattern name",
                                                           FALSE, FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The pattern width",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The pattern height",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("mask-bpp",
                                                          "mask bpp",
                                                          "Pattern bytes per pixel",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("length",
                                                          "length",
                                                          "Length of pattern mask data",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int8_array ("mask-data",
                                                               "mask data",
                                                               "The pattern mask data",
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
