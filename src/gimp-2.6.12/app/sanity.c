/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#include "config.h"

#include <glib.h>
#include <fontconfig/fontconfig.h>
#include <pango/pango.h>
#include <pango/pangoft2.h>
#include <gegl.h>

#include "libgimpbase/gimpbase.h"

#include "sanity.h"

#include "gimp-intl.h"


static gchar * sanity_check_gimp              (void);
static gchar * sanity_check_glib              (void);
static gchar * sanity_check_pango             (void);
static gchar * sanity_check_fontconfig        (void);
static gchar * sanity_check_freetype          (void);
static gchar * sanity_check_babl              (void);
static gchar * sanity_check_gegl              (void);
static gchar * sanity_check_filename_encoding (void);


/*  public functions  */

const gchar *
sanity_check (void)
{
  gchar *abort_message = sanity_check_gimp ();

  if (! abort_message)
    abort_message = sanity_check_glib ();

  if (! abort_message)
    abort_message = sanity_check_pango ();

  if (! abort_message)
    abort_message = sanity_check_fontconfig ();

  if (! abort_message)
    abort_message = sanity_check_freetype ();

  if (! abort_message)
    abort_message = sanity_check_babl ();

  if (! abort_message)
    abort_message = sanity_check_gegl ();

  if (! abort_message)
    abort_message = sanity_check_filename_encoding ();

  return abort_message;
}


/*  private functions  */

static gboolean
sanity_check_version (guint major_version, guint required_major,
                      guint minor_version, guint required_minor,
                      guint micro_version, guint required_micro)
{
  if (major_version > required_major)
    return TRUE;

  if (major_version < required_major)
    return FALSE;

  if (minor_version > required_minor)
    return TRUE;

  if (minor_version < required_minor)
    return FALSE;

  if (micro_version >= required_micro)
    return TRUE;

  return FALSE;
}

static gchar *
sanity_check_gimp (void)
{
  if (GIMP_MAJOR_VERSION != gimp_major_version ||
      GIMP_MINOR_VERSION != gimp_minor_version ||
      GIMP_MICRO_VERSION != gimp_micro_version)
    {
      return g_strdup_printf
        ("Libgimp version mismatch!\n\n"
         "The GIMP binary cannot run with a libgimp version\n"
         "other than its own. This is GIMP %d.%d.%d, but the\n"
         "libgimp version is %d.%d.%d.\n\n"
         "Maybe you have GIMP versions in both /usr and /usr/local ?",
         GIMP_MAJOR_VERSION, GIMP_MINOR_VERSION, GIMP_MICRO_VERSION,
         gimp_major_version, gimp_minor_version, gimp_micro_version);
    }

  return NULL;
}

