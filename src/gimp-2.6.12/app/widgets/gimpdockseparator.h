/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpdockseparator.h
 * Copyright (C) 2005 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_DOCK_SEPARATOR_H__
#define __GIMP_DOCK_SEPARATOR_H__


#define GIMP_TYPE_DOCK_SEPARATOR            (gimp_dock_separator_get_type ())
#define GIMP_DOCK_SEPARATOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_DOCK_SEPARATOR, GimpDockSeparator))
#define GIMP_DOCK_SEPARATOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_DOCK_SEPARATOR, GimpDockSeparatorClass))
#define GIMP_IS_DOCK_SEPARATOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_DOCK_SEPARATOR))
#define GIMP_IS_DOCK_SEPARATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_DOCK_SEPARATOR))
#define GIMP_DOCK_SEPARATOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_DOCK_SEPARATOR, GimpDockSeparatorClass))


typedef struct _GimpDockSeparatorClass GimpDockSeparatorClass;

struct _GimpDockSeparator
{
  GtkEventBox  parent_instance;

  GimpDock    *dock;

  GtkWidget   *frame;
  GtkWidget   *label;
};

struct _GimpDockSeparatorClass
{
  GtkEventBoxClass  parent_class;
};


GType       gimp_dock_separator_get_type       (void) G_GNUC_CONST;

GtkWidget * gimp_dock_separator_new            (GimpDock          *dock);

void        gimp_dock_separator_set_show_label (GimpDockSeparator *separator,
                                                gboolean           show);


#endif /* __GIMP_DOCK_SEPARATOR_H__ */
