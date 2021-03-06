/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationcolorize.c
 * Copyright (C) 2007 Michael Natterer <mitch@gimp.org>
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

#include <gegl.h>

#include "libgimpcolor/gimpcolor.h"

#include "gimp-gegl-types.h"

#include "gimpcolorizeconfig.h"
#include "gimpoperationcolorize.h"


static gboolean gimp_operation_colorize_process (GeglOperation       *operation,
                                                 void                *in_buf,
                                                 void                *out_buf,
                                                 glong                samples,
                                                 const GeglRectangle *roi);


G_DEFINE_TYPE (GimpOperationColorize, gimp_operation_colorize,
               GIMP_TYPE_OPERATION_POINT_FILTER)

#define parent_class gimp_operation_colorize_parent_class


static void
gimp_operation_colorize_class_init (GimpOperationColorizeClass *klass)
{
  GObjectClass                  *object_class    = G_OBJECT_CLASS (klass);
  GeglOperationClass            *operation_class = GEGL_OPERATION_CLASS (klass);
  GeglOperationPointFilterClass *point_class     = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  object_class->set_property   = gimp_operation_point_filter_set_property;
  object_class->get_property   = gimp_operation_point_filter_get_property;

  operation_class->name        = "gimp-colorize";
  operation_class->categories  = "color";
  operation_class->description = "GIMP Colorize operation";

  point_class->process         = gimp_operation_colorize_process;

  g_object_class_install_property (object_class,
                                   GIMP_OPERATION_POINT_FILTER_PROP_CONFIG,
                                   g_param_spec_object ("config",
                                                        "Config",
                                                        "The config object",
                                                        GIMP_TYPE_COLORIZE_CONFIG,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT));
}

static void
gimp_operation_colorize_init (GimpOperationColorize *self)
{
}

static gboolean
gimp_operation_colorize_process (GeglOperation       *operation,
                                 void                *in_buf,
                                 void                *out_buf,
                                 glong                samples,
                                 const GeglRectangle *roi)
{
  GimpOperationPointFilter *point  = GIMP_OPERATION_POINT_FILTER (operation);
  GimpColorizeConfig       *config = GIMP_COLORIZE_CONFIG (point->config);
  gfloat                   *src    = in_buf;
  gfloat                   *dest   = out_buf;
  GimpHSL                   hsl;

  if (! config)
    return FALSE;

  hsl.h = config->hue;
  hsl.s = config->saturation;

  while (samples--)
    {
      GimpRGB rgb;
      gfloat  lum = GIMP_RGB_LUMINANCE (src[RED_PIX],
                                        src[GREEN_PIX],
                                        src[BLUE_PIX]);

      if (config->lightness > 0)
        {
          lum = lum * (1.0 - config->lightness);

          lum += 1.0 - (1.0 - config->lightness);
        }
      else if (config->lightness < 0)
        {
          lum = lum * (config->lightness + 1.0);
        }

      hsl.l = lum;

      gimp_hsl_to_rgb (&hsl, &rgb);

      /*  the code in base/colorize.c would multiply r,b,g with lum,
       *  but this is a bug since it should multiply with 255. We
       *  don't repeat this bug here (this is the reason why the gegl
       *  colorize is brighter than the legacy one).
       */
      dest[RED_PIX]   = rgb.r; /* * lum; */
      dest[GREEN_PIX] = rgb.g; /* * lum; */
      dest[BLUE_PIX]  = rgb.b; /* * lum */;
      dest[ALPHA_PIX] = src[ALPHA_PIX];

      src  += 4;
      dest += 4;
    }

  return TRUE;
}
