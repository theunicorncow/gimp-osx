/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpconfig-path.h
 * Copyright (C) 2001-2002  Sven Neumann <sven@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_CONFIG_PATH_H__
#define __GIMP_CONFIG_PATH_H__


G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


/*
 * GIMP_TYPE_CONFIG_PATH
 */

#define GIMP_TYPE_CONFIG_PATH               (gimp_config_path_get_type ())
#define GIMP_VALUE_HOLDS_CONFIG_PATH(value) (G_TYPE_CHECK_VALUE_TYPE ((value), GIMP_TYPE_CONFIG_PATH))

GType               gimp_config_path_get_type        (void) G_GNUC_CONST;



/*
 * GIMP_TYPE_PARAM_CONFIG_PATH
 */

typedef enum
{
  GIMP_CONFIG_PATH_FILE,
  GIMP_CONFIG_PATH_FILE_LIST,
  GIMP_CONFIG_PATH_DIR,
  GIMP_CONFIG_PATH_DIR_LIST
} GimpConfigPathType;


#define GIMP_TYPE_PARAM_CONFIG_PATH            (gimp_param_config_path_get_type ())
#define GIMP_IS_PARAM_SPEC_CONFIG_PATH(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_CONFIG_PATH))

GType               gimp_param_config_path_get_type  (void) G_GNUC_CONST;

GParamSpec        * gimp_param_spec_config_path      (const gchar  *name,
                                                      const gchar  *nick,
                                                      const gchar  *blurb,
                                                      GimpConfigPathType  type,
                                                      const gchar  *default_value,
                                                      GParamFlags   flags);

GimpConfigPathType  gimp_param_spec_config_path_type (GParamSpec   *pspec);


/*
 * GimpConfigPath utilities
 */

gchar             * gimp_config_path_expand          (const gchar  *path,
                                                      gboolean      recode,
                                                      GError      **error) G_GNUC_MALLOC;

gchar             * gimp_config_build_data_path      (const gchar  *name) G_GNUC_MALLOC;
gchar             * gimp_config_build_writable_path  (const gchar  *name) G_GNUC_MALLOC;
gchar             * gimp_config_build_plug_in_path   (const gchar  *name) G_GNUC_MALLOC;


G_END_DECLS

#endif /* __GIMP_CONFIG_PATH_H__ */