static gchar *
sanity_check_glib (void)
{
#define GLIB_REQUIRED_MAJOR 2
#define GLIB_REQUIRED_MINOR 16
#define GLIB_REQUIRED_MICRO 1

  const gchar *mismatch = glib_check_version (GLIB_REQUIRED_MAJOR,
                                              GLIB_REQUIRED_MINOR,
                                              GLIB_REQUIRED_MICRO);

  if (mismatch)
    {
      return g_strdup_printf
        ("%s\n\n"
         "GIMP requires GLib version %d.%d.%d or later.\n"
         "Installed GLib version is %d.%d.%d.\n\n"
         "Somehow you or your software packager managed\n"
         "to install GIMP with an older GLib version.\n\n"
         "Please upgrade to GLib version %d.%d.%d or later.",
         mismatch,
         GLIB_REQUIRED_MAJOR, GLIB_REQUIRED_MINOR, GLIB_REQUIRED_MICRO,
         glib_major_version, glib_minor_version, glib_micro_version,
         GLIB_REQUIRED_MAJOR, GLIB_REQUIRED_MINOR, GLIB_REQUIRED_MICRO);
    }

#undef GLIB_REQUIRED_MAJOR
#undef GLIB_REQUIRED_MINOR
#undef GLIB_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_pango (void)
{
#define PANGO_REQUIRED_MAJOR 1
#define PANGO_REQUIRED_MINOR 18
#define PANGO_REQUIRED_MICRO 0

  const gchar *mismatch = pango_version_check (PANGO_REQUIRED_MAJOR,
                                               PANGO_REQUIRED_MINOR,
                                               PANGO_REQUIRED_MICRO);

  if (mismatch)
    {
      const gint pango_major_version = pango_version () / 100 / 100;
      const gint pango_minor_version = pango_version () / 100 % 100;
      const gint pango_micro_version = pango_version () % 100;

      return g_strdup_printf
        ("%s\n\n"
         "GIMP requires Pango version %d.%d.%d or later.\n"
         "Installed Pango version is %d.%d.%d.\n\n"
         "Somehow you or your software packager managed\n"
         "to install GIMP with an older Pango version.\n\n"
         "Please upgrade to Pango version %d.%d.%d or later.",
         mismatch,
         PANGO_REQUIRED_MAJOR, PANGO_REQUIRED_MINOR, PANGO_REQUIRED_MICRO,
         pango_major_version, pango_minor_version, pango_micro_version,
         PANGO_REQUIRED_MAJOR, PANGO_REQUIRED_MINOR, PANGO_REQUIRED_MICRO);
    }

#undef PANGO_REQUIRED_MAJOR
#undef PANGO_REQUIRED_MINOR
#undef PANGO_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_fontconfig (void)
{
  const gint fc_version = FcGetVersion ();

#define FC_REQUIRED_MAJOR 2
#define FC_REQUIRED_MINOR 2
#define FC_REQUIRED_MICRO 0

  if (fc_version < ((FC_REQUIRED_MAJOR * 10000) +
                    (FC_REQUIRED_MINOR *   100) +
                    (FC_REQUIRED_MICRO *     1)))
    {
      const gint fc_major_version = fc_version / 100 / 100;
      const gint fc_minor_version = fc_version / 100 % 100;
      const gint fc_micro_version = fc_version % 100;

      return g_strdup_printf
        ("The Fontconfig version being used is too old!\n\n"
         "GIMP requires Fontconfig version %d.%d.%d or later.\n"
         "The Fontconfig version loaded by GIMP is %d.%d.%d.\n\n"
         "This may be caused by another instance of libfontconfig.so.1\n"
         "being installed in the system, probably in /usr/X11R6/lib.\n"
         "Please correct the situation or report it to someone who can.",
         FC_REQUIRED_MAJOR, FC_REQUIRED_MINOR, FC_REQUIRED_MICRO,
         fc_major_version, fc_minor_version, fc_micro_version);
    }

#undef FC_REQUIRED_MAJOR
#undef FC_REQUIRED_MINOR
#undef FC_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_freetype (void)
{
  FT_Library ft_library;
  FT_Int     ft_major_version;
  FT_Int     ft_minor_version;
  FT_Int     ft_micro_version;
  FT_Int     ft_version;

#define FT_REQUIRED_MAJOR 2
#define FT_REQUIRED_MINOR 1
#define FT_REQUIRED_MICRO 7

  if (FT_Init_FreeType (&ft_library) != 0)
    g_error ("FT_Init_FreeType() failed");

  FT_Library_Version (ft_library,
                      &ft_major_version,
                      &ft_minor_version,
                      &ft_micro_version);

  if (FT_Done_FreeType (ft_library) != 0)
    g_error ("FT_Done_FreeType() failed");

  ft_version = (ft_major_version * 10000 +
                ft_minor_version *   100 +
                ft_micro_version *     1);

  if (ft_version < ((FT_REQUIRED_MAJOR * 10000) +
                    (FT_REQUIRED_MINOR *   100) +
                    (FT_REQUIRED_MICRO *     1)))
    {
      return g_strdup_printf
        ("FreeType version too old!\n\n"
         "GIMP requires FreeType version %d.%d.%d or later.\n"
         "Installed FreeType version is %d.%d.%d.\n\n"
         "Somehow you or your software packager managed\n"
         "to install GIMP with an older FreeType version.\n\n"
         "Please upgrade to FreeType version %d.%d.%d or later.",
         FT_REQUIRED_MAJOR, FT_REQUIRED_MINOR, FT_REQUIRED_MICRO,
         ft_major_version, ft_minor_version, ft_micro_version,
         FT_REQUIRED_MAJOR, FT_REQUIRED_MINOR, FT_REQUIRED_MICRO);
    }

#undef FT_REQUIRED_MAJOR
#undef FT_REQUIRED_MINOR
#undef FT_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_babl (void)
{
  gint babl_major_version;
  gint babl_minor_version;
  gint babl_micro_version;

#define BABL_REQUIRED_MAJOR 0
#define BABL_REQUIRED_MINOR 0
#define BABL_REQUIRED_MICRO 22

  babl_get_version (&babl_major_version,
                    &babl_minor_version,
                    &babl_micro_version);

  if (! sanity_check_version (babl_major_version, BABL_REQUIRED_MAJOR,
                              babl_minor_version, BABL_REQUIRED_MINOR,
                              babl_micro_version, BABL_REQUIRED_MICRO))
    {
      return g_strdup_printf
        ("BABL version too old!\n\n"
         "GIMP requires BABL version %d.%d.%d or later.\n"
         "Installed BABL version is %d.%d.%d.\n\n"
         "Somehow you or your software packager managed\n"
         "to install GIMP with an older BABL version.\n\n"
         "Please upgrade to BABL version %d.%d.%d or later.",
         BABL_REQUIRED_MAJOR, BABL_REQUIRED_MINOR, BABL_REQUIRED_MICRO,
         babl_major_version, babl_minor_version, babl_micro_version,
         BABL_REQUIRED_MAJOR, BABL_REQUIRED_MINOR, BABL_REQUIRED_MICRO);
    }

#undef BABL_REQUIRED_MAJOR
#undef BABL_REQUIRED_MINOR
#undef BABL_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_gegl (void)
{
  gint gegl_major_version;
  gint gegl_minor_version;
  gint gegl_micro_version;

#define GEGL_REQUIRED_MAJOR 0
#define GEGL_REQUIRED_MINOR 0
#define GEGL_REQUIRED_MICRO 18

  gegl_get_version (&gegl_major_version,
                    &gegl_minor_version,
                    &gegl_micro_version);

  if (! sanity_check_version (gegl_major_version, GEGL_REQUIRED_MAJOR,
                              gegl_minor_version, GEGL_REQUIRED_MINOR,
                              gegl_micro_version, GEGL_REQUIRED_MICRO))
    {
      return g_strdup_printf
        ("GEGL version too old!\n\n"
         "GIMP requires GEGL version %d.%d.%d or later.\n"
         "Installed GEGL version is %d.%d.%d.\n\n"
         "Somehow you or your software packager managed\n"
         "to install GIMP with an older GEGL version.\n\n"
         "Please upgrade to GEGL version %d.%d.%d or later.",
         GEGL_REQUIRED_MAJOR, GEGL_REQUIRED_MINOR, GEGL_REQUIRED_MICRO,
         gegl_major_version, gegl_minor_version, gegl_micro_version,
         GEGL_REQUIRED_MAJOR, GEGL_REQUIRED_MINOR, GEGL_REQUIRED_MICRO);
    }

#undef GEGL_REQUIRED_MAJOR
#undef GEGL_REQUIRED_MINOR
#undef GEGL_REQUIRED_MICRO

  return NULL;
}

static gchar *
sanity_check_filename_encoding (void)
{
  gchar  *result;
  GError *error = NULL;

  result = g_filename_to_utf8 ("", -1, NULL, NULL, &error);

  if (! result)
    {
      gchar *msg =
        g_strdup_printf
        (_("The configured filename encoding cannot be converted to UTF-8: "
           "%s\n\n"
           "Please check the value of the environment variable "
           "G_FILENAME_ENCODING."),
         error->message);

      g_error_free (error);

      return msg;
    }

  g_free (result);

  result = g_filename_to_utf8 (gimp_directory (), -1, NULL, NULL, &error);

  if (! result)
    {
      gchar *msg =
        g_strdup_printf
        (_("The name of the directory holding the GIMP user configuration "
           "cannot be converted to UTF-8: "
           "%s\n\n"
           "Your filesystem probably stores files in an encoding "
           "other than UTF-8 and you didn't tell GLib about this. "
           "Please set the environment variable G_FILENAME_ENCODING."),
         error->message);

      g_error_free (error);

      return msg;
    }

  g_free (result);

  return NULL;
}
