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

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "resolution-calibrate-dialog.h"

#include "gimp-intl.h"


static GtkWidget *calibrate_entry = NULL;
static gdouble    calibrate_xres  = 1.0;
static gdouble    calibrate_yres  = 1.0;
static gint       ruler_width     = 1;
static gint       ruler_height    = 1;


/**
 * resolution_calibrate_dialog:
 * @resolution_entry: a #GimpSizeEntry to connect the dialog to
 * @pixbuf:           an optional #GdkPixbuf for the upper left corner
 *
 * Displays a dialog that allows the user to interactively determine
 * her monitor resolution. This dialog runs it's own GTK main loop and
 * is connected to a #GimpSizeEntry handling the resolution to be set.
 **/
void
resolution_calibrate_dialog (GtkWidget *resolution_entry,
                             GdkPixbuf *pixbuf)
{
  GtkWidget    *dialog;
  GtkWidget    *table;
  GtkWidget    *vbox;
  GtkWidget    *hbox;
  GtkWidget    *ruler;
  GtkWidget    *label;
  GdkScreen    *screen;
  GdkRectangle  rect;
  gint          monitor;

  g_return_if_fail (GIMP_IS_SIZE_ENTRY (resolution_entry));
  g_return_if_fail (GTK_WIDGET_REALIZED (resolution_entry));
  g_return_if_fail (pixbuf == NULL || GDK_IS_PIXBUF (pixbuf));

  /*  this dialog can only exist once  */
  if (calibrate_entry)
    return;

  dialog = gimp_dialog_new (_("Calibrate Monitor Resolution"),
                            "gimp-resolution-calibration",
                            gtk_widget_get_toplevel (resolution_entry),
                            GTK_DIALOG_DESTROY_WITH_PARENT,
                            NULL, NULL,

                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,

                            NULL);

  gtk_dialog_set_alternative_button_order (GTK_DIALOG (dialog),
                                           GTK_RESPONSE_OK,
                                           GTK_RESPONSE_CANCEL,
                                           -1);

  screen = gtk_widget_get_screen (dialog);
  monitor = gdk_screen_get_monitor_at_window (screen,
                                              resolution_entry->window);
  gdk_screen_get_monitor_geometry (screen, monitor, &rect);

  ruler_width  = rect.width  - 300 - (rect.width  % 100);
  ruler_height = rect.height - 300 - (rect.height % 100);

  table = gtk_table_new (4, 4, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (table), 12);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), table);
  gtk_widget_show (table);

  if (pixbuf)
    {
      GtkWidget *image = gtk_image_new_from_pixbuf (pixbuf);

      gtk_table_attach (GTK_TABLE (table), image, 0, 1, 0, 1,
                        GTK_SHRINK, GTK_SHRINK, 4, 4);
      gtk_widget_show (image);
    }

  ruler = gimp_ruler_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_size_request (ruler, ruler_width, 32);
  gimp_ruler_set_range (GIMP_RULER (ruler), 0, ruler_width, ruler_width);
  gtk_table_attach (GTK_TABLE (table), ruler, 1, 3, 0, 1,
                    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (ruler);

  ruler = gimp_ruler_new (GTK_ORIENTATION_VERTICAL);
  gtk_widget_set_size_request (ruler, 32, ruler_height);
  gimp_ruler_set_range (GIMP_RULER (ruler), 0, ruler_height, ruler_height);
  gtk_table_attach (GTK_TABLE (table), ruler, 0, 1, 1, 3,
                    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (ruler);

  vbox = gtk_vbox_new (FALSE, 12);
  gtk_table_attach (GTK_TABLE (table), vbox, 1, 2, 1, 2,
                    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (vbox);

  label =
    gtk_label_new (_("Measure the rulers and enter their lengths:"));
  gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gimp_label_set_attributes (GTK_LABEL (label),
                             PANGO_ATTR_SCALE,  PANGO_SCALE_LARGE,
                             PANGO_ATTR_WEIGHT, PANGO_WEIGHT_BOLD,
                             -1);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  calibrate_xres =
    gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (resolution_entry), 0);
  calibrate_yres =
    gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (resolution_entry), 1);

  calibrate_entry =
    gimp_coordinates_new  (GIMP_UNIT_INCH, "%p",
                           FALSE, FALSE, 10,
                           GIMP_SIZE_ENTRY_UPDATE_SIZE,
                           FALSE,
                           FALSE,
                           _("_Horizontal:"),
                           ruler_width,
                           calibrate_xres,
                           1, GIMP_MAX_IMAGE_SIZE,
                           0, 0,
                           _("_Vertical:"),
                           ruler_height,
                           calibrate_yres,
                           1, GIMP_MAX_IMAGE_SIZE,
                           0, 0);
  gtk_widget_hide (GTK_WIDGET (GIMP_COORDINATES_CHAINBUTTON (calibrate_entry)));
  g_signal_connect (dialog, "destroy",
                    G_CALLBACK (gtk_widget_destroyed),
                    &calibrate_entry);

  gtk_box_pack_end (GTK_BOX (hbox), calibrate_entry, FALSE, FALSE, 0);
  gtk_widget_show (calibrate_entry);

  gtk_widget_show (dialog);

  switch (gimp_dialog_run (GIMP_DIALOG (dialog)))
    {
    case GTK_RESPONSE_OK:
      {
        GtkWidget *chain_button;
        gdouble    x, y;

        x = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (calibrate_entry), 0);
        y = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (calibrate_entry), 1);

        calibrate_xres = (gdouble) ruler_width  * calibrate_xres / x;
        calibrate_yres = (gdouble) ruler_height * calibrate_yres / y;

        chain_button = GIMP_COORDINATES_CHAINBUTTON (resolution_entry);

        if (ABS (x - y) > GIMP_MIN_RESOLUTION)
          gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain_button),
                                        FALSE);

        gimp_size_entry_set_refval (GIMP_SIZE_ENTRY (resolution_entry),
                                    0, calibrate_xres);
        gimp_size_entry_set_refval (GIMP_SIZE_ENTRY (resolution_entry),
                                    1, calibrate_yres);
      }

    default:
      break;
    }

  gtk_widget_destroy (dialog);
}
